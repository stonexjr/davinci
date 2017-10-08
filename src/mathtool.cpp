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

#include "mathtool.h"
namespace davinci{

vec2f fminf(const vec2f &a,const vec2f &b )
{
    return vec2f(fminf(a.x(),b.x()),
                 fminf(a.y(),b.y()));
}

vec2f fmaxf(const vec2f &a,const vec2f &b )
{
    return vec2f(fmaxf(a.x(),b.x()),
                 fmaxf(a.y(),b.y()));
}

vec3f fminf(const vec3f &a,const vec3f &b )
{
    return vec3f(fminf(a.x(),b.x()),
                 fminf(a.y(),b.y()),
                 fminf(a.z(),b.z()));
}

vec3f fmaxf(const vec3f &a,const vec3f &b )
{
    return vec3f(fmaxf(a.x(),b.x()),
                 fmaxf(a.y(),b.y()),
                 fmaxf(a.z(),b.z()));
}

vec3i mini(const vec3i &a,const vec3i &b )
{
    return vec3i(mini(a.x(),b.x()),
                 mini(a.y(),b.y()),
                 mini(a.z(),b.z()));
}

vec3i maxi(const vec3i &a,const vec3i &b )
{
    return vec3i(maxi(a.x(),b.x()),
                 maxi(a.y(),b.y()),
                 maxi(a.z(),b.z()));
}

vec3i minvi(const vec3i &a,const vec3i &b )
{
    return vec3i(min(a.x(),b.x()),
                 min(a.y(),b.y()),
                 min(a.z(),b.z()));
}

vec3i maxvi(const vec3i &a,const vec3i &b )
{
    return vec3i(max(a.x(),b.x()),
                 max(a.y(),b.y()),
                 max(a.z(),b.z()));
}
int int_compare(const void *pa,const void *pb)
{
    int *a = (int *)pa;
    int *b = (int *)pb;

    if (*a == *b) {
        return 0;
    } else {
        if (*a < *b)
            return -1;
        else
            return 1;
    }
}
}
