cmake_minimum_required(VERSION 3.10)
project(MasoRPG)

set(CMAKE_CXX_STANDARD 17)

# SDL2ライブラリ群を探す
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# インクルードディレクトリの追加
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

# ソースファイルを追加
add_executable(MasoRPG
    src/main.cpp
    src/Camera2D.cpp
)

# ライブラリをリンク
target_link_libraries(MasoRPG
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
)
