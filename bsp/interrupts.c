/**********************************************************************************************************************
File: interrupts.c                                                                

Description:
This is a interrupts .c file new source code.
System-level interrupt handlers are defined here.  Driver-specific handlers will be found in
their respective source files.

All SoC interrupts are in soc_integration.c

This might be too fragmented, so we reserve the right to change it up after we play with it for a while.

**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32InterruptsFlags;                     /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */

extern const Nrf51PinConfigurationType G_asBspButtonConfigurations[U8_TOTAL_BUTTONS]; /*!< @brief from board-specific file */

//extern volatile bool G_abButtonDebounceActive[TOTAL_BUTTONS];
//extern volatile u32 G_au32ButtonDebounceTimeStart[TOTAL_BUTTONS];


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Interrupts_" and be declared as static.
***********************************************************************************************************************/
//static u32 Interrupts_u32Timeout;                     /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*!--------------------------------------------------------------------------------------------------------------------
@fn bool InterruptSetup(void)
@brief Initializes main system interrupts via Soft Device.

Requires:
- SD is enabled

Promises:
- Returns TRUE if SoftDevice Interrupts are successfully enabled, FALSE otherwise.

*/
bool InterruptSetup(void)
{
  u32 u32Result = NRF_SUCCESS;

#ifdef SOFTDEVICE_ENABLED  
  /* Must enable the SoftDevice Interrupt first */
  u32Result |= sd_nvic_SetPriority(SD_EVT_IRQn, NRF_APP_PRIORITY_LOW);
  u32Result |= sd_nvic_EnableIRQ(SD_EVT_IRQn);

  /* GPIOE interrupts */
  u32Result |= sd_nvic_SetPriority(GPIOTE_IRQn, NRF_APP_PRIORITY_LOW);
  u32Result |= sd_nvic_EnableIRQ(GPIOTE_IRQn);
#endif
  
  return (u32Result == NRF_SUCCESS);

} /* end InterruptsSetup() */


/*----------------------------------------------------------------------------------------------------------------------
@fn bool SystemEnterCriticalSection(u8* pu8NestedStatus_)
@brief SD sourced function for disabling interrupts 

Requires:
- SoftDevice is enabled
@PARAM pu8NestedStatus_ is provided by the client to receive the 
nested status returned by the SVCALL.  This should be provided
back to SystemExitCriticalSection().

Promises:
- Application interrupts will be disabled. 

*/
bool SystemEnterCriticalSection(u8* pu8NestedStatus_)
{
  sd_nvic_critical_region_enter(pu8NestedStatus_);
  
  return (pu8NestedStatus_ == 0);
  
} /* end SystemEnterCriticalSection() */


/*----------------------------------------------------------------------------------------------------------------------
@fn bool SystemExitCriticalSection(u8 u8NestedStatus_)
@brief SD sourced function for re-enabling interrupts 

Requires:
- SoftDevice is enabled.
@PARAM pu8NestedStatus_ is provided by the client to receive the 
nested status returned by the SVCALL.

Promises:
- Application interrupts will be re-enabled if there are
no further nested critical sections.
*/
bool SystemExitCriticalSection(u8 u8NestedStatus_)
{
  sd_nvic_critical_region_exit(u8NestedStatus_);
  
  return (u8NestedStatus_ == 0);
  
} /* end SystemExitCriticalSection() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Handlers                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void HardFault_Handler(u32 u32ProgramCounter_, u32 u32LinkRegister_)
@brief Custom HardFault ISR

Requires:
@PARAM u32ProgramCounter_ provided via hardware?
@PARAM u32LinkRegister_ provided via hardware?

Promises:
- Red LED on and code is trapped

*/
void HardFault_Handler(u32 u32ProgramCounter_, u32 u32LinkRegister_)
{
  /* void variables to make this visible in the debugger */
  (void)u32ProgramCounter_;
  (void)u32LinkRegister_;

   /* Red LED on to indicate fault state */
   //NRF_GPIO->OUTCLR = P0_28_LED_YLW | P0_27_LED_GRN | P0_26_LED_BLU;
   NRF_GPIO->OUTSET = P0_23_RED0 | P0_28_RED2 | P0_26_RED4 | P0_22_RED6;
   
   /* RIP */
   while(1); 
   
} /* end HardFault_Handler() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void RTC1_IRQHandler(void)
@brief Custom RTC1 ISR for system tick

Requires:
@PARAM G_u32SystemTime1ms globally available and should only bit written by this ISR
@PARAM G_u32SystemTime1s  globally available and should only bit written by this ISR

Promises:
- G_u32SystemTime1ms is updated; G_u32SystemTime1s incremented every 1000 ticks

*/
void RTC1_IRQHandler(void)
{
  /* Clear the Tick Event */
  NRF_RTC1->EVENTS_TICK = 0;
  
  /* Update global counters */
  G_u32SystemTime1ms++;
  if ((G_u32SystemTime1ms % 1000) == 0)
  {
    G_u32SystemTime1s++;
    
    /* Optional heartbeat */
    //LedToggle(RED7);
  }

} /* end RTC1_IRQHandler() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void SD_EVT_IRQHandler(void)
@brief ISR to process soft device events.

Requires:
- enabled via sd_nvic_XXX

Promises:
- Sets global system flags indicating that BLE and ANT events are pending.
It is possible that either ANT or BLE events OR ANT & BLE events are pending.
The application shall handle all the cases. 

*/
void SD_EVT_IRQHandler(void)
{
  /* Set Flag that ANT and BLE Events pending. */
  G_u32SystemFlags |= (_SYSTEM_PROTOCOL_EVENT); 
  
} /* end SD_EVT_IRQHandler() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn ISR void GPIOTE_IRQHandler(void)

@brief Processes GPIOTE Events such as Pin and Port InterruptsB

Requires:
- Enabled via sd_nvic_XXX

Promises:
- Handles the GPIOTE events for the enabled pins. 

*/
void GPIOTE_IRQHandler(void)
{
  /* Check for button-related interrupts */
  for(u8 i = 0; i < U8_TOTAL_BUTTONS; i++)
  {
    /* First find a button channel event that is enabled and fired */
    if( (G_asBspButtonConfigurations[i].eChannelNumber != GPIOE_NO_CHANNEL) &&
        (NRF_GPIOTE->EVENTS_IN[G_asBspButtonConfigurations[i].eChannelNumber]) )
    {
      ButtonStartDebounce(G_asBspButtonConfigurations[i].eChannelNumber) ;
#if 0
      /* Disable interrupt and clear channel event */
      NRF_GPIOTE->INTENCLR = G_asBspButtonConfigurations[i].u32GpioeChannelBit;
      NRF_GPIOTE->EVENTS_IN[G_asBspButtonConfigurations[i].eChannelNumber]] = 0;   

      Button_asStatus[(u8)eButton].bDebounceActive = TRUE;
      Button_asStatus[(u8)eButton].u32DebounceTimeStart = G_u32SystemTime1ms;
#endif
    }
  } /* end for (i) */

} /* end GPIOTE_IRQHandler() */




#if 0
/* Original abbcn code - primed for kill */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Handlers                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

void HardFault_Handler(u32 u32ProgramCounter_, u32 u32LinkRegister_)
{
  (void)u32ProgramCounter_;
  (void)u32LinkRegister_;

   while(1); // loop for debugging
}


void TIMER1_IRQHandler(void)
{ 
  while(1);
}

void RTC1_IRQHandler(void)
{
  // Clear the Tick Event
  NRF_RTC1->EVENTS_TICK = 0;
  
  // Update global counters.
  G_u32SystemTime1ms++;
  if ((G_u32SystemTime1ms % 1000) == 0)
  {
    G_u32SystemTime1s++;
  }
}


/*!----------------------------------------------------------------------------------------------------------------------
@fn ISR void SD_EVT_IRQHandler(void)

@brief Processes soft device events.

Requires:
  - enabled via sd_nvic_XXX

Promises:
  -  Sets global system flags indicating that BLE and ANT events are pending.
     It is possible that either ANT or BLE events OR ANT & BLE events are pending.
     The application shall handle all the cases. 
  
*/
void SD_EVT_IRQHandler(void)
{
  /* Set Flag that ANT and BLE Events pending. */
  G_u32SystemFlags |= (_SYSTEM_PROTOCOL_EVENT); 
  
} /* end SD_EVT_IRQHandler() */

#endif





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
