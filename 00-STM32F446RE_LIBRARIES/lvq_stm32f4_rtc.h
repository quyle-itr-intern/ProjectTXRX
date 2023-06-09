/**
  ******************************************************************************

  RTC For SLVQ32F446RE
  Author:   LVQ
  Updated:  18st June 2022

  ******************************************************************************
*/
#ifndef __RTC_H
#define __RTC_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */
 
#include "lvq_stm32f4_rccConfig.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rcc.h"
#include <time.h>

/* RTC clock is: f_clk = RTCCLK(LSI or LSE) / ((RTC_SYNC_PREDIV + 1) * (RTC_ASYNC_PREDIV + 1)) */
/* Sync pre division for clock */
#ifndef RTC_SYNC_PREDIV
#define RTC_SYNC_PREDIV                 0x3FF
#endif
/* Async pre division for clock */
#ifndef RTC_ASYNC_PREDIV
#define RTC_ASYNC_PREDIV                0x1F
#endif
/* NVIC global Priority set */
#ifndef RTC_PRIORITY
#define RTC_PRIORITY                    0x04
#endif
/* Sub priority for wakeup trigger */
#ifndef RTC_WAKEUP_SUBPRIORITY
#define RTC_WAKEUP_SUBPRIORITY          0x01
#endif
/* Sub priority for alarm trigger */
#ifndef RTC_ALARM_SUBPRIORITY
#define RTC_ALARM_SUBPRIORITY           0x00
#endif

/**
 * @defgroup LVQ_RTC_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  RTC Struct for date/time
 */
typedef struct {
    uint8_t seconds;     /*!< Seconds parameter, from 00 to 59 */
    uint16_t subseconds; /*!< Subsecond downcounter. When it reaches zero, it's reload value is the same as
                         @ref RTC_SYNC_PREDIV, so in our case 0x3FF = 1023, 1024 steps in one second */
    uint8_t minutes;     /*!< Minutes parameter, from 00 to 59 */
    uint8_t hours;       /*!< Hours parameter, 24Hour mode, 00 to 23 */
    uint8_t day;         /*!< Day in a week, from 1 to 7 */
    uint8_t date;        /*!< Date in a month, 1 to 31 */
    uint8_t month;       /*!< Month in a year, 1 to 12 */
    uint8_t year;        /*!< Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
    uint32_t unix;       /*!< Seconds from 01.01.1970 00:00:00 */
} LVQ_RTC_t;

/**
 * @brief  Backward compatibility for RTC time
 */
typedef LVQ_RTC_t LVQ_RTC_Time_t;

/**
 * @brief RTC Result enumeration
 */
typedef enum {
    LVQ_RTC_Result_Ok,   /*!< Everything OK */
    LVQ_RTC_Result_Error /*!< An error occurred */
} LVQ_RTC_Result_t;

/**
 * @brief RTC date and time format
 */
typedef enum {
    LVQ_RTC_Format_BIN = 0x00, /*!< RTC data in binary format */
    LVQ_RTC_Format_BCD         /*!< RTC data in binary-coded decimal format */
} LVQ_RTC_Format_t;

/**
 * @brief  RTC Interrupt enumeration
 */
typedef enum {
    LVQ_RTC_Int_Disable = 0x00, /*!< Disable RTC wakeup interrupts */
    LVQ_RTC_Int_60s,            /*!< RTC Wakeup interrupt every 60 seconds */
    LVQ_RTC_Int_30s,            /*!< RTC Wakeup interrupt every 30 seconds */
    LVQ_RTC_Int_15s,            /*!< RTC Wakeup interrupt every 15 seconds */
    LVQ_RTC_Int_10s,            /*!< RTC Wakeup interrupt every 10 seconds */
    LVQ_RTC_Int_5s,             /*!< RTC Wakeup interrupt every 5 seconds */
    LVQ_RTC_Int_2s,             /*!< RTC Wakeup interrupt every 2 seconds */
    LVQ_RTC_Int_1s,             /*!< RTC Wakeup interrupt every 1 seconds */
    LVQ_RTC_Int_500ms,          /*!< RTC Wakeup interrupt every 500 milliseconds */
    LVQ_RTC_Int_250ms,          /*!< RTC Wakeup interrupt every 250 milliseconds */
    LVQ_RTC_Int_125ms           /*!< RTC Wakeup interrupt every 125 milliseconds */
} LVQ_RTC_Int_t;

/**
 * @brief  Select RTC clock source
 * @note   Internal clock is not accurate and should not be used in production
 */
typedef enum {
    LVQ_RTC_ClockSource_Internal = 0x00, /*!< Use internal clock source for RTC (LSI oscillator) */
    LVQ_RTC_ClockSource_External         /*!< Use external clock source for RTC (LSE oscillator) */
} LVQ_RTC_ClockSource_t;

/**
 * @brief  RTC Alarm type
 */
typedef enum {
    LVQ_RTC_AlarmType_DayInWeek, /*!< Trigger alarm every day in a week, days from 1 to 7 (Monday to Sunday) */
    LVQ_RTC_AlarmType_DayInMonth /*!< Trigger alarm every month */
} LVQ_RTC_AlarmType_t;

/**
 * @brief  Alarm identifier you will use for Alarm functions
 */
typedef enum {
    LVQ_RTC_Alarm_A = 0x00, /*!< Work with alarm A */
    LVQ_RTC_Alarm_B         /*!< Work with alarm B */
} LVQ_RTC_Alarm_t;

/**
 * @brief  RTC structure for alarm time
 */
typedef struct {
    LVQ_RTC_AlarmType_t alarmtype; /*!< Alarm type setting. @ref LVQ_RTC_AlarmType_t for more info */
    uint8_t seconds;               /*!< Alarm seconds value */
    uint8_t minutes;               /*!< Alarm minutes value */
    uint8_t hours;                 /*!< Alarm hours value */
    uint8_t day;                   /*!< Alarm day value. If you select trigger for alarm every week, then this parameter has value between
                                          1 and 7, representing days in a week, Monday to Sunday
                                          If you select trigger for alarm every month, then this parameter has value between
                                          1 - 31, representing days in a month. */
} LVQ_RTC_AlarmTime_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_RTC_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes RTC and starts counting
 * @param  source. RTC Clock source @ref LVQ_RTC_ClockSource_t to be used for RTC
 * @note   Internal clock source is not so accurate
 * @note   If you reset your MCU and RTC still has power, it will count independent of MCU status
 * @retval Returns RTC status.
 *            - 1: RTC has already been initialized and time is set
 *            - 0: RTC was now initialized first time. Now you can set your first clock
 */
uint32_t LVQ_RTC_Init(LVQ_RTC_ClockSource_t source);

/**
 * @brief  Get number of seconds from date and time since 01.01.1970 00:00:00
 * @param  *data: Pointer to @ref LVQ_RTC_t data structure
 * @retval Calculated seconds from date and time since 01.01.1970 00:00:00
 */
uint32_t LVQ_RTC_GetUnixTimeStamp(LVQ_RTC_t* data);

/**
 * @brief  Get formatted time from seconds till 01.01.1970 00:00:00
 *         It fills struct with valid data
 * @note   Valid if year is greater or equal (>=) than 2000
 * @param  *data: Pointer to @ref LVQ_RTC_Time_t struct to store formatted data in
 * @param  unix: Seconds from 01.01.1970 00:00:00 to calculate user friendly time
 * @retval None
 */
void LVQ_RTC_GetDateTimeFromUnix(LVQ_RTC_t* data, uint32_t unix);

/**
 * @brief  Select RTC wakeup interrupts interval
 * @note   This function can also be used to disable interrupt
 * @param  int_value: Look for @ref LVQ_RTC_Int_t for valid inputs
 * @retval None
 */
void LVQ_RTC_Interrupts(LVQ_RTC_Int_t int_value);

/**
 * @brief  Set date and time to internal RTC registers
 * @param  *data: Pointer to filled @ref LVQ_RTC_t structure with date and time
 * @param  format: Format of your structure. This parameter can be a value of @ref LVQ_RTC_Format_t enumeration
 * @retval RTC datetime status @ref LVQ_RTC_Result_t:
 *            - @ref LVQ_RTC_Result_Ok: Date and Time set OK
 *            - @ref LVQ_RTC_Result_Error: Date and time is wrong
 */
LVQ_RTC_Result_t LVQ_RTC_SetDateTime(LVQ_RTC_t* data, LVQ_RTC_Format_t format);

/**
 * @brief  Set date and time using string formatted date time
 * @note   Valid string format is: <b>dd.mm.YY.x;HH:ii:ss</b>
 *            - <b>dd</b>: date, 2 digits, decimal
 *            - <b>mm</b>: month, 2 digits, decimal
 *            - <b>YY</b>: year, last 2 digits, decimal
 *            - <b>x</b>: day in a week: 1 digit, 1 = Monday, 7 = Sunday
 *            - <b>HH</b>: hours, 24-hour mode, 2 digits, decimal
 *            - <b>ii</b>: minutes, 2 digits, decimal
 *            - <b>ss</b>: seconds, 2 digits, decimal
 * @param  *str: Pointer to string with datetime format
 * @retval RTC datetime status @ref LVQ_RTC_Result_t:
 *            - @ref LVQ_RTC_Result_Ok: Date and Time set OK
 *            - @ref LVQ_RTC_Result_Error: Date and time is wrong
 */
LVQ_RTC_Result_t LVQ_RTC_SetDateTimeString(char* str);

/**
 * @brief  Get date and time from internal RTC registers
 * @param  *data: Pointer to @ref LVQ_RTC_t structure to save data to
 * @param  format: Format of your structure. This parameter can be a value of @ref LVQ_RTC_Format_t enumeration
 * @retval None
 */
void LVQ_RTC_GetDateTime(LVQ_RTC_t* data, LVQ_RTC_Format_t format);

/**
 * @brief  Get number of days in month
 * @note   This function also detects if it is leap year and returns different number for February
 * @param  month: Month number in year, valid numbers are 1 - 12
 * @param  year: Year number where you want to get days in month, last 2 digits
 * @retval Number of days in specific month and year
 */
uint8_t LVQ_RTC_GetDaysInMonth(uint8_t month, uint8_t year);

/**
 * @brief  Get number of days in specific year
 * @note   This function also detects if it is leap year
 * @param  year: Year number where you want to get days in month, last 2 digits
 * @retval Number of days in year
 */
uint16_t LVQ_RTC_GetDaysInYear(uint8_t year);

/**
 * @brief  Write RTC backup register value.
 *            This method allows you to write 32bit value from backup register 0 - 18.
 * @note   RTC has 20 backup registers where you can store data which will be available all the time RTC is running and has power.
 *
 * @note   My library uses register 19 to store info about RTC settings and is not available for USER to store data there.
 *
 * @note   RTC HAS to be initialized first before you can use this method.
 * @param  location: RTC backup register location. 0 to 18 are valid
 * @param  value: 32-bit long value to be stored in RTC backup register
 * @retval Value at specific RTC backup register location
 */
void LVQ_RTC_WriteBackupRegister(uint8_t location, uint32_t value);

/**
 * @brief  Read RTC backup register value.
 *            This method allows you to read 32bit value from backup register 0 - 18.
 * @note   RTC has 20 backup registers where you can store data which will be available all the time RTC is running and has power.
 *
 * @note   My library uses register 19 to store info about RTC settings and is not available for USER to store data there.
 *
 * @note   RTC HAS to be initialized first before you can use this method.
 * @param  location: RTC backup register location. 0 to 18 are valid
 * @retval Value at specific RTC backup register location
 */
uint32_t LVQ_RTC_ReadBackupRegister(uint8_t location);

/**
 * @brief  Enables alarm A or alarm B
 * @param  Alarm: Specify which alarm to set. This parameter can be a value of @ref LVQ_RTC_Alarm_t enumeration
 * @param  *AlarmTime: Pointer to @ref LVQ_RTC_AlarmTime_t structure to get data from.
 * @param  format: RTC date and time format. This parameter can be a value of @ref LVQ_RTC_Format_t enumeration.
 * @retval None
 */
void LVQ_RTC_SetAlarm(LVQ_RTC_Alarm_t Alarm, LVQ_RTC_AlarmTime_t* AlarmTime, LVQ_RTC_Format_t format);

/**
 * @brief  Disables specific alarm
 * @param  Alarm: Select which alarm you want to disable. This parameter can be a value of @ref LVQ_RTC_Alarm_t enumeration
 * @retval None
 */
void LVQ_RTC_DisableAlarm(LVQ_RTC_Alarm_t Alarm);

/**
 * @brief  RTC Wakeup handler function. Called when wakeup interrupt is triggered
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_RTC_RequestHandler(void);

/**
 * @brief  RTC Alarm A handler function. Called when interrupt is triggered for alarm A
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_RTC_AlarmAHandler(void);

/**
 * @brief  RTC Alarm B handler function. Called when interrupt is triggered for alarm B.
 * @note   Called from my RTC library
 * @param  None
 * @retval None
 * @note   With __weak parameter to prevent link errors if not defined by user
 */
void LVQ_RTC_AlarmBHandler(void);

void LVQ_RTC_GetDateTimeFromUnixUseTime(LVQ_RTC_t* data, uint32_t unix);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

