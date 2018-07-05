/***********************************************************************************************************************
File: abbcn-ehdw-01.c                                                                

Description:
This file provides board support functions for the nRF51422 processor on the ABBCN-EHDW-01 development board.
***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/

/* New variables */

/*! LED locations: order must correspond to the order set in LedNameType in the header file. */
const Nrf51PinConfigurationType G_asBspLedConfigurations[U8_TOTAL_LEDS] = { {P0_23_RED0, 0, PIN30, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_21_RED1, 0, PIN21, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_28_RED2, 0, PIN28, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_27_RED3, 0, PIN27, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_26_RED4, 0, PIN26, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_25_RED5, 0, PIN25, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_22_RED6, 0, PIN22, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_24_RED7, 0, PIN24, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_11_GRN0, 0, PIN11, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_10_GRN1, 0, PIN10, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_08_GRN2, 0, PIN8,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_06_GRN3, 0, PIN6,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_04_GRN4, 0, PIN4,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_02_GRN5, 0, PIN2,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_00_GRN6, 0, PIN0,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_29_GRN7, 0, PIN29, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_12_BLU0, 0, PIN12, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_13_BLU1, 0, PIN13, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_09_BLU2, 0, PIN9,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_07_BLU3, 0, PIN7,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_05_BLU4, 0, PIN5,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_03_BLU5, 0, PIN3,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_01_BLU6, 0, PIN1,  ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                            {P0_30_BLU7, 0, PIN30, ACTIVE_HIGH, GPIOE_NO_CHANNEL, 0}, 
                                                                          };




/*! Button locations: order must correspond to the order set in ButtonNameType in the header file. */
const Nrf51PinConfigurationType G_asBspButtonConfigurations[U8_TOTAL_BUTTONS] = { {P0_20_BUTTON, BIT0, PIN20, ACTIVE_LOW, GPIOE_EVENT0, 0}, 
                                                                                };

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */
 

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Bsp_" and be declared as static.
***********************************************************************************************************************/


/***********************************************************************************************************************
Function Definitions
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public Functions */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected Functions */
/*--------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
Function: WatchDogSetup

Description:
Configures the watchdog timer.  The dog runs at 32.768kHz from the slow built-in RC clock source which varies over 
operating conditions from 30kHz to 60kHz.
Since the main loop time / sleep time should be 1 ms most of the time, choosing a value
of 5 seconds should be plenty to avoid watchdog resets.  

Note: the processor allows the MR register to be written just once.

Requires:
  - 

Promises:
  - Watchdog is set for 5 second timeout but not yet enabled
*/
void WatchDogSetup(void)
{
 
} /* end WatchDogSetup() */


/*----------------------------------------------------------------------------------------------------------------------
Function: GpioSetup

Description
Loads all registers required to set up GPIO on the processor.

Requires:
  - All configurations must match connected hardware.

Promises:
  - All I/O lines are set for their required function and start-state
*/
void GpioSetup(void)
{
  /* Set all of the pin function registers in port 0 */
  NRF_GPIO->PIN_CNF[P0_30_INDEX] = P0_30_BLU7_CNF;
  NRF_GPIO->PIN_CNF[P0_29_INDEX] = P0_29_GRN7_CNF;
  NRF_GPIO->PIN_CNF[P0_28_INDEX] = P0_28_RED2_CNF;
  NRF_GPIO->PIN_CNF[P0_27_INDEX] = P0_27_RED3_CNF;
  NRF_GPIO->PIN_CNF[P0_26_INDEX] = P0_26_RED4_CNF;
  NRF_GPIO->PIN_CNF[P0_25_INDEX] = P0_25_RED5_CNF;
  NRF_GPIO->PIN_CNF[P0_24_INDEX] = P0_24_RED7_CNF;
  NRF_GPIO->PIN_CNF[P0_23_INDEX] = P0_23_RED0_CNF;
  NRF_GPIO->PIN_CNF[P0_22_INDEX] = P0_22_RED6_CNF;
  NRF_GPIO->PIN_CNF[P0_21_INDEX] = P0_21_RED1_CNF;
  NRF_GPIO->PIN_CNF[P0_20_INDEX] = P0_20_BUTTON_CNF;
  NRF_GPIO->PIN_CNF[P0_19_INDEX] = P0_19_EXT1_CNF;
  NRF_GPIO->PIN_CNF[P0_18_INDEX] = P0_18_EXT2_CNF;
  NRF_GPIO->PIN_CNF[P0_17_INDEX] = P0_17_EXT3_CNF;
  NRF_GPIO->PIN_CNF[P0_16_INDEX] = P0_16_SCL_CNF;
  NRF_GPIO->PIN_CNF[P0_15_INDEX] = P0_15_SDA_CNF;
  NRF_GPIO->PIN_CNF[P0_14_INDEX] = P0_14_INT2_CNF;
  NRF_GPIO->PIN_CNF[P0_13_INDEX] = P0_13_BLU1_CNF;
  NRF_GPIO->PIN_CNF[P0_12_INDEX] = P0_12_BLU0_CNF;
  NRF_GPIO->PIN_CNF[P0_11_INDEX] = P0_11_GRN0_CNF;
  NRF_GPIO->PIN_CNF[P0_10_INDEX] = P0_10_GRN1_CNF;
  NRF_GPIO->PIN_CNF[P0_09_INDEX] = P0_09_BLU2_CNF;
  NRF_GPIO->PIN_CNF[P0_08_INDEX] = P0_08_GRN2_CNF;
  NRF_GPIO->PIN_CNF[P0_07_INDEX] = P0_07_BLU3_CNF;
  NRF_GPIO->PIN_CNF[P0_06_INDEX] = P0_06_GRN3_CNF;
  NRF_GPIO->PIN_CNF[P0_05_INDEX] = P0_05_BLU4_CNF;
  NRF_GPIO->PIN_CNF[P0_04_INDEX] = P0_04_GRN4_CNF;
  NRF_GPIO->PIN_CNF[P0_03_INDEX] = P0_03_BLU5_CNF;
  NRF_GPIO->PIN_CNF[P0_02_INDEX] = P0_02_GRN5_CNF;
  NRF_GPIO->PIN_CNF[P0_01_INDEX] = P0_01_BLU6_CNF;
  NRF_GPIO->PIN_CNF[P0_00_INDEX] = P0_00_GRN6_CNF;
   
} /* end GpioSetup() */


/*----------------------------------------------------------------------------------------------------------------------
Function: PowerSetup

Description
Loads registers to configure various power control features of the 51422.

Requires:
  - 

Promises:
  - 
*/
void PowerSetup(void)
{
  /* Set the sub power mode to constant latency (pg. 42 in the ref manual) */
  NRF_POWER->TASKS_LOWPWR   = 0;
  NRF_POWER->TASKS_CONSTLAT = 1;

} /* end PowerSetup() */


/*----------------------------------------------------------------------------------------------------------------------
Function: ClockSetup

Description:
Loads all registers required to set up the processor clocks.  The main clock, HFCLK is sourced from the
16MHz crystal.  The slow clock, LFCLK, will be synthesized from the 16MHz and configured to provide
a 1ms system tick.

Requires:
  - 

Promises:
  - 
*/
void ClockSetup(void)
{
  u32 u32ClockStartTimeout = OSC_STARTUP_TIMOUT;
  
  /* Start the main clock (HFCLK) and wait for the event to indicate it has started */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART = 1;
  while( !NRF_CLOCK->EVENTS_HFCLKSTARTED && (--u32ClockStartTimeout != 0) );
  
  /* Check for timeout - if the clock didn't start, turn it off and flag it */
  if(u32ClockStartTimeout == 0)
  {
    NRF_CLOCK->TASKS_HFCLKSTOP = 1;
    G_u32SystemFlags |= _SYSTEM_HFCLK_NO_START;
  }
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
   
  
  /* Setup and start the 32.768kHz (LFCLK) clock (synthesized from HFCLK) */
  NRF_CLOCK->LFCLKSRC = (CLOCK_LFCLKSRC_SRC_Synth << CLOCK_LFCLKSRC_SRC_Pos);
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_LFCLKSTART = 1;
  
  /* No need for timeout as an HFCLK of some sort is guaranteed to be running so LFCLK has to start */
  while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
 
#if 0  /* Can't use RTC because we synthesize LFCLK and therefore RTC would not be clocked when HFCLK is sleeping */  
  /* Configure the RTC to give a 1ms tick */
  NRF_RTC1->STOP = 1;
  NRF_RTC1->PRESCALER = RTC_PRESCALE_INIT;
  NRF_RTC1->EVTENSET = (1 << RTC_EVTEN_TICK_Pos);
  NRF_RTC1->INTENSET = (1 << RTC_INTENSET_TICK_Pos);
  
  /* Clear then start the RTC */
  NRF_RTC1->CLEAR = 1;
  NRF_RTC1->START = 1;
#endif
  
} /* end ClockSetup */


/*----------------------------------------------------------------------------------------------------------------------
Function: InterruptSetup

Description:
Performs initial interrupt setup.

Requires:
  -

Promises:
  - 
*/
void InterruptSetup(void)
{

  
} /* end InterruptSetup */


/*----------------------------------------------------------------------------------------------------------------------
Function: SysTickSetup

Description:
Initializes the 1ms and 1s System Ticks from the TIMER1 peripheral.
Since this application is not concerned about power, we can keep the 16MHz clock
on and power TIMER1 all the time.  

Requires:
  -

Promises:
  - Both system timers are zeroed and the Timer is configured to produce 1ms interrupts
*/
void SysTickSetup(void)
{
  G_u32SystemTime1ms = 0;      
  G_u32SystemTime1s  = 0;   
  
  /* Load the SysTick Timer */
  NRF_TIMER1->MODE      = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
  NRF_TIMER1->BITMODE   = TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos;
  NRF_TIMER1->PRESCALER = 0;
  NRF_TIMER1->SHORTS    = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos;
  NRF_TIMER1->CC[0]     = TIMER_COUNT_1MS;
  NRF_TIMER1->INTENSET  = TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;
  
  /* Enable TIMER1 interrupt */
  NVIC_SetPriority(TIMER1_IRQn, 0);
  //NVIC_EnableIRQ(TIMER1_IRQn);
  
  /* Start timer */
  NRF_TIMER1->TASKS_START = 1;

  
} /* end SysTickSetup() */


/*----------------------------------------------------------------------------------------------------------------------
Function: SystemSleep

Description:
Puts the system into sleep mode. 

TBD

Requires:
  - TBD

Promises:
  - Configures processor for maximum sleep while still allowing any required
    interrupt to wake it up.
*/
void SystemSleep(void)
{    
  /* Set the system control register for Sleep (but not Deep Sleep) */
   
   /* Set the sleep flag (cleared only in SysTick ISR */
   G_u32SystemFlags |= _SYSTEM_SLEEPING;

   while(NRF_TIMER1->EVENTS_COMPARE[0] == 0);
   NRF_TIMER1->EVENTS_COMPARE[0] = 0;
   
  /* Now sleep until an event wakes us up */
   //while(G_u32SystemFlags & _SYSTEM_SLEEPING)
   {
   }

  /* Clear the sleep mode status flags */
    
} /* end SystemSleep(void) */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/



