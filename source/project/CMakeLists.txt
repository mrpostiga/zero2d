cmake_minimum_required (VERSION 2.6) 
project(Zero2D)
#add_subdirectory(..)

set(CMAKE_CXX_FLAGS "-g -Wall")

set(SRCS
../Config.cpp 
../DisplayEngine.cpp  
../GameServer.cpp 
../LogFile.cpp
../LuaMachine.cpp 
../MathEngine.cpp
../Matrix3D.cpp 
../MatrixStack.cpp
../Module.cpp
../NetworkStream.cpp 
../Shader.cpp 
../ShaderProgram.cpp 
../ShaderVBO.cpp 
../SoundEngine.cpp
../Sprite.cpp 
../TestModule.cpp
../TextureModule.cpp 
../Thread.cpp
)

find_package(Boost 1.40 COMPONENTS system filesystem REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
link_directories(${OPENGL_LIBRARY_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
link_directories(${LUA51_LIBRARY_DIRS})

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_net REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_ttf REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

add_library(GLee STATIC ../GLee.c)

set(LIBRARIES
GLee

${Boost_LIBRARIES} 

${OPENGL_LIBRARIES} 

${SDL_LIBRARY} 
${SDLIMAGE_LIBRARY} 
${SDLNET_LIBRARY} 
${SDLTTF_LIBRARY} 
${SDLMIXER_LIBRARY} 
SDLmain 

${LUA_LIBRARIES}
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}../../../../)

add_executable(zero2d ../main.cpp ${SRCS}) 
target_link_libraries(zero2d ${LIBRARIES})


