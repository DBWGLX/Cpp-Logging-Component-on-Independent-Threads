# 设置 CMake 最低版本要求
# cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(AsyncLogger)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# 添加头文件搜索路径
include_directories(include include/dbwg)

# 查找所有源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 生成动态库
add_library(async_logger SHARED ${SOURCES})

# 设置安装前缀
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../cmake_out)
message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

# 设置动态库的安装路径
install(TARGETS async_logger DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)