/*******************************************************************************
* File: typedefs.h                                                               
* Description:
* Type definitions for use with the system.
*******************************************************************************/

#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H


typedef void(*fnCode_type)(void);

/* CHAR/SHORT/LONG types here for legacy code compatibility */
typedef char CHAR;              /* Signed 8-bits */
typedef unsigned char UCHAR;    /* Unsigned 8-bits */
typedef short SHORT;            /* Signed 16-bits */
typedef unsigned short USHORT;  /* Unsigned 16-bits */
typedef long LONG;              /* Signed 32-bits */
typedef unsigned long ULONG;    /* Unsigned 32-bits */
typedef unsigned char BOOL;     /* Boolean */


/* Standard Peripheral Library old types (maintained for legacy purpose) */
typedef long s32;
typedef short s16;
typedef signed char  s8;

typedef const long sc32;   /*!< Read Only */
typedef const short sc16;  /*!< Read Only */
typedef const char sc8;    /*!< Read Only */

typedef ULONG  u32;
typedef USHORT u16;
typedef UCHAR  u8;

typedef const ULONG uc32;   /*!< Read Only */
typedef const USHORT uc16;  /*!< Read Only */
typedef const USHORT uc8;   /*!< Read Only */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

/* Redefinition of allowed boolean nmenonics */
#define FALSE false
#define TRUE  true


/*! 
@enum ErrorStatus
@brief Real names for active low vs. active high GPIO.
*/
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatusType;

/*! 
@enum GpioActiveType
@brief Real names for active low vs. active high GPIO.
*/
typedef enum {ACTIVE_LOW = 0, ACTIVE_HIGH = 1} GpioActiveType;      


/*! 
@enum GpioeChannelType
@brief Self-documenting names for GPIOE event or task channel numbers.
*/
typedef enum {GPIOE_EVENT0 = 0, GPIOE_EVENT1 = 1, GPIOE_EVENT2 = 2, GPIOE_EVENT3 = 3,
              GPIOE_TASK0  = 0, GPIOE_TASK1  = 1, GPIOE_TASK2  = 2, GPIOE_TASK3  = 3, 
              GPIOE_NO_CHANNEL} GpioeChannelType;      


/*! 
@struct Nrf51PinConfigurationType
@brief Parameters necessary to fully describe a GPIO line.
*/
typedef struct
{
  u32 u32BitPosition;                 /*!< @brief Pin bit position within port */
  u32 u32GpioeChannelBit;             /*!< @brief Event or task channel bit position */
  u8  u8PinNumber;                    /*!< @brief Pin number within port */
  GpioActiveType eActiveState;        /*!< @brief Pin hardware active type */
  GpioeChannelType eChannelNumber;    /*!< @brief Event or task channel assigned to the pin */
  u8 u8Dummy;
}Nrf51PinConfigurationType;


#define BIT0    ((u8)0x01)
#define BIT1    ((u8)0x02)
#define BIT2    ((u8)0x04)
#define BIT3    ((u8)0x08)
#define BIT4    ((u8)0x10)
#define BIT5    ((u8)0x20)
#define BIT6    ((u8)0x40)
#define BIT7    ((u8)0x80)
#define BIT8    ((u16)0x0100)
#define BIT9    ((u16)0x0200)
#define BIT10   ((u16)0x0400)
#define BIT11   ((u16)0x0800)
#define BIT12   ((u16)0x1000)
#define BIT13   ((u16)0x2000)
#define BIT14   ((u16)0x4000)
#define BIT15   ((u16)0x8000)
#define BIT16   ((u32)0x00010000)
#define BIT17   ((u32)0x00020000)
#define BIT18   ((u32)0x00040000)
#define BIT19   ((u32)0x00080000)
#define BIT20   ((u32)0x00100000)
#define BIT21   ((u32)0x00200000)
#define BIT22   ((u32)0x00400000)
#define BIT23   ((u32)0x00800000)
#define BIT24   ((u32)0x01000000)
#define BIT25   ((u32)0x02000000)
#define BIT26   ((u32)0x04000000)
#define BIT27   ((u32)0x08000000)
#define BIT28   ((u32)0x10000000)
#define BIT29   ((u32)0x20000000)
#define BIT30   ((u32)0x40000000)
#define BIT31   ((u32)0x80000000)

#define PIN0    ((u8)0)
#define PIN1    ((u8)1)
#define PIN2    ((u8)2)
#define PIN3    ((u8)3)
#define PIN4    ((u8)4)
#define PIN5    ((u8)5)
#define PIN6    ((u8)6)
#define PIN7    ((u8)7)
#define PIN8    ((u8)8)
#define PIN9    ((u8)9)
#define PIN10   ((u8)10)
#define PIN11   ((u8)11)
#define PIN12   ((u8)12)
#define PIN13   ((u8)13)
#define PIN14   ((u8)14)
#define PIN15   ((u8)15)
#define PIN16   ((u8)16)
#define PIN17   ((u8)17)
#define PIN18   ((u8)18)
#define PIN19   ((u8)19)
#define PIN20   ((u8)20)
#define PIN21   ((u8)21)
#define PIN22   ((u8)22)
#define PIN23   ((u8)23)
#define PIN24   ((u8)24)
#define PIN25   ((u8)25)
#define PIN26   ((u8)26)
#define PIN27   ((u8)27)
#define PIN28   ((u8)28)
#define PIN29   ((u8)29)
#define PIN30   ((u8)30)
#define PIN31   ((u8)31


#endif /* __TYPEDEFS_H */

