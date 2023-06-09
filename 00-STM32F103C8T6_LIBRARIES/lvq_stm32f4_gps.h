/**
  ******************************************************************************

  GPS For SLVQ32F446RE
  Author:   LVQ
  Updated:  29st June 2022

  ******************************************************************************
*/

#ifndef __GPS_H
#define __GPS_H 
/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */

/**
 * @defgroup LVQ_GPS
 * @brief    GPS NMEA standard data parser for SLVQ32F4xx devices - http://stm32f4-discovery.net/2014/08/library-27-gps-stm32f4-devices/
 * @{
 *
*/

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_usart.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/**
 * @defgroup LVQ_GPS_Macros
 * @brief    Library private defines without any sense for USER
 * @{
 */

/* Default GPS USART used */
#ifndef GPS_USART
#define GPS_USART                   USART2
#define GPS_USART_PINSPACK          LVQ_USART_PinsPack_1
#endif

/* Checks if USART buffer for GPS is empty */
#ifndef GPS_USART_BUFFER_EMPTY
#define GPS_USART_BUFFER_EMPTY      LVQ_USART_BufferEmpty(GPS_USART)
#endif

/* Get character from USART buffer for GPS */
#ifndef GPS_USART_BUFFER_GET_CHAR
#define GPS_USART_BUFFER_GET_CHAR   LVQ_USART_Getc(GPS_USART)
#endif

/* Init function for GPS */
#ifndef GPS_USART_INIT
#define GPS_USART_INIT(baudrate)    LVQ_USART_Init(GPS_USART, GPS_USART_PINSPACK, baudrate)
#endif

/* Maximum number of custom GPGxx values */
#ifndef GPS_CUSTOM_NUMBER
#define GPS_CUSTOM_NUMBER       10
#endif

/* Is character a digit */
#define GPS_IS_DIGIT(x)         ((x) >= '0' && (x) <= '9')

/* Char 2 digit conversions */
#define GPS_C2N(a)              (((a) - 48))
#define GPS_C2NM(a, x)          GPS_C2N(a) * (x)
#define GPS_CONCAT(x, y)        ((x) << 5 | (y))

/* NMEA statements */
#define GPS_GPGGA               0
#define GPS_GPRMC               1
#define GPS_GPGSA               2
#define GPS_GPGSV               3
#define GPS_ERR                 4

/* GPGGA Flags */
#define GPS_FLAG_LATITUDE       0x00000001  //GPGGA
#define GPS_FLAG_LONGITUDE      0x00000002  //GPGGA
#define GPS_FLAG_SATS           0x00000004  //GPGGA
#define GPS_FLAG_FIX            0x00000008  //GPGGA
#define GPS_FLAG_ALTITUDE       0x00000010  //GPGGA
#define GPS_FLAG_EW             0x00000020  //GPGGA
#define GPS_FLAG_NS             0x00000040  //GPGGA
#define GPS_FLAG_TIME           0x00000080  //GPGGA
/* GPRMC Flags */
#define GPS_FLAG_SPEED          0x00000100  //GPRMC
#define GPS_FLAG_DATE           0x00000200  //GPRMC
#define GPS_FLAG_VALIDITY       0x00000400  //GPRMC
#define GPS_FLAG_DIRECTION      0x00008000  //GPRMC
/* GPGSA Flags */
#define GPS_FLAG_HDOP           0x00000800  //GPGSA
#define GPS_FLAG_VDOP           0x00001000  //GPGSA
#define GPS_FLAG_PDOP           0x00002000  //GPGSA
#define GPS_FLAG_FIXMODE        0x00004000  //GPGSA
#define GPS_FLAG_SATS1_12       0x00020000  //GPGSA
/* GPGSV Flags */
#define GPS_FLAG_SATSINVIEW     0x00010000  //GPGSV
#define GPS_FLAG_SATSDESC       0x00040000  //GPGSV

/* GPGGA Positions */
#define GPS_POS_LATITUDE        GPS_CONCAT(GPS_GPGGA, 2)    //
#define GPS_POS_LONGITUDE       GPS_CONCAT(GPS_GPGGA, 4)    //
#define GPS_POS_SATS            GPS_CONCAT(GPS_GPGGA, 7)    //
#define GPS_POS_FIX             GPS_CONCAT(GPS_GPGGA, 6)    //
#define GPS_POS_ALTITUDE        GPS_CONCAT(GPS_GPGGA, 9)    //
#define GPS_POS_TIME            GPS_CONCAT(GPS_GPGGA, 1)    //
#define GPS_POS_EW              GPS_CONCAT(GPS_GPGGA, 5)    //
#define GPS_POS_NS              GPS_CONCAT(GPS_GPGGA, 3)    //

/* GPRMC Positions */
#define GPS_POS_SPEED           GPS_CONCAT(GPS_GPRMC, 7)    //
#define GPS_POS_DATE            GPS_CONCAT(GPS_GPRMC, 9)    //
#define GPS_POS_VALIDITY        GPS_CONCAT(GPS_GPRMC, 2)    //
#define GPS_POS_DIRECTION       GPS_CONCAT(GPS_GPRMC, 8)    //

/* GPGSA Positions */
#define GPS_POS_PDOP            GPS_CONCAT(GPS_GPGSA, 15)   //
#define GPS_POS_HDOP            GPS_CONCAT(GPS_GPGSA, 16)   //
#define GPS_POS_VDOP            GPS_CONCAT(GPS_GPGSA, 17)   //
#define GPS_POS_FIXMODE         GPS_CONCAT(GPS_GPGSA, 2)    //

#define GPS_POS_SAT1            GPS_CONCAT(GPS_GPGSA, 3)    //
#define GPS_POS_SAT2            GPS_CONCAT(GPS_GPGSA, 4)    //
#define GPS_POS_SAT3            GPS_CONCAT(GPS_GPGSA, 5)    //
#define GPS_POS_SAT4            GPS_CONCAT(GPS_GPGSA, 6)    //
#define GPS_POS_SAT5            GPS_CONCAT(GPS_GPGSA, 7)    //
#define GPS_POS_SAT6            GPS_CONCAT(GPS_GPGSA, 8)    //
#define GPS_POS_SAT7            GPS_CONCAT(GPS_GPGSA, 9)    //
#define GPS_POS_SAT8            GPS_CONCAT(GPS_GPGSA, 10)   //
#define GPS_POS_SAT9            GPS_CONCAT(GPS_GPGSA, 11)   //
#define GPS_POS_SAT10           GPS_CONCAT(GPS_GPGSA, 12)   //
#define GPS_POS_SAT11           GPS_CONCAT(GPS_GPGSA, 13)   //
#define GPS_POS_SAT12           GPS_CONCAT(GPS_GPGSA, 14)   //

/* GPGSV Positions */
#define GPS_POS_SATSINVIEW      GPS_CONCAT(GPS_GPGSV, 3)    //

/* Earth radius */
#define GPS_EARTH_RADIUS        6371

/* Degrees to radians converter */
#define GPS_DEGREES2RADIANS(x)  ((x) * (float)0.01745329251994)
/* Radians to degrees */
#define GPS_RADIANS2DEGREES(x)  ((x) * (float)57.29577951308232)

/* Maximal number of satellites in view */
#define GPS_MAX_SATS_IN_VIEW    24

/**
* @}
*/

/**
 * @defgroup LVQ_GPS_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  GPS Result enumeration
 */
typedef enum {
    LVQ_GPS_Result_NewData,          /*!< New data are available to operate with */
    LVQ_GPS_Result_OldData,          /*!< We don't have new data available yet */
    LVQ_GPS_Result_FirstDataWaiting, /*!< We are waiting for first data from GPS module */
} LVQ_GPS_Result_t;


/**
 * @brief  Speed conversion enumeration
 * @note   Speed from GPS is in knots, use LVQ_GPS_ConvertSpeed() to convert to useable value
 */
typedef enum {
    /* Metric */
    LVQ_GPS_Speed_KilometerPerSecond,  /*!< Convert speed to kilo meters per second */
    LVQ_GPS_Speed_MeterPerSecond,      /*!< Convert speed to meters per second */
    LVQ_GPS_Speed_KilometerPerHour,    /*!< Convert speed to kilo meters per hour */
    LVQ_GPS_Speed_MeterPerMinute,      /*!< Convert speed to meter per minute */
    /* Imperial */
    LVQ_GPS_Speed_MilePerSecond,       /*!< Convert speed to miles per second */
    LVQ_GPS_Speed_MilePerHour,         /*!< Convert speed to miles per hour */
    LVQ_GPS_Speed_FootPerSecond,       /*!< Convert speed to foots per second */
    LVQ_GPS_Speed_FootPerMinute,       /*!< Convert speed to foots per minute */
    /* For Runners and Joggers */
    LVQ_GPS_Speed_MinutePerKilometer,  /*!< Convert speed to minutes per kilo meter */
    LVQ_GPS_Speed_SecondPerKilometer,  /*!< Convert speed to seconds per kilo meter */
    LVQ_GPS_Speed_SecondPer100Meters,  /*!< Convert speed to seconds per 100 meters */
    LVQ_GPS_Speed_MinutePerMile,       /*!< Convert speed to minutes per mile */
    LVQ_GPS_Speed_SecondPerMile,       /*!< Convert speed to seconds per mile */
    LVQ_GPS_Speed_SecondPer100Yards,   /*!< Convert speed to seconds per 100 yards */
    /* Nautical */
    LVQ_GPS_Speed_SeaMilePerHour,      /*!< Convert speed to sea miles per hour */
} LVQ_GPS_Speed_t;


/**
 * @brief  GPS float structure
 * @note   GPS has longitude, latitude, altitude, etc.. in float format.
 *
 *         But, if you want to make a tracker and save data to SD card, there is a problem, because FATFS library breaks %f with sprintf() function.
 *
 *         For that purpose I made a new function LVQ_GPS_ConvertFloat() which converts float number to an integer and decimal part.
 */
typedef struct {
    int32_t Integer;   /*!< Integer part of float number. */
    uint32_t Decimal;  /*!< Decimal part of float number, in integer format. */
} LVQ_GPS_Float_t;

/**
 * @brief  Date struct for GPS date
 */
typedef struct {
    uint8_t Date;  /*!< Date in month from GPS. */
    uint8_t Month; /*!< Month from GPS. */
    uint8_t Year;  /*!< Year from GPS. */
} LVQ_GPS_Date_t;

/**
 * @brief  Time structure for GPS
 */
typedef struct {
    uint8_t Hours;       /*!< Hours from GPS time. */
    uint8_t Minutes;     /*!< Minutes from GPS time. */
    uint8_t Seconds;     /*!< Seconds from GPS time. */
    uint16_t Hundredths; /*!< Hundredths from GPS time. */
} LVQ_GPS_Time_t;

/**
 * @brief  Satellite in view description structure
 */
typedef struct {
    uint8_t ID;        /*!< SV PRN number */
    uint8_t Elevation; /*!< Elevation in degrees, 90 maximum */
    uint16_t Azimuth;  /*!< Azimuth, degrees from true north, 000 to 359 */
    uint8_t SNR;       /*!< SNR, 00-99 dB (0 when not tracking) */
} LVQ_GPS_Satellite_t;

/**
 * @brief  Custom NMEA statement and term, selected by user
 */
typedef struct {
    char Statement[7];  /*!< Statement value, including "$" at beginning. For example, "$GPRMC" */
    uint8_t TermNumber; /*!< Term number position inside statement */
    char Value[15];     /*!< Value from GPS receiver at given statement and term number will be stored here.
                                @note Value will not be converted to number if needed, but will stay as a character */
    uint8_t Updated;    /*!< Updated flag. If this parameter is set to 1, then new update has been made. Meant for private use */
} LVQ_GPS_Custom_t;

/**
 * @brief  Main GPS data structure
 */
typedef struct {
#ifndef GPS_DISABLE_GPGGA
    float Latitude;                                       /*!< Latitude position from GPS, -90 to 90 degrees response. */
    float Longitude;                                      /*!< Longitude position from GPS, -180 to 180 degrees response. */
    uint8_t Satellites;                                   /*!< Number of satellites in use for GPS position. */
    uint8_t Fix;                                          /*!< GPS fix; 0: Invalid; 1: GPS Fix; 2: DGPS Fix. */
    float Altitude;                                       /*!< Altitude above the sea. */
    LVQ_GPS_Time_t Time;                                   /*!< Current time from GPS. @ref LVQ_GPS_Time_t. */
#endif
#ifndef GPS_DISABLE_GPRMC
    LVQ_GPS_Date_t Date;                                   /*!< Current data from GPS. @ref LVQ_GPS_Date_t. */
    float Speed;                                          /*!< Speed in knots from GPS. */
    uint8_t Validity;                                     /*!< GPS validation; 1: valid; 0: invalid. */
    float Direction;                                      /*!< Course on the ground in relation to North. */
#endif
#ifndef GPS_DISABLE_GPGSA
    float HDOP;                                           /*!< Horizontal dilution of precision. */
    float PDOP;                                           /*!< Position dilution od precision. */
    float VDOP;                                           /*!< Vertical dilution of precision. */
    uint8_t FixMode;                                      /*!< Current fix mode in use:; 1: Fix not available; 2: 2D; 3: 3D. */
    uint8_t SatelliteIDs[12];                             /*!< Array with IDs of satellites in use.
                                                               Only first data are valid, so if you have 5 satellites in use, only SatelliteIDs[4:0] are valid */
#endif
#ifndef GPS_DISABLE_GPGSV
    uint8_t SatellitesInView;                             /*!< Number of satellites in view */
    LVQ_GPS_Satellite_t SatDesc[30];                       /*!< Description of each satellite in view */
#endif
    LVQ_GPS_Result_t Status;                               /*!< GPS result. This parameter is value of @ref LVQ_GPS_Result_t */
    LVQ_GPS_Custom_t* CustomStatements[GPS_CUSTOM_NUMBER]; /*!< Array of pointers for custom GPS NMEA statements, selected by user.
                                                                  You can use @ref GPS_CUSTOM_NUMBER number of custom statements */
    uint8_t CustomStatementsCount;                        /*!< Number of custom GPS statements selected by user */
} LVQ_GPS_t;

/* Backward compatibility */
typedef LVQ_GPS_t LVQ_GPS_Data_t;

/**
 * @brief  GPS Distance and bearing struct
 */
typedef struct {
    float Latitude1;  /*!< Latitude of starting point. */
    float Longitude1; /*!< Longitude of starting point. */
    float Latitude2;  /*!< Latitude of ending point. */
    float Longitude2; /*!< Longitude of ending point. */
    float Distance;   /*!< Distance between 2 points which will be calculated. */
    float Bearing;    /*!< Bearing from start to stop point according to North. */
} LVQ_GPS_Distance_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_GPS_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes GPS and USART peripheral
 * @param  *GPS_Data: Pointer to @ref LVQ_GPS_t structure to set default values
 * @param  baudrate: Specify GPS baudrate for USART. Most common are 9600 or 115200 bauds
 * @note   GPS baudrate can have other values. Check GPS datasheet for proper info.
 * @retval None
 */
void LVQ_GPS_Init(LVQ_GPS_t* GPS_Data, uint32_t baudrate);

/**
 * @brief  Update GPS data.
 * @note   This function must be called periodically, as fast as possible.
 *         It basically checks if data is available on GPS USART and parse it to useful data for user.
 * @note   - When you first call this function and there is not available data from GPS, this function will return @ref LVQ_GPS_Result_FirstTimeWaiting.
 *         - This will be returning all the time we don't have any useful data.
 *         - When first time useful data is received from GPS (everything parsed), @ref LVQ_GPS_Result_NewData will be returned.
 *         - When we have already new data, next time we call this function, @ref LVQ_GPS_Result_OldData will be returning until we don't receive new packet of useful data.
 * @note   If you are making GPS logger, then when you receive @ref LVQ_GPS_Result_NewData it is time to save your data.
 * @param  *GPS_Data: Pointer to working @ref LVQ_GPS_t structure
 * @retval Returns value of @ref LVQ_GPS_Result_t structure
 */
LVQ_GPS_Result_t LVQ_GPS_Update(LVQ_GPS_t* GPS_Data);

/**
 * @brief  Converts speed in knots (from GPS) to user selectable speed
 * @param  speedInKnots: float value from GPS module
 * @param  toSpeed: Select to which speed you want conversion from knot. This parameter ca be a value of LVQ_GPS_Speed_t enumeration.
 * @retval Calculated speed from knots to user selectable format
 */
float LVQ_GPS_ConvertSpeed(float SpeedInKnots, LVQ_GPS_Speed_t toSpeed);

/**
 * @brief  Converts float number into integer and decimal part
 * @param  num: Float number to split into 2 parts
 * @param  *Float_Data: Pointer to LVQ_GPS_Float_t structure where to save result
 * @param  decimals: Number of decimal places for conversion
 * @note   Example: You have number 15.002 in float format.
 *            - You want to split this to integer and decimal part with 6 decimal places.
 *            - Call @ref LVQ_GPS_ConvertFloat(15.002, &Float_Struct, 6);
 *            - Result will be: Integer: 15; Decimal: 2000 (0.002 * 10^6)
 * @retval None
 */
void LVQ_GPS_ConvertFloat(float num, LVQ_GPS_Float_t* Float_Data, uint8_t decimals);

/**
 * @brief  Calculates distance between 2 coordinates on earth and bearing from start to end point in relation to the north
 * @param  *Distance_Data: Pointer to @ref LVQ_GPS_Distance_t structure with latitude and longitude set values
 * @note   Calculation results will be saved in *Distance_Data @ref LVQ_GPS_Distance_t structure
 * @retval None
 */
void LVQ_GPS_DistanceBetween(LVQ_GPS_Distance_t* Distance_Data);

/**
 * @brief  Adds custom GPG statement to array of user selectable statements.
 *            Array is available to user using @ref LVQ_GPS_t workign structure
 * @note   Functions uses @ref malloc() function to allocate memory, so make sure you have enough heap memory available.
 * @note   Also note, that your GPS receiver HAVE TO send statement type you use in this function, or
 *            @ref LVQ_GPS_Update() function will always return that there is not data available to read.
 * @param  *GPS_Data: Pointer to working @ref LVQ_GPS_t structure
 * @param  *GPG_Statement: String of NMEA starting line address, including "$" at beginning
 * @param  TermNumber: Position in NMEA statement
 * @retval Success status:
 *            - NULL: Malloc() failed or you reached limit of user selectable custom statements:
 *            - > NULL: Function succeded, pointer to @ref LVQ_GPS_Custom_t structure
 */
LVQ_GPS_Custom_t* LVQ_GPS_AddCustom(LVQ_GPS_t* GPS_Data, char* GPG_Statement, uint8_t TermNumber);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif
