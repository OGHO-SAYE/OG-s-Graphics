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
    link_directories(/src/assimp/5.3.1/lib)  # Update this path with the actual Assimp library path

ELSE()
    set(CMAKE_BUILD_TYPE Debug)

    set(CMAKE_CXX_STANDARD 11)

    set(CMAKE_PREFIX_PATH
        /encs # For ENCS lab computers
        /opt/local # Macports
    )

    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED
        HINTS /encs/pkg/glfw-3.3.4/root # ENCS installation of glfw
    )

    set(ALL_LIBS
        GLEW::glew
        OpenGL::GL
        glfw
    )

    # NOTE: ENCS glm installation is missing links to *.inl files so we need this line
    include_directories(/encs/pkg/glm-0.9.9.8/root/include)

    # Add Assimp library path
    link_directories(/opt/homebrew/Cellar/assimp/5.3.1/lib)

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

