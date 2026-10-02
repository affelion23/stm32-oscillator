cmake_minimum_required(VERSION 3.22)

# ============================================================
# Compiler / project settings
# ============================================================
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME stm32f103bsp)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

enable_language(C ASM)

# ============================================================
# Target
# ============================================================
add_executable(${CMAKE_PROJECT_NAME})

# ============================================================
# Compile definitions (user)
# ============================================================
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# ============================================================
# Source files (user)
# ============================================================
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_CURRENT_SOURCE_DIR}/user/drivers/lcd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/user/drivers/sgl_port.c
)

# ============================================================
# Include directories (user)
# ============================================================
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_CURRENT_SOURCE_DIR}/user/drivers/
    ${CMAKE_CURRENT_SOURCE_DIR}/user/config/
)

# ============================================================
# Subdirectories: generated code and third-party libraries
# ============================================================
add_subdirectory(cmake/stm32cubemx) # STM32CubeMX generated sources
add_subdirectory(user/thirdparty/sgl) # sgl gui library

# ============================================================
# Link directories (user)
# ============================================================
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# ============================================================
# Linked libraries
# ============================================================
target_link_libraries(${CMAKE_PROJECT_NAME}
    # generated
    stm32cubemx

    # third-party
    sgl

    # user
    # Add user defined libraries
)

# ============================================================
# Workaround
# ============================================================
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)