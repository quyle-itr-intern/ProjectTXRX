/**
  ******************************************************************************

  DHT11 For SLVQ32F446RE
  Author:   LVQ
  Updated:  11st August 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_dht11.h"

/* Private functions */
LVQ_DHT11_t LVQ_DHT11_INT_Read(LVQ_DHT11_Data_t* data);
LVQ_DHT11_t LVQ_DHT11_INT_InitPins(void);

LVQ_DHT11_t LVQ_DHT11_Init(void) 
{
    /* Initialize delay */
    Delay_Init();

    /* Initialize pin */
    LVQ_GPIO_Init(DHT11_PORT, DHT11_PIN, LVQ_GPIO_Mode_IN, LVQ_GPIO_OType_PP, LVQ_GPIO_PuPd_UP, LVQ_GPIO_Speed_Medium);

    /* Return OK */
    return LVQ_DHT11_OK;
}

LVQ_DHT11_t LVQ_DHT11_Read(LVQ_DHT11_Data_t* data) 
{
    /* Read data */
    return LVQ_DHT11_INT_Read(data);
}

/* Internal function */
LVQ_DHT11_t LVQ_DHT11_INT_Read(LVQ_DHT11_Data_t* data) 
{
    volatile uint32_t time;
    uint8_t i, j, d[5];

    /* Pin output */
    DHT11_PIN_OUT;
    /* Set pin low for >18 ms */
    DHT11_PIN_LOW;
    Delay_ms_systick(20);
    /* Set pin high to ~30 us */
    DHT11_PIN_HIGH;

    /* Wait 10us for acknowledge, low signal */
    Delay_us_systick(10);
		
		/* Read mode */
    DHT11_PIN_IN;
	
		time = 0;
    /* Wait 45us for acknowledge, low signal */
    while (DHT11_PIN_READ) 
		{
        if (time > 50) 
				{
            return LVQ_DHT11_CONNECTION_ERROR;
        }
        /* Increase time */
        time++;
        /* Wait 1 us */
        Delay_us_systick(1);
    }

    time = 0;
    /* Wait high signal, about 80-85us long (measured with logic analyzer) */
    while (!DHT11_PIN_READ) 
		{
        if (time > 85) 
				{
            return LVQ_DHT11_WAITHIGH_ERROR;
        }
        /* Increase time */
        time++;
        /* Wait 1 us */
        Delay_us_systick(1);
    }

    time = 0;
    /* Wait low signal, about 80-85us long (measured with logic analyzer) */
    while (DHT11_PIN_READ) 
		{
        if (time > 95) 
				{
            return LVQ_DHT11_WAITLOW_ERROR;
        }
        /* Increase time */
        time++;
        /* Wait 1 us */
        Delay_us_systick(1);
    }

    /* Read 5 bytes */
    for (j = 0; j < 5; j++) 
	  {
        d[j] = 0;
        for (i = 8; i > 0; i--) 
			  {
            /* We are in low signal now, wait for high signal and measure time */
            time = 0;
            /* Wait high signal, about 50us long (measured with logic analyzer) */
            while (!DHT11_PIN_READ) 
						{
                if (time > 65) 
								{
                    return LVQ_DHT11_WAITHIGH_LOOP_ERROR;
                }
                /* Increase time */
                time++;
                /* Wait 1 us */
                Delay_us_systick(1);
            }
            /* High signal detected, start measure high signal, it can be 26us for 0 or 70us for 1 */
            time = 0;
            /* Wait low signal, between 26 and 70us long (measured with logic analyzer) */
            while (DHT11_PIN_READ) 
						{
                if (time > 90) 
								{
                    return LVQ_DHT11_WAITLOW_LOOP_ERROR;
                }
                /* Increase time */
                time++;
                /* Wait 1 us */
                Delay_us_systick(1);
            }

            if (time > 18 && time < 37) 
						{
                /* We read 0 */
            } 
						else 
						{
                /* We read 1 */
                d[j] |= 1 << (i - 1);
            }
        }
    }

    /* Check for parity */
    if (((d[0] + d[1] + d[2] + d[3]) & 0xFF) != d[4]) 
		{
        /* Parity error, data not valid */
        return LVQ_DHT11_PARITY_ERROR;
    }

    /* Set humidity */
    data->Hum = d[0];
    /* Negative temperature */
		/*
    if (d[2] & 0x80) 
		{
        data->Temp = -((d[2] & 0x7F) << 8 | d[3]);
    } 
		else 
		{
        data->Temp = (d[2]) << 8 | d[3];
    }
		*/
		/* Set temperture */
		data->Temp = d[2];

    /* Data OK */
    return LVQ_DHT11_OK;
}
