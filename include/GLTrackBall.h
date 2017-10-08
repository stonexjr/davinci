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

#ifndef _GLTRACKBALL_H_
#define _GLTRACKBALL_H_
#pragma once
#include "GLQuaternion.h"
#include "mat4.h"

namespace davinci{

	class GLCamera;
	class GLTrackBall
	{
		public:
			 GLTrackBall(void);
			~GLTrackBall(void);
		//Pass the x and y coordinates of the last and current positions of
		//the mouse, scaled to (-1.0 ... 1.0).
		//The resulting rotation is returned as both a quaternion rotation in the
		//first parameter and a column major ordered mat4(which can be directly 
		//fed into OpenGL matrix stack using glLoadMatrixf(). User can use either of
		//them at their disposal.     
		//inv_view: inverse of viewing matrix.
		//The rotation axis is computed in the camera space, while the object
		//rotation is performed in world space. We need to transform the 
		//rotation axis from camera space back to world space by multiplying 
		//rotation axis with inverse of viewing matrix.
		static mat4 trackball(Quaternion &q, float p1x, float p1y,
							  float p2x, float p2y, const mat4 inv_view=mat4());

		static mat4 trackball(Quaternion &q, float p1x, float p1y,
							  float p2x, float p2y, const GLCamera *camera=NULL);

		//Pass the x and y coordinates of the last(p1x,p1y) and current(p2x,p2y)
		//positions of the mouse in screen coordinates as well as the size of the screen.
		//The resulting rotation is returned as both a quaternion rotation in the
		//first parameter and a column major ordered mat4(which can be directly 
		//fed into OpenGL matrix stack using glLoadMatrixf(). User can use either of
		//at their disposal.
		//inv_view: inverse of viewing matrix.
		//The rotation axis is computed in the camera space, while the object
		//rotation is performed in world space. We need to transform the 
		//rotation axis from camera space back to world space by multiplying
		//rotation axis with inverse of viewing matrix.
		static mat4 trackball(Quaternion &q, float p1x, float p1y, float p2x, float p2y,
							  float scrWidth, float scrHeight,const mat4 inv_view=mat4());

		static mat4 trackball(Quaternion &q, float p1x, float p1y, float p2x, float p2y,
							  float scrWidth, float scrHeight,const GLCamera *camera=NULL);
		/*
		 * Given two quaternions, add them together to get a third quaternion.
		 * Adding quaternions to get a compound rotation is analogous to adding
		 * translations to get a compound translation.  When incrementally
		 * adding rotations, the first argument here should be the new
		 * rotation, the second and third the total rotation (which will be
		 * over-written with the resulting new total rotation).
		 */
		static void add_quats(float *q1, float *q2, float *dest);
		/*
		 * Build a rotation matrix, given a quaternion rotation.
		 * A useful function, builds a rotation matrix in 4x4 matrix based on
		 * given quaternion.
		 * Note: the return matrix is column major order, which complies with
		 * OpenGL build-in matrix order.
		 */
		static void buildGLRotMatrix(float m[4][4], Quaternion &q);
		static void buildGLRotMatrix(mat4 &mtx, Quaternion &q, bool colMajor=true);
		/*
		 *
		 *Given an axis and angle, compute quaternion.
		 *
		 * This function computes a quaternion based on an axis (defined by
		 * the given vector) and an angle about which to rotate.  The angle is
		 * expressed in radians.  The result is put into the third argument.
		 */
		static float tb_project_to_sphere(float r, float x, float y);
		//static void axis_to_quat(float a[3], float phi, float q[4]);
	};
}
#endif