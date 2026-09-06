cmake_minimum_required(VERSION 3.22)
project(MacroButtons LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MacroButtons SHARED
    src/main.cpp
)

target_include_directories(MacroButtons PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Empacotamento em .levipack
add_custom_command(TARGET MacroButtons POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/MacroButtons.levipack" --format=zip 
            "${CMAKE_CURRENT_SOURCE_DIR}/manifest.json" 
            "$<TARGET_FILE:MacroButtons>"
    COMMENT "Criando arquivo .levipack"
)
