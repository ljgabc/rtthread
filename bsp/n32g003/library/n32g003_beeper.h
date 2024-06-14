/**
*     Copyright (c) 2022, Nations Technologies Inc.
* 
*     All rights reserved.
*
*     This software is the exclusive property of Nations Technologies Inc. (Hereinafter 
* referred to as NATIONS). This software, and the product of NATIONS described herein 
* (Hereinafter referred to as the Product) are owned by NATIONS under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NATIONS does not grant any license under its patents, copyrights, trademarks, or other 
* intellectual property rights. Names and brands of third party may be mentioned or referred 
* thereto (if any) for identification purposes only.
*
*     NATIONS reserves the right to make changes, corrections, enhancements, modifications, and 
* improvements to this software at any time without notice. Please contact NATIONS and obtain 
* the latest version of this software before placing orders.

*     Although NATIONS has attempted to provide accurate and reliable information, NATIONS assumes 
* no responsibility for the accuracy and reliability of this software.
* 
*     It is the responsibility of the user of this software to properly design, program, and test 
* the functionality and safety of any application made of this information and any resulting product. 
* In no event shall NATIONS be liable for any direct, indirect, incidental, special,exemplary, or 
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NATIONS Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property 
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NATIONS and hold NATIONS 
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not 
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NATIONS, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NATIONS products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
* User shall comply with any applicable export control laws and regulations promulgated and administered by 
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/

/**
*\*\file n32g003_beeper.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#ifndef __N32G003_BEEPER_H__
#define __N32G003_BEEPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g003.h"


#define BEEPER_ENABLE                 (BEEPER_CTRL_EN)   /* Enable Beeper */
#define BEEPER_DISABLE                (~BEEPER_CTRL_EN)  /* Disable Beeper */

#define BEEPER_INV_ENABLE             (BEEPER_CTRL_INV_EN)    /* Only one output, the other output is off */
#define BEEPER_INV_DISABLE            (~BEEPER_CTRL_INV_EN)   /* Both outputs are on, and the outputs are complementary */

#define BEEPER_FREQ_SEL_MASK          (~BEEPER_CTRL_FREQ_SEL)

#define BEEPER_FREQ_8KHZ              (BEEPER_CTRL_FREQ_SEL_8K)
#define BEEPER_FREQ_4KHZ              (BEEPER_CTRL_FREQ_SEL_4K)
#define BEEPER_FREQ_2KHZ              (BEEPER_CTRL_FREQ_SEL_2K)
#define BEEPER_FREQ_1KHZ              (BEEPER_CTRL_FREQ_SEL_1K)



void BEEPER_Reset(void);
void BEEPER_Frequency_Select(uint32_t freq_selection);
void BEEPER_Inverted_Enable(void);
void BEEPER_Inverted_Disable(void);
void BEEPER_Enable(void);
void BEEPER_Disable(void);

#ifdef __cplusplus
}
#endif

#endif /* __N32G003_BEEPER_H__ */



