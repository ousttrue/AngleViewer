SET(SUB_NAME gui)

FILE(GLOB SRC
	*.cpp
	*.h
    ../imgui/imgui.cpp
    ../imgui/imgui.h
    ../imgui/imgui_demo.cpp
    ../imgui/imgui_draw.cpp
    ../imgui/imgui_internal.h
    ../imgui/imconfig.h
    ../imgui/stb_rect_pack.h
    ../imgui/stb_textedit.h
    ../imgui/stb_truetype.h
    ../imgui/examples/imgui_impl_opengl3.h
    ../imgui/examples/imgui_impl_opengl3.cpp
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

