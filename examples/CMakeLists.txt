include_directories("../include")
link_directories(../lib)
find_package(Threads REQUIRED)

# 构建客户端示例
add_executable(stance_example StanceExample.cpp)
target_link_libraries(stance_example PRIVATE libfire_client_sdk.a Threads::Threads)

