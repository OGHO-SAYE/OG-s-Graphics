cmake_minimum_required(VERSION 3.8)

project(footballpitch) # The name of your choice for the project comes here

add_definitions(
    -DGLEW_STATIC
)

IF(WIN32)
    # set stuff for windows
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)

    # for windows
    include_directories(C:/glfw-3.3.9.bin.WIN64/include/)
    include_directories(C:/glew-2.1.0-src/include/)
    include_directories(C:/glew-2.1.0-src/include/GL/)
    include_directories(C:/GLM_0.9.9.8/g-truc-glm-bf71a83/)

    link_directories(C:/glew-2.1.0/lib/Release/x64/)
    link_directories(C:/glfw-3.3.9.bin.WIN64/lib-vc2019/)

    set(ALL_LIBS
        ${OPENGL_gl_LIBRARY}
        glew32s 
        glfw3
    )

    # Add Assimp library path
    link_directories(./lib)  # Update this path with the actual Assimp library path

    # Copy Assimp DLL to the output directory
    file(COPY dlls/assimp-vc140-mt.dll DESTINATION ${CMAKE_BINARY_DIR}/Debug)
    file(COPY dlls/assimp-vc140-mt.dll DESTINATION ${CMAKE_BINARY_DIR}/Release)

ELSE()
    # Your configuration for non-Windows systems goes here
ENDIF()

AUX_SOURCE_DIRECTORY(src/learnopengl SOURCES)
AUX_SOURCE_DIRECTORY(src/assimp SOURCES)

include_directories(src)

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})

# Link against Assimp library
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS} assimp)

file(COPY
    shaders/phong.frag.glsl
    shaders/phong.vert.glsl
    shaders/blinnPhong.frag.glsl
    shaders/blinnPhong.vert.glsl
   
    DESTINATION
    ${CMAKE_BINARY_DIR}
)
