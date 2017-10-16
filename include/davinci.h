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

#include <vec3i.h>
#include <vec4d.h>
#include <vec4f.h>
#include <vec4i.h>
#include <vec2d.h>
#include <vec2f.h>
#include <vec2i.h>
#include <vec3d.h>
#include <vec3f.h>
#include <mat2.h>
#include <mat3.h>
#include <mat4.h>
#include <mathtool.h>

#include <BBox2D.h>
#include <BBox.h>
#include <GLSpline.h>
#include <GLCubicHermiteSpline.h>
#include <GLBSpline.h>
#include <GLBSpline2D.h>
#include <GLArc.h>
#include <GLTriangle.h>
#include <GLTriangleCleaner.h>
>
#include <constant.h>
#include <svector.h>
#include <utility.h>
#include <DError.h>
#include <GLError.h>
#include <GLFrameBufferObject.h>
#include <GLTexture3D.h>
#include <GLTexture2D.h>
#include <GLTexture1D.h>
#include <GLTextureAbstract.h>
#include <GLUtilities.h>
#include <GLShader.h>
#include <GLComputeShader.h>
#include <GLClickable.h>
#include <GLSamplerObject.h>
#include <GLBufferObject.h>
#include <GLTextureBufferObject.h>
#include <GLPixelBufferObject.h>
#include <GLVertexArray.h>
#include <GLVertexBufferObject.h>
#include <GLTrackBall.h>
#include <GLUniform.h>
#include <GLUniformBlockBufferObject.h>
#include <GLAtomicCounter.h>
#include <GLShaderStorageBufferObject.h>
#include <GLTextureCubeMap.h>
#include <GLQuaternion.h>
#include <GLRenderBufferObject.h>
#include <GLIndexBufferObject.h>
#include <GLCamera.h>
#include <GLStereoCamera.h>
#include <GLContext.h>
#include <GLAttribute.h>

#define ENABLE_TEXT_RENDERING
#include <GLFont.h>
#include <GLColorMap.h>
#include <GLGizmo.h>
#include <GLGizmoScale.h>
#include <GLGizmoMove.h>
#include <GLGizmoTransform.h>
#endif
