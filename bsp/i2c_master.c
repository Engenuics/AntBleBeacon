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
#if 0
  /* Set up I²C-specific GPIO */
  NRF_GPIO->PIN_CNF[ANT_SPI_MOSI_BIT] = P0_13_ANT_SPI_MOSI_CNF;
  NRF_GPIO->PIN_CNF[ANT_SPI_MISO_BIT] = P0_12_ANT_SPI_MISO_CNF;
  NRF_GPIO->PIN_CNF[ANT_SPI_SCK_BIT]  = P0_11_ANT_SPI_SCK_CNF;
  NRF_GPIO->PIN_CNF[ANT_SEN_BIT]      = P0_10_ANT_SEN_CNF;

  NRF_SPI0->PSELMOSI = P0_13_ANT_SPI_MOSI_INDEX;
  NRF_SPI0->PSELMISO = P0_12_ANT_SPI_MISO_INDEX;  
  NRF_SPI0->PSELSCK  = P0_11_ANT_SPI_SCK_INDEX;

  /* Set up SPI Master peripheral */
  NRF_SPI0->FREQUENCY = SPI_FREQUENCY_FREQUENCY_M1 << SPI_FREQUENCY_FREQUENCY_Pos;
  NRF_SPI0->CONFIG   = (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos | \
                        SPI_CONFIG_CPHA_Leading   << SPI_CONFIG_CPHA_Pos  | \
                        SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos );
#endif
  /* Configure SPI driver */
  
  
  /* Activate SPI peripheral */

} /* end I2cMasterInitialize() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
