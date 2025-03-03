#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

#include "config.h"
/* RT-Thread Configuration */

/* RT-Thread Kernel */
#define RT_USING_NANO CONFIG_RT_USING_NANO

#define RT_NAME_MAX CONFIG_RT_NAME_MAX
#define RT_CPUS_NR CONFIG_RT_CPUS_NR
#define RT_ALIGN_SIZE CONFIG_RT_ALIGN_SIZE
#define RT_THREAD_PRIORITY_MAX CONFIG_RT_THREAD_PRIORITY_MAX
#define RT_TICK_PER_SECOND CONFIG_RT_TICK_PER_SECOND
// #define RT_USING_OVERFLOW_CHECK

// #define RT_BACKTRACE_LEVEL_MAX_NR 

#ifdef CONFIG_RT_USING_HOOK
#define RT_USING_HOOK
#define RT_HOOK_USING_FUNC_PTR

#ifdef CONFIG_RT_USING_IDLE_HOOK
#define RT_USING_IDLE_HOOK 
#define RT_IDLE_HOOK_LIST_SIZE CONFIG_RT_IDLE_HOOK_LIST_SIZE
#endif

#endif

#define IDLE_THREAD_STACK_SIZE CONFIG_IDLE_THREAD_STACK_SIZE

#ifdef CONFIG_RT_USING_TIMER_SOFT
#define RT_USING_TIMER_SOFT	1
#define RT_TIMER_THREAD_PRIO CONFIG_RT_TIMER_THREAD_PRIO
#define RT_TIMER_THREAD_STACK_SIZE CONFIG_RT_TIMER_THREAD_STACK_SIZE
#endif

/* kservice optimization */

// #define RT_KSERVICE_USING_TINY_SIZE
// #define RT_USING_TINY_FFS

// #define RT_USING_DEBUG
// #define RT_DEBUGING_COLOR
// #define RT_DEBUGING_CONTEXT

/* Inter-Thread communication */

#ifdef CONFIG_RT_USING_SEMAPHORE
#define RT_USING_SEMAPHORE
#endif

#ifdef CONFIG_RT_USING_MUTEX
#define RT_USING_MUTEX
#endif


// #define RT_USING_EVENT
// #define RT_USING_MAILBOX

#ifdef CONFIG_RT_USING_MESSAGEQUEUE
#define RT_USING_MESSAGEQUEUE
#endif

/* Memory Management */

// #define RT_USING_MEMPOOL
#ifdef CONFIG_RT_USING_SMALL_MEM
#define RT_USING_HEAP
#define RT_USING_SMALL_MEM
#define RT_USING_SMALL_MEM_AS_HEAP
#endif

#ifdef CONFIG_USING_DEVICE
#define RT_USING_DEVICE
#define RT_USING_DEVICE_OPS
#endif
// #define RT_USING_CONSOLE
// #define RT_CONSOLEBUF_SIZE 128
// #define RT_CONSOLE_DEVICE_NAME "uart1"
// #define RT_VER_NUM 0x50100
#define RT_BACKTRACE_LEVEL_MAX_NR 32

/* RT-Thread Components */

#ifdef CONFIG_RT_USING_COMPONENTS_INIT 
#define RT_USING_COMPONENTS_INIT
#endif

#ifdef CONFIG_RT_USING_USER_MAIN
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE CONFIG_RT_MAIN_THREAD_STACK_SIZE
#define RT_MAIN_THREAD_PRIORITY CONFIG_RT_MAIN_THREAD_PRIORITY
#endif

// #define RT_USING_MSH
// #define RT_USING_FINSH
// #define FINSH_USING_MSH
// #define FINSH_THREAD_NAME "tshell"
// #define FINSH_THREAD_PRIORITY 20
// #define FINSH_THREAD_STACK_SIZE 4096
// #define FINSH_USING_HISTORY
// #define FINSH_HISTORY_LINES 5
// #define FINSH_USING_SYMTAB
// #define FINSH_CMD_SIZE 80
// #define MSH_USING_BUILT_IN_COMMANDS
// #define FINSH_USING_DESCRIPTION
// #define FINSH_ARG_MAX 10
// #define FINSH_USING_OPTION_COMPLETION

/* DFS: device virtual file system */


/* Device Drivers */
#ifdef CONFIG_USING_PIN
#define RT_USING_PIN
#endif

#ifdef CONFIG_USING_TIM
#define RT_USING_HWTIMER
#endif

#ifdef CONFIG_USING_SERIAL
#define RT_USING_SERIAL
#endif

#ifdef CONFIG_USING_ADC
#define RT_USING_ADC
#endif

#ifdef CONFIG_USING_I2C
#define RT_USING_I2C
#endif

#ifdef CONFIG_USING_DAC
#define RT_USING_DAC
#endif

#ifdef CONFIG_USING_RINGBUFFER
#define RT_USING_RINGBUFFER
#endif

#ifdef CONFIG_USING_COMPLETION
#define RT_USING_COMPLETION
#endif

#ifdef CONFIG_USING_DATAQUEUE
#define RT_USING_DATAQUEUE
#endif

#ifdef CONFIG_USING_WORKQUEUE
#define RT_USING_WORKQUEUE
#endif
// #define RT_USING_DEVICE_IPC
// #define RT_UNAMED_PIPE_NUMBER 64
// #define RT_USING_SERIAL
// #define RT_USING_SERIAL_V1
// #define RT_SERIAL_USING_DMA
// #define RT_SERIAL_RB_BUFSZ 64
// #define RT_USING_PIN

/* Using USB */


/* C/C++ and POSIX layer */

/* ISO-ANSI C layer */

/* Timezone and Daylight Saving Time */

// #define RT_LIBC_USING_LIGHT_TZ_DST
// #define RT_LIBC_TZ_DEFAULT_HOUR 8
// #define RT_LIBC_TZ_DEFAULT_MIN 0
// #define RT_LIBC_TZ_DEFAULT_SEC 0

/* POSIX (Portable Operating System Interface) layer */


/* Interprocess Communication (IPC) */


/* Socket is in the 'Network' category */


/* Network */


/* Memory protection */


/* Utilities */


/* RT-Thread Utestcases */


/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* CYW43012 WiFi */


/* BL808 WiFi */


/* CYW43439 WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */

/* JSON: JavaScript Object Notation, a lightweight data-interchange format */


/* XML: Extensible Markup Language */


/* multimedia packages */

/* LVGL: powerful and easy-to-use embedded GUI library */


/* u8g2: a monochrome graphic library */


/* tools packages */


/* system packages */

/* enhanced kernel services */


/* acceleration: Assembly language or algorithmic acceleration packages */


/* CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */


/* Micrium: Micrium software products porting for RT-Thread */


/* peripheral libraries and drivers */

/* HAL & SDK Drivers */

/* STM32 HAL & SDK Drivers */


/* Kendryte SDK */


/* sensors drivers */


/* touch drivers */


/* AI packages */


/* Signal Processing and Control Algorithm Packages */


/* miscellaneous packages */

/* project laboratory */

/* samples: kernel and components samples */


/* entertainment: terminal games and other interesting software packages */


/* Arduino libraries */


/* Projects and Demos */


/* Sensors */


/* Display */


/* Timing */


/* Data Processing */


/* Data Storage */

/* Communication */


/* Device Control */


/* Other */


/* Signal IO */


/* Uncategorized */

/* Hardware Drivers Config */

// #define SOC_N32G452XX

/* Onboard Peripheral Drivers */

// #define BSP_USING_UART

/* On-chip Peripheral Drivers */

// #define N32G45X_PIN_NUMBERS_64
// #define N32G45X_PIN_NUMBERS 64
// #define BSP_USING_GPIO

/* Remap JTAG Port */

// #define BSP_RMP_SW_JTAG_SW_ENABLE
// #define BSP_USING_UART1
// #define BSP_USING_UART1_NO_RMP

#define __RT_KERNEL_SOURCE__
// #define __RT_IPC_SOURCE__

#endif
