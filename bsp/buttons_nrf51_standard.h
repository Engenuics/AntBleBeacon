/*!**********************************************************************************************************************
@file buttons_nrf51_standard.h        
@brief Header file for buttons_nrf51_standard.c                                                        
***********************************************************************************************************************/

#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "configuration.h"

/***********************************************************************************************************************
Type Definitions
***********************************************************************************************************************/

/*! 
@enum ButtonStateType
@brief Self-documenting button state type */
typedef enum {RELEASED, PRESSED} ButtonStateType; 


/*! 
@struct ButtonStatusType
@brief Required parameters for the task to track what each button is doing. 
*/
typedef struct 
{
  bool bDebounceActive;                   /*!< @brief TRUE by ISR if a button interrupt occurs */
  bool bNewPressFlag;                     /*!< @brief TRUE if the press has not been acknowledged */
  ButtonStateType eCurrentState;          /*!< @brief Current state of the button */
  ButtonStateType eNewState;              /*!< @brief New state of the button */
  u32 u32DebounceTimeStart;               /*!< @brief System time loaded by ISR when button interrupt occurs */
  u32 u32TimeStamp;                       /*!< @brief System time when the button was pressed */
}ButtonStatusType;


/***********************************************************************************************************************
Constants / Definitions
***********************************************************************************************************************/
#define U32_BUTTON_DEBOUNCE_TIME            (u32)10       /*!< @brief Time in ms for button debouncing */


/***********************************************************************************************************************
Function Declarations
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
bool IsButtonPressed(ButtonNameType eButton_);
bool WasButtonPressed(ButtonNameType eButton_);
void ButtonAcknowledge(ButtonNameType eButton_);
bool IsButtonHeld(ButtonNameType eButton_, u32 u32ButtonHeldTime_);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void ButtonInitialize(void);                        
void ButtonRunActiveState(void);
u8 ButtonGetActiveColumn(void);
void ButtonStartDebounce(u32 u32BitPosition_);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void ButtonRotateColumns(void);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void ButtonSM_Idle(void);                
static void ButtonSM_ButtonActive(void);        


#endif /* __BUTTONS_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
