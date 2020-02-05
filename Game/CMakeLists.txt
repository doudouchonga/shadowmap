add_library(Game worldbase.cpp)

include_directories(${PROJECT_SOURCE_DIR}/thirdpart/glfw_3_3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/Render/include)
include_directories(${PROJECT_SOURCE_DIR}/Camera/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/model_import/include)

target_include_directories(Game PRIVATE ${PROJECT_SOURCE_DIR}/thirdpart/glad/include)
