cmake_minimum_required(VERSION 3.1)
project(FinalFinal CXX)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(GLEW_USE_STATIC_LIBS TRUE)

include(add_resources)

if (MINGW)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/bin/dependencies/include/")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/bin/dependencies/lib/mingw")
elseif (MSVC)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/bin/dependencies/include/")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/bin/dependencies/lib/vc2015")
    set(USE_STRICT_COMPILE_WARNINGS OFF CACHE BOOL "" FORCE)
endif ()

option(USE_STRICT_COMPILE_WARNINGS "Use strict compilation warnings in debug mode." ON)

#set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

set(STRICT_COMPILE_FLAGS "-Wpedantic -Wall -Wno-c++98-compat -Wextra -Wconversion -Wsign-compare -Wuninitialized -Wno-unused-parameter")
if (USE_STRICT_COMPILE_WARNINGS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${STRICT_COMPILE_FLAGS}")
endif () 

find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
endif ()

if (NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "../_install")
endif ()

set(PPGSO_SHADER_SRC
        bin/ppgso/shader/color_vert.glsl bin/ppgso/shader/color_frag.glsl
        bin/ppgso/shader/convolution_vert.glsl bin/ppgso/shader/convolution_frag.glsl
        bin/ppgso/shader/diffuse_vert.glsl bin/ppgso/shader/diffuse_frag.glsl
        bin/ppgso/shader/texture_vert.glsl bin/ppgso/shader/texture_frag.glsl
        bin/ppgso/shader/phong_vert.glsl bin/ppgso/shader/phong_frag.glsl
        )
add_resources(shaders ${PPGSO_SHADER_SRC})

add_library(ppgso STATIC
        bin/ppgso/lib/mesh.cpp
        bin/ppgso/lib/tiny_obj_loader.cpp
        bin/ppgso/lib/shader.cpp
        bin/ppgso/lib/image.cpp
        bin/ppgso/lib/image_bmp.cpp
        bin/ppgso/lib/image_raw.cpp
        bin/ppgso/lib/texture.cpp
        bin/ppgso/lib/window.cpp
        )

target_compile_definitions(ppgso PUBLIC -DGLM_FORCE_RADIANS -DGLEW_STATIC)

target_link_libraries(ppgso PUBLIC ${GLFW_LIBRARIES} GLEW::GLEW ${OPENGL_LIBRARIES})

target_include_directories(ppgso PUBLIC
        bin/ppgso/lib
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR})

add_executable(FinalFinal
        src/main.cpp
        src/common/main_window.cpp
        src/common/scene.cpp
        src/common/day_scene.cpp
        src/common/camera.cpp
        src/objects/shadow.cpp
        src/objects/object.cpp
        src/objects/world.cpp
        src/objects/wall.cpp
        src/objects/shape.cpp
        src/objects/player.cpp
        src/objects/weather.cpp
        src/objects/light.cpp
        src/objects/common.h)

target_link_libraries(FinalFinal ppgso shaders)
install(TARGETS FinalFinal DESTINATION .)

file(COPY "resources/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(DIRECTORY resources/ DESTINATION .)
