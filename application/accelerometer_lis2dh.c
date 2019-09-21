/*!**********************************************************************************************************************
@file accelerometer_lis2di.c                                                                
@brief Driver file for communication to the ST LIS2DH accelerometer.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- 

CONSTANTS
- 
TYPES
-  struct ButtonStatusType

PUBLIC FUNCTIONS
- 

PROTECTED FUNCTIONS
- 

**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>Lis2dh"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32Lis2dhFlags;                   /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;          /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;           /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;            /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Lis2dh_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Lis2dh_pfStateMachine;        /*!< @brief The state machine function pointer */
static fnCode_type Lis2dh_pfNextState;           /*!< @brief The next state to select after a wait state */
static u32 Lis2dh_u32Timeout;                    /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhInitialize(void)

@brief Initializes the task and its variables.

Requires:
-

Promises:
  - 

*/
void Lis2dhInitialize(void)
{
  /* Request the I2C peripheral */
  if( !I2cTakeSemaphore() )
  {
    Lis2dhSetError(U8_LIS2DH_ERROR_NOI2C);
  }
  else
  {
    /* Setup for I2C initialization */
    I2cSetSlaveAddress(LIS2DH_BASE_ADDRESS);
    
    Lis2dh_pfStateMachine = Lis2dhSM_Configure;
    Lis2dh_pfNextState = Lis2dhSM_Configure;
  }

} /* end Lis2dhInitialize() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void Lis2dhRunActiveState(void)
{
  Lis2dh_pfStateMachine();

} /* end Lis2dhRunActiveState */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhSetResolution(void)

@brief Initializes the task and its variables.

Requires:
-

Promises:
  - 

*/
void Lis2dhSetResolution(void)
{

} /* end Lis2dhSetResolution() */

// Set for 8-bit 100Hz (Low power mode)
// Bypass mode (no FIFO)?
// Read temperature
// Send and read X command
// Don't forget we might have to solder jumper the IC to pull-up SA0

/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void Lis2dhSetError(u8 u8ErrorCode_)

@brief Sets up the LEDs to display an error code and then 
selects the error state.

Requires:
@param u8ErrorCode_ is the error code value to setup currently
allowed to be 0 to (U8_TOTAL_LEDS - 1).

Promises:
- Lis2dh_pfStateMachine set to error state 

*/
void Lis2dhSetError(u8 u8ErrorCode_)
{
  /* Turn off all LEDs */
  for(u8 i = 0; i < U8_TOTAL_LEDS; i++)
  {
    LedOff( (LedNameType)i);
  }
  
  /* Indicate the error code by turning on corresponding LED
  Yes, the user will have to count. */
  LedOn( (LedNameType)u8ErrorCode_);
  Lis2dh_pfStateMachine = Lis2dhSM_Error; 

} /* end Lis2dhSetError() */


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void Lis2dhSM_Configure(void)
{
    
} /* end Lis2dhSM_Configure() */


/* What does this state do? */
static void Lis2dhSM_Idle(void)
{
    
} /* end Lis2dhSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for I2C to finish */
static void Lis2dhSM_CommBusyWait(void)          
{
  if( !I2cCheckBusy() )
  {
    Lis2dh_pfStateMachine = Lis2dh_pfNextState;
  }
  
} /* end Lis2dhSM_CommBusyWait() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Hold the error state */
static void Lis2dhSM_Error(void)          
{
  
} /* end Lis2dhSM_Error() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
