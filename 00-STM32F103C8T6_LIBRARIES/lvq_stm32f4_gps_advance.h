/**
  ******************************************************************************

  GPS ADVANCE For SLVQ32F446RE
  Author:   LVQ
  Updated:  29st June 2022

  ******************************************************************************
*/

#ifndef __GPS_ADVANCE_H
#define __GPS_ADVANCE_H 
/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */

/**
 * @defgroup LVQ_GPS_ADVANCE
 * @brief    GPS NMEA standard data parser for SLVQ32F4xx devices - http://stm32f4-discovery.net/2014/08/library-27-gps-stm32f4-devices/
 * @{
 *
*/

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_usart.h"
#include <stdio.h>
#include <string.h>

#define GPSBUFSIZE 128

typedef struct{

    // calculated values
    float dec_longitude;
    float dec_latitude;
    float altitude_ft;

    // GGA - Global Positioning System Fixed Data
    float nmea_longitude;
    float nmea_latitude;
    float utc_time;
    char ns, ew;
    int lock;
    int satelites;
    float hdop;
    float msl_altitude;
    char msl_units;

    // RMC - Recommended Minimmum Specific GNS Data
    char rmc_status;
    float speed_k;
    float course_d;
    int date;

    // GLL
    char gll_status;

    // VTG - Course over ground, ground speed
    float course_t; // ground speed true
    char course_t_unit;
    float course_m; // magnetic
    char course_m_unit;
    char speed_k_unit;
    float speed_km; // speek km/hr
    char speed_km_unit;
} GPS_t;


void GPS_Init(void);
void GSP_USBPrint(char *data);
void GPS_print_val(char *data, int value);
void GPS_UART_CallBack(void);
int GPS_validate(char *nmeastr);
void GPS_parse(char *GPSstrParse);
float GPS_nmea_to_dec(float deg_coord, char nsew);

#endif
