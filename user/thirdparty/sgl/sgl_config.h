//********************************************************************
//* SGL Configuration File                                           *
//* You can modify the following parameters according to your needs. *
//*                                                                  *
//* This file is the user-specific configuration for SGL.            *
//* It is based on the standard defaults defined in sgl_cfgfix.h.    *
//* Only macros that exist in sgl_cfgfix.h are valid here.           *
//* Macros not present in the second file use the standard defaults. *
//*                                                                  *
//* Reference: source/include/sgl_cfgfix.h                           *
//* Docs: https://sgl-docs.readthedocs.io/zh-cn/latest/              *
//********************************************************************

#ifndef __SGL_CONFIG_H__
#define __SGL_CONFIG_H__

//====================================================================
// Display / Framebuffer Device (FBDEV)
//====================================================================

/*
 * Framebuffer pixel depth in bits per pixel.
 * Valid values: 16 (RGB565), 24 (RGB888), 32 (ARGB8888).
 * 16-bit is recommended for most MCU + SPI LCD setups.
 * Standard default: 16
 */
#define CONFIG_SGL_FBDEV_PIXEL_DEPTH            16

/*
 * Software rotation of the framebuffer in degrees (clockwise).
 * Valid values: 0, 90, 180, 270.
 * Rotation is performed during rendering and costs CPU time.
 * Set to 0 if the display is used in its native orientation.
 * Standard default: 0
 */
#define CONFIG_SGL_FBDEV_ROTATION               270

/*
 * Enable runtime (dynamic) display rotation.
 * When enabled, the rotation angle can be changed at runtime,
 * at the cost of extra memory and processing overhead.
 * Standard default: 0
 */
#define CONFIG_SGL_FBDEV_RUNTIME_ROTATION       0

/*
 * Align drawing coordinates to even pixel boundaries.
 * Can improve performance on some DMA or 16-bit bus architectures.
 * Usually not needed for SPI LCDs.
 * Standard default: 0
 */
#define CONFIG_SGL_FBDEV_EVEN_COORDS            0

/*
 * Use the framebuffer VRAM directly for rendering without an
 * intermediate buffer. Saves memory but may cause visible tearing.
 * Standard default: 0
 */
#define CONFIG_SGL_USE_FBDEV_VRAM               0

/*
 * Swap the byte order of 16-bit RGB565 color values.
 * Enable when the display controller expects big-endian RGB565
 * (high byte first) while the CPU produces little-endian values.
 * Standard default: 0
 */
#define CONFIG_SGL_COLOR16_SWAP                 1

//====================================================================
// System Tick / Timing
//====================================================================

/*
 * System tick interval in milliseconds.
 * Drives SGL's internal timer, animations, and event timers.
 * Smaller values give smoother animation but consume more CPU.
 * Standard default: 10
 */
#define CONFIG_SGL_SYSTICK_MS                   50

//====================================================================
// Event System
//====================================================================

/*
 * Maximum number of input events that can be queued before processing.
 * Must be a power of 2 (16, 32, 64, ...).
 * Increase if your application generates bursts of input events.
 * Standard default: 16
 */
#define CONFIG_SGL_EVENT_QUEUE_SIZE             16

//====================================================================
// Dirty Area / Partial Refresh
//====================================================================

/*
 * Maximum number of dirty (invalidated) rectangular regions tracked
 * simultaneously for partial screen refresh.
 * Higher values allow more scattered updates per frame but use more RAM.
 * Standard default: 16
 */
#define CONFIG_SGL_DIRTY_AREA_NUM_MAX           16

/*
 * Enable visual trace overlay for dirty regions (debug only).
 * Dirty areas are highlighted with CONFIG_SGL_DIRTY_AREA_TRACE_COLOR.
 * Standard default: 0
 */
#define CONFIG_SGL_DIRTY_AREA_TRACE             0

/*
 * Color used for the dirty region debug trace overlay.
 * Only effective when CONFIG_SGL_DIRTY_AREA_TRACE is 1.
 * Format: sgl_rgb(r, g, b)
 * Standard default: sgl_rgb(0, 0, 0)
 */
#define CONFIG_SGL_DIRTY_AREA_TRACE_COLOR       sgl_rgb(0, 0, 0)

//====================================================================
// Focus / UI Highlight
//====================================================================

/*
 * Focus highlight color for focusable widgets (buttons, sliders, etc.).
 * Format: sgl_rgb(r, g, b)
 * Standard default: sgl_rgb(0x00, 0xFF, 0x00)
 */
#define CONFIG_SGL_FOCUSED_COLOR                sgl_rgb(0x00, 0xFF, 0x00)

/*
 * Focus highlight border width in pixels.
 * Set to 0 to disable the focus indicator.
 * Standard default: 0
 */
#define CONFIG_SGL_FOCUSED_WIDTH                0

//====================================================================
// Monitor / Debug Overlay
//====================================================================

/*
 * Enable monitor debug trace overlay (frame timing info).
 * Displays rendering statistics on screen. Debug only.
 * Standard default: 0
 */
#define CONFIG_SGL_MONITOR_TRACE                0

/*
 * Monitor overlay width in pixels.
 * Standard default: 81
 */
#define CONFIG_SGL_MONITOR_COORDS_WIDTH         81

/*
 * Monitor overlay height in pixels.
 * Standard default: 30
 */
#define CONFIG_SGL_MONITOR_COORDS_HEIGHT        30

/*
 * Monitor overlay background color.
 * Standard default: SGL_COLOR_BLACK
 */
#define CONFIG_SGL_MONITOR_BG_COLOR             SGL_COLOR_BLACK

/*
 * Monitor overlay text color.
 * Standard default: SGL_COLOR_WHITE
 */
#define CONFIG_SGL_MONITOR_TEXT_COLOR           SGL_COLOR_WHITE

/*
 * Monitor overlay alpha (0-255).
 * Standard default: 128
 */
#define CONFIG_SGL_MONITOR_ALPHA                128

//====================================================================
// Image / Pixmap
//====================================================================

/*
 * Enable bilinear interpolation for pixmap scaling.
 * Produces smoother scaled images at higher CPU cost.
 * Standard default: 0
 */
#define CONFIG_SGL_PIXMAP_BILINEAR_INTERP       0

/*
 * Line buffer size (in lines) for decoding external images.
 * 0 = auto (use full image height).
 * Smaller values reduce RAM usage at the cost of more flash reads.
 * Standard default: 1
 */
#define CONFIG_SGL_IMG_BUFFER_LINES             1

//====================================================================
// Debug / Logging
//====================================================================

/*
 * Enable global debug logging.
 * Adds code size and runtime overhead. Set to 0 in production.
 * Standard default: 0
 */
#define CONFIG_SGL_DEBUG                        0

/*
 * The following logging macros are only defined when CONFIG_SGL_DEBUG == 1,
 * exactly as in sgl_cfgfix.h. Since CONFIG_SGL_DEBUG is 0 above, they are
 * not compiled into the build. The values below are taken from your
 * original file and will be used if you later enable debugging.
 */
#if (CONFIG_SGL_DEBUG == 1)
/*
 * Colored log output using ANSI escape sequences.
 * Standard default (when DEBUG=1): 0
 */
#   define CONFIG_SGL_LOG_COLOR                 1

/*
 * Log verbosity level:
 *   0 = all messages
 *   1 = error only
 *   2 = warning and above
 *   3 = info and above
 *   4 = debug only
 * Standard default (when DEBUG=1): 1
 */
#   define CONFIG_SGL_LOG_LEVEL                 0
#endif

//====================================================================
// Object System
//====================================================================

/*
 * Enable object name support for debugging.
 * Each sgl_obj_t stores a string name, increasing RAM usage per object.
 * Standard default: 0
 */
#define CONFIG_SGL_OBJ_USE_NAME                 0

//====================================================================
// Memory Management / Heap
//====================================================================

/*
 * Heap allocation algorithm.
 * Available options: lwmem, tlsf (depending on compiled sources).
 * Standard default: lwmem
 */
#define CONFIG_SGL_HEAP_ALGO                    lwmem

/*
 * Total size of SGL's static heap pool in bytes.
 * All GUI objects are allocated from this pool via sgl_malloc().
 * Standard default: 10240
 */
#define CONFIG_SGL_HEAP_MEMORY_SIZE             10240

/*
 * Note on CONFIG_SGL_TLSF_INDEX_MAX:
 * In sgl_cfgfix.h, CONFIG_SGL_TLSF_INDEX_MAX is only defaulted inside the
 * #ifndef CONFIG_SGL_HEAP_MEMORY_SIZE block. Since CONFIG_SGL_HEAP_MEMORY_SIZE
 * is explicitly defined above, the standard file will not define
 * CONFIG_SGL_TLSF_INDEX_MAX. Therefore it is intentionally omitted here.
 * If you ever need to override it, define it explicitly before including
 * sgl_cfgfix.h.
 */

//====================================================================
// Fonts
//====================================================================

/*
 * Enable font data compression (decompressed on-the-fly during rendering).
 * Reduces flash/ROM usage at slightly higher CPU cost.
 * Standard default: 0
 */
#define CONFIG_SGL_FONT_COMPRESSED              1

/*
 * Use a compact font lookup table instead of a full table.
 * Reduces ROM usage by storing only used glyphs.
 * Standard default: 1
 */
#define CONFIG_SGL_FONT_SMALL_TABLE             1

/*
 * Support font bitmaps stored in external flash (e.g., SPI NOR).
 * Requires an external flash read driver.
 * Standard default: 0
 */
#define CONFIG_SGL_FLASH_FONT                   0

/*
 * Temporary buffer size (bytes) for one glyph bitmap when using
 * CONFIG_SGL_FLASH_FONT. Must hold the largest glyph's bitmap.
 * Standard default: 512
 */
#define CONFIG_SGL_FLASH_FONT_GLYPH_BUF_SIZE    512

/*
 * Enable float format (%f) support in sgl_snprintf().
 * Adds code size for float-to-string conversion.
 * Standard default: 0
 */
#define CONFIG_SGL_SNPRINTF_FLOAT               0

/*
 * Enable Song 23pt font (Chinese serif).
 * Large flash footprint; enable only when Chinese text is required.
 * Standard default: 0
 */
#define CONFIG_SGL_FONT_SONG23                  0

/*
 * Enable Consolas 14pt font (small monospace).
 * Standard default: 0
 */
#define CONFIG_SGL_FONT_CONSOLAS14              1

/*
 * Enable Consolas 23pt font.
 * Standard default: 0
 */
#define CONFIG_SGL_FONT_CONSOLAS23              0

/*
 * Enable Consolas 24pt font (clean monospace, suitable for general UI).
 * Standard default: 0
 */
#define CONFIG_SGL_FONT_CONSOLAS24              0

/*
 * Enable Consolas 32pt font (large display font).
 * Standard default: 0
 */
#define CONFIG_SGL_FONT_CONSOLAS32              0

//====================================================================
// Theme
//====================================================================

/*
 * The following block is copied verbatim from sgl_cfgfix.h.
 * It ensures that if neither CONFIG_SGL_THEME_DARK nor
 * CONFIG_SGL_THEME_LIGHT is defined, the default theme is enabled.
 * You may uncomment one of the theme macros below to select a specific
 * theme, but do not enable more than one at a time.
 *
 * Available themes:
 *   CONFIG_SGL_THEME_DEFAULT - High contrast, black on white
 *   CONFIG_SGL_THEME_DARK    - Dark background, light foreground
 *   CONFIG_SGL_THEME_LIGHT   - Light blue accents, white background
 */
#define CONFIG_SGL_THEME_LIGHT 1

#if !(CONFIG_SGL_THEME_DARK || CONFIG_SGL_THEME_LIGHT)
#   ifndef CONFIG_SGL_THEME_DEFAULT
#   define CONFIG_SGL_THEME_DEFAULT             (1)  /* Default theme if not specified */
#   endif
#endif

//====================================================================
// Boot Logo
//====================================================================

/*
 * Show the boot logo / boot animation during initialization.
 * When enabled, sgl_boot_animation() can be called once during init.
 * Standard default: 1
 */
#define CONFIG_SGL_BOOT_LOGO                    0

#endif // __SGL_CONFIG_H__