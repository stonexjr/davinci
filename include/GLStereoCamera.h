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
//CREATED: 2015-05-09
//UPDATED: 2015-05-09
#ifndef _GL_STEREO_CAMERA_H_
#define _GL_STEREO_CAMERA_H_
#include "GLCamera.h"

namespace davinci{
//Stereo Camera is just aggregation of two 
//GLCameras that match to left and right eye.
//
//A useful tutorial about stereoscopic projection
//http://paulbourke.net/exhibition/vpac/theory.html
//http://www.binocularity.org/page26.php
//
class GLStereoCamera : public GLCamera
{
public:
	typedef enum{MIDDLE_EYE, LEFT_EYE, RIGHT_EYE} EyeMode;
	typedef enum{OFFAXIS, TOEIN} FrustumMode;

	GLStereoCamera(void);
	//set interpupillary distance.
	void  setIPD(float d);// { m_eyeDistance = d; }
	float getIPD()const{ return m_IPD; }
	//Only take effect when the Camera frustum mode is TOEIN.
	void  setFocalDistance(float d);// { m_focalDistance = d; }
	float getFocalDistance()const{ return m_focalDistance; }
	vec3f getFocalPos()const{ return m_centerFocalPos; }
	//Will affect camera viewing matrix.
	void  setFrustumMode(FrustumMode v);// { m_frustumMode = v; }
	FrustumMode getFrustumMode()const{ return m_frustumMode; }

	virtual void setLookAt(const vec3f& position, const vec3f& target, const vec3f& up=vec3f(0.0f,1.0f,0.0f) );

	//Depending on eyemode(LEFT_EYE or RIGHT_EYE), return viewing matrix of left/right eye.
	mat4 getViewingMatrix(EyeMode eyemode);

protected:
	float m_IPD;//interpupillary distance.
	float m_focalDistance;
	vec3f m_curLeftEyePos, m_curRightEyePos;
	vec3f m_centerFocalPos;
	FrustumMode m_frustumMode;
	//
	GLCamera m_leftCamera;
	GLCamera m_rightCamera;
};

typedef std::shared_ptr<GLStereoCamera> GLStereoCameraRef;

}//end of namespace lily
#endif

