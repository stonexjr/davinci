/*
Copyright (c) 2013-2017 Jinrong Xie (jrxie at ucdavis dot edu)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "GLBSpline2D.h"
#include <GLError.h>

GLBSpline2D::GLBSpline2D(int k1, int k2)
        :GLBSpline(k1),m_order2(k2),
	m_pVAO((GLVertexArrayObject*)NULL),
	m_pIbo((GLIndexBufferObject*)NULL)
{
}

GLBSpline2D::~GLBSpline2D(void)
{
}

void GLBSpline2D::createMesh()
{
    int dim1 = getDimensionY();
    int dim2 = getDimensionX();
    int k1 = m_order1;
    int k2 = m_order2;
    int n1 = dim1 + 1 - k1;
    int n2 = dim2 + 1 - k2;
    if(n1 < 1 || n2 < 1) {
        //dim < k
        std::stringstream ss;
        ss << __func__<<": (dim1="<<dim1<<"<k1="<<k1<<" or dim2="<<dim2
           <<"k2="<<k2<<"), try to decrease the order of BSpline!\n";
        std::string msg=ss.str();
        GLError::ErrorMessage(msg);
        return;
    }

    m_X1.clear();
    m_X2.clear();
    m_naturalKnots1.clear();
    m_naturalKnots2.clear();
    m_outputMeshValue.clear();

    m_X1 = linspace(0,1,100);
    m_X2 = linspace(0,1,100);
    m_naturalKnots1  = linspace(0,1,n1+1);
    m_naturalKnots2  = linspace(0,1,n2+1);
    //Add coincidental knots at both boundary of natual_knots.
    m_tau1.resize(m_naturalKnots1.size()+2*(k1-1));
    m_tau2.resize(m_naturalKnots2.size()+2*(k2-1));
    int l1 = (int)m_tau1.size();
    int l2 = (int)m_tau2.size();
    for (int i = 0 ; i < k1-1 ; i++)
    {
        m_tau1[i] = 0;
        m_tau1[l1-1-i] = 1;
    }
    for (int i = 0 ; i < k2-1 ; i++)
    {
        m_tau2[i] = 0;
        m_tau2[l2-1-i] = 1;
    }
    std::copy(m_naturalKnots1.begin(), m_naturalKnots1.end(), m_tau1.begin()+k1-1);
    std::copy(m_naturalKnots2.begin(), m_naturalKnots2.end(), m_tau2.begin()+k2-1);

#ifdef _DEBUG
    if (l1-k1 != dim1)
    {
        std::stringstream ss;
        ss <<__func__":m_controlPtArray.size()=dim="<<m_controlPtArrayBivariate[0].size()
          <<" != (l-k)="<<l1-k1<<"!";
        std::string msg=ss.str();
        GLError::ErrorMessage(msg);
    }
    if (l2-k2 != dim2)
    {
        std::stringstream ss;
        ss <<__func__":m_controlPtArray.size()=dim="<<m_controlPtArrayBivariate[0].size()
          <<" != (l-k)="<<l2-k2<<"!";
        std::string msg=ss.str();
        GLError::ErrorMessage(msg);
    }
#endif

    size_t sizeX2 = m_X2.size();
    //set mesh has X1.size() of rows and X2.size() of columns.
    m_outputMeshValue.resize(m_X1.size(), vector<float>(sizeX2,0));
    //using C++0X lamda expression.
    //http://stackoverflow.com/questions/7627098/what-is-a-lambda-expression-in-c11
    //set each row of mesh has dim2 of columns.
    //std::transform(m_outputMesh.begin(), m_outputMesh.end(),m_outputMesh.begin(),
    /*
    std::for_each(m_outputMesh.begin(), m_outputMesh.end(),
        [sizeX2](vector<vec3f>& v) ->
        vector<vec3f>& {
            v.resize(sizeX2); return v;
        });
    */
    vector<vector<float> > N1(m_X1.size(), vector<float>(l1-1,0));
    vector<vector<float> > N2(m_X2.size(), vector<float>(l2-1,0));
    vector<vector<float> >  V(m_X2.size(), vector<float>(l1-1,0));//weights for N1 basis function.
    //Evaluation the basis function at each point of first variable.
    for (int y=0 ; y < m_X1.size() ; y++)
    {
        float x1 = m_X1[y];
        compute_B_splines(m_tau1, k1, x1, N1[y]);
    }
    //Evaluation the basis function at each point of second variable.
    for (int x=0; x < m_X2.size() ; x++)
    {
        float x2 = m_X2[x];
        compute_B_splines(m_tau2, k2, x2, N2[x]);
        //Evaluate weights V for N1[1..dim1].
        for (int i=0 ; i < l1-k1 ; i++)
        {
            float tmp=1.0f;
            //dot product of Weights(i,:)*N2(x,:);
            for (int j=0 ; j < dim2 ; j++)
            {
                tmp+=m_controlPtArrayBivariate[i][j]*N2[x][j];
            }
            V[x][i]=tmp;
        }
    }

    for (int y=0; y < m_X1.size() ; y++)
    {
        for (int x=0; x < m_X2.size() ; x++)
        {
            //dot product of coefficient V(x,:)*N1(y,:)'
            float tmp=1.0f;
            for (int i=0; i < dim1 ; i++)
            {
                tmp+=V[x][i]*N1[y][i];
            }
            m_outputMeshValue[y][x] = tmp;
        }
    }

    createVBO();
}

void GLBSpline2D::createVBO()
{
    //Generate all mesh points based on m_X1(as y coordinates),
    // m_X2(as x coordinates) and m_outputMeshValue(as Z coordinates).
    m_bbox = BBox();
    vector<vec3f> meshGrids;//(m_X1.size()*m_X2.size());
    for (int i=0 ; i < m_X1.size() ; i++)
    {//y
        for (int j=0 ; j < m_X2.size() ; j++)
        {//x
            //The mesh points are arranged row by row and column by column
            //in the linear VBO.
            vec3f vtx(m_X2[j], m_X1[i], m_outputMeshValue[i][j]);
            m_bbox << vtx;
            //*m_pVbo << vtx;
            meshGrids.push_back(vtx);
        }
    }
    GLVertexBufferObjectRef vbo =
                    GLVertexBufferObjectRef(new GLVertexBufferObject());

    vbo->upload(meshGrids.size()*sizeof(vec3f), meshGrids.size(),
                reinterpret_cast<GLvoid*>(meshGrids.data()) );

    m_pVAO = GLVertexArrayObjectRef(new GLVertexArrayObject(GL_POLYGON));
    m_pVAO->enableVertex(true);
    m_pVAO->setVertexType(GL_FLOAT);
    m_pVAO->setVertexStrideInBytes(sizeof(vec3f));
    m_pVAO->setVertexOffsetInBytes(0);
    m_pVAO->attachDefaultVBO(vbo);
    m_pVAO->enable();

    m_pIbo = GLIndexBufferObjectRef(new GLIndexBufferObject(GL_POLYGON));
    m_pIbo->enableRestart(true);
    m_pIbo->setRestartIndex(-1);
    GLError::glCheckError(__func__);
    //Generate vertex index for each quad
    int sizeX1 = m_X1.size();
    int sizeX2 = m_X2.size();
    for (int i=0 ; i < sizeX1-1 ; i++)
    {
        for (int j=0 ; j < sizeX2-1 ; j++)
        {
            *m_pIbo << (i*sizeX2+j)
                    << (i+1)*sizeX2+j
                    << (i+1)*sizeX2+j+1
                    <<  i*sizeX2+j+1
                    << -1;
        }
    }
    m_pIbo->enable();
}

void GLBSpline2D::draw(GLfloat lineWidth/*=1.0*/)
{
    glMatrixMode(GL_MODELVIEW);
    glColor3fv((float*)(m_lineColor));
    glLineWidth(lineWidth);
    glPushMatrix();
    //m_pVbo->bind();
    //m_pIbo->bind();
	m_pIbo->attachVAO(m_pVAO);
    m_pIbo->draw();
    //m_pIbo->unbind();
    //m_pVbo->unbind();
    glPopMatrix();
}
