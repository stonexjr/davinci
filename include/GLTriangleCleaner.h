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

#ifndef _GL_TRIANGLE_CLEANER_H_
#define _GL_TRIANGLE_CLEANER_H_
#include "vec3i.h"
#include "vec3f.h"
#include <vector>

namespace davinci{
    template<class T>
    class VertexToTriangle{
    public:
        int triangleIdx;
        int vtxRank;//vertex rank that can be one of the values 0,1,2.
        T   coord;//vec2f, vec3f, vec4f
        VertexToTriangle(void) :triangleIdx(-1), vtxRank(-1), coord(0.0){};
        //c: vertex coordinates. could be 2D, 3D even 4D
        //vr: local vertex rank in the current triangle in question.
        VertexToTriangle(const T& c, const int vr, const int ti)
            : coord(c), vtxRank(vr), triangleIdx(ti){};

        friend int  compare(const void *pa, const void *pb);
        friend bool comparator(const VertexToTriangle<T>& a, const VertexToTriangle<T>& b);

    };

    template<class T>
    int compare(const void *pa, const void *pb)
    {
        VertexToTriangle<T> *a = (VertexToTriangle<T> *)pa;
        VertexToTriangle<T> *b = (VertexToTriangle<T> *)pb;

        if (a->coord == b->coord) {
            return 0;
        }
        else{
            if (a->coord < b->coord)
                return -1;
            else
                return 1;
        }

    }

    template<class T>
    bool comparator(const VertexToTriangle<T>& a, const VertexToTriangle<T>& b){
        return a.coord < b.coord;
    }

    template<class T>
    class GLTriangleCleaner{
         GLTriangleCleaner(void){};
        ~GLTriangleCleaner(){};
    public:
        //vertexArray: an array of triangle soup. Each triangle soup is merely an array of vertex coordinates(2D/3D/4D),
        //where three consecutive vertices defines a triangle.
        //vertexArrayUnique: output array that contains unique vertex coordinates of type T.
        //triangleMesh: output array that contains the connectivity information of triangle mesh, namely the index of triangle vertex into the vertexArrayUnique.
        static void createTriangleMeshFromTriangleSoup(const std::vector<std::vector<T> >& vertexArray,
            std::vector<T> &vertexArrayUnique,
            std::vector<vec3i>& triangleMesh);

        //Remove overlapping triangle from triangleMesh list.
        static void removeOverlappingTriangle(std::vector<vec3i> &triangleMesh);
    };

    template<class T>
    void davinci::GLTriangleCleaner<T>::removeOverlappingTriangle(std::vector<vec3i> &triangleMesh)
    {
        size_t nTriangleCount = triangleMesh.size();

        std::sort(triangleMesh.begin(), triangleMesh.end());

        int lastUniqueIndex = 0;
        for (int i = 1; i < nTriangleCount; i++)
        {
            vec3i curTriaVtxIdx = triangleMesh[i];
            if (curTriaVtxIdx != triangleMesh[lastUniqueIndex])
            {
                triangleMesh[++lastUniqueIndex] = curTriaVtxIdx;
            }
        }
        triangleMesh.resize(lastUniqueIndex + 1);
        cout << "Removed " << nTriangleCount - triangleMesh.size() << " overlapping triangles.\n";
    }

    template<class T>
    void GLTriangleCleaner<T>::createTriangleMeshFromTriangleSoup(const std::vector<std::vector<T> >& vertexArray,
        std::vector<T> &vertexArrayUnique,
        std::vector<vec3i>& triangleMesh)
    {
        cout << __func__ << endl;
        size_t vertexTotalCount = 0;
        size_t triangleTotalCount = 0;

        for (size_t i = 0; i < vertexArray.size(); i++)
        {
            size_t vertexCount = vertexArray[i].size();
            vertexTotalCount += vertexCount;
        }
        if (vertexTotalCount % 3 != 0)
        {
            cerr << "vertexTotalCount=" << vertexTotalCount << " is not multiple of 3!\n";
            exit(1);
        }
        triangleTotalCount = vertexTotalCount / 3;
        cout << "*****************************\n";
        cout << "Total vertex count: " << vertexTotalCount << endl;
        cout << "Total triangle count: " << triangleTotalCount << endl;

        std::vector<VertexToTriangle<T> > vertexToTriangleArray(vertexTotalCount);//reverse index from vertex to its instant triangle.
        //1.create triangle soup from all vertexArray pieces.
        size_t offsetTriangle = 0;

        //Build up reverse indexing from vertex to its instant triangle.
        for (size_t i = 0; i < vertexArray.size(); i++)
        {
            const std::vector<T>& curVertexSoupArray = vertexArray[i];
            if (curVertexSoupArray.size() % 3 != 0)
            {
                cerr << "vertexArray[" << i << "] is not valid triangle soup! Its count of vertex("
                    << curVertexSoupArray.size() << ") is not multiple of 3!\n";
                exit(1);
            }
            size_t curTriaCount = curVertexSoupArray.size() / 3;

            for (size_t j = 0; j < curTriaCount; j++)
            {//for each triangle of the ith piece.
                size_t curTriaGlobalIndex = j + offsetTriangle;
                const T& curVertices1 = curVertexSoupArray[3 * j + 0];
                const T& curVertices2 = curVertexSoupArray[3 * j + 1];
                const T& curVertices3 = curVertexSoupArray[3 * j + 2];
                VertexToTriangle<T>& curV2T1 = vertexToTriangleArray[3 * curTriaGlobalIndex];
                VertexToTriangle<T>& curV2T2 = vertexToTriangleArray[3 * curTriaGlobalIndex + 1];
                VertexToTriangle<T>& curV2T3 = vertexToTriangleArray[3 * curTriaGlobalIndex + 2];

                curV2T1.triangleIdx = (int)curTriaGlobalIndex;
                curV2T1.vtxRank = 0;
                curV2T1.coord = curVertices1;

                curV2T2.triangleIdx = (int)curTriaGlobalIndex;
                curV2T2.vtxRank = 1;
                curV2T2.coord = curVertices2;

                curV2T3.triangleIdx = (int)curTriaGlobalIndex;
                curV2T3.vtxRank = 2;
                curV2T3.coord = curVertices3;
            }

            offsetTriangle += curTriaCount;
        }
        //Make sure the # of vertex_triangle entries is multiple of 3.
        if (vertexToTriangleArray.size() % 3 != 0)
        {
            cerr << "The total number of VertexToTriangle entries is not multiple of 3!";
            exit(1);
        }

        triangleMesh.resize(triangleTotalCount);
        //2. Sort reverse index by vertex coordinates.
        std::sort(vertexToTriangleArray.begin(), vertexToTriangleArray.end(), comparator);

        //3. Remove duplicated vertex and update the corresponding vertex 
        //index of the triangle that shares the deleted vertex.
        int lastUniqueIndex = 0;
        VertexToTriangle<T> vtr = vertexToTriangleArray[lastUniqueIndex];
        davinci::vec3i& curTriangle = triangleMesh[vtr.triangleIdx];
        curTriangle[vtr.vtxRank] = lastUniqueIndex;

        vertexArrayUnique.resize(vertexTotalCount);

        //initialize verticeUniqueGlobal with the very first one, which is trivial.
        vertexArrayUnique[lastUniqueIndex] = vtr.coord;

        for (size_t i = 1; i < vertexTotalCount; i++)
        {//Scan the VertexToTriangle<T> array that may contains duplicates.
            vtr = vertexToTriangleArray[i];
            if (vtr.coord != vertexArrayUnique[lastUniqueIndex])
            {//found a new unique vertex.
                vertexArrayUnique[++lastUniqueIndex] = vtr.coord;
            }
            davinci::vec3i& curTriangle = triangleMesh[vtr.triangleIdx];

            curTriangle[vtr.vtxRank] = lastUniqueIndex;
        }

        vertexArrayUnique.resize(lastUniqueIndex + 1);
        cout << "# of unique vertex: " << vertexArrayUnique.size() << endl;
    }

}

#endif
