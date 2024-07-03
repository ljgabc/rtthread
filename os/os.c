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
#include "rtthread.h"

#include "os/klibc/kstdio.c"
#include "os/klibc/kstring.c"
#include "os/object.c"
#include "os/kservice.c"
#include "os/clock.c"
#include "os/components.c"
#include "os/idle.c"
#include "os/ipc.c"
#include "os/irq.c"
#ifdef RT_USING_SMALL_MEM
#include "os/mem.c"
#endif
#include "os/scheduler_comm.c"
#include "os/scheduler_up.c"
#include "os/thread.c"
#include "os/timer.c"