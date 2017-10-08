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

#include "GLTrackBall.h"
#include <math.h>
#include <GLCamera.h>
#include "GLQuaternion.h"
using namespace davinci;
/*
 * This size should really be based on the distance from the center of
 * rotation to the point on the object underneath the mouse.  That
 * point would then track the mouse as closely as possible.  This is a
 * simple example, though, so that is left as an Exercise for the
 * Programmer.
 */
#define TRACKBALLSIZE  (0.8)

/*
 * Local function prototypes (not defined in trackball.h)
 *
 * Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
float GLTrackBall::tb_project_to_sphere(float r, float x, float y)
{
    float d, t, z;

    d = sqrt(x*x + y*y);
    if (d < r * 0.70710678118654752440f) {    /* Inside sphere */
        z = sqrt(r*r - d*d);
    } else {           /* On hyperbola */
        t = r / 1.41421356237309504880f;
        z = t*t / d;
    }
    return z;
}

GLTrackBall::GLTrackBall(void)
{
}

GLTrackBall::~GLTrackBall(void)
{
}
/*
 * Simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
mat4 GLTrackBall::trackball( Quaternion &q, float p1x, float p1y, 
                             float p2x, float p2y,const mat4 inv_view/*=identity()*/)
{
    mat4 rotMtx;
    if (p1x == p2x && p1y == p2y) {
        // Zero rotation, simply return identity quaternion.
        q.setVector(vec3f(0.0f));
        q.setW(1.0f);
        return rotMtx;
    }
    vec3f p1 = vec3f(p1x, p1y, tb_project_to_sphere((float)(TRACKBALLSIZE),p1x,p1y) );
    vec3f p2 = vec3f(p2x, p2y, tb_project_to_sphere((float)(TRACKBALLSIZE),p2x,p2y) );
  //vec3f axis = p1.crose(p2);//generated matrix is row major.
    vec3f axis = p2.cross(p1);//generated matrix is column major.
    //The rotation axis is computed in the camera space, while the object
    //rotation is performed in world space. We need to transform the 
    //rotation axis from camera space back to world space by multiply axis
    //with inverse of viewing matrix.
    axis = inv_view * axis;
    //Figure out how much to rotate around that axis.
    p1.normalize(); p2.normalize();
    float phi = acosf(p1.dot(p2));
    q = Quaternion::fromAxisAndAngle(axis,phi);
    q.normalized();

    return rotMtx.rotate(phi, axis);
}

mat4 GLTrackBall::trackball( Quaternion &q, float p1x, float p1y, float p2x,
                             float p2y, float scrWidth, float scrHeight,
                             const mat4 inv_view/*=identity()*/ )
{
    return GLTrackBall::trackball(q,
            (2.0f*p1x - scrWidth)  / scrWidth,
            (scrHeight - 2.0f*p1y) / scrHeight,
            (2.0f*p2x - scrWidth)  / scrWidth,
            (scrHeight - 2.0f*p2y) / scrHeight, inv_view);
}

mat4 GLTrackBall::trackball(Quaternion &q, float p1x,
        float p1y, float p2x, float p2y, const GLCamera *camera/*=NULL*/ )
{
    if (!camera)
    {
        return trackball(q, p1x, p1y, p2x, p2y, mat4());
    }else{
        return trackball(q, p1x, p1y, p2x, p2y,
                         camera->getViewingMatrix().invertEuclidean());
    }
}

mat4 GLTrackBall::trackball(Quaternion &q, float p1x,
    float p1y, float p2x, float p2y, float scrWidth, float scrHeight,
    const GLCamera *camera/*=NULL*/ )
{
    if (!camera)
    {
        return trackball(q, p1x, p1y, p2x, p2y, scrWidth, scrHeight, mat4());
    }else{
        return trackball(q, p1x, p1y, p2x, p2y, scrWidth, scrHeight,
                         camera->getViewingMatrix().invertEuclidean());
    }
}

void GLTrackBall::buildGLRotMatrix( float m[4][4], Quaternion &q )
{   //0:x, 1:y, 2:z, 3:w
    m[0][0] = 1.0f - 2.0f * (q.y()*q.y() + q.z() * q.z());
    m[0][1] = 2.0f * (q.x() * q.y() - q.z() * q.w());
    m[0][2] = 2.0f * (q.z() * q.x() + q.y() * q.w());
    m[0][3] = 0.0f;

    m[1][0] = 2.0f * (q.x() * q.y() + q.z() * q.w());
    m[1][1] = 1.0f - 2.0f * (q.z() * q.z() + q.x() * q.x());
    m[1][2] = 2.0f * (q.y() * q.z() - q.x() * q.w());
    m[1][3] = 0.0f;

    m[2][0] = 2.0f * (q.z() * q.x() - q.y() * q.w());
    m[2][1] = 2.0f * (q.y() * q.z() + q.x() * q.w());
    m[2][2] = 1.0f - 2.0f * (q.y() * q.y() + q.x() * q.x());
    m[2][3] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

void davinci::GLTrackBall::buildGLRotMatrix( mat4 &mtx, Quaternion &q, bool colMajor/*=true*/ )
{
    if (colMajor)
    {
        mtx[0][0] = 1.0f - 2.0f * (q.y()*q.y() + q.z() * q.z());
        mtx[0][1] = 2.0f * (q.x() * q.y() - q.z() * q.w());
        mtx[0][2] = 2.0f * (q.z() * q.x() + q.y() * q.w());
        mtx[0][3] = 0.0f;

        mtx[1][0] = 2.0f * (q.x() * q.y() + q.z() * q.w());
        mtx[1][1] = 1.0f - 2.0f * (q.z() * q.z() + q.x() * q.x());
        mtx[1][2] = 2.0f * (q.y() * q.z() - q.x() * q.w());
        mtx[1][3] = 0.0f;

        mtx[2][0] = 2.0f * (q.z() * q.x() - q.y() * q.w());
        mtx[2][1] = 2.0f * (q.y() * q.z() + q.x() * q.w());
        mtx[2][2] = 1.0f - 2.0f * (q.y() * q.y() + q.x() * q.x());
        mtx[2][3] = 0.0f;

        mtx[3][0] = 0.0f;
        mtx[3][1] = 0.0f;
        mtx[3][2] = 0.0f;
        mtx[3][3] = 1.0f;
    }else{
        mtx[0][0] = 1.0f - 2.0f * (q.y()*q.y() + q.z() * q.z());
        mtx[1][0] = 2.0f * (q.x() * q.y() - q.z() * q.w());
        mtx[2][0] = 2.0f * (q.z() * q.x() + q.y() * q.w());
        mtx[3][0] = 0.0f;

        mtx[0][1] = 2.0f * (q.x() * q.y() + q.z() * q.w());
        mtx[1][1] = 1.0f - 2.0f * (q.z() * q.z() + q.x() * q.x());
        mtx[2][1] = 2.0f * (q.y() * q.z() - q.x() * q.w());
        mtx[3][1] = 0.0f;

        mtx[0][2] = 2.0f * (q.z() * q.x() - q.y() * q.w());
        mtx[1][2] = 2.0f * (q.y() * q.z() + q.x() * q.w());
        mtx[2][2] = 1.0f - 2.0f * (q.y() * q.y() + q.x() * q.x());
        mtx[3][2] = 0.0f;

        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[3][3] = 1.0f;
    }

}
