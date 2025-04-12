cmake_minimum_required(VERSION 3.10)
project(MasoRPG)

set(CMAKE_CXX_STANDARD 17)

# SDL2ライブラリ群を探す
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# インクルードディレクトリの追加
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# 実行ファイル作成
add_executable(main src/main.cpp)

# SDL2, SDL2_ttf, SDL2_imageをリンク
target_link_libraries(main SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image)

