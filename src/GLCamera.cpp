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

//AUTHOR: Jinrong Xie (stonexjr at gmail.com)
//UPDATED: 2014-05-06
#if defined(__APPLE__) || defined(MACOSX)
#include <gl.h>
#include <gl3.h>
#include <OpenGL.h>
#else
#include <GL/glew.h>
#endif
#include "GLCamera.h"
#include <GL/glu.h>
#include <GL/freeglut.h>
#include "constant.h"
#include "GLError.h"
#include "mathtool.h"

namespace davinci{

GLCamera::GLCamera(void)
		:m_posDest(0.0f,0.0f,0.0f), m_backward(0.0f,0.0f,-1.0f),
		  m_up(0.0f, 1.0f, 0.0f),m_right(1.0f,0.0f,0.0f),m_speed(0.2f)
		,m_flipped(1), m_inertiaXYZ(1.0f,1.0f,1.0f),m_targetSphericalAngleDest(PI,0.0f)
		,m_inertiaLookAt(1.0f), m_lookAtMode(FREE_FORM), m_iStep(0),m_bIsRoaming(false)
		,m_staringPos(0.0f)
{
	m_posLag = m_posDest;
	m_targetSphericalAngleLag = m_targetSphericalAngleDest;
}

void GLCamera::toggleRoaming( bool val )
{
	m_bIsRoaming = val;
	//m_iStep=0;
}

void GLCamera::setLookAt( const vec3f& position, const vec3f& target,
						  const vec3f& up/*=vec3f(0.0f,1.0f,0.0f)*/ )
{
	m_posLag = position;
	m_target = target;

	//The GLCamera reference frame is similar to the world-space reference frame,
	//which positive x-axis goes right, positive y-axis goes upward and z-axis
	//points towards viewer.
	//However, after viewing transformation, the subsequent projection transformation
	//would transform the eye coordinates into Clip coordinates, and then transformed
	// into NDC, where the z-axis in reversed, by dividing x,y and z by the w component.
	// which is explained in the following reference docs.
	//http://stackoverflow.com/questions/4124041/is-opengl-coordinate-system-left-handed-or-right-handed
	//in perspective/orthographic projection the z-axis is reversed, which points away from viewer.
	//http://www.songho.ca/opengl/gl_lookattoaxes.html
	m_backward = position - target;//target - position;
	m_backward.normalize();
	//Extra care to take when forward vector is on the Y axis.
	if (fabsf(m_backward.x()) < 0.00001f//FLOAT_EPSILON
					&& fabsf(m_backward.z()) < 0.00001f)//FLOAT_EPSILON)
	{
		m_flipped *= -1.0f;
		if (m_backward.y() > 0)//looking upward
		{
			m_up = vec3f(0.0f, 0.0f, 1.0f);
			//printf("m_up+++++\n");
		}
		else//looking downward
		{
			m_up = vec3f(0.0f, 0.0f, -1.0f);
			//printf("m_up-----n");
		}
		//cout << "m_up"<<m_up;
	}else{
		// in general, up vector is straight up
		m_up = up;//m_fliped*up;//roaming mode
		//m_up = m_up;//flight mode
		m_up.normalize();
	}
	//compute right vector in left handed system
	m_right = m_up.cross(m_backward);//m_forward.cross(m_up);//
	m_right.normalize();
	//re-calculate the orthonormal up vector.
	m_up = m_backward.cross(m_right);//m_left.cross(m_forward);//
	m_up.normalize();
	//Create the Viewing matrix using row major order,
	//which is slight different from the following link, where it use column major.
	//http://en.wikibooks.org/wiki/GLSL_Programming/Vertex_Transformations
	//Create inverse of viewing matrix, since its easier to specify manually.
	//V-1 = [ R | T ]
	//      [ --+-- ]    (R denotes 3x3 rotation matrix, whose columns are camera's right, up and backward vector.
	//      [ 0 | 1 ]    (T denotes 1x3 translation matrix, which is the position of the camera)
	//V = [ R^T | -R^T*T]
	//	  [ ----+-------]
	//	  [  0  |   1	]
	/*
	mat4 inv_view;
	inv_view.rows[0][0]=m_right[0];
	inv_view.rows[1][0]=m_right[1];
	inv_view.rows[2][0]=m_right[2];

	inv_view.rows[0][1]=m_up[0];
	inv_view.rows[1][1]=m_up[1];
	inv_view.rows[2][1]=m_up[2];

	inv_view.rows[0][2]=m_backward[0];
	inv_view.rows[1][2]=m_backward[1];
	inv_view.rows[2][2]=m_backward[2];

	inv_view.rows[0][3]=position[0];
	inv_view.rows[1][3]=position[1];
	inv_view.rows[2][3]=position[2];

	m_viewMtx = inv_view.invertEuclidean();
	*/
	//Another intuitive way to think about viewing transformation is to:
	//First, transform the the coordinates from object space to a reference frame I
	//where its x,y,z axis have the same orientation as those of object space respectively,
	//but its origin is shifted to the camera(eye) position. This transformation can be 
	//achieved by left-multiplying the following "translateMtx" to the vertex coordinates in object space.
	//Then, the transformation of the intermediate coordinates in reference frame I to the camera reference
	//frame can be considered as projecting the intermediate coordinate to the x,y,z axis
	//of the camera reference. It can be achieved by left-multiplying a matrix whose row vectors
	//correspond to the x,y and z axis of the camera reference frame.
	/*
	mat4 translateMtx(  1,      0,		0,  -position.x(),\
						0,      1,		0,  -position.y(),\
						0,	    0,		1,	-position.z(),\
						0,	    0,		0,	 1);
	mat4 world2EyeMtx(m_right.x(),    m_right.y(),	  m_right.z(),    0,\
					  m_up.x(),		  m_up.y(),	      m_up.z(),       0,\
					  m_backward.x(), m_backward.y(), m_backward.z(), 0,\
					  0,		  0,		0,						  1);
	m_viewMtx = world2EyeMtx * translateMtx;
	*/
	//a quick shortcut of the above implementation by simply writing down the matrix multiplication result. 
	m_viewMtx = mat4(m_right.x(),    m_right.y(),	  m_right.z(),    -(m_right.dot(position)),\
					  m_up.x(),		  m_up.y(),	      m_up.z(),       -(m_up.dot(position)),\
					  m_backward.x(), m_backward.y(), m_backward.z(), -(m_backward.dot(position)),\
					  0,		  0,		0,						  1);
}

mat4 GLCamera::setFrustum( float l, float r, float b, float t, float n, float f )
{
	m_projMtx = mat4::createPerpProjMatrix(l,r,b,t,n,f);
	return m_projMtx;
}

mat4 GLCamera::setFrustum( float fovY, float aspect, float front, float back )
{
	m_projMtx = mat4::createPerpProjMatrix(fovY, aspect, front, back);
	return m_projMtx;
}

mat4 GLCamera::setOrthoFrustum( float l, float r, float b, float t, float n, float f )
{
	m_projMtx = mat4::createOrthoProjMatrix(l,r,b,t,n,f);
	return m_projMtx;
}

void GLCamera::moveForward( float stepsize)
{
	m_posDest += (-m_backward*stepsize);
}

void GLCamera::moveRight( float stepsize)
{
	m_posDest += (m_right * stepsize);
}

void GLCamera::moveUp( float stepsize)
{
	m_posDest += (m_up * stepsize);
}

void GLCamera::forwardTime()
{
	//The camera current position is a function of time t.
	//It starts with the most recent value i.e. m_posLag and gradually
	//move to the m_posDest, as t -> infinity, m_posLag -> m_posDest.
	//the rate of the change satisfies the exponential decay(http://en.wikipedia.org/wiki/Exponential_decay)
	//in other words the distance between current position(m_pos) and destination
	//decreases at a rate proportional to its current distance value.
	//Let y be the current position, it follows the differential equation:
	// y'=-a*y+d*a, where a is the decay constant(corresponds to m_inertiaXYZ)
	//  d: is the destination position(corresponds to m_posDest)
	//An analytical solution of the above differential equation is:
	// y(t) = (y0-d)exp(-a*t)+d....(1), where y0 is the initial value( which is the initial value of m_posLag)
	// When t=0, y(0)=y0, which corresponds to the camera initial position.
	// as t->infinity, y(t)->d, which is the destination.
	// Instead of evaluating the formula (1) to compute y(t) at each time step,
	// an efficient computation of y(t) is to use Euler method(http://en.wikipedia.org/wiki/Euler_method)
	// to forward in time.
	// By the definition of derivative, y'=y(t+delta_t)-y(t)/(delta_t), and assume
	// delta_t=1 and using finite different method, it follows from formula (1) that
	// y(t+1)-y(t)=-a*y(t)+d*a <=> y(t+1)=(d-y(t))*a+y(t) <=>
	// m_posLag = (m_posDest-m_posLag)*m_inertiaXYZ + m_posLag.
	// Similar derivation applies to the camera viewing target.
	m_posLag += (m_posDest - m_posLag)*m_inertiaXYZ;
	m_targetSphericalAngleLag +=
					(m_targetSphericalAngleDest - m_targetSphericalAngleLag)*m_inertiaLookAt;
	float yt = sin(m_targetSphericalAngleLag[1]);
	float xt = cos(m_targetSphericalAngleLag[1])*sin(m_targetSphericalAngleLag[0]);//
	float zt = cos(m_targetSphericalAngleLag[1])*cos(m_targetSphericalAngleLag[0]);
	vec3f viewDirLag(xt,yt,zt);
	m_target = m_posLag + viewDirLag;

	if (m_bIsRoaming)
	{
		if (m_bLoop)
		{//enable loop. Once camera finished traveling the entire path, it will
			//restart again from the very begining.
			m_iStep = (m_iStep+1) % camera_path.getOutputPtArrayRef().size();
		}else{
			//or else stay at the end, waiting for user to exit to interactive mode.
			m_iStep = (GLuint)min(size_t(m_iStep+1),camera_path.getOutputPtArrayRef().size()-1);
		}
		setDestPosition(camera_path.getOutputPtArrayRef()[m_iStep]);
		setTranslateInertia(vec3f(1.0f));
		if (m_lookAtMode == FOCUS_OBJECT)
		{//focus on a object
			setLookAt(getCurrentPosition(), m_staringPos);

		}else if (m_lookAtMode == FOLLOW_PATH)
		{//look along the current tangent
			vec3f next_pos =
							camera_path.getOutputPtArrayRef()[(m_iStep+1)%camera_path.getOutputPtArrayRef().size()];
			setLookAt(getCurrentPosition(), next_pos);

		}else if (m_lookAtMode == FREE_FORM)
		{//using the interpolated camera viewing direction.
			setLookAt(getCurrentPosition(), getCurrentPosition()+getCameraDirs()[m_iStep]);
		}
	}else{
		setLookAt(getCurrentPosition(), getTarget());// , m_up);
	}
}

void GLCamera::forwardTime(GLuint step)
{

	m_posLag += (m_posDest - m_posLag)*m_inertiaXYZ;
	m_targetSphericalAngleLag +=
					(m_targetSphericalAngleDest - m_targetSphericalAngleLag)*m_inertiaLookAt;
	float yt = sin(m_targetSphericalAngleLag[1]);
	float xt = cos(m_targetSphericalAngleLag[1])*sin(m_targetSphericalAngleLag[0]);//
	float zt = cos(m_targetSphericalAngleLag[1])*cos(m_targetSphericalAngleLag[0]);
	vec3f viewDirLag(xt,yt,zt);
	m_target = m_posLag + viewDirLag;

	if (m_bIsRoaming)
	{
		//m_iStep = (m_iStep+1) % camera_path.getOutputPtArrayRef().size();
		setDestPosition(camera_path.getOutputPtArrayRef()[step]);
		setTranslateInertia(vec3f(1.0f));
		if (m_lookAtMode == FOCUS_OBJECT)
		{//focus on a object
			setLookAt(getCurrentPosition(), m_staringPos);

		}else if (m_lookAtMode == FOLLOW_PATH)
		{//look along the current tangent
			vec3f next_pos =
							camera_path.getOutputPtArrayRef()[(step+1)%camera_path.getOutputPtArrayRef().size()];
			setLookAt(getCurrentPosition(), next_pos);

		}else if (m_lookAtMode == FREE_FORM)
		{//using the interpolated camera viewing direction.
			vec3f& dir = getCameraDirs()[step];
			setLookAt(getCurrentPosition(), getCurrentPosition()+dir);
		}
	}else{
		setLookAt(getCurrentPosition(), getTarget());
	}
}
void GLCamera::mouseMotion( int x, int y,int mouseButton )
{
	if (mouseButton == GLUT_LEFT_BUTTON)
	{
	}else if (mouseButton == GLUT_MIDDLE_BUTTON)
	{
	}else if (mouseButton == GLUT_RIGHT_BUTTON)
	{
		/**Currently working solution
	//Spherical coordinates to control target.
	*/
		m_targetSphericalAngleDest[0] += ((m_prevX-x)*DEG2RAD)*0.5f;
		m_targetSphericalAngleDest[1] += ((m_prevY-y)*DEG2RAD)*0.5f;
	}
	m_prevX = x;
	m_prevY = y;
}

void GLCamera::mouseButton(int x, int y, int mouseButton)
{
	m_prevX = x; m_prevY = y;
}

void GLCamera::SpaceballMotion(int x, int y, int z)
{
	if (x || y || z)
	{
		davinci::vec3f e_dir(x, y, z);//direction in eye/camera space
		e_dir.normalize();
		float scalef = 1.0;
		vec3f curPos = getCurrentPosition();
		vec3f curDst = getDestPosition();
		//camera.setDestPosition(curDst + dir * scalef);
		//must use m_posLag instead of m_posDest, because spaceball, once touched,
		//would generate user input event at very high frequency. The windows 
		//event loop would keeps processing all these event before the 
		//rendering/display callback get executed causing m_posDest quickly blow 
		//up its value. On the contrary, m_posLag only get updated in the
		//rendering/display callback, thus, its value is not sensitive as that 
		//of m_posDest and grows more slowly.
		davinci::vec3f w_dir(0.0f);
		w_dir += m_right * e_dir.x();
		w_dir += m_up * e_dir.y();
		w_dir += m_backward * e_dir.z();

		m_posDest = (m_posLag + w_dir * scalef);
		//cout << "(curPos + dir * scalef)=" << (curPos + dir * scalef) << endl;
	}
}

void GLCamera::SpaceballRotation(int rx, int ry, int rz)
{
	/**Currently working solution
	//Spherical coordinates to control target.
	*/
	if (rx || ry || rz)
	{
		float scalef = 0.5f;
		m_targetSphericalAngleDest[0] = m_targetSphericalAngleLag[0] + ry * scalef * DEG2RAD;//((m_prevX - x)*DEG2RAD)*0.5f;
		m_targetSphericalAngleDest[1] = m_targetSphericalAngleLag[1] + rx * scalef * DEG2RAD;//((m_prevY - y)*DEG2RAD)*0.5f;
	}
}

mat4 GLCamera::rotated( float angle, const vec3f& axis )
{
	mat4 rotMtx;
	rotMtx = rotMtx.rotated(angle, axis);
	m_backward = rotMtx * m_backward;
	m_right  = rotMtx * m_right;
	m_up	= rotMtx * m_up;
	setLookAt(m_posDest, m_posDest-m_backward, m_up);
	return getViewingMatrix();
}

mat4 GLCamera::pitchd( float angle )//x
{
	return rotated(angle, m_right);
}

mat4 GLCamera::yawd( float angle )//y
{
	return rotated(angle, m_up);
}

mat4 GLCamera::rolld( float angle )
{
	return rotated(angle, m_backward);
}

mat4 GLCamera::rotate( float angle, const vec3f& axis )
{
	mat4 rotMtx;
	rotMtx = rotMtx.rotate(angle, axis);
	m_backward = rotMtx * m_backward;
	m_right = rotMtx * m_right;
	m_up	= rotMtx * m_up;
	setLookAt(m_posDest, m_posDest-m_backward, m_up);
	return getViewingMatrix();
}

davinci::mat4 GLCamera::rotate( const mat4& rotMtx )
{
	m_backward = rotMtx * m_backward;
	m_right = rotMtx * m_right;
	m_up	= rotMtx * m_up;
	setLookAt(m_posDest, m_posDest-m_backward, m_up);
	return getViewingMatrix();
}

mat4 GLCamera::pitch( float angle )
{
	return rotate(angle, m_right);
}

mat4 GLCamera::yaw( float angle )
{
	return rotate(angle, m_up);
}

mat4 GLCamera::roll( float angle )
{
	return rotate(angle, m_backward);
}

void GLCamera::addKeyframe()
{ 
	camera_path.addControlPt(getCurrentPosition());
	vec3f dir = (getTarget()-getCurrentPosition());
	dir.normalize();
	camera_keydirs.push_back(dir);
}

void GLCamera::generatePath()
{
	camera_path.createCurve();
	cout << "# of points on path="<<camera_path.getOutputPtArrayRef().size()<<endl;
	int res = getInterpRes();
	float delta_t = 1.0f/float(res+1);
	size_t nCtlPt = camera_path.getControlPtArrayRef().size();
	cout << "nCtlPt="<<nCtlPt<<endl;
	camera_interpDirs.clear();
	for (size_t i = 0 ; i < nCtlPt - 1; i++)
	{
		//for each interval [P_{i}, P_{i+1}]
		vec3f& dir1 = camera_keydirs[i];
		vec3f& dir2 = camera_keydirs[i+1];
		for (float t = 0 ; t <= 1.0f ; t += delta_t)
		{
			camera_interpDirs.push_back(slerp(dir1, dir2, t));
		}
	}
}

void GLCamera::save( const string& file, GLuint iomode/*=0*/ )
{
	ofstream ofs(file, (ios_base::openmode)iomode);
	if (!ofs)
	{
		string msg=file+" not exists.";
		GLError::ErrorMessage(msg);
	}

	camera_path.save(ofs, iomode);

	if(iomode == ios::binary)
	{
		GLuint size = (GLuint) camera_keydirs.size();
		ofs.write(reinterpret_cast<char*>(&size), sizeof(GLuint));
		ofs.write(reinterpret_cast<char*>((float*)(camera_keydirs.data()))
				  , size* sizeof(vec3f));

		size = (GLuint) camera_interpDirs.size();
		ofs.write(reinterpret_cast<char*>(&size), sizeof(GLuint));
		ofs.write(reinterpret_cast<char*>((float*)(camera_interpDirs.data()))
				  , size* sizeof(vec3f));

	}else
	{	//1. Save tangent knot array
		ofs << camera_keydirs.size() << "\n";
		for (int i = 0 ; i <  camera_keydirs.size() ; i++)
		{
			ofs << camera_keydirs[i];
		}
		//2. Save tangent vector array
		ofs << camera_interpDirs.size() << "\n";
		for (int i = 0 ; i <  camera_interpDirs.size() ; i++)
		{
			ofs << camera_interpDirs[i];
		}
	}
	ofs.close();
}

void GLCamera::load( const string& file, GLuint iomode/*=0*/ )
{
	ifstream ifs(file, ios::in | (ios_base::openmode) iomode);
	if (!ifs)
	{
		string msg=file+" not exists.";
		GLError::ErrorMessage(msg);
	}
	camera_path.load(ifs, iomode);

	camera_keydirs.clear();
	camera_interpDirs.clear();
	GLuint size=0;
	if (iomode == ios::binary)
	{
		ifs.read(reinterpret_cast<char*>(&size), sizeof(GLuint));
		camera_keydirs.resize(size);
		cout << "# of camera key dirs="<<size<<endl;
		ifs.read(reinterpret_cast<char*>(camera_keydirs.data())
				 ,sizeof(vec3f)*size);
		ifs.read(reinterpret_cast<char*>(&size), sizeof(GLuint));
		cout << "# of camera interp dirs="<<size<<endl;
		camera_interpDirs.resize(size);
		ifs.read(reinterpret_cast<char*>(camera_interpDirs.data())
				 ,sizeof(vec3f)*size);
	}
	else
	{
		//1. Load control points.
		ifs >> size;
		camera_keydirs.resize(size);
		for (GLuint i = 0 ; i < size; i++)
		{
			ifs >> camera_keydirs[i];
		}
		//2. Load points on curve.
		ifs >> size;
		camera_interpDirs.resize(size);
		for (GLuint i = 0 ; i < size; i++)
		{
			ifs >> camera_interpDirs[i];
		}
	}
	ifs.close();
}

void GLCamera::draw()
{
	camera_path.draw();
	std::vector<vec3f>& pathPts = camera_path.getOutputPtArrayRef();
	size_t nPts = pathPts.size();
	if (m_bDrawViewDirs)
	{
		glBegin(GL_LINES);
		for (size_t i=0 ; i < nPts ; i++ )
		{
			vec3f& start = pathPts[i];
			vec3f& dir = camera_interpDirs[i];
			vec3f end = start+dir*0.1f;
			glColor3f(1,0,0);
			glVertex3fv((float*)start);
			glVertex3fv((float*)end);
		}
		glEnd();
	}
}

void GLCamera::clearCameraPath()
{
	camera_path.clear();
	camera_keydirs.clear();
	camera_interpDirs.clear();
}

}//end of namespace.
