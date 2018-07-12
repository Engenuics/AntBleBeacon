/***********************************************************************************************************************
* File: abbcn-ehdw-01.h                                                                
* 
* Description:
* This file provides header information for the board support functions for nRF51422 processor on the 
abbcn-ehdw-01 board.
***********************************************************************************************************************/

#ifndef __ABBCN_H
#define __ABBCN_H

/***********************************************************************************************************************
Type Definitions
***********************************************************************************************************************/

/*----------------------------------------------------------------------------------------------------------------------
%LED% LED Configuration                                                                                                  
----------------------------------------------------------------------------------------------------------------------*/
/*! 
@enum LedNameType
@brief Logical names for LEDs in the system.

The order of the LEDs in LedNameType must match the order of the definition 
in G_asBspLedConfigurations from abbcn-ehdw-01.c 
*/
typedef enum {RED0 = 0, RED1, RED2, RED3, RED4, RED5, RED6, RED7,
              GRN0, GRN1, GRN2, GRN3, GRN4, GRN5, GRN6, GRN7,
              BLU0, BLU1, BLU2, BLU3, BLU4, BLU5, BLU6, BLU7
             } LedNameType;

#define U8_TOTAL_LEDS             (u8)24      /*!< @brief Total number of LEDs in the system */

#define U8_LED_COLOR_OFFSET_RED   (u8)0       /*!< @brief Offset between Red and Red in LedNameType */
#define U8_LED_COLOR_OFFSET_GRN   (u8)8       /*!< @brief Offset between Green and Red in LedNameType */
#define U8_LED_COLOR_OFFSET_BLU   (u8)16      /*!< @brief Offset between Blue and Red in LedNameType */

/*----------------------------------------------------------------------------------------------------------------------
%BUTTON% Button Configuration                                                                                                  
----------------------------------------------------------------------------------------------------------------------*/
/*! 
@enum ButtonNameType
@brief Logical names for buttons in the system.

The order of the buttons in ButtonNameType must match the order of the definition 
in G_asBspButtonConfigurations Buttons_au32BitPositions from eief1-pcb-01.c 
*/
typedef enum {BUTTON0 = 0, NOBUTTON} ButtonNameType;

#define U8_TOTAL_BUTTONS          (u8)1       /*!< @brief Total number of Buttons in the system */

/*! All buttons on each port must be ORed together here: set to 0 if no buttons on the port */
#define GPIO_BUTTONS             (u32)( P0_20_BUTTON )

/* GPIOE Events: A maximum of 4 events/tasks can be configured for the GPIOE which
means that this driver is currently limited to a maximum of 4 buttons as long as
the development board does not require and other GPIOE events or tasks. */



/***********************************************************************************************************************
* Constants
***********************************************************************************************************************/
#define BUTTON0_GPIOE_EVENT GPIOE_EVENT0


/***********************************************************************************************************************
* Macros
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Declarations
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public Functions */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected Functions */
/*--------------------------------------------------------------------------------------------------------------------*/
void WatchDogSetup(void);
void PowerSetup(void);
void GpioSetup(void);
void ClockSetup(void);
void InterruptSetup(void);
void SysTickSetup(void);
void SystemSleep(void);


/***********************************************************************************************************************
Perihperal Setup Initializations

Bookmarks:
@@@@@ Clock, Power Control, Systick and Watchdog setup values
!!!!! GPIO pin names
##### GPIO initial setup values
$$$$$ PWM setup values

***********************************************************************************************************************/

/***********************************************************************************************************************
@@@@@ Clock, Systick and Power Control setup values
***********************************************************************************************************************/
#define FOSC                    __SYSTEM_CLOCK    /* Crystal speed from system_nrf51.c */
#define OSC_STARTUP_TIMOUT      (u32)1000000      /* Timeout for oscillator to start up */
  
/* Timer 1
To get roughly a 1ms tick, set the prescale register value to 0 which results in a prescale value of 1.
Then we can count up to 33 to get a 1.0071ms tick.  That gives 993 for the value to count to for the 1 second tick. */

#define LFCLK_FREQ               (u32)32768
#define HFCLK_FREQ               (u32)16000000

#define RTC_COMPARE_PERIOD       (u32)33
#define RTC_TICK_PER_SECOND      (u32)993

/* Watch Dog Values */

/* TIMER
The built-in timer will provide the system tick
It is clocked from HFCLK.  To get the desired 1ms tick use a compare period of 0.001 / (1/HFCLK) or HFCLK/1000.
*/
#define TIMER_COUNT_1MS        (u32)(HFCLK_FREQ / 1000)


/***********************************************************************************************************************
!!!!! GPIO pin names
***********************************************************************************************************************/
/* Hardware Definition for nRF51422 on ABBCN-EHDW-01 */

/* Port 0 bit positions */
#define P0_31_               (u32)0x80000000
#define P0_30_BLU7           (u32)0x40000000
#define P0_29_GRN7           (u32)0x20000000
#define P0_28_RED2           (u32)0x10000000
#define P0_27_RED3           (u32)0x08000000
#define P0_26_RED4           (u32)0x04000000 
#define P0_25_RED5           (u32)0x02000000
#define P0_24_RED7           (u32)0x01000000
#define P0_23_RED0           (u32)0x00800000
#define P0_22_RED6           (u32)0x00400000 
#define P0_21_RED1           (u32)0x00200000
#define P0_20_BUTTON         (u32)0x00100000
#define P0_19_EXT1           (u32)0x00080000
#define P0_18_EXT2           (u32)0x00040000
#define P0_17_EXT3           (u32)0x00020000
#define P0_16_SCL            (u32)0x00010000
#define P0_15_SDA            (u32)0x00008000
#define P0_14_INT2           (u32)0x00004000
#define P0_13_BLU1           (u32)0x00002000
#define P0_12_BLU0           (u32)0x00001000
#define P0_11_GRN0           (u32)0x00000800
#define P0_10_GRN1           (u32)0x00000400
#define P0_09_BLU2           (u32)0x00000200
#define P0_08_GRN2           (u32)0x00000100
#define P0_07_BLU3           (u32)0x00000080
#define P0_06_GRN3           (u32)0x00000040
#define P0_05_BLU4           (u32)0x00000020
#define P0_04_GRN4           (u32)0x00000010
#define P0_03_BLU5           (u32)0x00000008
#define P0_02_GRN5           (u32)0x00000004
#define P0_01_BLU6           (u32)0x00000002 
#define P0_00_GRN6           (u32)0x00000001 

#define P0_31_INDEX          (u32)31
#define P0_30_INDEX          (u32)30
#define P0_29_INDEX          (u32)29
#define P0_28_INDEX          (u32)28
#define P0_27_INDEX          (u32)27
#define P0_26_INDEX          (u32)26
#define P0_25_INDEX          (u32)25
#define P0_24_INDEX          (u32)24
#define P0_23_INDEX          (u32)23
#define P0_22_INDEX          (u32)22
#define P0_21_INDEX          (u32)21
#define P0_20_INDEX          (u32)20
#define P0_19_INDEX          (u32)19
#define P0_18_INDEX          (u32)18
#define P0_17_INDEX          (u32)17
#define P0_16_INDEX          (u32)16
#define P0_15_INDEX          (u32)15
#define P0_14_INDEX          (u32)14
#define P0_13_INDEX          (u32)13
#define P0_12_INDEX          (u32)12
#define P0_11_INDEX          (u32)11
#define P0_10_INDEX          (u32)10
#define P0_09_INDEX          (u32)9
#define P0_08_INDEX          (u32)8
#define P0_07_INDEX          (u32)7
#define P0_06_INDEX          (u32)6
#define P0_05_INDEX          (u32)5
#define P0_04_INDEX          (u32)4
#define P0_03_INDEX          (u32)3
#define P0_02_INDEX          (u32)2
#define P0_01_INDEX          (u32)1
#define P0_00_INDEX          (u32)0



/***********************************************************************************************************************
##### GPIO setup values
***********************************************************************************************************************/

/* Pin configuration (pg. 58) - Pins are set up using the PIN_CNF registers (one register per pin)
*/
#define P0_30_BLU7_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_29_GRN7_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_28_RED2_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_27_RED3_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_26_RED4_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_25_RED5_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_24_RED7_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_23_RED0_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_22_RED6_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_21_RED1_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_20_BUTTON_CNF    ( (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_19_EXT1_CNF      ( (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Pullup      << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_18_EXT2_CNF      ( (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Pullup      << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_17_EXT3_CNF      ( (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Pulldown    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_16_SCL_CNF       ( (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_15_SDA_CNF       ( (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_14_INT2_CNF      ( (GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Connect    << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_13_BLU1_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_12_BLU0_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_11_GRN0_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_10_GRN1_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_09_BLU2_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_08_GRN2_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_07_BLU3_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_06_GRN3_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_05_BLU4_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_04_GRN4_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_03_BLU5_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_02_GRN5_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_01_BLU6_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

#define P0_00_GRN6_CNF      ( (GPIO_PIN_CNF_DIR_Output       << GPIO_PIN_CNF_DIR_Pos)   | \
                              (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) | \
                              (GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)  | \
                              (GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos) | \
                              (GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos) )

                                
                                

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/

#define TEMPLATE_INIT (u32)0x
/*
    31 [0] P0_31_
    30 [0] P0_30_BLU7
    29 [0] P0_29_GRN7
    28 [0] P0_28_RED2

    27 [0] P0_27_RED3
    26 [0] P0_26_LED_BLUE
    25 [0] P0_25_RED5
    24 [0] P0_24_RED7

    23 [0] P0_23_RED0
    22 [0] P0_22_RED6
    21 [0] P0_21_RED1
    20 [0] P0_20_BUTTON

    19 [0] P0_19_EXT1
    18 [0] P0_18_EXT2
    17 [0] P0_17_EXT3
    16 [0] P0_16_SCL

    15 [0] P0_15_SDA
    14 [0] P0_14_INT2
    13 [0] P0_13_BLU1
    12 [0] P0_12_BLU0

    11 [0] P0_11_GRN0
    10 [0] P0_10_GRN1
    09 [0] P0_09_BLU2
    08 [0] P0_08_GRN2

    07 [0] P0_07_BLU3
    06 [0] P0_06_GRN3
    05 [0] P0_05_BLU4
    04 [0] P0_04_GRN4

    03 [0] P0_03_BLU5
    02 [0] P0_02_GRN5
    01 [0] P0_01_BLU6
    00 [0] P0_00_GRN6
*/




#endif /* __ABBCN_H */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
