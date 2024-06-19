/**
 * @file bsp.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "config.h"

#ifdef CONFIG_BSP_N32G003
#include "bsp/n32g003/board.c"
#endif

#ifdef CONFIG_BSP_N32G430
#include "bsp/n32g430/board.c"
#endif