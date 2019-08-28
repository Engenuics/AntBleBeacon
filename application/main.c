/***********************************************************************************************************************
File: main.c                                                                

Description:
Container for the nRF51422 firmware.  
***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32SystemTime1ms = 0;     /*!< @brief Global system time incremented every ms, max 2^32 (~49 days) */
volatile u32 G_u32SystemTime1s  = 0;     /*!< @brief Global system time incremented every second, max 2^32 (~136 years) */
volatile u32 G_u32SystemFlags   = 0;     /*!< @brief Global system flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Main_" and be declared as static.
***********************************************************************************************************************/
static u32 Main_u32ErrorCode;
//static u8 Main_u8TestMessage[] = "9876 test message from ANT";


/***********************************************************************************************************************
Function Definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Main Program
Main has two sections:

1. Initialization which is run once on power-up or reset.  All drivers and applications are setup here without timing
contraints but must complete execution regardless of success or failure of starting the application. 

2. Main loop.  This is an event-driven system that sequentially executes tasks that require servicing in the event queue.

***********************************************************************************************************************/
void main(void)
{
#ifdef SOFTDEVICE_ENABLED  
  /* This must be done before any RAM accesses.
  Enable the s310 SoftDevice Stack. If Failure, we shall not progress as 
  successive code is dependent on SD success. 
  Once the SD is enabled, the application shall only use the SD provided system calls
  for access to processor resources that are restricted by the SD. */
  Main_u32ErrorCode = sd_softdevice_enable(NRF_CLOCK_LFCLKSRC_SYNTH_250_PPM, SocSoftdeviceAssertCallback);
  
  if ( Main_u32ErrorCode != NRF_SUCCESS )
  {
    NRF_GPIO->OUTSET = P0_23_RED0 | P0_24_RED7;
    while (1);
  }
#endif
  
  G_u32SystemFlags |= _SYSTEM_INITIALIZING;

  /* Low Level Initialization Modules */
  WatchDogSetup(); 
  GpioSetup();
  ClockSetup();
  InterruptSetup();
  PowerSetup();
  SysTickSetup();
    
  /* Driver initialization */
  LedInitialize();
  ButtonInitialize();
  I2cMasterInitialize();

#ifdef SOFTDEVICE_ENABLED
  ANTIntegrationInitialize();
  BLEIntegrationInitialize();
  bleperipheralInitialize();
#endif
  
  /* Application initialization */
  //AccelInitialize();
  PovInitialize();
  UserApp1Initialize();
  
  /* Exit initialization */
  G_u32SystemFlags &= ~_SYSTEM_INITIALIZING;
  
  /* Main loop */  
  while(1)
  {
    SocIntegrationHandler();

    /* Driver and Application State Machines */
    LedRunActiveState();
    ButtonRunActiveState();
    //I2cRunActiveState(); Currently not required.
   
    //AccelRunActiveState();
    PovRunActiveState();
    UserApp1RunActiveState();

    SystemSleep();
    
  } /* end while(1) main super loop */
  
} /* end main() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
