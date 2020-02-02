add_library(Camera camera.cpp)

set(CAMERA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GLM_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/thirdpart/glm/include)
set(GLAD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/thirdpart/glad/include)

target_include_directories(Camera PRIVATE  
${CMAKE_CURRENT_SOURCE_DIR} 
${GLM_INCLUDE_PATH} 
${CAMERA_INCLUDE_DIR} 
${GLAD_INCLUDE_DIR})