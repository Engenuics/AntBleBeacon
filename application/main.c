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
static u8 Main_u8TestMessage[] = "9876 test message from ANT";


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
  G_u32SystemFlags |= _SYSTEM_INITIALIZING;
  
  /*
  Due to the switch that the application makes from the User mode system to SoftDevice
  mode System, this module sets up the basic clocks and I/Os, followed by enabling the SD.
  Once the SD is enabled, the application shall only use the SD provided system calls
  */
  __disable_interrupt();

  /* Clock, GPIO and SoftDevice setup */  
  ClockSetup();
  GpioSetup();

  __enable_interrupt();
  
  /* Enable the s310 SoftDevice Stack. If Failure, we shall not progress as 
  successive code is dependent on SD success. */
  if (!SocIntegrationInitialize())
  {
    /* Lite up all the RED LEDs to indicate this state */
    NRF_GPIO->OUTSET = (RED0 | RED1 | RED2 | RED3 | RED4 | RED5 | RED6 | RED7);
    while (1);
  }
  
  /* Low Level Initialization Modules */
  WatchDogSetup(); 
  SysTickSetup();
  InterruptsInitialize();
    
  /* Driver initialization */
  LedInitialize();
  ButtonInitialize();
  // I2cInitialize();

  ANTIntegrationInitialize();
  BLEIntegrationInitialize();
  bleperipheralInitialize();
  
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
    //I2cRunActiveState();
   
    //AccelRunActiveState();
    PovRunActiveState();
    UserApp1RunActiveState();

    SystemSleep();
    
  } /* end while(1) main super loop */
  
} /* end main() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
