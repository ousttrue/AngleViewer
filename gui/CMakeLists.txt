SET(SUB_NAME gui)
SET(IMGUI_DIR ../dependencies/imgui)

FILE(GLOB SRC
	*.cpp
	*.h
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/stb_rect_pack.h
    ${IMGUI_DIR}/stb_textedit.h
    ${IMGUI_DIR}/stb_truetype.h
    ${IMGUI_DIR}/examples/imgui_impl_opengl3.h
    ${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
	) 

ADD_LIBRARY(${SUB_NAME} 
    ${SRC})

TARGET_COMPILE_DEFINITIONS(${SUB_NAME} PUBLIC
	UNICODE=1
	_UNICODE=1
    USE_GL_ES3
    GL_GLEXT_PROTOTYPES
	)

TARGET_INCLUDE_DIRECTORIES(${SUB_NAME} PUBLIC
    ../glm
    ../imgui
    ../scene
	${CMAKE_INSTALL_PREFIX}/include
	)

