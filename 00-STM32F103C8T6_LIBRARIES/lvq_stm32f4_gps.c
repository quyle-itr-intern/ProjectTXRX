/**
  ******************************************************************************

  GPS For SLVQ32F446RE
  Author:   LVQ
  Updated:  29st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_gps.h"

static char GPS_Term[15];
static uint8_t GPS_Term_Number;
static uint8_t GPS_Term_Pos;
static uint8_t LVQ_GPS_CRC, LVQ_GPS_CRC_Received;
static uint8_t LVQ_GPS_Star;
static uint8_t LVQ_GPS_Statement = GPS_ERR;
static uint32_t GPS_Flags = 0, GPS_Flags_OK;
static LVQ_GPS_Data_t LVQ_GPS_INT_Data;
static uint8_t LVQ_GPS_FirstTime;
static char GPS_Statement_Name[7];

#ifndef GPS_DISABLE_GPGSV
uint8_t GPGSV_StatementsCount = 0;
uint8_t GPSGV_StatementNumber = 0;
uint8_t GPGSV_Term_Number = 0;
uint8_t GPGSV_Term_Mod = 0;
#endif

/* Private */
LVQ_GPS_Result_t LVQ_GPS_INT_Do(LVQ_GPS_t* GPS_Data, char c);
void LVQ_GPS_INT_CheckTerm(LVQ_GPS_t* GPS_Data);
LVQ_GPS_Result_t LVQ_GPS_INT_Return(LVQ_GPS_t* GPS_Data);
uint8_t LVQ_GPS_INT_StringStartsWith(char* string, const char* str);
uint8_t LVQ_GPS_INT_Atoi(char* str, uint32_t* val);
uint32_t LVQ_GPS_INT_Pow(uint8_t x, uint8_t y);
uint8_t LVQ_GPS_INT_Hex2Dec(char c);
uint8_t LVQ_GPS_INT_FlagsOk(LVQ_GPS_t* GPS_Data);
void LVQ_GPS_INT_ClearFlags(LVQ_GPS_t* GPS_Data);
void LVQ_GPS_INT_CheckEmpty(LVQ_GPS_t* GPS_Data);

#define LVQ_GPS_INT_Add2CRC(c)                            (LVQ_GPS_CRC ^= c)
#define LVQ_GPS_INT_ReturnWithStatus(GPS_Data, status)    (GPS_Data)->Status = status; return status;
#define LVQ_GPS_INT_SetFlag(flag)                         (GPS_Flags |= (flag))

/* Public */
void LVQ_GPS_Init(LVQ_GPS_t* GPS_Data, uint32_t baudrate) 
{
    /* Initialize USART */
    GPS_USART_INIT(baudrate);
    /* Set first-time variable */
    LVQ_GPS_FirstTime = 1;

    /* Reset everything */
    GPS_Data->CustomStatementsCount = 0;

    /* Clear all flags */
    LVQ_GPS_INT_ClearFlags(GPS_Data);

    /* Set flags used */
#ifndef GPS_DISABLE_GPGGA
    GPS_Flags_OK |= GPS_FLAG_LATITUDE;
    GPS_Flags_OK |= GPS_FLAG_NS;
    GPS_Flags_OK |= GPS_FLAG_LONGITUDE;
    GPS_Flags_OK |= GPS_FLAG_EW;
    GPS_Flags_OK |= GPS_FLAG_SATS;
    GPS_Flags_OK |= GPS_FLAG_FIX;
    GPS_Flags_OK |= GPS_FLAG_ALTITUDE;
    GPS_Flags_OK |= GPS_FLAG_TIME;
#endif
#ifndef GPS_DISABLE_GPRMC
    GPS_Flags_OK |= GPS_FLAG_SPEED;
    GPS_Flags_OK |= GPS_FLAG_DATE;
    GPS_Flags_OK |= GPS_FLAG_VALIDITY;
    GPS_Flags_OK |= GPS_FLAG_DIRECTION;
#endif
#ifndef GPS_DISABLE_GPGSA
    GPS_Flags_OK |= GPS_FLAG_HDOP;
    GPS_Flags_OK |= GPS_FLAG_VDOP;
    GPS_Flags_OK |= GPS_FLAG_PDOP;
    GPS_Flags_OK |= GPS_FLAG_FIXMODE;
    GPS_Flags_OK |= GPS_FLAG_SATS1_12;
#endif
#ifndef GPS_DISABLE_GPGSV
    GPS_Flags_OK |= GPS_FLAG_SATSINVIEW;
    GPS_Flags_OK |= GPS_FLAG_SATSDESC;
#endif
}

LVQ_GPS_Result_t LVQ_GPS_Update(LVQ_GPS_t* GPS_Data) 
{
    /* Check for data in USART */
    if (!GPS_USART_BUFFER_EMPTY) 
		{
        /* Go through all buffer */
        while (!GPS_USART_BUFFER_EMPTY) 
				{
            /* Do character by character */
            LVQ_GPS_INT_Do(GPS_Data, (char)GPS_USART_BUFFER_GET_CHAR);
            /* If new data available, return to user */
            if (GPS_Data->Status == LVQ_GPS_Result_NewData) 
						{
                return GPS_Data->Status;
            }
        }
    }

    if (LVQ_GPS_FirstTime) 
		{
        /* No any valid data, return First Data Waiting */
        /* Returning only after power up and calling when no all data is received */
        LVQ_GPS_INT_ReturnWithStatus(GPS_Data, LVQ_GPS_Result_FirstDataWaiting);
    }

    /* We have old data */
    LVQ_GPS_INT_ReturnWithStatus(GPS_Data, LVQ_GPS_Result_OldData);
}

LVQ_GPS_Custom_t* LVQ_GPS_AddCustom(LVQ_GPS_t* GPS_Data, char* GPG_Statement, uint8_t TermNumber) 
{
    LVQ_GPS_Custom_t* temp;

    /* Check if available */
    if (GPS_Data->CustomStatementsCount >= GPS_CUSTOM_NUMBER) {
        /* Error */
        return NULL;
    }

    /* Allocate memory */
    temp = (LVQ_GPS_Custom_t*) malloc(sizeof(LVQ_GPS_Custom_t));
    /* Check malloc success */
    if (temp == NULL) {
        return NULL;
    }

    /* Fill settings */
    strcpy(temp->Statement, GPG_Statement);
    temp->TermNumber = TermNumber;

    /* Add to array */
    GPS_Data->CustomStatements[GPS_Data->CustomStatementsCount] = temp;

    /* Increase memory count */
    GPS_Data->CustomStatementsCount++;

    /* Return pointer */
    return temp;
}

float LVQ_GPS_ConvertSpeed(float SpeedInKnots, LVQ_GPS_Speed_t toSpeed) 
{
    switch ((uint8_t)toSpeed) {
        /* Metric */
        case LVQ_GPS_Speed_KilometerPerSecond:
            return SpeedInKnots * (float)0.000514;
        case LVQ_GPS_Speed_MeterPerSecond:
            return SpeedInKnots * (float)0.5144;
        case LVQ_GPS_Speed_KilometerPerHour:
            return SpeedInKnots * (float)1.852;
        case LVQ_GPS_Speed_MeterPerMinute:
            return SpeedInKnots * (float)30.87;

        /* Imperial */
        case LVQ_GPS_Speed_MilePerSecond:
            return SpeedInKnots * (float)0.0003197;
        case LVQ_GPS_Speed_MilePerHour:
            return SpeedInKnots * (float)1.151;
        case LVQ_GPS_Speed_FootPerSecond:
            return SpeedInKnots * (float)1.688;
        case LVQ_GPS_Speed_FootPerMinute:
            return SpeedInKnots * (float)101.3;

        /* For Runners and Joggers */
        case LVQ_GPS_Speed_MinutePerKilometer:
            return SpeedInKnots * (float)32.4;
        case LVQ_GPS_Speed_SecondPerKilometer:
            return SpeedInKnots * (float)1944;
        case LVQ_GPS_Speed_SecondPer100Meters:
            return SpeedInKnots * (float)194.4;
        case LVQ_GPS_Speed_MinutePerMile:
            return SpeedInKnots * (float)52.14;
        case LVQ_GPS_Speed_SecondPerMile:
            return SpeedInKnots * (float)3128;
        case LVQ_GPS_Speed_SecondPer100Yards:
            return SpeedInKnots * (float)177.7;

        /* Nautical */
        case LVQ_GPS_Speed_SeaMilePerHour:
            return SpeedInKnots * (float)1;
        default:
            return 0;
    }
}

void LVQ_GPS_ConvertFloat(float num, LVQ_GPS_Float_t* Float_Data, uint8_t decimals) 
{
    if (decimals > 9) {
        decimals = 9;
    }

    /* Get integer part */
    Float_Data->Integer = (int32_t)num;

    /* Get decimal part */
    if (num < 0) {
        Float_Data->Decimal = (int32_t)((float)(Float_Data->Integer - num) * LVQ_GPS_INT_Pow(10, decimals));
    } else {
        Float_Data->Decimal = (int32_t)((float)(num - Float_Data->Integer) * LVQ_GPS_INT_Pow(10, decimals));
    }
}

void LVQ_GPS_DistanceBetween(LVQ_GPS_Distance_t* Distance_Data) 
{
    float f1, f2, l1, l2, df, dfi, a;

    /* Calculate distance between 2 pointes */
    f1 = GPS_DEGREES2RADIANS(Distance_Data->Latitude1);
    f2 = GPS_DEGREES2RADIANS(Distance_Data->Latitude2);
    l1 = GPS_DEGREES2RADIANS(Distance_Data->Longitude1);
    l2 = GPS_DEGREES2RADIANS(Distance_Data->Longitude2);
    df = GPS_DEGREES2RADIANS(Distance_Data->Latitude2 - Distance_Data->Latitude1);
    dfi = GPS_DEGREES2RADIANS(Distance_Data->Longitude2 - Distance_Data->Longitude1);

    a = sin(df * (float)0.5) * sin(df * (float)0.5) + cos(f1) * cos(f2) * sin(dfi * (float)0.5) * sin(dfi * (float)0.5);
    /* Get distance in meters */
    Distance_Data->Distance = GPS_EARTH_RADIUS * 2 * atan2(sqrt(a), sqrt(1 - a)) * 1000;

    /* Calculate bearing between two points from point1 to point2 */
    df = sin(l2 - l1) * cos(f2);
    dfi = cos(f1) * sin(f2) - sin(f1) * cos(f2) * cos(l2 - l1);
    Distance_Data->Bearing = (GPS_RADIANS2DEGREES(atan2(df, dfi)));

    /* Make bearing always positive from 0 - 360 degrees instead of -180 to 180 */
    if (Distance_Data->Bearing < 0) {
        Distance_Data->Bearing += 360;
    }
}

/* Private */
LVQ_GPS_Result_t LVQ_GPS_INT_Do(LVQ_GPS_t* GPS_Data, char c) 
{
    if (LVQ_GPS_INT_FlagsOk(GPS_Data)) 
		{
        /* Data were valid before, new data are coming, not new anymore */
        LVQ_GPS_INT_ClearFlags(GPS_Data);
        /* Data were "new" on last call, now are only "Old data", no NEW data */
        GPS_Data->Status = LVQ_GPS_Result_OldData;
    }
    if (c == '$')
		{
        /* Star detection reset */
        LVQ_GPS_Star = 0;
        /* Reset CRC */
        LVQ_GPS_CRC = 0;
        /* First term in new statement */
        GPS_Term_Number = 0;
        /* At position 0 of a first term */
        GPS_Term_Pos = 0;
        /* Add character to first term */
        GPS_Term[GPS_Term_Pos++] = c;
    } 
		else if (c == ',') 
		{
        /* Add to parity */
        LVQ_GPS_INT_Add2CRC(c);
        /* Add 0 at the end */
        GPS_Term[GPS_Term_Pos++] = 0;

        /* Check empty */
        LVQ_GPS_INT_CheckEmpty(GPS_Data);

        /* Check term */
        LVQ_GPS_INT_CheckTerm(GPS_Data);

        /* Increase term number */
        GPS_Term_Number++;
        /* At position 0 of a first term */
        GPS_Term_Pos = 0;
    } 
		else if (c == '\n') 
		{
        /* Reset term number */
        GPS_Term_Number = 0;

#ifndef GPS_DISABLE_GPGSV
        /* Check for GPGSV statement */
        if (LVQ_GPS_Statement == GPS_GPGSV && GPGSV_StatementsCount == GPSGV_StatementNumber) 
				{
            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_SATSDESC);
        }
#endif
    } 
		else if (c == '\r') 
		{
        GPS_Term[GPS_Term_Pos++] = 0;

        /* Between * and \r are 2 characters of Checksum */
        LVQ_GPS_CRC_Received = LVQ_GPS_INT_Hex2Dec(GPS_Term[0]) * 16 + LVQ_GPS_INT_Hex2Dec(GPS_Term[1]);

        if (LVQ_GPS_CRC_Received != LVQ_GPS_CRC) {
            /* CRC is not OK, data failed somewhere */
            /* Clear all flags */
            LVQ_GPS_INT_ClearFlags(GPS_Data);
        }

        /* Reset term number */
        GPS_Term_Number = 0;
    } 
		else if (c == '*') 
		{
        /* Star detected */
        LVQ_GPS_Star = 1;
        /* Add 0 at the end */
        GPS_Term[GPS_Term_Pos++] = 0;

        /* Check empty */
        LVQ_GPS_INT_CheckEmpty(GPS_Data);

        /* Check term */
        LVQ_GPS_INT_CheckTerm(GPS_Data);

        /* Increase term number */
        GPS_Term_Number++;
        /* At position 0 of a first term */
        GPS_Term_Pos = 0;
    } 
		else 
		{
        /* Other characters detected */

        /* If star is not detected yet */
        if (!LVQ_GPS_Star) {
            /* Add to parity */
            LVQ_GPS_INT_Add2CRC(c);
        }

        /* Add to term */
        GPS_Term[GPS_Term_Pos++] = c;
    }

    /* Return */
    return LVQ_GPS_INT_Return(GPS_Data);
}

void LVQ_GPS_INT_CheckTerm(LVQ_GPS_t* GPS_Data) 
{
    uint32_t temp;
#ifndef GPS_DISABLE_GPGSA
    static uint8_t ids_count = 0;
#endif
    uint8_t count, i;
    if (GPS_Term_Number == 0) {
        /* Statement indicator */
        if (LVQ_GPS_INT_StringStartsWith(GPS_Term, "$GPGGA")) 
				{
            LVQ_GPS_Statement = GPS_GPGGA;
        } 
				else if (LVQ_GPS_INT_StringStartsWith(GPS_Term, "$GPRMC")) 
				{
            LVQ_GPS_Statement = GPS_GPRMC;
        } 
				else if (LVQ_GPS_INT_StringStartsWith(GPS_Term, "$GPGSA")) 
				{
            LVQ_GPS_Statement = GPS_GPGSA;
        } 
				else if (LVQ_GPS_INT_StringStartsWith(GPS_Term, "$GPGSV")) 
				{
            LVQ_GPS_Statement = GPS_GPGSV;
        } 
				else 
				{
            LVQ_GPS_Statement = GPS_ERR;
        }

        /* Copy term to variable */
        strcpy(GPS_Statement_Name, GPS_Term);

        /* Do nothing */
        return;
    }

    /* Check custom terms one by one */
    for (i = 0; i < GPS_Data->CustomStatementsCount; i++) {
        /* Term is inside current statement */
        if (LVQ_GPS_INT_StringStartsWith(GPS_Statement_Name, GPS_Data->CustomStatements[i]->Statement)) {
            /* Term number is correct */
            if (GPS_Term_Number == GPS_Data->CustomStatements[i]->TermNumber) {
                /* Copy string value */
                strcpy(GPS_Data->CustomStatements[i]->Value, GPS_Term);

                /* Set updated flag */
                GPS_Data->CustomStatements[i]->Updated = 1;
            }
        }
    }

    switch (GPS_CONCAT(LVQ_GPS_Statement, GPS_Term_Number)) {
#ifndef GPS_DISABLE_GPGGA
        case GPS_POS_LATITUDE:  /* GPGGA */
            /* Convert latitude */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Latitude = temp / 100;
            LVQ_GPS_INT_Data.Latitude += (float)(temp % 100) / (float)60;

            count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.Latitude += temp / (LVQ_GPS_INT_Pow(10, count) * 60.0);

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_LATITUDE);
            break;
        case GPS_POS_NS: /* GPGGA */
            if (GPS_Term[0] == 'S') {
                /* South has negative coordinate */
                LVQ_GPS_INT_Data.Latitude = -LVQ_GPS_INT_Data.Latitude;
            }

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_NS);
            break;
        case GPS_POS_LONGITUDE: /* GPGGA */
            /* Convert longitude */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Longitude = temp / 100; /* Degrees */
            LVQ_GPS_INT_Data.Longitude += (float)(temp % 100) / (float)60;

            count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.Longitude += temp / (LVQ_GPS_INT_Pow(10, count) * 60.0);

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_LONGITUDE);
            break;
        case GPS_POS_EW: /* GPGGA */
            if (GPS_Term[0] == 'W') {
                /* West has negative coordinate */
                LVQ_GPS_INT_Data.Longitude = -LVQ_GPS_INT_Data.Longitude;
            }

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_EW);
            break;
        case GPS_POS_SATS: /* GPGGA */
            /* Satellites in use */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Satellites = temp;

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_SATS);
            break;
        case GPS_POS_FIX: /* GPGGA */
            /* GPS Fix */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Fix = temp;

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_FIX);
            break;
        case GPS_POS_ALTITUDE: /* GPGGA */
            /* Convert altitude above sea */
            if (GPS_Term[0] == '-') {
                count = LVQ_GPS_INT_Atoi(&GPS_Term[1], &temp);
                LVQ_GPS_INT_Data.Altitude = temp;

                count++;
                count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
                LVQ_GPS_INT_Data.Altitude += temp / (LVQ_GPS_INT_Pow(10, count) * 1.0);

                LVQ_GPS_INT_Data.Altitude = -LVQ_GPS_INT_Data.Altitude;
            } else {
                count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
                LVQ_GPS_INT_Data.Altitude = temp;

                count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
                LVQ_GPS_INT_Data.Altitude += temp / (LVQ_GPS_INT_Pow(10, count) * 1.0);
            }

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_ALTITUDE);
            break;
        case GPS_POS_TIME: /* GPGGA */
            /* Set time */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Time.Seconds = temp % 100;
            LVQ_GPS_INT_Data.Time.Minutes = (int)(temp * (float) 0.01) % 100;
            LVQ_GPS_INT_Data.Time.Hours = (int)(temp * (float)0.0001) % 100;
            /* Hundredths */
            LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.Time.Hundredths = temp;

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_TIME);
            break;
#endif
#ifndef GPS_DISABLE_GPRMC
        case GPS_POS_SPEED: /* GPRMC */
            /* Convert speed */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Speed = (float)temp;

            count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.Speed += (float)((float)temp / (LVQ_GPS_INT_Pow(10, count) * 1.0));

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_SPEED);
            break;
        case GPS_POS_DATE: /* GPRMC */
            /* Set date */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Date.Year = temp % 100;
            LVQ_GPS_INT_Data.Date.Month = (int)(temp * (float)0.01) % 100;
            LVQ_GPS_INT_Data.Date.Date = (int)(temp * (float) 0.0001) % 100;

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_DATE);
            break;
        case GPS_POS_VALIDITY: /* GPRMC */
            /* GPS valid status */
            LVQ_GPS_INT_Data.Validity = GPS_Term[0] == 'A';

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_VALIDITY);
            break;
        case GPS_POS_DIRECTION: /* GPRMC */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.Direction = (float)temp;

            count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.Direction += (float)((float)temp / (LVQ_GPS_INT_Pow(10, count) * 1.0));

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_DIRECTION);
            break;
#endif
#ifndef GPS_DISABLE_GPGSA
        case GPS_POS_HDOP: /* GPGSA */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.HDOP = (float)temp;

            count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.HDOP += (float)((float)temp / (LVQ_GPS_INT_Pow(10, count) * 1.0));

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_HDOP);
            break;
        case GPS_POS_PDOP: /* GPGSA */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.PDOP = (float)temp;

            count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.PDOP += (float)((float)temp / (LVQ_GPS_INT_Pow(10, count) * 1.0));

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_PDOP);
            break;
        case GPS_POS_VDOP: /* GPGSA */
            count = LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.VDOP = (float)temp;

            count = LVQ_GPS_INT_Atoi(&GPS_Term[++count], &temp);
            LVQ_GPS_INT_Data.VDOP += (float)((float)temp / (LVQ_GPS_INT_Pow(10, count) * 1.0));

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_VDOP);
            break;
        case GPS_POS_FIXMODE: /* GPGSA */
            /* Satellites in view */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.FixMode = temp;

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_FIXMODE);
            break;
        case GPS_POS_SAT1:
        case GPS_POS_SAT2:
        case GPS_POS_SAT3:
        case GPS_POS_SAT4:
        case GPS_POS_SAT5:
        case GPS_POS_SAT6:
        case GPS_POS_SAT7:
        case GPS_POS_SAT8:
        case GPS_POS_SAT9:
        case GPS_POS_SAT10:
        case GPS_POS_SAT11:
        case GPS_POS_SAT12:
            /* Satellite numbers */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.SatelliteIDs[GPS_Term_Number - 3] = temp;

            /* Increase number of satellites found */
            ids_count++;

            if (ids_count == LVQ_GPS_INT_Data.Satellites) {
                ids_count = 0;

                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_SATS1_12);
            }
            break;
#endif
#ifndef GPS_DISABLE_GPGSV
        case GPS_POS_SATSINVIEW: /* GPGSV */
            /* Satellites in view */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            LVQ_GPS_INT_Data.SatellitesInView = temp;

            /* Set flag */
            LVQ_GPS_INT_SetFlag(GPS_FLAG_SATSINVIEW);
            break;
#endif
        default:
            break;
    }

#ifndef GPS_DISABLE_GPGSV
    /* Check for GPGSV statement separatelly */
    if (LVQ_GPS_Statement == GPS_GPGSV) {
        /* Check term number */

        if (GPS_Term_Number == 1) {
            /* Save number of GPGSV statements */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            GPGSV_StatementsCount = temp;
        }
        if (GPS_Term_Number == 2) {
            /* Save current of GPGSV statement number */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);
            GPSGV_StatementNumber = temp;
        }

        /* Data */
        if (GPS_Term_Number >= 4) {
            /* Convert to number */
            LVQ_GPS_INT_Atoi(GPS_Term, &temp);

            /* Get proper value */
            GPGSV_Term_Number = GPS_Term_Number - 4;
            GPGSV_Term_Mod = GPGSV_Term_Number % 4;
            GPGSV_Term_Number = (GPSGV_StatementNumber - 1) * 4 + (GPGSV_Term_Number / 4);

            /* If still memory available */
            if (GPGSV_Term_Number < GPS_MAX_SATS_IN_VIEW) {
                /* Check offset from 4 */
                if (GPGSV_Term_Mod == 0) {
                    LVQ_GPS_INT_Data.SatDesc[GPGSV_Term_Number].ID = temp;
                } else if (GPGSV_Term_Mod == 1) {
                    LVQ_GPS_INT_Data.SatDesc[GPGSV_Term_Number].Elevation = temp;
                } else if (GPGSV_Term_Mod == 2) {
                    LVQ_GPS_INT_Data.SatDesc[GPGSV_Term_Number].Azimuth = temp;
                } else if (GPGSV_Term_Mod == 3) {
                    LVQ_GPS_INT_Data.SatDesc[GPGSV_Term_Number].SNR = temp;
                }
            }
        }
    }
#endif
}

LVQ_GPS_Result_t
LVQ_GPS_INT_Return(LVQ_GPS_t* GPS_Data) {
    uint8_t i;
    if (LVQ_GPS_INT_FlagsOk(GPS_Data)) {
        /* Clear first time */
        LVQ_GPS_FirstTime = 0;

        /* Set data */
#ifndef GPS_DISABLE_GPGGA
        GPS_Data->Latitude = LVQ_GPS_INT_Data.Latitude;
        GPS_Data->Longitude = LVQ_GPS_INT_Data.Longitude;
        GPS_Data->Satellites = LVQ_GPS_INT_Data.Satellites;
        GPS_Data->Fix = LVQ_GPS_INT_Data.Fix;
        GPS_Data->Altitude = LVQ_GPS_INT_Data.Altitude;
        GPS_Data->Time = LVQ_GPS_INT_Data.Time;
#endif
#ifndef GPS_DISABLE_GPRMC
        GPS_Data->Speed = LVQ_GPS_INT_Data.Speed;
        GPS_Data->Date = LVQ_GPS_INT_Data.Date;
        GPS_Data->Validity = LVQ_GPS_INT_Data.Validity;
        GPS_Data->Direction = LVQ_GPS_INT_Data.Direction;
#endif
#ifndef GPS_DISABLE_GPGSA
        GPS_Data->HDOP = LVQ_GPS_INT_Data.HDOP;
        GPS_Data->VDOP = LVQ_GPS_INT_Data.VDOP;
        GPS_Data->PDOP = LVQ_GPS_INT_Data.PDOP;
        GPS_Data->FixMode = LVQ_GPS_INT_Data.FixMode;
        for (i = 0; i < 12; i++) {
            GPS_Data->SatelliteIDs[i] = LVQ_GPS_INT_Data.SatelliteIDs[i];
        }
#endif
#ifndef GPS_DISABLE_GPGSV
        GPS_Data->SatellitesInView = LVQ_GPS_INT_Data.SatellitesInView;
        for (i = 0; i < GPS_MAX_SATS_IN_VIEW; i++) {
            GPS_Data->SatDesc[i] = LVQ_GPS_INT_Data.SatDesc[i];
        }
#endif

        /* Return new data */
        LVQ_GPS_INT_ReturnWithStatus(GPS_Data, LVQ_GPS_Result_NewData);
    }

    /* We are first time */
    if (LVQ_GPS_FirstTime) {
        LVQ_GPS_INT_ReturnWithStatus(GPS_Data, LVQ_GPS_Result_FirstDataWaiting);
    }

    /* Return old data */
    LVQ_GPS_INT_ReturnWithStatus(GPS_Data, LVQ_GPS_Result_OldData);
}

uint8_t
LVQ_GPS_INT_StringStartsWith(char* string, const char* str) {
    while (*str) {
        if (*str++ != *string++) {
            return 0;
        }
    }
    return 1;
}

uint8_t
LVQ_GPS_INT_Atoi(char* str, uint32_t* val) {
    uint8_t count = 0;
    *val = 0;
    while (GPS_IS_DIGIT(*str)) {
        *val = *val * 10 + GPS_C2N(*str++);
        count++;
    }
    return count;
}

uint32_t
LVQ_GPS_INT_Pow(uint8_t x, uint8_t y) {
    uint32_t ret = 1;
    while (y--) {
        ret *= x;
    }
    return ret;
}

uint8_t
LVQ_GPS_INT_Hex2Dec(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';         /* 0 - 9 */
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;    /* 10 - 15 */
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;    /* 10 - 15 */
    }
    return 0;
}

uint8_t LVQ_GPS_INT_FlagsOk(LVQ_GPS_t* GPS_Data) 
{
    /* Check main flags */
    if (GPS_Flags == GPS_Flags_OK) {
        uint8_t i;
        /* Check custom terms */
        for (i = 0; i < GPS_Data->CustomStatementsCount; i++) {
            /* If not flag set */
            if (GPS_Data->CustomStatements[i]->Updated == 0) {
                /* Return, flags not OK */
                return 0;
            }
        }

        /* Flags valid */
        return 1;
    }

    /* Not valid */
    return 0;
}

void LVQ_GPS_INT_ClearFlags(LVQ_GPS_t* GPS_Data) 
{
    uint8_t i;

    /* Reset main flags */
    GPS_Flags = 0;

    /* Clear custom terms */
    for (i = 0; i < GPS_Data->CustomStatementsCount; i++) {
        /* If not flag set */
        GPS_Data->CustomStatements[i]->Updated = 0;
    }
}

void LVQ_GPS_INT_CheckEmpty(LVQ_GPS_t* GPS_Data) 
{
    if (GPS_Term_Pos == 1) 
		{
        switch (GPS_CONCAT(LVQ_GPS_Statement, GPS_Term_Number)) 
				{
#ifndef GPS_DISABLE_GPGGA
            case GPS_POS_LATITUDE:  /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_LATITUDE);
                break;
            case GPS_POS_NS: /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_NS);
                break;
            case GPS_POS_LONGITUDE: /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_LONGITUDE);
                break;
            case GPS_POS_EW: /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_EW);
                break;
            case GPS_POS_SATS: /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_SATS);
                break;
            case GPS_POS_FIX: /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_FIX);
                break;
            case GPS_POS_ALTITUDE: /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_ALTITUDE);
                break;
            case GPS_POS_TIME: /* GPGGA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_TIME);
                break;
#endif
#ifndef GPS_DISABLE_GPRMC
            case GPS_POS_SPEED: /* GPRMC */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_SPEED);
                break;
            case GPS_POS_DATE: /* GPRMC */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_DATE);
                break;
            case GPS_POS_VALIDITY: /* GPRMC */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_VALIDITY);
                break;
            case GPS_POS_DIRECTION: /* GPRMC */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_DIRECTION);
                break;
#endif
#ifndef GPS_DISABLE_GPGSA
            case GPS_POS_HDOP: /* GPGSA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_HDOP);
                break;
            case GPS_POS_PDOP: /* GPGSA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_PDOP);
                break;
            case GPS_POS_VDOP: /* GPGSA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_VDOP);
                break;
            case GPS_POS_FIXMODE: /* GPGSA */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_FIXMODE);
                break;
            case GPS_POS_SAT1:
            case GPS_POS_SAT2:
            case GPS_POS_SAT3:
            case GPS_POS_SAT4:
            case GPS_POS_SAT5:
            case GPS_POS_SAT6:
            case GPS_POS_SAT7:
            case GPS_POS_SAT8:
            case GPS_POS_SAT9:
            case GPS_POS_SAT10:
            case GPS_POS_SAT11:
            case GPS_POS_SAT12:
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_SATS1_12);
                break;
#endif
#ifndef GPS_DISABLE_GPGSV
            case GPS_POS_SATSINVIEW: /* GPGSV */
                /* Set flag */
                LVQ_GPS_INT_SetFlag(GPS_FLAG_SATSINVIEW);
                break;
#endif
            default:
                break;
        }
    }
}


