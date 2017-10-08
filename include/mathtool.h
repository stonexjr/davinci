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

#ifndef _MATHTOOL_H_
#define _MATHTOOL_H_
#include <stdint.h>
#include "vec2f.h"
#include "vec3f.h"
#include "vec4f.h"
#include "vec2i.h"
#include "vec3i.h"
#include "vec4i.h"
#include "vec2d.h"
#include "vec3d.h"
#include "vec4d.h"

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
namespace davinci{
#define SQUARE(x)\
    ((x)*(x))

	//id: rank of PE in current dimension
	//p: # of processor in current dimension
	//n: size of domain in current dimension
#define BLOCK_LOW(id, p, n) \
	((uint64_t)(((float)(id)) /(p) * (n)))

#define BLOCK_HIGH(id, p, n) \
	(BLOCK_LOW((id) + 1, p, n) - 1)

#define BLOCK_SIZE(id, p, n) \
	(BLOCK_LOW((id) + 1, p, n) - BLOCK_LOW(id, p, n))

	//id: rank of PE in current dimension
	//p: # of processor in current dimension
	//n: size of domain in current dimension
	//ss:: sampling step size
#define DOWNSAMPLE_BLOCK_LOW(id, p, n, ss)\
	 ( BLOCK_LOW((id),(p),(n)) + (ss) - 1 )/(ss)

#define DOWNSAMPLE_BLOCK_HIGH(id, p, n, ss)\
	 ( BLOCK_HIGH((id),(p),(n)) )/(ss)

#define DOWNSAMPLE_BLOCK_SIZE(id, p, n, ss) \
	(DOWNSAMPLE_BLOCK_LOW((id) + 1, p, n, (ss)) - DOWNSAMPLE_BLOCK_LOW(id, p, n, (ss)))

inline vec3i BLOCK_HIGH_3D(const vec3i &id3d, const vec3i &procDim, const vec3i &domainDim)
{
	return vec3i(
		BLOCK_HIGH(id3d.x(), procDim.x(), domainDim.x()),
		BLOCK_HIGH(id3d.y(), procDim.y(), domainDim.y()),
		BLOCK_HIGH(id3d.z(), procDim.z(), domainDim.z())
		);
}

inline vec3i BLOCK_LOW_3D(const vec3i &id3d, const vec3i &procDim, const vec3i &domainDim)
{
	return vec3i(
		BLOCK_LOW(id3d.x(), procDim.x(), domainDim.x()),
		BLOCK_LOW(id3d.y(), procDim.y(), domainDim.y()),
		BLOCK_LOW(id3d.z(), procDim.z(), domainDim.z())
		);
}

inline vec3i BLOCK_SIZE_3D(const vec3i &id3d, const vec3i &procDim, const vec3i &domainDim)
{
	return vec3i(
		BLOCK_SIZE(id3d.x(), procDim.x(), domainDim.x()),
		BLOCK_SIZE(id3d.y(), procDim.y(), domainDim.y()),
		BLOCK_SIZE(id3d.z(), procDim.z(), domainDim.z())
		);
}

inline vec3i DOWNSAMPLE_BLOCK_HIGH_3D(const vec3i &id3d, const vec3i &procDim, const vec3i &domainDim, const vec3i &ss)
{
	return vec3i(
		DOWNSAMPLE_BLOCK_HIGH(id3d.x(), procDim.x(), domainDim.x(), ss.x()),
		DOWNSAMPLE_BLOCK_HIGH(id3d.y(), procDim.y(), domainDim.y(), ss.y()),
		DOWNSAMPLE_BLOCK_HIGH(id3d.z(), procDim.z(), domainDim.z(), ss.z())
		);
}

inline vec3i DOWNSAMPLE_BLOCK_LOW_3D(const vec3i &id3d, const vec3i &procDim, const vec3i &domainDim, const vec3i &ss)
{
	return vec3i(
		DOWNSAMPLE_BLOCK_LOW(id3d.x(), procDim.x(), domainDim.x(), ss.x()),
		DOWNSAMPLE_BLOCK_LOW(id3d.y(), procDim.y(), domainDim.y(), ss.y()),
		DOWNSAMPLE_BLOCK_LOW(id3d.z(), procDim.z(), domainDim.z(), ss.z())
		);
}

inline vec3i DOWNSAMPLE_BLOCK_SIZE_3D(const vec3i &id3d, const vec3i &procDim, const vec3i &domainDim, const vec3i &ss)
{
	return vec3i(
		DOWNSAMPLE_BLOCK_SIZE(id3d.x(), procDim.x(), domainDim.x(), ss.x()),
		DOWNSAMPLE_BLOCK_SIZE(id3d.y(), procDim.y(), domainDim.y(), ss.y()),
		DOWNSAMPLE_BLOCK_SIZE(id3d.z(), procDim.z(), domainDim.z(), ss.z())
		);
}

#define BLOCK_LOW_FLOAT(id, p, n) \
    ((float)(((float)(id)) /(p) * (n)))

#define BLOCK_HIGH_FLOAT(id, p, n) \
    (BLOCK_LOW_FLOAT((id) + 1, p, n))

#define BLOCK_SIZE_FLOAT(id, p, n) \
    (BLOCK_LOW_FLOAT((id) + 1, p, n) - BLOCK_LOW_FLOAT(id, p, n))

inline vec3f BLOCK_HIGH_FLOAT3D(const vec3i &id3d,const vec3i &procDim,const vec3f &domainDim)
{
    return vec3f(
        BLOCK_HIGH_FLOAT(id3d.x(), procDim.x(), domainDim.x()),
        BLOCK_HIGH_FLOAT(id3d.y(), procDim.y(), domainDim.y()),
        BLOCK_HIGH_FLOAT(id3d.z(), procDim.z(), domainDim.z())
        );
}
inline vec3f BLOCK_LOW_FLOAT3D(const vec3i &id3d,const vec3i &procDim,const vec3f &domainDim)
{
    return vec3f(
        BLOCK_LOW_FLOAT(id3d.x(), procDim.x(), domainDim.x()),
        BLOCK_LOW_FLOAT(id3d.y(), procDim.y(), domainDim.y()),
        BLOCK_LOW_FLOAT(id3d.z(), procDim.z(), domainDim.z())
        );
}
inline vec3f BLOCK_SIZE_FLOAT3D(const vec3i &id3d,const vec3i &procDim,const vec3f &domainDim)
{
    return vec3f(
        BLOCK_SIZE_FLOAT(id3d.x(), procDim.x(), domainDim.x()),
        BLOCK_SIZE_FLOAT(id3d.y(), procDim.y(), domainDim.y()),
        BLOCK_SIZE_FLOAT(id3d.z(), procDim.z(), domainDim.z())
        );
}
//Return true if all components are none-zero
inline bool allTrue(const vec3f& v3){
    return (v3[0]!=0.0f && v3[1]!=0.0f && v3[2]!=0.0f);
}
inline bool allTrue(const vec3i& v3){
    return (v3[0] && v3[1] && v3[2]);
}
//Return true if any component is none-zero
inline bool anyTrue(const vec3f& v3){
    return (v3[0]!=0.0f || v3[1]!=0.0f || v3[2]!=0.0f);
}
inline bool anyTrue(const vec3i& v3){
    return (v3[0] || v3[1] || v3[2]);
}
inline float fminf( float a, float b )
{
    return a < b ? a : b;
}

inline float fmaxf(float a, float b)
{
    return a > b ? a : b;
}

inline int maxi(int a, int b)
{
    return a > b ? a : b;
}

inline int mini(int a, int b)
{
    return a < b ? a : b;
}
inline int sign(float a)
{
    return a > 0 ? 1 : ( a < 0 ? -1 : 0);
}

template <class T>
inline bool isnan(T a)
{
    return a!=a;
}

template <class T>
inline T clamp(const T& val, const T &minV, const T &maxV)
{
   return max(minV, min(val, maxV));
}

template<>
inline vec2f clamp<vec2f>(const vec2f &val, const vec2f &minV, const vec2f &maxV)
{
	return vec2f(
		clamp(val[0], minV[0], maxV[0]),
		clamp(val[1], minV[1], maxV[1]));
}

template<>
inline vec3f clamp<vec3f>(const vec3f &val, const vec3f &minV, const vec3f &maxV)
{
	return vec3f(
		clamp(val[0], minV[0], maxV[0]),
		clamp(val[1], minV[1], maxV[1]),
		clamp(val[2], minV[2], maxV[2]));
}

template<>
inline vec4f clamp<vec4f>(const vec4f &val, const vec4f &minV, const vec4f &maxV)
{
	return vec4f(
		clamp(val[0], minV[0], maxV[0]),
		clamp(val[1], minV[1], maxV[1]),
		clamp(val[2], minV[2], maxV[2]),
		clamp(val[3], minV[3], maxV[3])
		);
}

template<>
inline vec2i clamp<vec2i>(const vec2i &val, const vec2i &minV, const vec2i &maxV)
{
	return vec2i(
		clamp(val[0], minV[0], maxV[0]),
		clamp(val[1], minV[1], maxV[1]));
}

template<>
inline vec3i clamp<vec3i>(const vec3i &val, const vec3i &minV, const vec3i &maxV)
{
	return vec3i(
			clamp(val[0], minV[0], maxV[0]),
			clamp(val[1], minV[1], maxV[1]),
			clamp(val[2], minV[2], maxV[2]));
}

template<>
inline vec4i clamp<vec4i>(const vec4i &val, const vec4i &minV, const vec4i &maxV)
{
	return vec4i(
			clamp(val[0], minV[0], maxV[0]),
			clamp(val[1], minV[1], maxV[1]),
			clamp(val[2], minV[2], maxV[2]),
			clamp(val[3], minV[3], maxV[3])
			);
}

template<>
inline vec2d clamp<vec2d>(const vec2d &val, const vec2d &minV, const vec2d &maxV)
{
	return vec2d(
		clamp(val[0], minV[0], maxV[0]),
		clamp(val[1], minV[1], maxV[1]));
}

template<>
inline vec3d clamp<vec3d>(const vec3d &val, const vec3d &minV, const vec3d &maxV)
{
	return vec3d(
			clamp(val[0], minV[0], maxV[0]),
			clamp(val[1], minV[1], maxV[1]),
			clamp(val[2], minV[2], maxV[2]));
}

template<>
inline vec4d clamp<vec4d>(const vec4d &val, const vec4d &minV, const vec4d &maxV)
{
	return vec4d(
			clamp(val[0], minV[0], maxV[0]),
			clamp(val[1], minV[1], maxV[1]),
			clamp(val[2], minV[2], maxV[2]),
			clamp(val[3], minV[3], maxV[3])
			);
}
//Reference:http://graphics.stanford.edu/~seander/bithacks.html
//return sign of a, -1,0 or +1.
inline int _sign(float a)
{
    //return (a != 0) | (((int&)a) >> (sizeof(float)*CHAR_BIT-1));
    return (a > 0.0f) - (a < 0.0f);
}
inline int _sign(int a)
{
    //return (a != 0) | (a >> (sizeof(int)*CHAR_BIT-1));
    return (a > 0) - (a < 0);
}
//if a < 0 then 0, else 1
inline int nonnegative(int a)
{
    return 1^(((unsigned int)a) >> (sizeof(int)*CHAR_BIT-1) );
}
inline int nonnegative(float a)
{
    return 1^(((unsigned int&)a) >> (sizeof(float)*CHAR_BIT-1) );
}
//if x,y has opposite signs
inline bool opposigns(int x, int y){
    return (x^y)<0;
}
inline bool opposigns(float x, float y){
    return ((int&)x^(int&)y)<0;
}
//compute integer absolute value w/o branching.
inline int abs_wob(int v){
    int const mask = v >> (sizeof(int)*CHAR_BIT-1);
    return (v^mask)-mask;
}
inline float lerp(float a, float b, float t)
{
    return a + t*(b-a);
}
inline vec3f lerp(const vec3f &a, const vec3f &b, float t)
{
    return a + t*(b-a);
}
inline vec3f slerp(const vec3f& v1, const vec3f& v2, float t)
{
	float dot = v1.dot(v2);
	//Numerical error is finicky, the inner product of two normalized vectors 
	//that are parallel to each other is not guaranteed to be within [0,1],
	//clamping is required.
	dot = fminf(dot,1.0f);
	float theta = acosf(dot);

	if (abs(theta) < 1E-3)
	{//Degenerated case.
		return lerp(v1, v2, t);
	}else if (abs(theta-PI) < 1E-3)
	{//Degenerated case.
	  //if theta is close to PI, set it to 179 degree.
		theta = 3.124087f;//179 degree
	}
	float invSinTheta = 1.0f/sinf(theta);
	return invSinTheta*(sinf((1.0f-t)*theta)*v1 + sinf(t*theta)*v2);
}
//************************************************************************
// Method:    blerp
// Bilinearly interpolate the sampling value in a box.
//   /|\y
//    |
//   [2]---sx1----[3]
//    |     |      |
//    |    sy0     |
//    |     |      |
//   [0]---sx0----[1]--->x
// Returns:   interpolate value
// Parameter: const float v[4]: The scalar value on the eight vertices
// Parameter: const vec2f txy: The offset[0,1] of the sampling point with respect to vertex[0].
//************************************************************************
inline float blerp(const float v[4], const vec2f& txy)
{
    //linear interpolation along x axis
    float sx0 = v[0]*(1-txy.x()) + v[1]*txy.x();
    float sx1 = v[2]*(1-txy.x()) + v[3]*txy.x();
    //linear interpolation along y axis
    return sx0 * (1-txy.y()) + sx1 * txy.y();
}
inline float blerp(const vec2f& v01, const vec2f& v23, const vec2f& txy)
{
    //linear interpolation along x axis
    float sx0 = v01[0]*(1-txy.x()) + v01[1]*txy.x();
    float sx1 = v23[0]*(1-txy.x()) + v23[1]*txy.x();
    //linear interpolation along y axis
    return sx0 * (1-txy.y()) + sx1 * txy.y();
}

/*need vec2d
inline double blerp(const vec2d& v01, const vec2d& v23, const vec2d& txy)
{
    //linear interpolation along x axis
    float sx0 = v01[0]*(1-txy.x()) + v01[1]*txy.x();
    float sx1 = v23[2]*(1-txy.x()) + v23[3]*txy.x();
    //linear interpolation along y axis
    return sx0 * (1-txy.y()) + sx1 * txy.y();
}
*/
//************************************************************************
// Method:    tlerp
// Trilinearly interpolate the sampling value in a box.
//   /|\y
//    | [6]-----sx3----[7]
//    | /|		 |    / |
//    |/ | /z	sy1  /  |
//   [2]--/-sx1--|-[3]  |
//    |[4]---|--sx2-|--[5]
//    |  /	sy0	    |  /
//    | /	 |	    | /
//   [0]----sx0---[1]--->x
//vertex_pos[8]={{0,0,0},{1,0,0},{0,1,0},{1,1,0},
//				 {0,0,1},{1,0,1},{0,1,1},{1,1,1}}
// Returns:   interpolate value
// Parameter: const float v[8]: The scalar value on the eight vertices
// Parameter: const vec3f txyz: The offset[0,1] of the sampling point with respect to vertex[0].
//************************************************************************
inline float tlerp(const float v[8], const vec3f& txyz)
{
    float sx0 = v[0]*(1-txyz.x()) + v[1]*txyz.x();
    float sx1 = v[2]*(1-txyz.x()) + v[3]*txyz.x();
    float sx2 = v[4]*(1-txyz.x()) + v[5]*txyz.x();
    float sx3 = v[6]*(1-txyz.x()) + v[7]*txyz.x();
    //linear interpolation along y axis
    float sy0 = sx0 * (1-txyz.y()) + sx1 * txyz.y();
    float sy1 = sx2 * (1-txyz.y()) + sx3 * txyz.y();
    //linear interpolation along z axis
    return sy0 * (1-txyz.z()) + sy1 * txyz.z();//fabsf(sz0);
}
//Given lat/lon Coordinates lat(vec2f.x())/lon(vec2f.y()) in radian,
//Return corresponding Euclidian coordinates xyz on a sphere
//with radius r,
inline vec3f fromWGS84ToSphericalXYZ(float lat, float lon, float radius)
{
	float y = radius * sin(lat);//WGS84.y := latitude
	float x = radius * cos(lat) * sin(lon);//WGS84.x := longitude
	float z = radius * cos(lat) * cos(lon);
	return vec3f(x,y,z);
}
//Given lat/lon Coordinates lat(vec2d.x())/lon(vec2d.y()) in radian,
//Return corresponding Euclidian coordinates xyz on a sphere
//with radius r,
inline vec3d fromWGS84ToSphericalXYZ(double lat, double lon, float r)
{
	double y = r * sin(lat);//WGS84.y := latitude
	double x = r * cos(lat) * sin(lon);//WGS84.x := longitude
	double z = r * cos(lat) * cos(lon);
	return vec3d(x,y,z);
}
//Given Eucledian Coordinates xyz on a sphere
//Return corresponding lat(vec2f.x())/lon(vec2f.y()) coordinates in radian.
inline vec2f SphericalXYZtoWGS84(const vec3f& xyz){
	float r		= xyz.length();
	float theta = asin(xyz.y()/r);//[-pi/2, pi/2]
	float fi	= atan2f(xyz.x(), xyz.z());//[-pi,+pi]
	return vec2f(fi, theta);
}
//Given Eucledian Coordinates xyz on a sphere
//Return corresponding lat(vec2f.x())/lon(vec2f.y()) coordinates in radian.
inline vec2d SphericalXYZtoWGS84(const vec3d& xyz){
	double r	 = xyz.length();
	double theta = asin(xyz.y()/r);//[-pi/2, pi/2]
	double fi	 = atan2(xyz.x(), xyz.z());//[-pi,+pi]
	return vec2d(fi, theta);
}
// 
inline vec2f floor(const vec2f &v){
	return vec2f(::floor(v[0]), ::floor(v[1]));
}
inline vec3f floor(const vec3f &v){
	return vec3f(::floor(v[0]), ::floor(v[1]), ::floor(v[2]));
}
inline vec4f floor(const vec4f &v){
	return vec4f(::floor(v[0]), ::floor(v[1]), 
				 ::floor(v[2]), ::floor(v[3]));
}
inline vec2f ceil(const vec2f &v){
	return vec2f(::ceil(v[0]), ::ceil(v[1]));
}
inline vec3f ceil(const vec3f &v){
	return vec3f(::ceil(v[0]), ::ceil(v[1]), ::ceil(v[2]));
}
inline vec4f ceil(const vec4f &v){
	return vec4f(::ceil(v[0]), ::ceil(v[1]),
		::ceil(v[2]), ::ceil(v[3]));
}
//return the component-wise min
vec2f fminf(const vec2f &a,const vec2f &b);
vec3f fminf(const vec3f &a,const vec3f &b);
//return the component-wise max
vec2f fmaxf(const vec2f &a,const vec2f &b);
vec3f fmaxf(const vec3f &a,const vec3f &b);
//return the component-wise min
vec3i mini(const vec3i &a,const vec3i &b);
//return the component-wise max
vec3i maxi(const vec3i &a,const vec3i &b);
//return the component-wise min
vec3i minvi(const vec3i &a,const vec3i &b);
//return the component-wise max
vec3i maxvi(const vec3i &a,const vec3i &b);
int int_compare(const void *pa,const void *pb);

}
#endif
