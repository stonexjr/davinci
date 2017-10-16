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
//CREATED: 2013-11-25
//UPDATED: 2017-10-15
#ifndef _GL_CAMERA_H_
#define _GL_CAMERA_H_
#pragma once
#include <memory>
#include "vec3f.h"
#include "vec2f.h"
#include "mat4.h"
#include "GLCubicHermiteSpline.h"

namespace davinci{
//The GLCamera coordinate system is right handed which is in compliance with 
//the one used in OpenGL Programming Guide. Addison-Wesley
//http://www.hitl.washington.edu/artoolkit/documentation/cs.htm
//It has backward(Z), up(Y) and right(X) vectors as well as position to
//uniquely define the camera orientation and position.

class GLCamera
{
public:
	enum lookAtMode{
		FOCUS_OBJECT, FOLLOW_PATH, FREE_FORM
	} m_lookAtMode;
	GLuint m_iStep;//The index of the points on the generated camera path.
	bool m_bIsRoaming;//
	bool m_bLoop;//toggle if camera will auto restart roaming
	vec3f m_staringPos;
	bool getIsAutoRestart() const { return m_bLoop; }
	void setAutoRestart(bool val) { m_bLoop = val; }
	//staring pos is merely effective when lookAtMode is FOCUS_OBJECT.
	davinci::vec3f getStaringPos() const { return m_staringPos; }
	void setStaringPos(davinci::vec3f val) { m_staringPos = val; }
	//Set where camera should staring at during roaming.
	//FOCUS_OBJECT: Staring at a specified point.
	//FOLLOW_PATH: Look along the tangent of the roaming path.
	//FREE_FORM: Look at a sequence of directions interpolated 
	//using two nearest consecutive key frame.
	void setLookAtMode(lookAtMode mode){
		m_lookAtMode = mode;
	}
	lookAtMode getLookAtMode() const{ return m_lookAtMode;}
	GLuint getStep() const { return m_iStep; }
	void setStep(GLuint val) { m_iStep = val; }

	bool IsRoaming() const { return m_bIsRoaming; }
	void toggleRoaming(bool val);// { m_bIsRoaming = val; }
public:
	GLCamera(void);
	//Move the camera in the direction of forward by stepsize.
	void moveForward(float stepsize);
	void moveRight(float stepsize);
	void moveUp(float stepsize);
	void forwardTime();
	void forwardTime(GLuint step);
	//Response to mouse interaction
	//mouseButton: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
	//x,y: screen coordinates
	void mouseMotion(int x, int y, int mouseButton=0);
	void mouseButton(int x, int y, int mouseButton=0);
	void SpaceballMotion(int x, int y, int z);
	void SpaceballRotation(int x, int y, int z);
	//angle in degree.
	mat4 rotated(float angle, const vec3f& axis);
	mat4 pitchd(float angle);
	mat4 yawd(float angle);
	mat4 rolld(float angle);
	//angle in radian.
	mat4 rotate(float angle, const vec3f& axis);
	mat4 pitch(float angle);
	mat4 yaw(float angle);
	mat4 roll(float angle);
	//take a rotation matrix and return viewing matrix.
	mat4 rotate(const mat4& rotMtx);
	//Set camera look at parameters.
	//position: Where the camera is.
	//target: Where the camera is looking at.
	//up: Up vector of the camera. (0,1,0) by default
	virtual void setLookAt(const vec3f& position, const vec3f& target, 
						   const vec3f& up=vec3f(0.0f,1.0f,0.0f));
	//set viewing frustum and return projection matrix
	//Equivalent to glFrustum()
	mat4 setFrustum(float l, float r, float b, float t, float n, float f);
	//Equivalent to gluPerspective()
	mat4 setFrustum(float fovY, float aspect, float front, float back);
	//Equivalent to glOrtho()
	mat4 setOrthoFrustum(float l, float r, float b, float t, float n, float f);

	void setTranslateInertia(const vec3f& inertiaXYZ){ m_inertiaXYZ = inertiaXYZ;}
	void setRotationInertia(const float inertia){ m_inertiaLookAt = inertia;}
	void setDestPosition(const vec3f& pos){ m_posDest = pos;}
	vec3f getDestPosition()const { return m_posDest;}
	void  setCurrentPosition(const vec3f& pos){ m_posLag = pos;}
	vec3f getCurrentPosition() const { return m_posLag;}
	vec3f getBackward() const {return m_backward;}
	vec3f getUp() const { return m_up;}
	vec3f getRight() const { return m_right;}
	vec3f getTarget() const { return m_target;}

	mat4  getViewingMatrix()	const { return m_viewMtx;}
	mat4  getProjectionMatrix() const { return m_projMtx;}

	//status for camera motion interpolation.
	//Interpolation resolution, which determine how smooth the camera path will be.
	int   getInterpRes() const { return camera_path.getResolution();}
	void  setInterpRes(int n) { camera_path.setResolution(n);}
	void  addKeyframe();
	GLCubicHermiteSpline& getCameraPath() { return camera_path;}
	std::vector<vec3f>& getCameraDirs() { return camera_interpDirs;}
	void  generatePath();
	void  clearCameraPath();
	//Save camera path to designated files.
	//iomode: 0 : text format
	//ios::binary: binary format
	void save(const string& file, GLuint iomode=0);
	//Load camera path from designated files.
	//iomode: 0 : text format
	//ios::binary: binary format
	void load(const string& file, GLuint iomode=0);
	//visualize camera path
	void draw();
	void toggleDrawViewingDirs(bool val){ m_bDrawViewDirs = val;}
	void toggleDrawCtlPt(bool val){ camera_path.enableDrawCtlPt(val);}
	void toggleDrawTangentKnot(bool val){ camera_path.enableDrawTangentKnot(val);}

protected:
	bool m_bDrawViewDirs;
	GLCubicHermiteSpline camera_path;
	//camera viewing direction at key frames.
	std::vector<vec3f> camera_keydirs;
	//interpolated camera viewing directions.
	std::vector<vec3f> camera_interpDirs;
	float m_flipped;//if up has flipped
	vec3f m_posDest;//where camera is located.
	vec3f m_posLag;//lagged position of the camera
	vec3f m_inertiaXYZ;//inertia in x,y,z dimension.
	float m_inertiaLookAt;//inertia exerted on change of viewing target.
	vec3f m_backward;//To which direction the camera is looking at.
	vec3f m_target;//target
	//Spherical coordinates to control camera target vector.
	vec2f m_targetSphericalAngleDest;//(lon, lat)//(-90*DEG2RAD, 0);
	vec2f m_targetSphericalAngleLag;//(cameraTargetSphericalAngle);
	float m_prevX, m_prevY;
	vec3f m_up;//The orthonormal up vector
	vec3f m_right;//The left vector
	float m_speed;//How fast the camera moves.
	mat4  m_projMtx;//Projection matrix
	mat4  m_viewMtx;//Viewing matrix
};

typedef std::shared_ptr<GLCamera> GLCameraRef;

}//end of namespace lily
#endif

