add_library(Utils filesystem.cpp global.cpp)

include_directories(${CMAKE_BINARY_DIR}/configuration)
include_directories(${PROJECT_SOURCE_DIR}/Game/include)
include_directories(${PROJECT_SOURCE_DIR}/Render/include)
include_directories(${PROJECT_SOURCE_DIR}/Camera/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/glfw_3_3/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/glm/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdpart/glad/include)
target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)