/*!**********************************************************************************************************************
@file buttons_nrf51_standard.c                                                                
@brief Button functions and state machine.  

The application handles all debouncing and button press / hold detection for buttons
used on development boards with the nRF51 processor.  All buttons use dedicated
hardware lines (active high or active low), with interrupts to trigger the 
start and end of the action.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- U32_DEBOUNCE_TIME

TYPES
- enum ButtonStateType
- struct ButtonStatusType

PUBLIC FUNCTIONS
- bool IsButtonPressed(ButtonNameType eButton_)
- bool WasButtonPressed(ButtonNameType eButton_)
- void ButtonAcknowledge(ButtonNameType eButton_)
- bool IsButtonHeld(ButtonNameType eButton_, u32 u32ButtonHeldTime_)

PROTECTED FUNCTIONS
- void ButtonInitialize(void)
- void ButtonRunActiveState(void)
- void ButtonStartDebounce(u32 u32BitPosition_, PortOffsetType ePort_)


***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>Button"
***********************************************************************************************************************/
/* New variables */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */

extern const Nrf51PinConfigurationType G_asBspButtonConfigurations[U8_TOTAL_BUTTONS]; /*!< @brief from board-specific file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Button_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Button_pfnStateMachine;                  /* The Button application state machine function pointer */

static ButtonStatusType Button_asStatus[U8_TOTAL_BUTTONS];  /*!< @brief Individual status parameters for buttons */


/***********************************************************************************************************************
Function Definitions
***********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn bool IsButtonPressed(ButtonNameType eButton_)

@brief Determine if a particular button is currently pressed at the moment in time when 
the function is called.

The button must still be pressed at the time of this inquiry for the function
to return TRUE.

Requires:
- Button_asStatus[eButton_] is a valid index

@param eButton_ is a valid button 
 
Promises:
- Returns TRUE if Button_asStatus[eButton_].eCurrentState is PRESSED 
- Otherwise returns FALSE

*/
bool IsButtonPressed(ButtonNameType eButton_)
{
  if( Button_asStatus[(u8)eButton_].eCurrentState == PRESSED)
  {
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }

} /* end IsButtonPressed() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool WasButtonPressed(ButtonNameType eButton_)

@brief Determines if a particular button was pressed since last time it was checked. 

This is effectively a latching function so that button presses are
not missed and are potentially available to multiple tasks.

The button may or may not still be pressed when this inquiry is made.  Mulitple
button presses are not tracked.  The user should call ButtonAcknowledge immediately
following this function to clear the state. If multiple tasks need the button 
information, only the last function should call ButtonAcknowledge.

Requires:
- Button_asStatus[eButton_] is a valid index

@param eButton_ is a valid button 
 
Promises:
- Returns TRUE if Button_asStatus[eButton_].bNewPressFlag is TRUE 
- Otherwise returns FALSE

*/
bool WasButtonPressed(ButtonNameType eButton_)
{
  if( Button_asStatus[(u8)eButton_].bNewPressFlag == TRUE)
  {
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }

} /* end WasButtonPressed() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void ButtonAcknowledge(ButtonNameType eButton_)

@brief Clears the "New Press" flag of a button so WasButtonPressed() no longer returns TRUE
unless a new button press occurs.

This function is generally always called after WasButtonPressed() returns TRUE unless 
it is known that other tasks may need the button pressed information.

Requires:
@param eButton_ is a valid button index
 
Promises:
- The flag at Button_asStatus[eButton_].bNewPressFlag is set to FALSE

*/
void ButtonAcknowledge(ButtonNameType eButton_)
{
  Button_asStatus[(u8)eButton_].bNewPressFlag = FALSE;

} /* end ButtonAcknowledge() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool IsButtonHeld(ButtonNameType eButton_, u32 u32ButtonHeldTime_)

@brief Queries to see if a button has been held for a certain time.  

The button must still be pressed when this function is called if it is to return TRUE.
This is a non-latching function.

Requires:
@param eButton_ is a valid button index
@param u32ButtonHeldTime is a time in ms 
 
Promises:
- Returns TRUE if eButton_ has been held longer than u32ButtonHeldTime_
- Otherwise returns FALSE

*/
bool IsButtonHeld(ButtonNameType eButton_, u32 u32ButtonHeldTime_)
{
 if( (Button_asStatus[(u8)eButton_].eCurrentState == PRESSED) && 
      IsTimeUp(&Button_asStatus[eButton_].u32DebounceTimeStart, u32ButtonHeldTime_ ) )
 {
   return(TRUE);
 }
 else
 {
   return(FALSE);
 }

} /* end IsButtonHeld() */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void ButtonInitialize(void)

@brief Runs required initialization for the task.  

Should only be called once in main init section.
Configures the button system for the product including enabling button GPIO interrupts.  

Requires:
- GPIO configuration is already complete for all button inputs
- Button interrupt initializations and handler functions are ready
 
Promises:
- G_abButtonDebounceActive, LGaeButtonPreviousState and Button_aeCurrentState 
  are intialized
- GGstButtonTrackballPosition fields are all initialized to default values
- The button state machine is initialized to Idle

*/
void ButtonInitialize(void)
{
  /* Setup default data and interrupts for all of the buttons in the system */
  for(u8 i = 0; i < U8_TOTAL_BUTTONS; i++)
  {
    /* Default status data */
    Button_asStatus[i].bNewPressFlag = FALSE;
    Button_asStatus[i].eCurrentState = RELEASED;
    Button_asStatus[i].eNewState     = RELEASED;
    Button_asStatus[i].u32TimeStamp  = 0;

    /* Event configuration for toggle events */
    nrf_gpiote_event_config(G_asBspButtonConfigurations[i].eChannelNumber, 
                            G_asBspButtonConfigurations[i].u8PinNumber, 
                            NRF_GPIOTE_POLARITY_TOGGLE);
    
    /* GPIOE Interrupt setup */
    if(G_asBspButtonConfigurations[i].eChannelNumber != GPIOE_NO_CHANNEL)
    {
      NRF_GPIOTE->INTENSET = G_asBspButtonConfigurations[i].u32GpioeChannelBit; 
    }
  }
  
  /* Init complete: set function pointer */
  Button_pfnStateMachine = ButtonSM_Idle;
  
 } /* end ButtonInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function ButtonRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void ButtonRunActiveState(void)
{
  Button_pfnStateMachine();

} /* end ButtonRunActiveState */



/*!----------------------------------------------------------------------------------------------------------------------
@fn void ButtonStartDebounce(GpioeChannelType eEventChannel_)

@brief Called only from ISR: sets the "debounce active" flag and debounce start time  

Requires:
- Only the GPIOE ISR should call this function

@param eEventChannel_ is the nRF Event Channel on which the interrupt occurred and 
therefore should start debouncing

Promises:
- If the indicated button is found in G_asBspButtonConfigurations, then the
corresponding interrupt is disabled and debounce information is set in Button_asStatus

*/
void ButtonStartDebounce(GpioeChannelType eEventChannel_)
{
  u8 u8Button = 0;
  
  /* Parse through to find the button */
  for(u8 i = 0; i < U8_TOTAL_BUTTONS; i++)
  {
    if(G_asBspButtonConfigurations[i].eChannelNumber == eEventChannel_)
    {
      u8Button = i;
      break;
    }
  }
  
  /* If the button has been found, disable the interrupt and update debounce status */
  if(u8Button != NOBUTTON)
  {
    NRF_GPIOTE->INTENCLR = G_asBspButtonConfigurations[u8Button].u32GpioeChannelBit;
    NRF_GPIOTE->EVENTS_IN[G_asBspButtonConfigurations[u8Button].eChannelNumber] = 0;   

    Button_asStatus[u8Button].bDebounceActive = TRUE;
    Button_asStatus[u8Button].u32DebounceTimeStart = G_u32SystemTime1ms;
  }
  
} /* end ButtonStartDebounce() */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Function Definitions

The button state machine monitors button activity and manages debouncing and
maintaining the global button states.
***********************************************************************************************************************/

/*!-------------------------------------------------------------------------------------------------------------------
@fn static void ButtonSM_Idle(void)

@brief Look for at least one button to enter debouncing state
*/
static void ButtonSM_Idle(void)                
{
  for(u8 i = 0; i < U8_TOTAL_BUTTONS; i++)
  {
    if(Button_asStatus[i].bDebounceActive)
    {
      Button_pfnStateMachine = ButtonSM_ButtonActive;
      break;
    }
  }
  
} /* end ButtonSM_Idle(void) */



/*!-------------------------------------------------------------------------------------------------------------------
@fn static void ButtonSM_ButtonActive(void)

@brief Process each button that is debouncing.

Time out the debounce period and set the "pressed" state if button action is confirmed.
Manage the hold timers.
*/
static void ButtonSM_ButtonActive(void)         
{
//  u32 *pu32PortAddress;
//  u32 *pu32InterruptAddress;
  u32 u32Input;

  /* Start by resetting back to Idle in case no buttons are active */
  Button_pfnStateMachine = ButtonSM_Idle;

  /* Check for buttons that are debouncing */
  for(u8 i = 0; i < U8_TOTAL_BUTTONS; i++)
  {
    /* Load address offsets for the current button */
//    pu32PortAddress = (u32*)(&(AT91C_BASE_PIOA->PIO_PDSR) + G_asBspButtonConfigurations[i].ePort);
//    pu32InterruptAddress = (u32*)(&(AT91C_BASE_PIOA->PIO_IER) + G_asBspButtonConfigurations[i].ePort);
    
    /* Check if the current button is debouncing */
    if( Button_asStatus[i].bDebounceActive )
    {
      /* Still have an active button */
      Button_pfnStateMachine = ButtonSM_ButtonActive;
      
      /* Check if debounce period is over */
      if( IsTimeUp(&Button_asStatus[i].u32DebounceTimeStart, U32_BUTTON_DEBOUNCE_TIME) )
      {
        /* Read the pin state and invert for ACTIVE_LOW */
        u32Input = NRF_GPIO->IN;
        if(G_asBspButtonConfigurations[i].eActiveState == ACTIVE_LOW)
        {
          u32Input = ~u32Input;
        }
        
        /* Mask the bit of interest to check its state */
        if( u32Input & G_asBspButtonConfigurations[i].u32BitPosition )
        {          
          Button_asStatus[i].eNewState = PRESSED;
        }
        else
        {
          Button_asStatus[i].eNewState = RELEASED;
        }
        
        /* Update if the button state has changed */
        if( Button_asStatus[i].eNewState != Button_asStatus[i].eCurrentState )
        {
          Button_asStatus[i].eCurrentState = Button_asStatus[i].eNewState;
          
          /* If the new state is PRESSED, update the new press flag */
          if(Button_asStatus[i].eCurrentState == PRESSED)
          {
            Button_asStatus[i].bNewPressFlag = TRUE;
            Button_asStatus[i].u32TimeStamp  = G_u32SystemTime1ms;
          }
        }

        /* Regardless of a good press or not, clear the debounce active flag and re-enable the interrupts */
        Button_asStatus[i].bDebounceActive = FALSE;
        NRF_GPIOTE->INTENSET = G_asBspButtonConfigurations[i].u32GpioeChannelBit;
        
      } /* end if( IsTimeUp...) */
    } /* end if(Button_asStatus[i].bDebounceActive) */
  } /* end for (u8 i = 0; i < U8_TOTAL_BUTTONS; i++) */
  
} /* end ButtonSM_ButtonActive() */





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
