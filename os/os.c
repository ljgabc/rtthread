/**
 * @file os.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "config.h"

#include "os/object.c"
#include "os/kservice.c"

#if CONFIG_USE_RTOS
#include "os/clock.c"
#include "os/components.c"
#include "os/idle.c"
#include "os/ipc.c"
#include "os/irq.c"
#include "os/mem.c"
#include "os/scheduler.c"
#include "os/thread.c"
#include "os/timer.c"
#endif
