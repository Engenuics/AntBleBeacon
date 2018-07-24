/**********************************************************************************************************************
File: i2c_master.c                                                                

Description:
I2C master implementation using I2C master perihperal but not Nordic SDK.
**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_xxI2C"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32I2cMasterFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "I2cMaster_" and be declared as static.
***********************************************************************************************************************/
static u32 I2cMaster_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: I2cMasterTxByte

Description:
Sends one byte of data to the slave.

Requires:
  - 

Promises:
  - Returns TRUE if slave ACKs sent byte
*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: I2cMasterInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void I2cMasterInitialize(void)
{
  /* Set up I²C-specific GPIO */
  NRF_TWI0->PSELSCL = P0_16_INDEX;
  NRF_TWI0->PSELSDA = P0_15_INDEX;  

  /* Set up I²C Master peripheral and Interrupts */
  NRF_TWI0->FREQUENCY = TWI_FREQUENCY_FREQUENCY_K100;
  NRF_TWI0->INTENSET = 
  
  /* Configure driver */
  
  
  /* Activate peripheral and enable interrupts */
  NRF_TWI0->ENABLE = 0x5;
  
#ifdef SOFTDEVICE_ENABLED  
  /* Must enable the SoftDevice Interrupt first */
  u32Result |= sd_nvic_SetPriority(SD_EVT_IRQn, NRF_APP_PRIORITY_LOW);
  u32Result |= sd_nvic_EnableIRQ(SD_EVT_IRQn);

  /* GPIOE interrupts */
  u32Result |= sd_nvic_SetPriority(GPIOTE_IRQn, NRF_APP_PRIORITY_LOW);
  u32Result |= sd_nvic_EnableIRQ(GPIOTE_IRQn);
#endif

} /* end I2cMasterInitialize() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
