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
//UPDATED: 2015-05-09
#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include "GLStereoCamera.h"
//#include <GL/glu.h>
#include <GL/freeglut.h>
#include "constant.h"
#include "GLError.h"
#include "mathtool.h"

namespace davinci{

	GLStereoCamera::GLStereoCamera(void)
		:GLCamera()
		, m_IPD(0.02f), m_focalDistance(10.0f)
		, m_frustumMode(OFFAXIS)
	{
	}

	void GLStereoCamera::setIPD(float d)
	{
		m_IPD = d;
		setLookAt(m_posDest, m_posDest - m_backward, m_up);
	}

	void GLStereoCamera::setFocalDistance(float d)
	{
		m_focalDistance = d;
		setLookAt(m_posDest, m_posDest - m_backward, m_up);
	}

	void GLStereoCamera::setFrustumMode(FrustumMode v)
	{
		m_frustumMode = v;
		setLookAt(m_posDest, m_posDest - m_backward, m_up);
	}

	void GLStereoCamera::setLookAt(const vec3f& position, 
		const vec3f& target, const vec3f& up/* =vec3f(0.0f,1.0f,0.0f) */)
	{
		m_target = target;

		m_backward = position - target;
		m_backward.normalize();
		//Extra care to take when forward vector is on the Y axis.
		if (fabsf(m_backward.x()) < 0.00001f//FLOAT_EPSILON
			&& fabsf(m_backward.z()) < 0.00001f)//FLOAT_EPSILON)
		{
			m_flipped *= -1.0f;
			if (m_backward.y() > 0)//looking upward
			{
				m_up = vec3f(0.0f, 0.0f, 1.0f);
			}
			else//looking downward
			{
				m_up = vec3f(0.0f, 0.0f, -1.0f);
			}
		}else{
			// in general, up vector is straight up
			m_up = up;//m_fliped*up;//roaming mode
			//m_up = m_up;//flight mode
			m_up.normalize();
		}
		//compute right vector in left handed system
		m_right = m_up.cross(m_backward);
		m_right.normalize();
		//re-calculate the orthonormal up vector.
		m_up = m_backward.cross(m_right);
		m_up.normalize();

		m_centerFocalPos = (m_backward * (-m_focalDistance) + position);

		vec3f leftFocalPos, rightFocalPos;
		if (m_frustumMode == TOEIN)
		{
			leftFocalPos  = m_centerFocalPos;
			rightFocalPos = m_centerFocalPos;
		}else{
			leftFocalPos  = m_curLeftEyePos  + (m_backward * (-m_focalDistance));
			rightFocalPos = m_curRightEyePos + (m_backward * (-m_focalDistance));
		}

		m_curLeftEyePos  = position - (m_right*(m_IPD*0.5f));
		m_curRightEyePos = position + (m_right*(m_IPD*0.5f));

		m_leftCamera.setLookAt(m_curLeftEyePos,   leftFocalPos, m_up);
		m_rightCamera.setLookAt(m_curRightEyePos, rightFocalPos, m_up);

	}
	
	davinci::mat4 GLStereoCamera::getViewingMatrix(EyeMode eyemode)
	{
		if (eyemode==LEFT_EYE)
		{
			return m_leftCamera.getViewingMatrix();
		}else if (eyemode==RIGHT_EYE)
		{
			return m_rightCamera.getViewingMatrix();
		}else if (eyemode==MIDDLE_EYE)
		{
			return m_viewMtx;
		}else{
			cerr << "Invalid eyemode=" << (int)eyemode << " given.\n";
			return mat4();
		}
	}

	/*
	void GLStereoCamera::setLookAtOffAxis(const vec3f& position,
		const vec3f& target, const vec3f& up)
	{
		m_target = target;

		m_backward = position - target;
		m_backward.normalize();
		//Extra care to take when forward vector is on the Y axis.
		if (fabsf(m_backward.x()) < 0.00001f//FLOAT_EPSILON
			&& fabsf(m_backward.z()) < 0.00001f)//FLOAT_EPSILON)
		{
			m_flipped *= -1.0f;
			if (m_backward.y() > 0)//looking upward
			{
				m_up = vec3f(0.0f, 0.0f, 1.0f);
			}
			else//looking downward
			{
				m_up = vec3f(0.0f, 0.0f, -1.0f);
			}
		}
		else{
			// in general, up vector is straight up
			m_up = up;//m_fliped*up;//roaming mode
			//m_up = m_up;//flight mode
			m_up.normalize();
		}
		//compute right vector in left handed system
		m_right = m_up.cross(m_backward);
		m_right.normalize();
		//re-calculate the orthonormal up vector.
		m_up = m_backward.cross(m_right);
		m_up.normalize();

		m_centerFocalPos = (m_backward * (-m_focalDistance) + position);
		m_curLeftEyePos  = position - (m_right*(m_IPD*0.5f));
		m_curRightEyePos = position + (m_right*(m_IPD*0.5f));
		vec3f leftFocalPos  = m_curLeftEyePos  + (m_backward * (-m_focalDistance));
		vec3f rightFocalPos = m_curRightEyePos + (m_backward * (-m_focalDistance));

		m_leftCamera.setLookAt(m_curLeftEyePos,   leftFocalPos,  m_up);
		m_rightCamera.setLookAt(m_curRightEyePos, rightFocalPos, m_up);

	}
	*/

	/*
	lily::mat4 GLStereoCamera::setFrustum(float l, float r, float b, float t, float n, float f)
	{
		m_l = l; m_r = r; m_b = b; m_t = t; m_n = n; m_f = f;
		if (m_frustumMode==OFFAXIS)
		{
			return setFrustumOffAxis(l, r, b, t, n, f);
		}else if (m_frustumMode==TOEIN)
		{
			return GLCamera::setFrustum(l, r, b, t, n, f);
		}else{
			cerr <<__func__<< "Unknown frustum mode:" << (int)m_frustumMode << endl;
		}
	}

	lily::mat4 GLStereoCamera::setFrustum(float fovY, float aspect, float n, float f)
	{
		m_fov = fovY;
		m_aspect = aspect;
		m_n = n;
		m_f = f;
		if (m_frustumMode==OFFAXIS)
		{
			return setFrustumOffAxis(fovY, aspect, n, f);
		}else if (m_frustumMode==TOEIN)
		{
			float tangent = tanf(fovY*0.5f * DEG2RAD);
			float height = n * tangent;//half height of near plane
			float width = height * aspect;//half width of near plane
			m_l = -width; m_r = width;
			m_b = -height; m_t = height;
			m_n = n; m_f = f;
			m_middlePrjMtx = GLCamera::setFrustum(fovY, aspect, n, f);
			return m_middlePrjMtx;
		}else{
			cerr <<__func__<< "Unknown frustum mode:" << (int)m_frustumMode << endl;
		}
	}

	lily::mat4 GLStereoCamera::setFrustumOffAxis(float l, float r, float b, float t, float n, float f)
	{
		float offset = 0.0f;// m_IPD*0.5f;
		m_leftPrjMtx = m_leftCamera.setFrustum(l + offset, r + offset, b, t, n, f);
		m_rightPrjMtx= m_rightCamera.setFrustum(l - offset, r - offset, b, t, n, f);
		m_middlePrjMtx = GLCamera::setFrustum(l, r, b, t, n, f);
		return m_middlePrjMtx;
	}

	lily::mat4 GLStereoCamera::setFrustumOffAxis(float fovY, float aspect, float front, float back)
	{
		float tangent = tanf(fovY*0.5f * DEG2RAD);
		float height = front * tangent;//half height of near plane
		float width = height * aspect;//half width of near plane
		m_l = -width; m_r = width;
		m_b = -height; m_t = height;
		m_n = front; m_f = back;
		return GLStereoCamera::setFrustum(-width, width, -height, height, front, back);
	}

	lily::mat4 GLStereoCamera::getProjectionMatrix(EyeMode eyemode)
	{
		if (m_frustumMode == TOEIN)
		{
			return m_middlePrjMtx;
		}else if (m_frustumMode == OFFAXIS)
		{
			if (eyemode == LEFT_EYE)
			{
				return m_leftPrjMtx;
			}
			else if (eyemode == RIGHT_EYE)
			{
				return m_rightPrjMtx;
			}
			else if (eyemode == MIDDLE_EYE)
			{
				return m_middlePrjMtx;
			}
			else{
				cerr << "Invalid eyemode=" << (int)eyemode << " given.\n";
				return mat4();
			}
		}
	}
	*/

}
//end of namespace.
