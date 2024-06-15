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
*\*\file n32g003_tim.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
**/
#ifndef __N32G003_TIM_H__
#define __N32G003_TIM_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "bsp/n32g003/library/n32g003.h"
#include <stdbool.h>


/**
*\*\brief  TIM Time Base Init structure definition
*\*\note   This structure is used with all TIMx except for TIM6 and TIM7.
**/

typedef struct
{
    uint16_t Prescaler; /* Specifies the prescaler value used to divide the TIM clock.
                                 This parameter can be a number between 0x0000 and 0xFFFF */

    uint16_t CntMode; /* Specifies the counter mode.
                                   This parameter can be a value of TIM_Counter_Mode */

    uint16_t Period; /* Specifies the period value to be loaded into the active
                              Auto-Reload Register at the next update event.
                              This parameter must be a number between 0x0000 and 0xFFFF.  */

    uint16_t ClkDiv; /* Specifies the clock division.
                                    This parameter can be a value of TIM_Clock_Division_CKD */

    uint8_t RepetCnt; /* Specifies the repetition counter value. Each time the REPCNT downcounter
                                        reaches zero, an update event is generated and counting restarts
                                        from the REPCNT value (N).
                                        This means in PWM mode that (N+1) corresponds to:
                                           - the number of PWM periods in edge-aligned mode
                                           - the number of half PWM period in center-aligned mode
                                        This parameter must be a number between 0x00 and 0xFF.
                                        @note This parameter is valid only for TIM1 and TIM8. */

    bool CapCh1Sel;    /* channel 1 select capture in from comp if 1, from IOM if 0
                                Tim1,Tim3 valid*/
    bool CapEtrClrFromCompEn; /* etr clearref select from comp if 1, from ETR IOM if 0
                                Tim1,Tim3 valid*/
    bool CapEtrSelFromTscEn;  
} TIM_TimeBaseInitType;

/**
*\*\brief  TIM Output Compare Init structure definition
**/

typedef struct
{
    uint16_t OcMode; /* Specifies the TIM mode.
                              This parameter can be a value of TIM_Output_Compare_and_PWM_modes */

    uint16_t OutputState; /* Specifies the TIM Output Compare state.
                                   This parameter can be a value of TIM_Output_Compare_state */

    uint16_t OutputNState; /* Specifies the TIM complementary Output Compare state.
                                    This parameter can be a value of TIM_Output_Compare_N_state
                                    - note This parameter is valid only for TIM1. */

    uint16_t Pulse; /* Specifies the pulse value to be loaded into the Capture Compare Register.
                             This parameter can be a number between 0x0000 and 0xFFFF */

    uint16_t OcPolarity; /* Specifies the output polarity.
                                  This parameter can be a value of TIM_Output_Compare_Polarity */

    uint16_t OcNPolarity; /* Specifies the complementary output polarity.
                                   This parameter can be a value of TIM_Output_Compare_N_Polarity
                                   - note This parameter is valid only for TIM1. */

    uint16_t OcIdleState; /* Specifies the TIM Output Compare pin state during Idle state.
                                   This parameter can be a value of TIM_Output_Compare_Idle_State
                                   - note This parameter is valid only for TIM1. */

    uint16_t OcNIdleState; /* Specifies the TIM Output Compare pin state during Idle state.
                                    This parameter can be a value of TIM_Output_Compare_N_Idle_State
                                    - note This parameter is valid only for TIM1. */
} OCInitType;

/**
*\*\brief  TIM Input Capture Init structure definition
**/

typedef struct
{
    uint16_t Channel; /*! Specifies the TIM channel.This parameter can be a value of Channel */

    uint16_t IcPolarity; /* Specifies the active edge of the input signal. */

    uint16_t IcSelection; /* Specifies the input. */

    uint16_t IcPrescaler; /* Specifies the Input Capture Prescaler. */

    uint16_t IcFilter; /* Specifies the input capture filter. */
} TIM_ICInitType;

/**
*\*\brief  BKDT structure definition
*\*\note   This structure is used only with TIM1 and TIM8.
**/

typedef struct
{
    uint16_t OssrState; /* Specifies the Off-State selection used in Run mode.
                                 This parameter can be a value of OSSR_Off_State_Selection_for_Run_mode_state */

    uint16_t OssiState; /* Specifies the Off-State used in Idle state.
                                 This parameter can be a value of OSSI_Off_State_Selection_for_Idle_mode_state */

    uint16_t LockLevel; /* Specifies the LOCK level parameters.
                                 This parameter can be a value of Lock_level */

    uint16_t DeadTime; /* Specifies the delay time between the switching-off and the
                                switching-on of the outputs.
                                This parameter can be a number between 0x00 and 0xFF  */

    uint16_t Break; /* Specifies whether the TIM Break input is enabled or not.
                             This parameter can be a value of Break_Input_enable_disable */

    uint16_t BreakPolarity; /* Specifies the TIM Break Input pin polarity.
                                     This parameter can be a value of Break_Polarity */

    uint16_t AutomaticOutput; /* Specifies whether the TIM Automatic Output feature is enabled or not.
                                       This parameter can be a value of TIM_AOE_Bit_Set_Reset */
    bool IomBreakEn;          /* EXTENDMODE valid, open iom as break in*/
    bool LockUpBreakEn;       /* EXTENDMODE valid, open lockup(hardfault) as break in*/
    bool PvdBreakEn;          /* EXTENDMODE valid, open pvd(sys voltage too high or too low) as break in*/
} TIM_BDTRInitType;




#define TIM_REG16_BIT_ZERO    ((uint16_t)0x0000)
#define TIM_REG32_BIT_ZERO  ((uint32_t)0x00000000)

#define TIM_AR_PRELOAD_MASK     (TIM_CTRL1_ARPEN)
/** TIM_AR_PRELOAD **/
#define TIM_AR_PRELOAD_ENABLE   (TIM_CTRL1_ARPEN)
#define TIM_AR_PRELOAD_DISABLE  (TIM_REG32_BIT_ZERO)

#define TIM_UPRS_MASK       (TIM_CTRL1_UPRS)
/** TIM_Update_Source **/
#define TIM_UPRS_GLOBAL     (TIM_REG32_BIT_ZERO)
#define TIM_UPRS_REGULAR    (TIM_CTRL1_UPRS)

#define TIM_UPDATE_SRC_GLOBAL   (TIM_REG16_BIT_ZERO) /* Source of update is the counter overflow/underflow              \
                                                         or the setting of UG bit, or an update generation             \
                                                         through the slave mode controller. */
#define TIM_UPDATE_SRC_REGULAR ((uint16_t)0x0001) /* Source of update is counter overflow/underflow. */

#define TIM_UPDIS_MASK             (TIM_CTRL1_UPDIS)
/** TIM_Update_Disable **/
#define TIM_UPDATE_EVENT_DISABLE   (TIM_CTRL1_UPDIS)
#define TIM_UPDATE_EVENT_ENABLE    (TIM_REG32_BIT_ZERO)

#define TIM_ON_MASK                (TIM_CTRL1_CNTEN)
/** TIM_On **/
#define TIM_ON                     (TIM_CTRL1_CNTEN)

/* TIM_Break_In_Source */
#define TIM_LOCKUP_AS_BREAK_IN                  (TIM_CTRL1_LBKPEN)
#define TIM_PVD_AS_BREAK_IN                     (TIM_CTRL1_PBKPEN)
#define TIM_COMP_AS_BREAK_IN                    (TIM_CTRL1_IOMBKPEN)

#define TIM_CCUSEL_MASK                         (TIM_CTRL2_CCUSEL)
/** TIM_CCUSEL **/
#define TIM_CCUSEL_COM_AND_TRIG                 (TIM_CTRL2_CCUSEL)
#define TIM_CCUSEL_COM                          (TIM_REG32_BIT_ZERO)

/** TIM_CCPCTL **/
#define TIM_CCPCTL_MASK                         (TIM_CTRL2_CCPCTL)
#define TIM_CCPCTL_PRELOAD                      (TIM_CTRL2_CCPCTL)
#define TIM_CCPCTL_NOT_PRELOAD                  (TIM_REG32_BIT_ZERO)

/** TIM_REPCNT MASK**/
#define TIM_REPCNT_MASK             (TIM_REPCNT_REPCNT)

#define TIM_OC1MODE_MASK            (TIM_CCMOD1_OC1MD)
#define TIM_OC2MODE_MASK            (TIM_CCMOD1_OC2MD)
#define TIM_OC3MODE_MASK            (TIM_CCMOD2_OC3MD)
#define TIM_OC4MODE_MASK            (TIM_CCMOD2_OC4MD)
#define TIM_OC5MODE_MASK            (TIM_CCMOD3_OC5MD)
/** TIM_Output_Compare_and_PWM_modes**/
#define TIM_OCMODE_TIMING           (TIM_REG16_BIT_ZERO)
#define TIM_OCMODE_ACTIVE           (TIM_CCMOD1_OC1MD_0)
#define TIM_OCMODE_INACTIVE         (TIM_CCMOD1_OC1MD_1)
#define TIM_OCMODE_TOGGLE           (TIM_CCMOD1_OC1MD_0 | TIM_CCMOD1_OC1MD_1)
#define TIM_OCMODE_PWM1             (TIM_CCMOD1_OC1MD_2 | TIM_CCMOD1_OC1MD_1)
#define TIM_OCMODE_PWM2             (TIM_CCMOD1_OC1MD_2 | TIM_CCMOD1_OC1MD_1 | TIM_CCMOD1_OC1MD_2 | TIM_CCMOD1_OC1MD_0)
#define TIM_FORCED_ACTION_ACTIVE    (TIM_CCMOD1_OC1MD_2 | TIM_CCMOD1_OC1MD_0)
#define TIM_FORCED_ACTION_INACTIVE  (TIM_CCMOD1_OC1MD_2)

#define TIM_OPMODE_MASK     (TIM_CTRL1_ONEPM)
/** TIM_One_Pulse_Mode **/
#define TIM_OPMODE_SINGLE   (TIM_CTRL1_ONEPM)
#define TIM_OPMODE_REPET    (TIM_REG16_BIT_ZERO)

/** Channel **/
#define TIM_CH_1 ((uint16_t)0x0000)
#define TIM_CH_2 ((uint16_t)0x0004)
#define TIM_CH_3 ((uint16_t)0x0008)
#define TIM_CH_4 ((uint16_t)0x000C)
#define TIM_CH_5 ((uint16_t)0x0010)

#define TIM_CLK_DIV_MASK     (TIM_CTRL1_CLKD)
/** TIM_Clock_Division_CKD **/
#define TIM_CLK_DIV1         (TIM_REG16_BIT_ZERO)
#define TIM_CLK_DIV2         (TIM_CTRL1_CLKD_0)
#define TIM_CLK_DIV4         (TIM_CTRL1_CLKD_1)

#define TIM_CNT_MODE_MASK          (TIM_CTRL1_DIR | TIM_CTRL1_CAMSEL)
/** TIM_Counter_Mode **/
#define TIM_CNT_MODE_UP            (TIM_REG16_BIT_ZERO)
#define TIM_CNT_MODE_DOWN          (TIM_CTRL1_DIR)
#define TIM_CNT_MODE_CENTER_ALIGN1 (TIM_CTRL1_CAMSEL_0)
#define TIM_CNT_MODE_CENTER_ALIGN2 (TIM_CTRL1_CAMSEL_1)
#define TIM_CNT_MODE_CENTER_ALIGN3 (TIM_CTRL1_CAMSEL_1 | TIM_CTRL1_CAMSEL_0)





/** TIM_CHxSEL **/
#define TIM_CH1_SEL                (TIM_CTRL1_C1SEL)


/** TIM_OCREF_CLEAR_SEL **/
#define TIM_OCREF_CLEAR_SEL        (TIM_CTRL1_CLRSEL)

/** TIM_TI1_SEL **/
#define TIM_TI1_SEL                (TIM_CTRL2_TI1SEL)


/** TIM_ETR_Selection **/
#define TIM_ETR_SEL                (TIM_CTRL2_TSCSEL)


#define TIM_OC_POLARITY_MASK       (TIM_CCEN_CC1P)
/** TIM_Output_Compare_Polarity **/
#define TIM_OC_POLARITY_HIGH       (TIM_REG16_BIT_ZERO)
#define TIM_OC_POLARITY_LOW        (TIM_CCEN_CC1P)

#define TIM_OCN_POLARITY_MASK      (TIM_CCEN_CC1NP)
/** TIM_Output_Compare_N_Polarity **/
#define TIM_OCN_POLARITY_HIGH      (TIM_REG16_BIT_ZERO)
#define TIM_OCN_POLARITY_LOW       (TIM_CCEN_CC1NP)

/** TIM_Output_Compare_state **/
#define TIM_OUTPUT_STATE_DISABLE (TIM_REG16_BIT_ZERO)
#define TIM_OUTPUT_STATE_ENABLE  (TIM_CCEN_CC1EN)

/** TIM_Output_Compare_N_state **/
#define TIM_OUTPUT_NSTATE_DISABLE (TIM_REG16_BIT_ZERO)
#define TIM_OUTPUT_NSTATE_ENABLE  (TIM_CCEN_CC1NEN)

/** TIM_Capture_Compare_state **/
#define TIM_CAP_CMP_ENABLE    (TIM_CCEN_CC1EN)
#define TIM_CAP_CMP_DISABLE   (TIM_REG16_BIT_ZERO)

/** TIM_Capture_Compare_N_state **/
#define TIM_CAP_CMP_N_ENABLE    (TIM_CCEN_CC1NEN)
#define TIM_CAP_CMP_N_DISABLE   (TIM_REG16_BIT_ZERO)

/** Break_Input_enable_disable **/
#define TIM_BREAK_IN_ENABLE      (TIM_BKDT_BKEN)
#define TIM_BREAK_IN_DISABLE     (TIM_REG16_BIT_ZERO)

/** Break_Polarity **/
#define TIM_BREAK_POLARITY_LOW       (TIM_REG16_BIT_ZERO)
#define TIM_BREAK_POLARITY_HIGH      (TIM_BKDT_BKP)

/** TIM_AOEN_Bit_Set_Reset **/
#define TIM_AUTO_OUTPUT_ENABLE      (TIM_BKDT_AOEN)
#define TIM_AUTO_OUTPUT_DISABLE     (TIM_REG16_BIT_ZERO)

/** TIM_MOEN_Bit_Set_Reset **/
#define TIM_MAIN_OUTPUT_ENABLE      (TIM_BKDT_MOEN)
#define TIM_MAIN_OUTPUT_DISABLE     (TIM_REG16_BIT_ZERO)

/** Lock_level **/
#define TIM_LOCK_LEVEL_OFF (TIM_REG16_BIT_ZERO)
#define TIM_LOCK_LEVEL_1   (TIM_BKDT_LCKCFG_0)
#define TIM_LOCK_LEVEL_2   (TIM_BKDT_LCKCFG_1)
#define TIM_LOCK_LEVEL_3   (TIM_BKDT_LCKCFG_1 | TIM_BKDT_LCKCFG_0)

/** OSSI_Off_State_Selection_for_Idle_mode_state **/
#define TIM_OSSI_STATE_ENABLE  (TIM_BKDT_OSSI)
#define TIM_OSSI_STATE_DISABLE (TIM_REG16_BIT_ZERO)

/** OSSR_Off_State_Selection_for_Run_mode_state **/
#define TIM_OSSR_STATE_ENABLE  (TIM_BKDT_OSSR)
#define TIM_OSSR_STATE_DISABLE (TIM_REG16_BIT_ZERO)

#define TIM_OC1_IDLE_STATE_MASK           (TIM_CTRL2_OI1)
#define TIM_OC2_IDLE_STATE_MASK           (TIM_CTRL2_OI2)
#define TIM_OC3_IDLE_STATE_MASK           (TIM_CTRL2_OI3)
#define TIM_OC4_IDLE_STATE_MASK           (TIM_CTRL2_OI4)
#define TIM_OC5_IDLE_STATE_MASK           (TIM_CTRL2_OI5)
/** TIM_Output_Compare_Idle_State **/
#define TIM_OC_IDLE_STATE_SET             (TIM_CTRL2_OI1)
#define TIM_OC_IDLE_STATE_RESET           (TIM_REG16_BIT_ZERO)

#define TIM_OC1N_IDLE_STATE_MASK           (TIM_CTRL2_OI1N)
#define TIM_OC2N_IDLE_STATE_MASK           (TIM_CTRL2_OI2N)
#define TIM_OC3N_IDLE_STATE_MASK           (TIM_CTRL2_OI3N)
/** TIM_Output_Compare_N_Idle_State **/
#define TIM_OCN_IDLE_STATE_SET   (TIM_CTRL2_OI1N)
#define TIM_OCN_IDLE_STATE_RESET (TIM_REG16_BIT_ZERO)

/** TIM_Input_Capture_Polarity **/
#define TIM_IC_POLARITY_RISING   (TIM_REG16_BIT_ZERO)
#define TIM_IC_POLARITY_FALLING  (TIM_CCEN_CC1P)
#define TIM_IC_POLARITY_BOTHEDGE (TIM_CCEN_CC1P | TIM_CCEN_CC1NP)

#define TIM_IC1_SELECTION_MASK         (TIM_CCMOD1_CC1SEL)
#define TIM_IC2_SELECTION_MASK         (TIM_CCMOD1_CC2SEL)
#define TIM_IC3_SELECTION_MASK         (TIM_CCMOD2_CC3SEL)
#define TIM_IC4_SELECTION_MASK         (TIM_CCMOD2_CC4SEL)
/** TIM_Input_Capture_Selection **/
#define TIM_IC_SELECTION_DIRECTTI     (TIM_CCMOD1_CC1SEL_0) /* TIM Input 1, 2, 3 or 4 is selected to be connected to IC1, IC2, IC3 or IC4, respectively */
#define TIM_IC_SELECTION_INDIRECTTI   (TIM_CCMOD1_CC1SEL_1) /* TIM Input 1, 2, 3 or 4 is selected to be connected to IC2, IC1, IC4 or IC3, respectively. */
#define TIM_IC_SELECTION_TRC (TIM_CCMOD1_CC1SEL_1 | TIM_CCMOD1_CC1SEL_0) /* TIM Input 1, 2, 3 or 4 is selected to be connected to TRC. */

/** TIM_Input_Capture1_Filter **/
#define TIM_IC1_FILTER_MASK   (TIM_CCMOD1_IC1F)

/** TIM_Input_Capture2_Filter **/
#define TIM_IC2_FILTER_MASK   (TIM_CCMOD1_IC2F)

/** TIM_Input_Capture3_Filter **/
#define TIM_IC3_FILTER_MASK   (TIM_CCMOD2_IC3F)

/** TIM_Input_Capture4_Filter **/
#define TIM_IC4_FILTER_MASK   (TIM_CCMOD2_IC4F)

#define TIM_IC1_PSC_MASK     (TIM_CCMOD1_IC1PSC)
#define TIM_IC2_PSC_MASK     (TIM_CCMOD1_IC2PSC)
#define TIM_IC3_PSC_MASK     (TIM_CCMOD2_IC3PSC)
#define TIM_IC4_PSC_MASK     (TIM_CCMOD2_IC4PSC)
/** TIM_Input_Capture_Prescaler **/
#define TIM_IC_PSC_DIV1      (TIM_REG16_BIT_ZERO)
#define TIM_IC_PSC_DIV2      (TIM_CCMOD1_IC1PSC_0) /* Capture performed once every 2 events. */
#define TIM_IC_PSC_DIV4      (TIM_CCMOD1_IC1PSC_1) /* Capture performed once every 4 events. */
#define TIM_IC_PSC_DIV8      (TIM_CCMOD1_IC1PSC_1 | TIM_CCMOD1_IC1PSC_0) /* Capture performed once every 8 events. */

/** TIM_interrupt_sources **/
#define TIM_INT_UPDATE (TIM_STS_UDITF)
#define TIM_INT_CC1    (TIM_STS_CC1ITF)
#define TIM_INT_CC2    (TIM_STS_CC2ITF)
#define TIM_INT_CC3    (TIM_STS_CC3ITF)
#define TIM_INT_CC4    (TIM_STS_CC4ITF)
#define TIM_INT_COM    (TIM_STS_COMITF)
#define TIM_INT_TRIG   (TIM_STS_TITF)
#define TIM_INT_BREAK  (TIM_STS_BITF)


/** TIM_External_Trigger_Prescaler **/
#define TIM_EXT_TRG_PSC_MASK     (TIM_SMCTRL_EXTPS)
#define TIM_EXT_TRG_PSC_OFF      (TIM_REG16_BIT_ZERO)
#define TIM_EXT_TRG_PSC_DIV2     (TIM_SMCTRL_EXTPS_0)
#define TIM_EXT_TRG_PSC_DIV4     (TIM_SMCTRL_EXTPS_1)
#define TIM_EXT_TRG_PSC_DIV8     (TIM_SMCTRL_EXTPS_1 | TIM_SMCTRL_EXTPS_0)

#define TIM_TRIG_SEL_MASK    (TIM_SMCTRL_TSEL)
/** TIM_Internal_Trigger_Selection **/
#define TIM_TRIG_SEL_IN_TR0  (TIM_REG16_BIT_ZERO)
#define TIM_TRIG_SEL_IN_TR1  (TIM_SMCTRL_TSEL_0)
#define TIM_TRIG_SEL_IN_TR2  (TIM_SMCTRL_TSEL_1)
#define TIM_TRIG_SEL_IN_TR3  (TIM_SMCTRL_TSEL_1 | TIM_SMCTRL_TSEL_0)
#define TIM_TRIG_SEL_TI1F_ED (TIM_SMCTRL_TSEL_2)
#define TIM_TRIG_SEL_TI1FP1  (TIM_SMCTRL_TSEL_2 | TIM_SMCTRL_TSEL_0)
#define TIM_TRIG_SEL_TI2FP2  (TIM_SMCTRL_TSEL_2 | TIM_SMCTRL_TSEL_1)
#define TIM_TRIG_SEL_ETRF    (TIM_SMCTRL_TSEL_2 | TIM_SMCTRL_TSEL_1 | TIM_SMCTRL_TSEL_0)

/** TIM_TIx_External_Clock_Source **/
#define TIM_EXT_CLK_SRC_TI1   (TIM_SMCTRL_TSEL_2 | TIM_SMCTRL_TSEL_0)
#define TIM_EXT_CLK_SRC_TI2   (TIM_SMCTRL_TSEL_2 | TIM_SMCTRL_TSEL_1)
#define TIM_EXT_CLK_SRC_TI1ED (TIM_SMCTRL_TSEL_2)

/** TIM_External_Trigger_Polarity **/
#define TIM_EXT_TRIG_POLARITY_INVERTED    (TIM_SMCTRL_EXTP)
#define TIM_EXT_TRIG_POLARITY_NONINVERTED (TIM_REG16_BIT_ZERO)

/** TIM_TIx_External_Clock_Filter **/
#define TIM_EXT_TRIG_FILTER_MASK    (TIM_SMCTRL_EXTF)

/** TIM_External_Clock_Mode2 **/
#define TIM_EXT_CLOCK_MODE2_ENABLE    (TIM_SMCTRL_EXCEN)

#define TIM_PSC_RELOAD_MODE_MASK      (TIM_EVTGEN_UDGN)
/** TIM_Prescaler_Reload_Mode **/
#define TIM_PSC_RELOAD_MODE_UPDATE    (TIM_REG16_BIT_ZERO)
#define TIM_PSC_RELOAD_MODE_IMMEDIATE (TIM_EVTGEN_UDGN)



/** TIM_Event_Source **/
#define TIM_EVT_SRC_UPDATE  (TIM_EVTGEN_UDGN)
#define TIM_EVT_SRC_CC1     (TIM_EVTGEN_CC1GN)
#define TIM_EVT_SRC_CC2     (TIM_EVTGEN_CC2GN)
#define TIM_EVT_SRC_CC3     (TIM_EVTGEN_CC3GN)
#define TIM_EVT_SRC_CC4     (TIM_EVTGEN_CC4GN)
#define TIM_EVT_SRC_COM     (TIM_EVTGEN_CCUDGN)
#define TIM_EVT_SRC_TRIG    (TIM_EVTGEN_TGN)
#define TIM_EVT_SRC_BREAK   (TIM_EVTGEN_BGN)

#define TIM_OC1_PRELOAD_MASK       (TIM_CCMOD1_OC1PEN)
#define TIM_OC2_PRELOAD_MASK       (TIM_CCMOD1_OC2PEN)
#define TIM_OC3_PRELOAD_MASK       (TIM_CCMOD2_OC3PEN)
#define TIM_OC4_PRELOAD_MASK       (TIM_CCMOD2_OC4PEN)
#define TIM_OC5_PRELOAD_MASK       (TIM_CCMOD3_OC5PEN)
/** TIM_Output_Compare_Preload_State **/
#define TIM_OC_PRELOAD_ENABLE      (TIM_CCMOD1_OC1PEN)
#define TIM_OC_PRELOAD_DISABLE     (TIM_REG16_BIT_ZERO)




#define TIM_OC1_FAST_MASK       (TIM_CCMOD1_OC1FEN)
#define TIM_OC2_FAST_MASK       (TIM_CCMOD1_OC2FEN)
#define TIM_OC3_FAST_MASK       (TIM_CCMOD2_OC3FEN)
#define TIM_OC4_FAST_MASK       (TIM_CCMOD2_OC4FEN)
#define TIM_OC5_FAST_MASK       (TIM_CCMOD3_OC5FEN)
/** TIM_Output_Compare_Fast_State **/
#define TIM_OC_FAST_ENABLE      (TIM_CCMOD1_OC1FEN)
#define TIM_OC_FAST_DISABLE     (TIM_REG16_BIT_ZERO)


#define TIM_OC1_CLEAR_MASK       (TIM_CCMOD1_OC1CEN)
#define TIM_OC2_CLEAR_MASK       (TIM_CCMOD1_OC2CEN)
#define TIM_OC3_CLEAR_MASK       (TIM_CCMOD2_OC3CEN)
#define TIM_OC4_CLEAR_MASK       (TIM_CCMOD2_OC4CEN)
#define TIM_OC5_CLEAR_MASK       (TIM_CCMOD3_OC5CEN)
/** TIM_Output_Compare_Clear_State **/
#define TIM_OC_CLEAR_ENABLE      (TIM_CCMOD1_OC1CEN)
#define TIM_OC_CLEAR_DISABLE     (TIM_REG16_BIT_ZERO)

#define TIM_TRGO_SRC_MASK            (TIM_CTRL2_MMSEL)
/** TIM_Trigger_Output_Source **/
#define TIM_TRGO_SRC_RESET           (TIM_REG16_BIT_ZERO)
#define TIM_TRGO_SRC_ENABLE          (TIM_CTRL2_MMSEL_0)
#define TIM_TRGO_SRC_UPDATE          (TIM_CTRL2_MMSEL_1)
#define TIM_TRGO_SRC_OC1             (TIM_CTRL2_MMSEL_1 | TIM_CTRL2_MMSEL_0)
#define TIM_TRGO_SRC_OC1REF          (TIM_CTRL2_MMSEL_2)
#define TIM_TRGO_SRC_OC2REF          (TIM_CTRL2_MMSEL_2 | TIM_CTRL2_MMSEL_0)
#define TIM_TRGO_SRC_OC3REF          (TIM_CTRL2_MMSEL_2 | TIM_CTRL2_MMSEL_1)
#define TIM_TRGO_SRC_OC4REF          (TIM_CTRL2_MMSEL_2 | TIM_CTRL2_MMSEL_1 | TIM_CTRL2_MMSEL_0)

/** ETR selection **/
#define TIM_ETR_Seletct_ExtGpio                 (TIM_REG16_BIT_ZERO)
#define TIM_ETR_Seletct_innerTsc                (TIM_CTRL2_TSCSEL)

#define TIM_SLAVE_MODE_MASK  (TIM_SMCTRL_SMSEL)
/** TIM_Slave_Mode **/
#define TIM_SLAVE_MODE_RESET (TIM_SMCTRL_SMSEL_2)
#define TIM_SLAVE_MODE_GATED (TIM_SMCTRL_SMSEL_2 | TIM_SMCTRL_SMSEL_0)
#define TIM_SLAVE_MODE_TRIG  (TIM_SMCTRL_SMSEL_2 | TIM_SMCTRL_SMSEL_1)
#define TIM_SLAVE_MODE_EXT1  (TIM_SMCTRL_SMSEL_2 | TIM_SMCTRL_SMSEL_1 | TIM_SMCTRL_SMSEL_0)

#define TIM_MASTER_SLAVE_MODE_MASK    (TIM_SMCTRL_MSMD)
/** TIM_Master_Slave_Mode **/
#define TIM_MASTER_SLAVE_MODE_ENABLE  (TIM_SMCTRL_MSMD)
#define TIM_MASTER_SLAVE_MODE_DISABLE (TIM_REG16_BIT_ZERO)

/** TIM_Flags **/
#define TIM_FLAG_UPDATE (TIM_STS_UDITF)
#define TIM_FLAG_CC1    (TIM_STS_CC1ITF)
#define TIM_FLAG_CC2    (TIM_STS_CC2ITF)
#define TIM_FLAG_CC3    (TIM_STS_CC3ITF)
#define TIM_FLAG_CC4    (TIM_STS_CC4ITF)
#define TIM_FLAG_COM    (TIM_STS_COMITF)
#define TIM_FLAG_TRIG   (TIM_STS_TITF)
#define TIM_FLAG_BREAK  (TIM_STS_BITF)
#define TIM_FLAG_CC1OF  (TIM_STS_CC1OCF)
#define TIM_FLAG_CC2OF  (TIM_STS_CC2OCF)
#define TIM_FLAG_CC3OF  (TIM_STS_CC3OCF)
#define TIM_FLAG_CC4OF  (TIM_STS_CC4OCF)
#define TIM_FLAG_CC5    (TIM_STS_CC5ITF)

/** TIM_CCxEN **/
#define TIM_CC1EN        (TIM_CCEN_CC1EN)
#define TIM_CC1NEN       (TIM_CCEN_CC1NEN)
#define TIM_CC2EN        (TIM_CCEN_CC2EN)
#define TIM_CC2NEN       (TIM_CCEN_CC2NEN)
#define TIM_CC3EN        (TIM_CCEN_CC3EN)
#define TIM_CC3NEN       (TIM_CCEN_CC3NEN)
#define TIM_CC4EN        (TIM_CCEN_CC4EN)
#define TIM_CC5EN        (TIM_CCEN_CC5EN)


/** TIM_CCxP **/
#define TIM_CC1P    (TIM_CCEN_CC1P)
#define TIM_CC1NP   (TIM_CCEN_CC1NP)
#define TIM_CC2P    (TIM_CCEN_CC2P)
#define TIM_CC2NP   (TIM_CCEN_CC2NP)
#define TIM_CC3P    (TIM_CCEN_CC3P)
#define TIM_CC3NP   (TIM_CCEN_CC3NP)
#define TIM_CC4P    (TIM_CCEN_CC4P)
#define TIM_CC5P    (TIM_CCEN_CC5P)

/* TIM_CCDATx*/
#define TIM_CCDAT1_MASK           (TIM_CCDAT1_CCDAT1)
#define TIM_CCDAT2_MASK           (TIM_CCDAT2_CCDAT2)
#define TIM_CCDAT3_MASK           (TIM_CCDAT3_CCDAT3)
#define TIM_CCDAT4_MASK           (TIM_CCDAT4_CCDAT4)


/* ---------------------- TIM registers bit mask ------------------------ */
#define SMCTRL_ETRP_MASK            ((uint16_t)0x8000)
#define SMCTRL_ETRCEN_MASK          ((uint16_t)0x4000)
#define SMCTRL_ETRPSC_MASK          ((uint16_t)0x3000)
#define SMCTRL_ETRFIL_MASK          ((uint16_t)0x0f00)
#define CAPCMPMOD_OFFSET            ((uint16_t)0x0018)
#define CAPCMPEN_CCE_SET            ((uint16_t)0x0001)
#define CAPCMPEN_CCNE_SET           ((uint16_t)0x0004)


void TIM_Reset(TIM_Module* TIMx);

/** TIM_Base functions **/
void TIM_Base_Count_Mode_Set(TIM_Module* TIMx, uint32_t cnt_mode);

/* function in N32G003 */
void TIM_Base_Auto_Reload_Set(TIM_Module* TIMx, uint16_t auto_reload);
void TIM_Base_Prescaler_Set(TIM_Module* TIMx, uint16_t prescaler);
void TIM_Base_Reload_Mode_Set(TIM_Module* TIMx, uint16_t reload_mode);
void TIM_Base_Count_Set(TIM_Module* TIMx, uint16_t count);
uint16_t TIM_Base_Count_Get(TIM_Module* TIMx);
uint16_t TIM_Auto_Reload_Get(TIM_Module* TIMx);
uint16_t TIM_Base_Prescaler_Get(TIM_Module* TIMx);
void TIM_Base_Repeat_Count_Set(TIM_Module* TIMx, uint8_t repeat_cnt);
void TIM_Base_Channel1(TIM_Module* TIMx, bool channel_selection);
void TIM_Base_OCrefClear(TIM_Module* TIMx, bool OCrefClear_selection);
void TIM_Base_Initialize(TIM_Module* TIMx, TIM_TimeBaseInitType* TIM_TimeBaseInitStruct);
void TIM_Base_Struct_Initialize(TIM_TimeBaseInitType* TIM_TimeBaseInitStruct);
void TIM_On(TIM_Module* TIMx);
void TIM_Off(TIM_Module* TIMx);

/** Preload functions **/
void TIM_Output_Channel1_Preload_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_preload);
void TIM_Output_Channel2_Preload_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_preload);
void TIM_Output_Channel3_Preload_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_preload);
void TIM_Output_Channel4_Preload_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_preload);
void TIM_Output_Channel5_Preload_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_preload);
void TIM_Auto_Reload_Preload_Enable(TIM_Module* TIMx);
void TIM_Auto_Reload_Preload_Disable(TIM_Module* TIMx);
void TIM_Update_Event_Enable(TIM_Module* TIMx);
void TIM_Update_Event_Disable(TIM_Module* TIMx);
void TIM_Update_Request_Source_Set(TIM_Module* TIMx, uint16_t TIM_update_source);
void TIM_Event_Generate(TIM_Module* TIMx, uint16_t TIM_event_source);
void TIM_Commutation_Event_Enable(TIM_Module* TIMx); 
void TIM_Commutation_Event_Disable(TIM_Module* TIMx); 
void TIM_Capture_Compare_Control_Preload_Enable(TIM_Module* TIMx);
void TIM_Capture_Compare_Control_Preload_Disable(TIM_Module* TIMx);

/** Channel functions **/
void TIM_Compare1_Set(TIM_Module* TIMx, uint16_t compare1);
void TIM_Compare2_Set(TIM_Module* TIMx, uint16_t compare2);
void TIM_Compare3_Set(TIM_Module* TIMx, uint16_t compare3);
void TIM_Compare4_Set(TIM_Module* TIMx, uint16_t compare4);
void TIM_Compare5_Set(TIM_Module* TIMx, uint16_t compare5);
uint16_t TIM_Compare_Capture1_Get(TIM_Module* TIMx);
uint16_t TIM_Compare_Capture2_Get(TIM_Module* TIMx);
uint16_t TIM_Compare_Capture3_Get(TIM_Module* TIMx);
uint16_t TIM_Compare_Capture4_Get(TIM_Module* TIMx);
uint16_t TIM_Compare_Capture5_Get(TIM_Module* TIMx);
FlagStatus TIM_CCEN_Status_Get(TIM_Module* TIMx, uint32_t TIM_CCEN);

/** Chanel input&Cascade functions **/
void TIM_Trigger_Source_Select(TIM_Module* TIMx, uint16_t TIM_trigger_source);
void Input_Channel1_Config(TIM_Module* TIMx, uint16_t input_channel_polarity, 
                           uint16_t input_channel_selection, uint16_t input_channel_filter);
void Input_Channel2_Config(TIM_Module* TIMx, uint16_t input_channel_polarity, 
                           uint16_t input_channel_selection, uint16_t input_channel_filter);
void Input_Channel3_Config(TIM_Module* TIMx, uint16_t input_channel_polarity, 
                           uint16_t input_channel_selection, uint16_t input_channel_filter);
void Input_Channel4_Config(TIM_Module* TIMx, uint16_t input_channel_polarity, 
                           uint16_t input_channel_selection, uint16_t input_channel_filter);
void TIM_External_Trigger_Polarity_Set(TIM_Module* TIMx,
                                       uint16_t external_trigger_polarity);
void TIM_External_Trigger_Filter_Set(TIM_Module* TIMx,
                                     uint16_t external_trigger_filter);
void TIM_External_Trigger_Prescaler_Set(TIM_Module* TIMx,
                                        uint16_t external_trigger_prescaler);
void TIM_Clock_Division_Set(TIM_Module* TIMx, uint16_t clock_division);
void TIM_Internal_Clock_Set(TIM_Module* TIMx);
void TIM_Input_Capture1_Prescaler_Set(TIM_Module* TIMx, uint16_t TIM_input_capture_prescaler);
void TIM_Input_Capture2_Prescaler_Set(TIM_Module* TIMx, uint16_t TIM_input_capture_prescaler);
void TIM_Input_Capture3_Prescaler_Set(TIM_Module* TIMx, uint16_t TIM_input_capture_prescaler);
void TIM_Input_Capture4_Prescaler_Set(TIM_Module* TIMx, uint16_t TIM_input_capture_prescaler);
void TIM_Internal_Trig_To_Ext_Set(TIM_Module* TIMx, uint16_t input_trigger_source);
void TIM_Trigger_As_External_Clock(TIM_Module* TIMx, uint16_t external_clock_source, uint16_t input_channel_polarity, uint16_t input_channel_filter);
void TIM_External_Clock_Mode1_Set(TIM_Module* TIMx, uint16_t external_trigger_prescaler, uint16_t external_trigger_polarity, uint16_t external_trigger_filter);
void TIM_External_Clock_Mode2_Set(TIM_Module* TIMx, uint16_t external_trigger_prescaler, uint16_t external_trigger_polarity, uint16_t external_trigger_filter);
void TIM_Slave_Mode_Select(TIM_Module* TIMx, uint16_t TIM_slave_mode);
void TIM_Master_Slave_Mode_Set(TIM_Module* TIMx, uint16_t TIM_master_slave_mode);
void TIM_Output_Trigger_Select(TIM_Module* TIMx, uint16_t TIM_trigger_output_source);
void TIM_One_Pulse_Mode_Select(TIM_Module* TIMx, uint16_t TIM_one_pulse_mode);
void TIM_Input_Channel_Initialize(TIM_Module* TIMx, TIM_ICInitType* TIM_ICInitStruct);
void TIM_PWM_Input_Channel_Config(TIM_Module* TIMx, TIM_ICInitType* TIM_ICInitStruct);
void TIM_Input_Struct_Initialize(TIM_ICInitType* TIM_ICInitStruct);

/** Channel output functions **/
void TIM_Output_Channel_Polarity_Set(TIM_Module* TIMx, uint32_t output_channel_polarity, uint16_t channel);
void TIM_Output_Channel_N_Polarity_Set(TIM_Module* TIMx, uint32_t output_channel_N_polarity, uint16_t channel);
void TIM_Capture_Compare_Ch_Enable(TIM_Module* TIMx, uint16_t channel);
void TIM_Capture_Compare_Ch_Disable(TIM_Module* TIMx, uint16_t channel);
void TIM_Capture_Compare_Ch_N_Enable(TIM_Module* TIMx, uint16_t channel);
void TIM_Capture_Compare_Ch_N_Disable(TIM_Module* TIMx, uint16_t channel);
void TIM_Output_Channel_Mode_Set(TIM_Module* TIMx, uint32_t output_channel_mode, uint16_t channel);
void TIM_Forced_Output_Channel1_Set(TIM_Module* TIMx, uint16_t TIM_ForcedAction);
void TIM_Forced_Output_Channel2_Set(TIM_Module* TIMx, uint16_t TIM_ForcedAction);
void TIM_Forced_Output_Channel3_Set(TIM_Module* TIMx, uint16_t TIM_ForcedAction);
void TIM_Forced_Output_Channel4_Set(TIM_Module* TIMx, uint16_t TIM_ForcedAction);
void TIM_Forced_Output_Channel5_Set(TIM_Module* TIMx, uint16_t TIM_ForcedAction);
void TIM_Output_Channel1_Fast_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_fast);
void TIM_Output_Channel2_Fast_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_fast);
void TIM_Output_Channel3_Fast_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_fast);
void TIM_Output_Channel4_Fast_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_fast);
void TIM_Output_Channel5_Fast_Set(TIM_Module* TIMx, uint16_t TIM_output_channel_fast);
void TIM_Output_Channel1_Reference_Clear(TIM_Module* TIMx, uint16_t TIM_output_chanenl_clear);
void TIM_Output_Channel2_Reference_Clear(TIM_Module* TIMx, uint16_t TIM_output_chanenl_clear);
void TIM_Output_Channel3_Reference_Clear(TIM_Module* TIMx, uint16_t TIM_output_chanenl_clear);
void TIM_Output_Channel4_Reference_Clear(TIM_Module* TIMx, uint16_t TIM_output_chanenl_clear);
void TIM_Output_Channel5_Reference_Clear(TIM_Module* TIMx, uint16_t TIM_output_chanenl_clear);
void TIM_PWM_Output_Enable(TIM_Module* TIMx);
void TIM_PWM_Output_Disable(TIM_Module* TIMx);
void TIM_Output_Channel1_Initialize(TIM_Module* TIMx, OCInitType* TIM_OCInitStruct);
void TIM_Output_Channel2_Initialize(TIM_Module* TIMx, OCInitType* TIM_OCInitStruct);
void TIM_Output_Channel3_Initialize(TIM_Module* TIMx, OCInitType* TIM_OCInitStruct);
void TIM_Output_Channel4_Initialize(TIM_Module* TIMx, OCInitType* TIM_OCInitStruct);
void TIM_Output_Channel5_Initialize(TIM_Module* TIMx, OCInitType* TIM_OCInitStruct);
void TIM_Output_Channel_Struct_Initialize(OCInitType* TIM_OCInitStruct);

/** Brake functions **/
void TIM_Lock_Up_Break_Enable(TIM_Module* TIMx);
void TIM_Lock_Up_Break_Disable(TIM_Module* TIMx);
void TIM_Pvd_Break_Enable(TIM_Module* TIMx);
void TIM_Pvd_Break_Disable(TIM_Module* TIMx);
void TIM_IOM_Comp_Break(TIM_Module* TIMx, bool IOM_Comp_Selection);
void TIM_Break_And_Dead_Time_Set(TIM_Module* TIMx, TIM_BDTRInitType* TIM_BDTRInitStruct);
void TIM_Break_And_Dead_Time_Struct_Initialize(TIM_BDTRInitType* TIM_BDTRInitStruct);

/** Interrupt functions **/
void TIM_Interrupt_Enable(TIM_Module* TIMx, uint16_t TIM_IT);
void TIM_Interrupt_Disable(TIM_Module* TIMx, uint16_t TIM_IT);
FlagStatus TIM_Flag_Status_Get(TIM_Module* TIMx, uint32_t TIM_FLAG);
ITStatus TIM_Interrupt_Status_Get(TIM_Module* TIMx, uint32_t TIM_IT);
void TIM_Flag_Clear(TIM_Module* TIMx, uint32_t TIM_FLAG);
void TIM_Interrupt_Status_Clear(TIM_Module* TIMx, uint32_t TIM_IT);






#ifdef __cplusplus
}
#endif

#endif /* _N32G003_TIM_H__ */
