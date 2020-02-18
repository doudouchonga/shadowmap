add_library(Render render_mesh.cpp game_texture.cpp shader.cpp shadowmap_mgr.cpp model.cpp mesh.cpp gbuff.cpp game_object.cpp pbr_mgr.cpp)

set(GLAD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/thirdpart/glad/include)
set(RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(FILE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/utils/include)
set(GLM_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/thirdpart/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/Camera/include)
include_directories(${CMAKE_BINARY_DIR}/configuration)
include_directories(${PROJECT_SOURCE_DIR}/Game/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/glfw_3_3/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/model_import/include)

target_include_directories(Render PUBLIC  
${CMAKE_CURRENT_SOURCE_DIR} 
${GLAD_INCLUDE_DIR} 
${FILE_INCLUDE_DIR} 
${RENDER_INCLUDE_DIR} 
${GLM_INCLUDE_PATH})