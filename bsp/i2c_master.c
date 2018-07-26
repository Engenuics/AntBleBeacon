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

static bool I2cMaster_bBusy;       

static u8 I2cMaster_u8NumberBytes;       
static u8* I2cMaster_pu8TransferBytes;   


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn bool I2cMasterTx(u8 u8Size_, u8* pu8Data_)

@brief Sends u8Size_ bytes starting at pu8Data_ to the Slave.

Requires:
@param u8Size_ is the number of bytes to send
@param pu8Data_ points to the start of the data

Promises:
- Returns TRUE if the transmission is started
- Returns FALSE if the peripheral is already busy
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

  /* Interrupts on STOPPED (event 1), RXDRDY (event 2), and TXDSENT (event 7) */
  NRF_TWI0->INTENSET  = (1 << 1) || (1 << 2) || (1 << 7);
  
  /* Configure driver and enable interrupts */
  I2cMaster_bBusy = FALSE;
  NRF_TWI0->ENABLE = 0x5;
  
  
#ifdef SOFTDEVICE_ENABLED  
  /* TWI0 interrupts */
  u32Result |= sd_nvic_SetPriority(SPI0_TWI0_IRQn, NRF_APP_PRIORITY_LOW);
  u32Result |= sd_nvic_EnableIRQ(SPI0_TWI0_IRQn);
#else
  NVIC_SetPriority(SPI0_TWI0_IRQn, NRF_APP_PRIORITY_LOW);
  NVIC_EnableIRQ(SPI0_TWI0_IRQn);
#endif

} /* end I2cMasterInitialize() */



/*!----------------------------------------------------------------------------------------------------------------------
@fn ISR void TWI0_IRQHandler(void)
@brief Custom TWI0 ISR for handling TWI transmit, receive and Stop 

Requires:

Promises:
- G_u32SystemTime1ms is updated; G_u32SystemTime1s incremented every 1000 ticks

*/
void TWI0_IRQHandler(void)
{
  /* Transmit complete event */
  if(NRF_TWI0->EVENTS_TXDSENT)
  {
    /* Clear the event and check if any more bytes need to be sent */
    NRF_TWI0->EVENTS_TXDSENT = 0;
    I2cMaster_u8NumberBytes--;
    if(I2cMaster_u8NumberBytes == 0)
    {
      /* If finished, then release the TWI resource */
      I2cMaster_bBusy = FALSE;
      I2cMaster_pu8TransferBytes = NULL;
    }
    else
    {
      /* Load the next byte and advance the pointer */
      NRF_TWI0->TXD = *I2cMaster_pu8TransferBytes;
    }
  }

} /* end TWI0_IRQHandler() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
