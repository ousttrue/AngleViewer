FILE(GLOB SRC
	*.cpp
	*.h
    resource.rc
    shader.vs
    shader.fs
	) 

ADD_EXECUTABLE(${PROJECT_NAME} WIN32
    ${SRC})

TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC
	UNICODE=1
	_UNICODE=1
	)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    ../scene
    ../gui
    ../gles3renderer
	)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    winmm
    scene
    gui
    gles3renderer
	debug 
	${CMAKE_INSTALL_PREFIX}/debug/lib/libegl.lib
	${CMAKE_INSTALL_PREFIX}/debug/lib/libGLESv2.lib
	optimized 
	${CMAKE_INSTALL_PREFIX}/lib/libegl.lib
	${CMAKE_INSTALL_PREFIX}/lib/libGLESv2.lib
	)

