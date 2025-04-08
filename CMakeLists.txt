cmake_minimum_required(VERSION 3.10)
project(MySDLProject)

set(CMAKE_CXX_STANDARD 17)

# SDL2を探す
find_package(SDL2 REQUIRED)

# インクルードディレクトリ追加
include_directories(${SDL2_INCLUDE_DIRS})

# 実行ファイル作成
add_executable(main src/main.cpp)

# SDL2をリンク
target_link_libraries(main ${SDL2_LIBRARIES})

