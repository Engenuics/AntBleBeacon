/*!**********************************************************************************************************************
@file i2c_master.c                                                                
@brief I2C master implementation using I2C master perihperal but not Nordic SDK.

This is interrupt-driven send and receive function that will not block the 
processor from running other code while read or write transfers are taking place.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- 
TYPES
- 
PUBLIC FUNCTIONS
- 
PROTECTED FUNCTIONS
- 


***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_xxI2C"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32I2cMasterFlags;                       /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "I2cMaster_" and be declared as static.
***********************************************************************************************************************/
// static u32 I2cMaster_u32Timeout;               /*!< @brief Timeout counter used across states */

static bool I2cMaster_bBusy;                   /*!< @brief TRUE when TWI peripheral is in use */
static bool I2cMaster_bSemaphoreTaken;         /*!< @brief TRUE when a task claims the TWI peripheral */    
static bool I2cMaster_bRepeatedStart;          /*!< @brief TRUE when TWI peripheral has completed a repeated start */   

static u8 I2cMaster_u8NumberBytes;             
static u8* I2cMaster_pu8TransferBytes;         
static bool I2cMaster_bStopAfterTransfer;      /*!< @brief TRUE if stop condition should be set after transfer */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn u32 I2cSetSlaveAddress(u8 u8Address_)

@brief Sets the 7-bit Slave address

Requires:
@param u8Address_ is the RIGHT-justified 7-bit address of the target I2C device.

Promises:
- Attempts to load TWI0 ADDRESS with u8Address_
- Returns 0 if success, else returns error accumulator

*/
u32 I2cSetSlaveAddress(u8 u8Address_)
{
  /* Check that no high bit is set */
  if( (u8Address_ & 0x80) != 0)
  {
    return 1;
  }
  
  NRF_TWI0->ADDRESS = u8Address_;
  return 0;
  
} /* end I2cSetSlaveAddress() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool I2cMasterTx(u8 u8Size_, u8* pu8Data_, bool bStopAfterTransfer_)

@brief Sends u8Size_ bytes starting at pu8Data_ to the Slave.

Client can check if peripheral is busy to determine if transmission
is complete.  

Requires:
@param u8Size_ is the number of bytes to send
@param pu8Data_ points to the start of the data
@param bStopAfterTransfer_ is TRUE if STOP condition should be set after transfer

Promises:
- Returns TRUE if the transmission is started
- Returns FALSE if the peripheral is already busy or the semaphore hasn't been given
*/
bool I2cMasterTx(u8 u8Size_, u8* pu8Data_, bool bStopAfterTransfer_)
{
  /* Exit immediately if peripheral is already busy */
  if( (I2cMaster_bBusy == TRUE) || (I2cMaster_bSemaphoreTaken == FALSE) )
  {
    return FALSE;
  }
  
  /* Mark peripheral busy and set up globals */
  I2cMaster_bBusy = TRUE;
  I2cMaster_u8NumberBytes = u8Size_;
  I2cMaster_pu8TransferBytes = pu8Data_;
  
  /* Load first byte and start the transfer */
  NRF_TWI0->TXD = *I2cMaster_pu8TransferBytes;
  NRF_TWI0->TASKS_STARTTX = 1;
  
  return TRUE;

} /* end bool I2cMasterTx() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool I2cMasterRx(u8 u8Size_, u8* pu8Data_)

@brief Receives u8Size_ bytes into pu8Data_ from the Slave.

Client can check if peripheral is busy to determine if transmission
is complete.  

Requires:
@param u8Size_ is the number of bytes to receive and must fit in the buffer
@param pu8Data_ points to the start of the data reception buffer
@param bStopAfterTransfer_ is TRUE if STOP condition should be set after transfer

Promises:
- Returns TRUE if the transmission is started
- Returns FALSE if the peripheral is already busy or the semaphore hasn't been given
*/
bool I2cMasterRx(u8 u8Size_, u8* pu8Data_)
{
  /* Exit immediately if peripheral is already busy */
  if( (I2cMaster_bBusy == TRUE) || (I2cMaster_bSemaphoreTaken == FALSE) )
  {
    return FALSE;
  }
  
  /* Mark peripheral busy and set up globals */
  I2cMaster_bBusy = TRUE;
  I2cMaster_u8NumberBytes = u8Size_;
  I2cMaster_pu8TransferBytes = pu8Data_;
  
  /* Load first byte and start the transfer */
  NRF_TWI0->TASKS_STARTRX = 1;
  
  return TRUE;

} /* end bool I2cMasterRx() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool I2cTakeSemaphore(void)

@brief Lets a client task take the TWI semaphore and gain access 
to the TWI peripheral (if available)

Requires:
- I2cMaster_bSemaphoreTaken is in the correct state

Promises:
- Returns TRUE if I2cMaster_bSemaphoreTaken is available (currently FALSE)
  and sets I2cMaster_bSemaphoreTaken to TRUE
- Otherwise returns FALSE

*/
bool I2cTakeSemaphore(void)
{
  if(I2cMaster_bSemaphoreTaken)
  {
    return(FALSE);
  }
  else
  {
    I2cMaster_bSemaphoreTaken = TRUE;
    return(TRUE);
  }
  
} /* end I2cTakeSemaphore() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool I2cGiveSemaphore(void)

@brief Returns the TWI semaphore

Currently there is no provision to prevent a task from returning
the semaphore that they don't have, which would then make it 
available.  This is obviously incorrect and can be updated
in the future to improve the robustness of the system.

Requires:
- The client currently has the semaphore.
- I2cMaster_bSemaphoreTaken is TRUE.

Promises:
- I2cMaster_bSemaphoreTaken is FALSE

*/
void I2cGiveSemaphore(void)
{
  I2cMaster_bSemaphoreTaken = FALSE;
  
} /* end I2cGiveSemaphore() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool I2cCheckBusy(void)

@brief Returns the value of I2cMaster_bBusy.

Client can check if peripheral is busy.  

Requires:
- NONE

Promises:
- Returns TRUE if I2cMaster_bBusy is TRUE

*/
bool I2cCheckBusy(void)
{
  return(I2cMaster_bBusy);
  
} /* end I2cCheckBusy() */
   


/*!----------------------------------------------------------------------------------------------------------------------
@fn bool I2cCheckRepeatedStart(void)

@brief Returns the value of I2cMaster_bRepeatedStart.

Client can check if peripheral has generated a repeated start.  

Requires:
- NONE

Promises:
- Returns TRUE if I2cMaster_bRepeatedStart is TRUE

*/
bool I2cCheckRepeatedStart(void)
{
  return(I2cMaster_bRepeatedStart);
  
} /* end I2cCheckRepeatedStart() */

                 
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
  u32 u32Result = 0;
  
  /* Configure driver and enable interrupts */
  I2cMaster_u8NumberBytes = 0;
  I2cMaster_pu8TransferBytes = NULL;
  I2cMaster_bStopAfterTransfer = TRUE;
    
  I2cMaster_bBusy = FALSE;
  I2cMaster_bSemaphoreTaken = FALSE;
  I2cMaster_bRepeatedStart = FALSE;

  /* Set up I²C-specific GPIO */
  NRF_TWI0->PSELSCL = P0_16_INDEX;
  NRF_TWI0->PSELSDA = P0_15_INDEX;  

  /* Set up I²C Master peripheral and Interrupts */
  NRF_TWI0->FREQUENCY = TWI_FREQUENCY_FREQUENCY_K100;

  /* Interrupts on STOPPED (event 1), RXDRDY (event 2), and TXDSENT (event 7) */
  NRF_TWI0->INTENSET  = (1 << 1) || (1 << 2) || (1 << 7);
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
@fn ISR void SPI0_TWI0_IRQHandler(void)
@brief Custom TWI0 ISR for handling TWI transmit, receive and Stop 

Requires:

Promises:
- TXD_SENT: loads the next byte to send if bytes remain, or 
            resets I2cMaster_bBusy so another task may use the peripheral

*/
void SPI0_TWI0_IRQHandler(void)
{
  /* Transmit complete event */
  if(NRF_TWI0->EVENTS_TXDSENT)
  {
    /* Clear the event and check if any more bytes need to be sent */
    NRF_TWI0->EVENTS_TXDSENT = 0;
    I2cMaster_u8NumberBytes--;
    if(I2cMaster_u8NumberBytes == 0)
    {
      I2cMaster_pu8TransferBytes = NULL;
 
      /* Manage STOP request */
      if(I2cMaster_bStopAfterTransfer)
      {
        NRF_TWI0->TASKS_STOP = 1;
        I2cMaster_bBusy = FALSE;
        I2cMaster_bRepeatedStart = FALSE;
      }
      else
      {
        /* If no stop, begin repeated start sequence and update status bool */
        NRF_TWI0->TASKS_STARTRX = 1;
        I2cMaster_bRepeatedStart = TRUE;
      }
    }
    else
    {
      /* Load the next byte and advance the pointer */
      NRF_TWI0->TXD = *I2cMaster_pu8TransferBytes;
      I2cMaster_pu8TransferBytes++;
    }
  }

  /* Receive complete event */
  if(NRF_TWI0->EVENTS_RXDREADY)
  {
    /* Clear the event */
    NRF_TWI0->EVENTS_RXDREADY = 0;
    
    /* Place the byte into the target receive pointer and advance the pointer */
    if(I2cMaster_pu8TransferBytes != NULL)
    {
      /* Check for last byte: must trigger STOP before RXD if at end */
      I2cMaster_u8NumberBytes--;
      if(I2cMaster_u8NumberBytes == 0)
      {
        NRF_TWI0->TASKS_STOP = 1;
      }

      *I2cMaster_pu8TransferBytes = NRF_TWI0->RXD;
      I2cMaster_pu8TransferBytes++;
    }
  }

  /* Stop condition event */
  if(NRF_TWI0->EVENTS_STOPPED)
  {
    I2cMaster_bBusy = FALSE;
    I2cMaster_pu8TransferBytes = NULL;
  }

} /* end SPI0_TWI0_IRQHandler() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
