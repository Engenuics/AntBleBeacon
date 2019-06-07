/*!*********************************************************************************************************************
@file pov.c                                                                
@brief Persistance of Vision driver for 8-bit display

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void PovInitialize(void)
- void PovRunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>Pov"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32PovFlags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */

extern const u8 G_aau8SmallFonts[][LCD_SMALL_FONT_ROWS][LCD_SMALL_FONT_COLUMN_BYTES]; /*!< @brief From lcd_bitmaps.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Pov_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Pov_pfStateMachine;               /*!< @brief The state machine function pointer */
static u32 Pov_u32Timer;                             /*!< @brief Timeout counter used across states */

static u32 Pov_u16UpdateRate;  
static PovColorType Pov_sMessageColor;  
static u8 Pov_au8ScreenBitmap[U8_SCREEN_WIDTH_PX];

static u8 Pov_au8DefaultMessage[] = "enGENIUS";


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!----------------------------------------------------------------------------------------------------------------------
@fn void PovSetTiming(void)

@brief Adjusts the main cycle time on which all character display is based.

This function can be hard-coded for a known cycle time, or have a timing input
based on reed switch, accelerometer, etc.

Requires:
- 

Promises:
- Pov_u16UpdateRate updated for the current timing

*/
void PovSetTiming(void)
{
  Pov_u16UpdateRate = U16_DEFAULT_TIMING_MS / U8_SCREEN_WIDTH_PX;
  
} /* end PovSetTiming() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void PovSetMessageColorRGB(LedRateType eRed_, 
                               LedRateType eGreen_, 
                               LedRateType eBlue_)

@brief Sets message text color

Requires:
@param eRed_ PWM setting for red color
@param eGreen_ PWM setting for green color
@param eBlue_ PWM setting for blue color

Promises:
- Next message will be in specified colors 

*/
void PovSetMessageColorRGB(LedRateType eRed_, 
                        LedRateType eGreen_, 
                        LedRateType eBlue_)
{
  Pov_sMessageColor.eRed = eRed_;
  Pov_sMessageColor.eGreen = eGreen_;
  Pov_sMessageColor.eBlue = eBlue_;
  
} /* end PovSetMessageColorRGB() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void PovQueueMessage(u8* pu8Message_)

@brief Loads the screen with a message

Requires:
@param pu8Message_ points to a null-terminated ASCII string

Promises:
- ASCII chars are converted to pixels and loaded to Pov_aau8ScreenBitmap

*/
void PovQueueMessage(u8* pu8Message_)
{
  u8 u8CharCounter = 0;
  u8* pu8CurrentChar;
  
  u8 u8BitMask = 0x01;
  u8 u8ScreenColumnIndex = 0;
  
  pu8CurrentChar = pu8Message_;
  
  /* Clear the current screen */
  for(u8 i = 0; i < U8_SCREEN_WIDTH_PX; i++)
  {
    Pov_au8ScreenBitmap[i] = 0;
  }
  
  /* Loop through each character in the message and load its bitmap */
  while( (*pu8CurrentChar != NULL) &&
         (u8CharCounter < U8_SCREEN_CHARS) )
  {
    /* Start with the left-most column */
    u8BitMask = 0x01;
    
    /* Load the bitmap column-by-column to Pov_au8ScreenBitmap
    j controls the bitmask to select the letter's bitmap column */
    for(u8 j = 0; j < U8_CHAR_WIDTH_PX; j++)
    {
      /* k indexes each pixel in the current column of the letter */
      for(u8 k = 0; k < U8_FONT_HEIGHT_PX; k++)
      {
        /* Add the pixel to Pov_au8ScreenBitmap if it is lit in the character bitmap */
        if(u8BitMask & (G_aau8SmallFonts[(*pu8CurrentChar - U8_ASCII_PRINTABLES)][k][0]) )
        {
          Pov_au8ScreenBitmap[u8ScreenColumnIndex] |= (0x1 << k);
        }
      } /* end for k */
      
      u8ScreenColumnIndex++;
      u8BitMask <<= 1;
      
    } /* end for j */
    
    /* Move to next char */
    pu8CurrentChar++;
    u8CharCounter++;

    /* One more index to insert the space between chars */
    u8ScreenColumnIndex++;

  } /* end while() */
  
} /* end PovQueueMessage() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void LedDuty(void)

@brief Sets the LEDs to a blend from red to blue.

Requires:
- NONE

Promises:
- LEDs set for purple scale 

*/
void LedDuty(void)
{
  LedOn(RED0);
  LedOff(GRN0);
  LedOff(BLU0);
  
  LedOn(RED1);
  LedOff(GRN1);
  LedPWM(BLU1, LED_PWM_15);
  
  LedOn(RED2);
  LedOff(GRN2);
  LedPWM(BLU2, LED_PWM_30);

  LedOn(RED3);
  LedOff(GRN3);
  LedPWM(BLU3, LED_PWM_45);

  LedOn(RED4);
  LedOff(GRN4);
  LedPWM(BLU4, LED_PWM_60);
  
  LedOn(RED5);
  LedOff(GRN5);
  LedPWM(BLU5, LED_PWM_75);
 
  LedOn(RED6);
  LedOff(GRN6);
  LedPWM(BLU6, LED_PWM_90);

  LedOn(RED7);
  LedOff(GRN7);
  LedOn(BLU7);
  
} /* end LedDuty() */


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void PovInitialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void PovInitialize(void)
{
  LedRainbow();
  
  Pov_sMessageColor.eRed   = LED_PWM_100; 
  Pov_sMessageColor.eGreen = LED_PWM_0; 
  Pov_sMessageColor.eBlue  = LED_PWM_100; 
  
  PovQueueMessage(Pov_au8DefaultMessage);

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    Pov_pfStateMachine = PovSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    Pov_pfStateMachine = PovSM_Error;
  }

} /* end PovInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void PovRunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void PovRunActiveState(void)
{
  Pov_pfStateMachine();

} /* end PovRunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* Display static colors and wait for button press to advance to POV mode */
static void PovSM_Idle(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LedDuty();
    Pov_pfStateMachine = PovSM_PovDuty;
  }
    
} /* end PovSM_Idle() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Display static colors showing varying duty cycles  */
static void PovSM_PovDuty(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    PovSetTiming();
    Pov_pfStateMachine = PovSM_Pov;
  }
    
} /* end PovSM_PovDuty() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Runs the POV display */
static void PovSM_Pov(void)
{
  static u16 u16UpdateTimer = 0;
  static u16 u16BitmapIndex = 0;
  
  /* Update current timing */
  PovSetTiming();
  
  /* Check if it's time to switch pixels columns */
  u16UpdateTimer++;
  if(u16UpdateTimer >= Pov_u16UpdateRate)
  {
    u16UpdateTimer = 0;
    
    /* Loop through each pixel to determine if on or off */
    for(u8 i = 0; i < U8_CHAR_HEIGHT_PX; i++)
    {
      if( Pov_au8ScreenBitmap[u16BitmapIndex] & (0x1 << i) )
      {
        /* Pixel is on */
        LedPWM( (LedNameType)(i + U8_LED_COLOR_OFFSET_RED), Pov_sMessageColor.eRed);
        LedPWM( (LedNameType)(i + U8_LED_COLOR_OFFSET_GRN), Pov_sMessageColor.eGreen);
        LedPWM( (LedNameType)(i + U8_LED_COLOR_OFFSET_BLU), Pov_sMessageColor.eBlue);
      }
      else
      {
        /* Pixel is off */
        LedOff( (LedNameType)(i + U8_LED_COLOR_OFFSET_RED) );
        LedOff( (LedNameType)(i + U8_LED_COLOR_OFFSET_GRN) );
        LedOff( (LedNameType)(i + U8_LED_COLOR_OFFSET_BLU) );
      }
    } /* end for i */
                
    /* Move to next pixel and wrap back if at end */
    u16BitmapIndex++;
    if(u16BitmapIndex == U8_SCREEN_WIDTH_PX)
    {
      u16BitmapIndex = 0;
    }
               
  } /* end pixel column update */

  /* Check for mode exit */
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LedRainbow();
    Pov_pfStateMachine = PovSM_Idle;
  }
  
} /* end PovSM_Pov() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void PovSM_Error(void)          
{
  
} /* end PovSM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
