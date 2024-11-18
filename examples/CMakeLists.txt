include_directories("../include")
link_directories(../lib)
find_package(Threads REQUIRED)

# 构建客户端示例
add_executable(walk_example walkExample.cpp)
target_link_libraries(walk_example PRIVATE libfire_client_sdk.a Threads::Threads)

