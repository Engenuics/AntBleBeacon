/*!*********************************************************************************************************************
@file pov.h                                                                
@brief Header file for pov.c


**********************************************************************************************************************/

#ifndef __POV_H
#define __POV_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
/*! 
@struct PovColorType
@brief RGB values for message display 
*/
typedef struct 
{
  LedRateType eRed;
  LedRateType eGreen;
  LedRateType eBlue;
}PovColorType;


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void PovSetTiming(void);
void PovSetMessageColorRGB(LedRateType eRed_, LedRateType eGreen_, LedRateType eBlue_);
void PovQueueMessage(u8* pu8Message_);

void LedDuty(void);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void PovInitialize(void);
void PovRunActiveState(void);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void PovSM_Idle(void);    
static void PovSM_PovDuty(void);
static void PovSM_Pov(void);

static void PovSM_Error(void);         



/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define U8_ASCII_PRINTABLES    (u8)32         /*!< @brief First printable ASCII code */


#define U8_CHAR_HEIGHT_PX      (u8)8         /*!< @brief Number of vertical pixels in char bitmap */
#define U8_CHAR_WIDTH_PX       (u8)5         /*!< @brief Number of horizontal pixels in char bitmap */
#define U8_SPACE_WIDTH_PX      (u8)1         /*!< @brief Number of pixels between chars */
#define U8_FONT_HEIGHT_PX      LCD_SMALL_FONT_ROWS

#define U8_SCREEN_HEIGHT_PX    (u8)8         /*!< @brief Number of vertical pixels of "screen" */
//#define U8_SCREEN_HEIGHT_CHARS (u8)(U8_SCREEN_HEIGHT_PX / 8) /*!< @brief Number of vertical pixels of "screen" */
#define U8_SCREEN_CHARS        (u8)16        /*!< @brief Number of characters to support on "screen" */
#define U8_SCREEN_WIDTH_PX     (u8)( (U8_CHAR_WIDTH_PX + U8_SPACE_WIDTH_PX) * U8_SCREEN_CHARS)     /*!< @brief Number of horizontal pixels of "screen" */
//#define U8_SCREEN_WIDTH_CHARS  (u8)( (U8_SCREEN_WIDTH_PX / 8) + 1)    /*!< @brief Number of horizontal pixels of "screen" */

#define U16_DEFAULT_TIMING_MS  (u16)250


#endif /* __POV_H */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
