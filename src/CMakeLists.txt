#For building DAVINCI static library
PROJECT(Davinci)

SET(MATH_HEADER
${DAVINCI_INC_DIR}/vec3i.h
${DAVINCI_INC_DIR}/vec4d.h
${DAVINCI_INC_DIR}/vec4f.h
${DAVINCI_INC_DIR}/vec4i.h
${DAVINCI_INC_DIR}/vec2d.h
${DAVINCI_INC_DIR}/vec2f.h
${DAVINCI_INC_DIR}/vec2i.h
${DAVINCI_INC_DIR}/vec3d.h
${DAVINCI_INC_DIR}/vec3f.h
${DAVINCI_INC_DIR}/mat2.h
${DAVINCI_INC_DIR}/mat3.h
${DAVINCI_INC_DIR}/mat4.h
${DAVINCI_INC_DIR}/mathtool.h
)

SET(MATH_SOURCE
${DAVINCI_SRC_DIR}/vec3i.cpp
${DAVINCI_SRC_DIR}/vec4d.cpp
${DAVINCI_SRC_DIR}/vec4f.cpp
${DAVINCI_SRC_DIR}/vec4i.cpp
${DAVINCI_SRC_DIR}/vec2d.cpp
${DAVINCI_SRC_DIR}/vec2f.cpp
${DAVINCI_SRC_DIR}/vec2i.cpp
${DAVINCI_SRC_DIR}/vec3d.cpp
${DAVINCI_SRC_DIR}/vec3f.cpp
${DAVINCI_SRC_DIR}/mat2.cpp
${DAVINCI_SRC_DIR}/mat3.cpp
${DAVINCI_SRC_DIR}/mat4.cpp
${DAVINCI_SRC_DIR}/mathtool.cpp
)



SET(GEOM_HEADER
${DAVINCI_INC_DIR}/BBox2D.h
${DAVINCI_INC_DIR}/BBox.h
${DAVINCI_INC_DIR}/GLSpline.h
${DAVINCI_INC_DIR}/GLCubicHermiteSpline.h
${DAVINCI_INC_DIR}/GLBSpline.h
${DAVINCI_INC_DIR}/GLBSpline2D.h
${DAVINCI_INC_DIR}/GLArc.h
${DAVINCI_INC_DIR}/GLTriangle.h
${DAVINCI_INC_DIR}/GLTriangleCleaner.h
)

SET(GEOM_SOURCE
${DAVINCI_SRC_DIR}/BBox2D.cpp
${DAVINCI_SRC_DIR}/BBox.cpp
${DAVINCI_SRC_DIR}/GLSpline.cpp
${DAVINCI_SRC_DIR}/GLCubicHermiteSpline.cpp
${DAVINCI_SRC_DIR}/GLBSpline.cpp
${DAVINCI_SRC_DIR}/GLBSpline2D.cpp
${DAVINCI_SRC_DIR}/GLArc.cpp
${DAVINCI_SRC_DIR}/GLTriangleCleaner.cpp
)


SET(CORE_HEADER
${DAVINCI_INC_DIR}/constant.h
${DAVINCI_INC_DIR}/svector.h
${DAVINCI_INC_DIR}/utility.h
${DAVINCI_INC_DIR}/DError.h
${DAVINCI_INC_DIR}/GLError.h
${DAVINCI_INC_DIR}/GLFrameBufferObject.h
${DAVINCI_INC_DIR}/GLTexture3D.h
${DAVINCI_INC_DIR}/GLTexture2D.h
${DAVINCI_INC_DIR}/GLTexture1D.h
${DAVINCI_INC_DIR}/GLTextureAbstract.h
${DAVINCI_INC_DIR}/GLUtilities.h
${DAVINCI_INC_DIR}/GLShader.h
${DAVINCI_INC_DIR}/GLComputeShader.h
${DAVINCI_INC_DIR}/GLClickable.h
${DAVINCI_INC_DIR}/GLSamplerObject.h
${DAVINCI_INC_DIR}/GLBufferObject.h
${DAVINCI_INC_DIR}/GLTextureBufferObject.h
${DAVINCI_INC_DIR}/GLPixelBufferObject.h
${DAVINCI_INC_DIR}/GLVertexArray.h
${DAVINCI_INC_DIR}/GLVertexBufferObject.h
${DAVINCI_INC_DIR}/GLTrackBall.h
${DAVINCI_INC_DIR}/GLUniform.h
${DAVINCI_INC_DIR}/GLUniformBlockBufferObject.h
${DAVINCI_INC_DIR}/GLAtomicCounter.h
${DAVINCI_INC_DIR}/GLShaderStorageBufferObject.h
${DAVINCI_INC_DIR}/GLTextureCubeMap.h
${DAVINCI_INC_DIR}/GLQuaternion.h
${DAVINCI_INC_DIR}/GLRenderBufferObject.h
${DAVINCI_INC_DIR}/GLIndexBufferObject.h
${DAVINCI_INC_DIR}/GLCamera.h
${DAVINCI_INC_DIR}/GLStereoCamera.h
${DAVINCI_INC_DIR}/GLContext.h
${DAVINCI_INC_DIR}/GLAttribute.h
)

SET(CORE_SOURCE
${DAVINCI_SRC_DIR}/GLError.cpp
${DAVINCI_SRC_DIR}/GLFrameBufferObject.cpp
${DAVINCI_SRC_DIR}/GLTexture3D.cpp
${DAVINCI_SRC_DIR}/GLTexture2D.cpp
${DAVINCI_SRC_DIR}/GLTexture1D.cpp
${DAVINCI_SRC_DIR}/GLTextureAbstract.cpp
${DAVINCI_SRC_DIR}/GLUtilities.cpp
${DAVINCI_SRC_DIR}/GLShader.cpp
${DAVINCI_SRC_DIR}/GLComputeShader.cpp
${DAVINCI_SRC_DIR}/GLClickable.cpp
${DAVINCI_SRC_DIR}/GLSamplerObject.cpp
${DAVINCI_SRC_DIR}/GLBufferObject.cpp
${DAVINCI_SRC_DIR}/GLTextureBufferObject.cpp
${DAVINCI_SRC_DIR}/GLPixelBufferObject.cpp
${DAVINCI_SRC_DIR}/GLVertexArray.cpp
${DAVINCI_SRC_DIR}/GLVertexBufferObject.cpp
${DAVINCI_SRC_DIR}/GLTrackBall.cpp
${DAVINCI_SRC_DIR}/GLTriangle.cpp
${DAVINCI_SRC_DIR}/GLUniform.cpp
${DAVINCI_SRC_DIR}/GLUniformBlockBufferObject.cpp
${DAVINCI_SRC_DIR}/GLAtomicCounter.cpp
${DAVINCI_SRC_DIR}/GLShaderStorageBufferObject.cpp
${DAVINCI_SRC_DIR}/GLTextureCubeMap.cpp
${DAVINCI_SRC_DIR}/GLQuaternion.cpp
${DAVINCI_SRC_DIR}/GLRenderBufferObject.cpp
${DAVINCI_SRC_DIR}/GLIndexBufferObject.cpp
${DAVINCI_SRC_DIR}/GLCamera.cpp
${DAVINCI_SRC_DIR}/GLStereoCamera.cpp
${DAVINCI_SRC_DIR}/GLContext.cpp
${DAVINCI_SRC_DIR}/GLAttribute.cpp
)

IF(DAVINCI_ENABLE_TEXT_RENDERING)
    SET(TEXT_HEADER 
        ${DAVINCI_INC_DIR}/GLFont.h
        ${DAVINCI_INC_DIR}/GLColorMap.h
        ${DAVINCI_INC_DIR}/GLGizmo.h
        ${DAVINCI_INC_DIR}/GLGizmoScale.h
        ${DAVINCI_INC_DIR}/GLGizmoMove.h
        ${DAVINCI_INC_DIR}/GLGizmoTransform.h
        )
    SET(TEXT_SOURCE 
        ${DAVINCI_SRC_DIR}/GLFont.cpp
        ${DAVINCI_SRC_DIR}/GLColorMap.cpp
        ${DAVINCI_SRC_DIR}/GLGizmo.cpp
        ${DAVINCI_SRC_DIR}/GLGizmoScale.cpp
        ${DAVINCI_SRC_DIR}/GLGizmoMove.cpp
        ${DAVINCI_SRC_DIR}/GLGizmoTransform.cpp
        )
ENDIF()

SET(SHADER_SOURCE ${DAVINCI_SRC_DIR}/../shader/common.glsl)

SOURCE_GROUP("math"		FILES ${MATH_SOURCE})
SOURCE_GROUP("math"		FILES ${MATH_HEADER})
SOURCE_GROUP("geometry" FILES ${GEOM_SOURCE})
SOURCE_GROUP("geometry" FILES ${GEOM_HEADER})
SOURCE_GROUP("core"		FILES ${CORE_SOURCE} ${TEXT_SOURCE})
SOURCE_GROUP("core"		FILES ${CORE_HEADER} ${TEXT_HEADER})

SOURCE_GROUP("shader"	FILES ${SHADER_SOURCE})


ADD_LIBRARY(
    ${PROJECT_NAME} ${MATH_HEADER} ${GEOM_HEADER} ${CORE_HEADER} ${TEXT_HEADER}
    ${MATH_SOURCE} ${GEOM_SOURCE} ${CORE_SOURCE} ${TEXT_SOURCE} ${SHADER_SOURCE} 
    )

if(${UNIX})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} GLEW glut GLU freetype ${LIBS})
else()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} glew freeglutd freetype ${LIBS})
endif()

SET_PROPERTY(TARGET Davinci PROPERTY FOLDER davinci)