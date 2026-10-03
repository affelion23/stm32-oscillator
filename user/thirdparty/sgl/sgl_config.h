//********************************************************************
//* SGL Configuration File                                           *
//* You can modify the following parameters according to your needs. *
//********************************************************************

#ifndef __SGL_CONFIG_H__
#define __SGL_CONFIG_H__

#define CONFIG_SGL_FBDEV_PIXEL_DEPTH 16   // 颜色深度，这里是16位，即RGB565
#define CONFIG_SGL_FBDEV_PIXEL_MODE_MSB 1 // 自定义配置, 适配ST7789MSB模式
#define CONFIG_SGL_FBDEV_ROTATION                                              \
    270 // 屏幕旋转角度，软件旋转，这里设置为0度，即不做旋转
#define CONFIG_SGL_FBDEV_RUNTIME_ROTATION 0 // 屏幕实时旋转角度
#define CONFIG_SGL_SYSTICK_MS 50            // SGL图形刷新事件间隔
#define CONFIG_SGL_EVENT_QUEUE_SIZE 16      // 事件队列大小
#define CONFIG_SGL_DIRTY_AREA_NUM_MAX 16    // 脏区域最大数量
#define CONFIG_SGL_ANIMATION 1              // 是否启用动画，这里启用动画
#define CONFIG_SGL_DEBUG                                                       \
    0 // 是否启用日志，这里不启用日志，项目发布时，请关闭日志
#define CONFIG_SGL_LOG_COLOR 1 // 是否启用日志颜色，这里启用日志颜色
#define CONFIG_SGL_LOG_LEVEL                                                   \
    0 // 日志等级，0为全部输出，1为错误输出，2为警告输出，3为信息输出，4为调试输出
#define CONFIG_SGL_OBJ_USE_NAME 0    // 是否启用对象名称，这里不启用对象名称
#define CONFIG_SGL_FONT_COMPRESSED 1 // 是否启用字体压缩，这里启用字体压缩
#define CONFIG_SGL_BOOT_LOGO 1       /// 是否启用启动logo，这里启用开机logo
#define CONFIG_SGL_HEAP_ALGO lwmem   // 内存管理算法，这里选择lwmem
#define CONFIG_SGL_HEAP_MEMORY_SIZE 10240 // 内存大小，这里设置为10KB
#define CONFIG_SGL_FONT_SONG23 0          // 是否启用宋体23号字体, 默认关闭
#define CONFIG_SGL_FONT_CONSOLAS14 0      // 是否启用Consolas14号字体, 默认关闭
#define CONFIG_SGL_FONT_CONSOLAS23 0      // 是否启用Consolas23号字体, 默认关闭
#define CONFIG_SGL_FONT_CONSOLAS24 1      // 是否启用Consolas24号字体, 默认开启
#define CONFIG_SGL_FONT_CONSOLAS32 0      // 是否启用Consolas32号字体, 默认关闭
#define CONFIG_SGL_FONT_CONSOLAS24_COMPRESS                                    \
    1 // 是否启用Consolas24号字体压缩，这里启用字体压缩

#endif // __SGL_CONFIG_H__