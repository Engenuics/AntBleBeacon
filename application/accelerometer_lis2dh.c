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
volatile u32 G_u32Lis2dhFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Lis2dh_<type>" and be declared as static.
***********************************************************************************************************************/
static u32 Lis2dh_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
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

} /* end Lis2dhInitialize() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/