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

#include "BBox2D.h"
#include "mathtool.h"
#include "constant.h"

namespace davinci{

BBox2D::BBox2D( void )
    :pMin(FLOAT_MAX),pMax(-FLOAT_MAX)
{
}

BBox2D::BBox2D( const vec2f& c, float r )
{
    pMin = c - r;
    pMax = c + r;
}

BBox2D::BBox2D( const vec2f& p1, const vec2f& p2 )
{
    pMin = davinci::fminf(p1,p2);
    pMax = davinci::fmaxf(p1,p2);
}

BBox2D BBox2D::Union( const vec2f& pt ) const
{
    BBox2D ret = *this;
    ret.pMin = davinci::fminf(pMin, pt);
    ret.pMax = davinci::fmaxf(pMax, pt);
    return ret;
}

BBox2D BBox2D::Union( const BBox2D& box ) const
{
    BBox2D ret = *this;
    ret.pMin = fminf(pMin, box.pMin);
    ret.pMax = fmaxf(pMax, box.pMax);
    return ret;
}

bool BBox2D::Overlap( const vec2f& p ) const
{
    bool x = (pMax.x() >= p.x()) && (pMin.x() <= p.x());
    bool y = (pMax.y() >= p.y()) && (pMin.y() <= p.y());
    return (x && y);
}

bool BBox2D::Overlap( const BBox2D& b ) const
{
    bool x = (pMax.x() >= b.pMin.x()) && (pMin.x() <= b.pMax.x());
    bool y = (pMax.y() >= b.pMin.y()) && (pMin.y() <= b.pMax.y());
    return (x && y);
}

bool BBox2D::Overlap( const BBox2D& b, float tolerance) const
{
    bool x = ((pMax.x() - b.pMin.x()) > tolerance ) && (tolerance < (b.pMax.x() - pMin.x()));
    bool y = ((pMax.y() - b.pMin.y()) > tolerance ) && (tolerance < (b.pMax.y() - pMin.y()));
    return (x && y);
}

bool BBox2D::Inside( const vec2f& pt) const
{
    return (pt.x() >= pMin.x() && pt.x() <= pMax.x() &&
            pt.y() >= pMin.y() && pt.y() <= pMax.y());
}

bool BBox2D::Inside( const BBox2D& bbox ) const
{
	return Inside(bbox.pMin) && Inside(bbox.pMax);
}

void BBox2D::Expand( float delta )
{
    pMax += delta;
    pMin -= delta;
}

float BBox2D::SurfaceArea() const
{
    vec2f d = pMax - pMin;
    return d.x() * d.y();
}

int BBox2D::MaximumExtent() const {
    vec2f diag = pMax - pMin;
    if (diag.x() > diag.y())
        return 0;
    else 
        return 1;
}

float BBox2D::MaximumExtentf() const {
    return max_extend2f(pMax, pMin);
}

vec2f BBox2D::LerpPt( float tx, float ty ) const
{
    return vec2f(lerp(pMin.x(), pMax.x(), tx),
                 lerp(pMin.y(), pMax.y(), ty));
}

vec2f BBox2D::Offset( const vec2f& p ) const
{
    return (p - pMin) / (pMax - pMin);
}

vec2f BBox2D::Center() const
{
    return 0.5f * ( pMax + pMin );
}

vec2f BBox2D::getDimension() const
{
    return (pMax - pMin);
}
}//end of namespace geometry