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

#include "GLTriangle.h"
#include "constant.h"
#include "mathtool.h"
#include "vec3d.h"
using namespace davinci;

bool GLTriangle::point_in_triangle_barycentric(const vec3f &P) const
{
    float u,vv, invDenom, dot00,dot01,dot02,dot11,dot12;
    vec3f v0 = v[2] - v[0];
    vec3f v1 = v[1] - v[0];
    vec3f v2 =    P - v[0];
    dot00 = v0.dot(v0);
    dot01 = v0.dot(v1);
    dot02 = v0.dot(v2);
    dot11 = v1.dot(v1);
    dot12 = v1.dot(v2);
    invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    u  = (dot11 * dot02 - dot01 * dot12) * invDenom;
    vv = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (vv >= 0) && (u + vv < 1);
}

bool GLTriangle::point_in_triangle_barycentric_double(const vec3f &Pf) const
{
    double u,vv, invDenom, dot00,dot01,dot02,dot11,dot12;
    vec3d P(Pf);
    vec3d v0(v[2]-v[0]);
    vec3d v1(v[1]-v[0]);
    vec3d v2(P-vec3d(v[0]));

    dot00 = v0.dot(v0);
    dot01 = v0.dot(v1);
    dot02 = v0.dot(v2);
    dot11 = v1.dot(v1);
    dot12 = v1.dot(v2);
    double ddot1= dot00 * dot11 , ddot2=dot01 * dot01;
    double denom= ddot1 - ddot2;
    invDenom = 1.0 / (denom);
    double prod1 = dot11 * dot02, prod2= dot01 * dot12;
    double drod1 =dot00 * dot12, drod2=dot01 * dot02; 
    u  = (prod1 - prod2) * invDenom;
    vv = (drod1 - drod2) * invDenom;

    return (u >= 0.0) && (vv >= 0.0) && (u + vv < 1.0);
}

float GLTriangle::vertex_triangle_distance(const vec3f &v0) const
{
    float l,m,n, t, X, Y, Z, dist, min_dist=FLOAT_MAX;
    const vec3f &v1 = v[0];
    const vec3f &v2 = v[1];
    const vec3f &v3 = v[2];
    vec3f v0_1, v1_0, v2_0, v1_2;
    l = (v2.y()-v1.y())*(v3.z()-v1.z())-(v3.y()-v1.y())*(v2.z()-v1.z());
    m = (v2.z()-v1.z())*(v3.x()-v1.x())-(v3.z()-v1.z())*(v2.x()-v1.x());
    n = (v2.x()-v1.x())*(v3.y()-v1.y())-(v3.x()-v1.x())*(v2.y()-v1.y());

    v1_0 = v1 - v0;
    v0_1 = v0 - v1;
    t = (l*v1_0.x()+m*v1_0.y()+n*v1_0.z())/(l*l+m*m+n*n);
    X = v0.x() + t*l;
    Y = v0.y() + t*m;
    Z = v0.z() + t*n;
    vec3f Q(X, Y, Z);
    vec3f avg_normal = (vn[0]+vn[1]+vn[2])/3.0f;
    vec3f center = (v[0]+v[1]+v[2])/3.0f;
    float dot_product = (center-v0).dot(avg_normal);
    int isign = sign(dot_product);
    if(point_in_triangle_barycentric_double(Q))
    {
        t = normal.dot(v1_0);
        t = fabsf(t);
        return t;
    }

    for (int i = 0; i < 3 ; i++)
    {
        const vec3f &v1 = v[i];
        const vec3f &v2 = v[(i+1)%3];
        v1_0 = v1 - v0;
        v1_2 = v1 - v2;

        t = v1_0.dot(v1_2)/v1_2.lengthSquared();

        if (0 <= t && t <= 1){
            Q = vec3f(v1.x() + t*(-v1_2.x()),
                v1.y() + t*(-v1_2.y()),
                v1.z() + t*(-v1_2.z()));

            dist = (Q-v0).length();
        }
        else if (t < 0){
            dist = v1_0.length();
        }else if (t > 1){
            v2_0 = v2 - v0;
            dist = v2_0.length();
        }

        if (min_dist > dist){
            min_dist = dist;
        }
    }
    return min_dist;
}

bool davinci::GLTriangle::triangle_overlap_sphere( davinci::vec3f &pt_center, float radius ) const
{
	//compute triangle center
	//simple schema
	davinci::vec3f triangle_center = (v[0]+v[1]+v[2])/3.0f;
	float dist = (pt_center - triangle_center).length();
	//radius of minimum sphere that circumscribes the triangle.
	float triangle_radius = (v[0]-triangle_center).length();
	if (dist < (triangle_radius+radius))
	{
		float dist_precise = fabsf(vertex_triangle_distance(pt_center));
		return dist_precise < radius;
	}else{
		return false;
	}
}

