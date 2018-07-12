/*!**********************************************************************************************************************
@file leds_nrf51.c                                                                
@brief LED driver that provides on, off, toggle, blink and PWM functionality.

The basic on/off/toggle functionality is applied directly to the LEDs.
Blinking and PWMing of LEDs rely on a 1ms system tick to provide timing at
regular 1ms calls to LedUpdate().

This driver relies on a standard LED 

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- LedNameType (devboard-specific)

 (from eief1-pcb-01):
 {WHITE, PURPLE, BLUE, CYAN, 
  GREEN, YELLOW, ORANGE, RED, 
  LCD_RED, LCD_GREEN, LCD_BLUE}

- LedRateType:
  {LED_0HZ = 0, LED_0_5HZ = 1000, LED_1HZ = 500, LED_2HZ = 250, LED_4HZ = 125, LED_8HZ = 63,
   LED_PWM_0 = 0,   LED_PWM_5 = 1,   LED_PWM_10 = 2,  LED_PWM_15 = 3,  LED_PWM_20 = 4, 
   LED_PWM_25 = 5,  LED_PWM_30 = 6,  LED_PWM_35 = 7,  LED_PWM_40 = 8,  LED_PWM_45 = 9, 
   LED_PWM_50 = 10, LED_PWM_55 = 11, LED_PWM_60 = 12, LED_PWM_65 = 13, LED_PWM_70 = 14, 
   LED_PWM_75 = 15, LED_PWM_80 = 16, LED_PWM_85 = 17, LED_PWM_90 = 18, LED_PWM_95 = 19, 
   LED_PWM_100 = 20} 

PUBLIC FUNCTIONS
- void LedOn(LedNameType eLED_)
- void LedOff(LedNameType eLED_)
- void LedToggle(LedNameType eLED_)
- void LedBlink(LedNameType eLED_, LedRateType eBlinkRate_)
- void LedPWM(LedNameType eLED_, LedRateType ePwmRate_)

PROTECTED FUNCTIONS
- void LedInitialize(void)
- void LedRunActiveState(void)

DISCLAIMER: THIS CODE IS PROVIDED WITHOUT ANY WARRANTY OR GUARANTEES.  USERS MAY
USE THIS CODE FOR DEVELOPMENT AND EXAMPLE PURPOSES ONLY.  ENGENUICS TECHNOLOGIES
INCORPORATED IS NOT RESPONSIBLE FOR ANY ERRORS, OMISSIONS, OR DAMAGES THAT COULD
RESULT FROM USING THIS FIRMWARE IN WHOLE OR IN PART.
***********************************************************************************************************************/

#include "configuration.h"


/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_xxLed"
***********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------*/
/* New variables (all shall start with G_xxLed*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */
extern volatile u32 G_u32SystemTime1ms;                /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                 /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                  /*!< @brief From main.c */

extern const Nrf51PinConfigurationType G_asBspLedConfigurations[U8_TOTAL_LEDS]; /*!< @brief from board-specific file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Led_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Led_StateMachine;                   /*!< @brief The state machine function pointer */

static LedControlType Led_asControl[U8_TOTAL_LEDS];    /*!< @brief Holds individual control parameters for LEDs */


/***********************************************************************************************************************
* Function Definitions
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions */
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedOn(LedNameType eLED_)

@brief Turn the specified LED on.  

This function automatically takes care of the active low vs. active high LEDs.
The function works immediately (it does not require the main application
loop to be running). 

Currently it only supports one LED at a time.

Example:

LedOn(BLUE);


Requires:
- Definitions in G_asBspLedConfigurations[eLED_] and Led_asControl[eLED_] are correct

@param eLED_ is a valid LED index

Promises:
- eLED_ is turned on 
- eLED_ is set to LED_NORMAL_MODE mode

*/
void LedOn(LedNameType eLED_)
{

  if(G_asBspLedConfigurations[eLED_].eActiveState == ACTIVE_HIGH)
  {
    NRF_GPIO->OUTSET = G_asBspLedConfigurations[eLED_].u32BitPosition;
  }
  else
  {
    NRF_GPIO->OUTCLR = G_asBspLedConfigurations[eLED_].u32BitPosition;
  }
  
  /* Always set the LED back to LED_NORMAL_MODE mode */
	Led_asControl[eLED_].eMode = LED_NORMAL_MODE;

} /* end LedOn() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedOff(LedNameType eLED_)

@brief Turn the specified LED off.

This function automatically takes care of the active low vs. active high LEDs.
It works immediately (it does not require the main application
loop to be running). 

Currently it only supports one LED at a time.

Example:

LedOff(BLUE);


Requires:
- Definitions in G_asBspLedConfigurations[eLED_] and Led_asControl[eLED_] are correct

@param eLED_ is a valid LED index

Promises:
- eLED_ is turned off 
- eLED_ is set to LED_NORMAL_MODE mode

*/
void LedOff(LedNameType eLED_)
{
  if(G_asBspLedConfigurations[eLED_].eActiveState == ACTIVE_HIGH)
  {
    NRF_GPIO->OUTCLR = G_asBspLedConfigurations[eLED_].u32BitPosition;
  }
  else
  {
    NRF_GPIO->OUTSET = G_asBspLedConfigurations[eLED_].u32BitPosition;
  }
  
  /* Always set the LED back to LED_NORMAL_MODE mode */
	Led_asControl[(u8)eLED_].eMode = LED_NORMAL_MODE;
  
} /* end LedOff() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedToggle(LedNameType eLED_)

@brief Toggles the specified LED from on to off or vise-versa.

This function automatically takes care of the active low vs. active high LEDs.
It works immediately (it does not require the main application
loop to be running). 

Currently it only supports one LED at a time.

Example:

LedToggle(BLUE);


Requires:
@param eLED_ is a valid LED index

Promises:
- eLED_ is toggled 
- eLED_ is set to LED_NORMAL_MODE

*/
void LedToggle(LedNameType eLED_)
{
  u32 u32Current_Leds;
  
  u32Current_Leds = NRF_GPIO->IN;
  u32Current_Leds ^= G_asBspLedConfigurations[(u8)eLED_].u32BitPosition;
  NRF_GPIO->OUT = u32Current_Leds;
                                            
} /* end LedToggle() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedPWM(LedNameType eLED_, LedRateType ePwmRate_)

@brief Sets an LED to PWM mode

Requires:
@param eLED_ is a valid LED index
@param ePwmRate_ is an allowed duty cycle:
    LED_PWM_0, LED_PWM_5, LED_PWM_10, ..., LED_PWM_95, LED_PWM_100

Promises:
- Requested LED is set to PWM mode at the duty cycle specified

*/
void LedPWM(LedNameType eLED_, LedRateType ePwmRate_)
{
	Led_asControl[(u8)eLED_].eMode = LED_PWM_MODE;
	Led_asControl[(u8)eLED_].eRate = ePwmRate_;
	Led_asControl[(u8)eLED_].u16Count = (u16)ePwmRate_;
  Led_asControl[(u8)eLED_].eCurrentDuty = LED_PWM_DUTY_HIGH;

} /* end LedPWM() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedBlink(LedNameType eLED_, LedRateType eBlinkRate_)

@brief Sets an LED to BLINK mode.

Requires:
@param eLED_ is a valid LED index
@param eBlinkRate_ is an allowed frequency:
    LED_0_5HZ, LED_1HZ, LED_2HZ, LED_4HZ, LED_8HZ

Promises:
- Requested LED is set to BLINK mode at the rate specified

*/
void LedBlink(LedNameType eLED_, LedRateType eBlinkRate_)
{
	Led_asControl[(u8)eLED_].eMode = LED_BLINK_MODE;
	Led_asControl[(u8)eLED_].eRate = eBlinkRate_;
	Led_asControl[(u8)eLED_].u16Count = eBlinkRate_;

} /* end LedBlink() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedRainbow(void)

@brief Sets a rainbow pattern on the LEDs

Requires:
- NONE

Promises:
- LED0 white to LED7 red 

*/
void LedRainbow(void)
{
  LedOn(RED0);
  LedOn(GRN0);
  LedOn(BLU0);
  
  LedOn(RED1);
  LedOff(GRN1);
  LedOn(BLU1);
  
  LedOff(RED2);
  LedOff(GRN2);
  LedOn(BLU2);

  LedOff(RED3);
  LedOn(GRN3);
  LedOn(BLU3);

  LedOff(RED4);
  LedOn(GRN4);
  LedOff(BLU4);
  
  LedOn(RED5);
  LedOn(GRN5);
  LedOff(BLU5);
 
  LedOn(RED6);
  LedPWM(GRN6, LED_PWM_30);
  LedOff(BLU6);

  LedOn(RED7);
  LedOff(GRN7);
  LedOff(BLU7);
  
} /* end LedRainbow() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions */
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedInitialize(void)

@brief Initialization of LED system paramters and visual LED check.

Requires:
- G_u32SystemTime1ms ticking
- All LEDs already initialized to LED_NORMAL_MODE mode ON

Promises:
- All LEDs in LED_NORMAL_MODE mode with OFF

*/
void LedInitialize(void)
{
  LedNameType aeLedSequence[] = {RED0, RED1, RED2, RED3, RED4, RED5, RED6, RED7,
                                 GRN0, GRN1, GRN2, GRN3, GRN4, GRN5, GRN6, GRN7,
                                 BLU0, BLU1, BLU2, BLU3, BLU4, BLU5, BLU6, BLU7};

  /* Static Display of all colors */
  LedRainbow();

  
  //while(1);
  /* end of static display */

#if 0
  {
    /* Sequentially light up a specific color of LEDs */
    for(u8 k = 0; k < 3; k++)
    {
      for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
      {
        LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * k)]);
        for(u32 j = 0; j < 200000; j++);
      }
      
      /* Pause for show */
      for(u32 i = 0; i < 500000; i++);

      /* Turn off the current LEDs */
      for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
      {
        LedOff(aeLedSequence[i + (8 * k)]);
      }
    }
#endif

#if 0 /* one-way sequence */
  {
    /* Red */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Pause for show */
    for(u32 i = 0; i < PAUSE_TIME; i++);

    /* All off */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[i + (8 * 0)]);
      LedOff(aeLedSequence[i + (8 * 1)]);
      LedOff(aeLedSequence[i + (8 * 2)]);
    }

    /* Yellow */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Pause for show */
    for(u32 i = 0; i < PAUSE_TIME; i++);

    /* All off */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[i + (8 * 0)]);
      LedOff(aeLedSequence[i + (8 * 1)]);
      LedOff(aeLedSequence[i + (8 * 2)]);
    }

    /* Green */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Pause for show */
    for(u32 i = 0; i < PAUSE_TIME; i++);

    /* All off */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[i + (8 * 0)]);
      LedOff(aeLedSequence[i + (8 * 1)]);
      LedOff(aeLedSequence[i + (8 * 2)]);
    }

    /* Cyan */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Pause for show */
    for(u32 i = 0; i < PAUSE_TIME; i++);

    /* All off */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[i + (8 * 0)]);
      LedOff(aeLedSequence[i + (8 * 1)]);
      LedOff(aeLedSequence[i + (8 * 2)]);
    }

    /* Blue */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Pause for show */
    for(u32 i = 0; i < PAUSE_TIME; i++);

    /* All off */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[i + (8 * 0)]);
      LedOff(aeLedSequence[i + (8 * 1)]);
      LedOff(aeLedSequence[i + (8 * 2)]);
    }

    
    /* Purple */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Pause for show */
    for(u32 i = 0; i < PAUSE_TIME; i++);

    /* All off */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[i + (8 * 0)]);
      LedOff(aeLedSequence[i + (8 * 1)]);
      LedOff(aeLedSequence[i + (8 * 2)]);
    }

    /* White */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Pause for show */
    for(u32 i = 0; i < PAUSE_TIME; i++);

    /* All off */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[i + (8 * 0)]);
      LedOff(aeLedSequence[i + (8 * 1)]);
      LedOff(aeLedSequence[i + (8 * 2)]);
    }

  }
#endif
  
#if 0  
  {
    /* Red */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    for(u32 j = 0; j < STEP_TIME; j++);

    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 0)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Yellow */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    for(u32 j = 0; j < STEP_TIME; j++);

    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 1)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Green */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    for(u32 j = 0; j < STEP_TIME; j++);

    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 1)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Cyan */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    for(u32 j = 0; j < STEP_TIME; j++);

    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 1)]);
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Blue */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    for(u32 j = 0; j < STEP_TIME; j++);

   for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* Purple */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    for(u32 j = 0; j < STEP_TIME; j++);

    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    /* While */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 1)]);
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    

    for(u32 j = 0; j < STEP_TIME; j++);

    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 0)]);
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 1)]);
      LedOff(aeLedSequence[7 - i + (NUM_LEDS_PER_COLOR * 2)]);
      for(u32 j = 0; j < STEP_TIME; j++);
    }    
  }
#endif

#if 0
  {
    /* Red */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
      for(u32 j = 0; j < STEP_TIME; j++);
      LedOff(aeLedSequence[i + (NUM_LEDS_PER_COLOR * 0)]);
    }    

    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[(7 - i) + (NUM_LEDS_PER_COLOR * 0)]);
      for(u32 j = 0; j < STEP_TIME; j++);
      LedOff(aeLedSequence[(7 - i) + (NUM_LEDS_PER_COLOR * 0)]);
    }    

  }
  
  /* Finish with all white */
    for(u8 i = 0; i < NUM_LEDS_PER_COLOR; i++)
    {
      LedOn(aeLedSequence[i]);
      LedOn(aeLedSequence[i + NUM_LEDS_PER_COLOR]);
      LedOn(aeLedSequence[i + (2 * NUM_LEDS_PER_COLOR)]);
      for(u32 j = 0; j < 100000; j++);
    }

    /* Pause for show */
  for(u32 i = 0; i < 2000000; i++);
#endif

#if 0
  /* Turn all LEDs on full, then fade them out over a few seconds */
  for(u8 i = 20; i > 0; i--)
  {
    
    /* Spend 40ms in each level of intensity */
    for(u16 j = 40; j > 0; j--)
    {
      u32Timer = G_u32SystemTime1ms;
      while( !IsTimeUp(&u32Timer, 1) );
      LedUpdate();
    }
    /* Pause for a bit on the first iteration to show the LEDs on for little while */
    if(i == 20)
    {
      while( !IsTimeUp(&u32Timer, 1500) );
    }
    
    /* Set the LED intensity for the next iteration */
    for(u8 j = 0; j < TOTAL_LEDS; j++)
    {
      Led_asControl[j].eRate = (LedRateType)(i - 1);
    }
  }

  /* Final update to set last state, hold for a short period */
  LedSM_Idle();
  while( !IsTimeUp(&u32Timer, 200) );
#endif

  Led_StateMachine = LedSM_Idle;  
  
} /* end LedInitialize() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void LedRunActiveState(void)
{
  Led_StateMachine();

} /* end LedRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions */
/*--------------------------------------------------------------------------------------------------------------------*/



/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/

/*!-------------------------------------------------------------------------------------------------------------------
@fn static void LedSM_Idle(void)

@brief Run through all the LEDs to check for blinking updates.
*/
static void LedSM_Idle(void)
{
	/* Loop through each LED to check for blinkers */
  for(u8 i = 0; i < U8_TOTAL_LEDS; i++)
  {
    /* Check if LED is in LED_BLINK_MODE */
    if(Led_asControl[(LedNameType)i].eMode == LED_BLINK_MODE)
    {
      /* Decrement counter and check for 0 */
      Led_asControl[(LedNameType)i].u16Count--;
      if( Led_asControl[(LedNameType)i].u16Count == 0)
      {
        /* Toggle based on current state */
        if(NRF_GPIO->IN & G_asBspLedConfigurations[(LedNameType)i].u32BitPosition)
        {
          NRF_GPIO->OUTCLR = G_asBspLedConfigurations[i].u32BitPosition;
        }
        else
        {
          NRF_GPIO->OUTSET = G_asBspLedConfigurations[i].u32BitPosition;
        }
        
        /* Reload the blink counter value */
        Led_asControl[(LedNameType)i].u16Count = Led_asControl[(LedNameType)i].eRate;
      }
    } /* end LED_BLINK_MODE */
    
    /* Check if LED is in LED_PWM_MODE */
    if(Led_asControl[(LedNameType)i].eMode == LED_PWM_MODE)
    {
      /* Handle special case of 0% duty cycle */
      if( Led_asControl[i].eRate == LED_PWM_0 )
      {
        LedOff( (LedNameType)i );
      }
      
      /* Handle special case of 100% duty cycle */
      else if( Led_asControl[i].eRate == LED_PWM_100 )
      {
        LedOn( (LedNameType)i );
      }
  
      /* Otherwise, regular PWM: decrement counter; toggle and reload if counter reaches 0 */
      else
      {
        Led_asControl[(LedNameType)i].u16Count--;
        if(Led_asControl[(LedNameType)i].u16Count == 0)
        {
          if(Led_asControl[(LedNameType)i].eCurrentDuty == LED_PWM_DUTY_HIGH)
          {
            /* Turn the LED off and update the counters for the next cycle */
            LedOff( (LedNameType)i );
            Led_asControl[(LedNameType)i].u16Count = LED_PWM_100 - Led_asControl[(LedNameType)i].eRate;
            Led_asControl[(LedNameType)i].eCurrentDuty = LED_PWM_DUTY_LOW;
          }
          else
          {
            /* Turn the LED on and update the counters for the next cycle */
            LedOn( (LedNameType)i );
            Led_asControl[i].u16Count = Led_asControl[i].eRate;
            Led_asControl[i].eCurrentDuty = LED_PWM_DUTY_HIGH;
          }
        }
      }

      /* Set the LED back to PWM mode since LedOff and LedOn set it to normal mode */
     	Led_asControl[(LedNameType)i].eMode = LED_PWM_MODE;
      
    } /* end LED_PWM_MODE */
    
  } /* end for(u8 i = 0; i < U8_TOTAL_LEDS; i++) */
   
} /* end LedSM_Idle() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/


