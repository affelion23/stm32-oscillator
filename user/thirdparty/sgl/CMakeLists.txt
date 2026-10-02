# ============================================================
#  SGL GUI Library
#  - 可独立编译：  cmake -S . -B build && cmake --build build
#  - 可被嵌入：    上层 add_subdirectory(user/thirdparty/sgl)
#  - 配置只由     sgl_config.h 手动指定，CMake 不做任何开关
# ============================================================
cmake_minimum_required(VERSION 3.12)

# ---- 独立构建时才创建 project()，嵌入时由父工程提供 ----
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(sgl VERSION 0.0.1 LANGUAGES C)
    set(SGL_STANDALONE TRUE)
else()
    set(SGL_STANDALONE FALSE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SGL_ROOT_DIR    ${CMAKE_CURRENT_SOURCE_DIR})
set(SGL_INCLUDE_DIR ${SGL_ROOT_DIR}/include)

# ---- 兼容检查：sgl_config.h 可以在根目录，也可以在 include 目录 ----
if(EXISTS ${SGL_ROOT_DIR}/sgl_config.h)
    set(SGL_CONFIG_DIR ${SGL_ROOT_DIR})
elseif(EXISTS ${SGL_INCLUDE_DIR}/sgl_config.h)
    set(SGL_CONFIG_DIR ${SGL_INCLUDE_DIR})
else()
    message(FATAL_ERROR
        "[SGL] 缺少 sgl_config.h，请将其放在 ${SGL_ROOT_DIR} 或 ${SGL_INCLUDE_DIR} 下。")
endif()

# ============================================================
# 收集源文件
# ------------------------------------------------------------
#  - 只包含当前 sgl_config.h 实际启用的目录
#  - components 下排除 games/2048（示例程序）
#  - mm 下只保留 lwmem（CONFIG_SGL_HEAP_ALGO = lwmem）
# ============================================================
file(GLOB_RECURSE SGL_SOURCES
    # ---- 内核 / 绘图 / 字体 / 文件系统 / 控件 ----
    ${SGL_ROOT_DIR}/core/*.c
    ${SGL_ROOT_DIR}/draw/*.c
    ${SGL_ROOT_DIR}/fonts/*.c
    ${SGL_ROOT_DIR}/fs/*.c
    ${SGL_ROOT_DIR}/widgets/*.c

    # ---- components：只要 3dvortex 和 timer，排除 games/2048 ----
    ${SGL_ROOT_DIR}/components/3dvortex/*.c
    ${SGL_ROOT_DIR}/components/timer/*.c

    # ---- mm：只用 lwmem，其它算法（tlsf/mtlsf/umm_malloc/other）全部排除 ----
    ${SGL_ROOT_DIR}/mm/lwmem/*.c
)

if(NOT SGL_SOURCES)
    message(FATAL_ERROR "[SGL] 未找到任何 .c 源文件，请确认目录结构")
endif()

# ============================================================
# 目标
# ============================================================
add_library(sgl STATIC ${SGL_SOURCES})

target_include_directories(sgl PUBLIC
    ${SGL_ROOT_DIR}
    ${SGL_INCLUDE_DIR}
)

set_target_properties(sgl PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

# ---- 独立构建时支持 make install（可选）----
if(SGL_STANDALONE)
    include(GNUInstallDirs)
    install(TARGETS sgl
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(DIRECTORY ${SGL_ROOT_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sgl
        FILES_MATCHING PATTERN "*.h")
endif()