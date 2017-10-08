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

#ifndef _CONSTANT_H_
#define _CONSTANT_H_
namespace davinci{
#define FLOAT_MAX        3.402823466e+38f          //;/* max value */
#define FLOAT_MIN        1.175494351e-38f          //;/* min positive value */
#define FLOAT_EPSILON    1.192092896e-07f          //;/* smallest such that 1.0+FLT_EPSILON != 1.0 */
#define DOUBLE_MAX       1.7976931348623158e+308   //;/* max value */
#define DOUBLE_MIN       2.2250738585072014e-308   //;/* min positive value */
#define DOUBLE_EPSILON   2.2204460492503131e-016   //;/* smallest such that 1.0+DBL_EPSILON != 1.0 */
#define PI               3.1415926f                //;
#define INVPI			 0.318310
#define INV2PI			 0.159154
#define DEGREE_TO_RADIAN 0.017453f                 //;
#define SQRT_2			 1.4142136f
#define SQRT_3			 1.7320508f
#define DEG2RAD			 0.017453294f
#define INT_MAX_POSITIVE 0x7FFFFFFF
#define INT_MAX_NEGATIVE -0x7FFFFFFF
#define UINT_MAX_POSITVE 0xFFFFFFFF
}
#endif