include_directories("../include")
find_package(Threads REQUIRED)

# 根据系统架构选择库路径
if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    message(STATUS "Detected ARM64 architecture, using aarch64 library")
    link_directories(../lib/aarch64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "amd64")
    message(STATUS "Detected x86_64 architecture, using x86_64 library")
    link_directories(../lib/x86_64)
else ()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# 构建客户端示例
add_executable(walk_example walkExample.cpp)
target_link_libraries(walk_example PRIVATE fire_client_sdk Threads::Threads)