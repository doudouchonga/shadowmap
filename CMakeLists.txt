cmake_minimum_required(VERSION 3.12)

project(shadowmap)

set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/glfw_3_3/include)
set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/glad/include)
set(GLAD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/glad/src)
set(GLAD_SRC_FILE1 ${GLAD_SRC_DIR}/glad.c)
set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/glfw_3_3/lib)
set(GLFW_LIBS ${GLFW_LIB_DIR}/glfw3.lib)
set(ASSIMP_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/model_import/lib/assimp.lib)

add_subdirectory(Render)
add_subdirectory(Camera)
add_subdirectory(Utils)
add_subdirectory(Game)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/Camera/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/model_import/include)

set(GAME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Game/include)

add_executable(shadowmap main.cpp ${GLAD_SRC_FILE1} stb_image.cpp)

target_include_directories(shadowmap PUBLIC 
${GLFW_INCLUDE_DIR} 
${GLAD_INCLUDE_DIR} 
${PROJECT_BINARY_DIR} 
${GAME_INCLUDE_DIR} 
)

set_property(TARGET shadowmap PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dlls)
target_link_libraries(shadowmap PUBLIC ${GLFW_LIBS} Render Camera Utils Game ${ASSIMP_LIBS_DIR})
# message(STATUS "msg:  " ${GLFW_LIBS})
 