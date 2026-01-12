cmake_minimum_required(VERSION 3.16)

project(
    custom-mini-vm
    VERSION 0.1.0
    LANGUAGES C
)

# ---- Language standard ----
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ---- Target ----
add_executable(${PROJECT_NAME})

# ---- Sources ----
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    src/*.c
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCES}
)

# ---- Include directories ----
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# ---- Warnings ----
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
elseif (MSVC)
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /W4
    )
endif()

# ---- Example of SDL2 ----
# find_package(SDL2 REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)