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

#ifndef _BBOX2D_H_
#define _BBOX2D_H_

#pragma once
#include "vec2f.h"
#include "constant.h"

namespace davinci{

class BBox2D{
public:
    vec2f pMin;
    vec2f pMax;
    //float  m_dist
    BBox2D(void);
    BBox2D(const vec2f &p) : pMin(p), pMax(p) { }
    //c: center of the bbox,
    //r: radius of the bbox.
    BBox2D(const vec2f& c, float r);
    //lower and upper corner of the bbox.
    BBox2D(const vec2f& p1, const vec2f& p2);
    BBox2D Union(const vec2f& pt) const;
    BBox2D Union(const BBox2D& box) const;
    vec2f getDimension() const;
    bool Overlap(const vec2f& p ) const;
    bool Overlap(const BBox2D& box) const;
    bool Overlap(const BBox2D& box, float tolerance) const;
    //if a point is inside the box.
    bool Inside(const vec2f& pt) const;
    bool Inside(const BBox2D& bbox) const;
    void Expand(float delta);
    float SurfaceArea() const;
    int   MaximumExtent()  const;
    float MaximumExtentf() const;
    vec2f LerpPt(float tx, float ty) const;
    //returns the position of a point relative to the corners of the box,
    //where a point at the minimum corner has offset (0, 0, 0), a point at the maximum corner
    //has offset (1, 1, 1).
    vec2f Offset(const vec2f& p) const;
    vec2f Center() const;
    //Radius() only make sense when the box is assumed as cube.
    float BoxRadius() const
    {
        return MaximumExtentf()*0.5f;
    }

    float SphericalRadius() const
    {
        return BoxRadius() * SQRT_2;
    }

    friend BBox2D& operator << (BBox2D& bbox, const vec2f& v)
    {
        bbox = bbox.Union(v);
        return bbox;
    }

    friend BBox2D& operator << (BBox2D& bbox, const BBox2D& bbox2)
    {
        bbox = bbox.Union(bbox2);
        return bbox;
    }

    friend std::ostream& operator << (std::ostream& os,const BBox2D& b)
    {
        os<<"["<<b.pMin<<", "<<b.pMax<<"]\n";
        return os;
    }

};

};//end of namespace
#endif

