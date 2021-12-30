#include "unity.h"
#include "aht10.h"
#include <stdio.h>



aht10_status_fnc   read_I2C_STM32L432_port(uint8_t addr, uint8_t* buffer, uint8_t amount);
aht10_status_fnc   write_I2C_STM32L432_port(uint8_t addr, uint8_t* buffer, uint8_t amount);
void   delay_STM32L432_port(uint8_t delay);


uint8_t comando;

aht10_status_fnc  read_I2C_STM32L432_port(uint8_t addr,uint8_t* buffer, uint8_t amount)
{
    uint8_t trama[6]={0,0,0,0,0,0};
    uint8_t data2=0;

    if (amount==1)
    {
        *buffer=data2;
    }
    else if (amount==6)
    {
        for (int i = 0; i<7; i++)
        {
           *buffer=trama[i];
           buffer++;
        }
    }

    return aht10_OK;
}
aht10_status_fnc   write_I2C_STM32L432_port(uint8_t addr, uint8_t* buffer, uint8_t amount)
{

    comando=0xBA;
    return aht10_OK;
}

void   delay_STM32L432_port(uint8_t delay)
{
    
}

aht10_config_t aht10config;

void setUp(void)
{
    aht10Init(&aht10config,write_I2C_STM32L432_port,read_I2C_STM32L432_port,delay_STM32L432_port,AHT10_ADDRESS_SLAVE  );
}

void test_probar_la_funcion_de_init_del_sensor(void)
{
    aht10Init(&aht10config,write_I2C_STM32L432_port,read_I2C_STM32L432_port,delay_STM32L432_port,AHT10_ADDRESS_SLAVE  );
    TEST_ASSERT_EQUAL(aht10config.writeI2C,write_I2C_STM32L432_port);
    TEST_ASSERT_EQUAL(aht10config.readI2C,read_I2C_STM32L432_port);
    TEST_ASSERT_EQUAL(aht10config.delay_ms_I2C,delay_STM32L432_port);
    TEST_ASSERT_EQUAL(aht10config.addresSlave,AHT10_ADDRESS_SLAVE);
}   

void test_probar_funcion_obtener_estado_del_sensor(void)
{
    TEST_ASSERT_EQUAL(SENSOR_IDLE,aht10_get_status(&aht10config));
}

void test_probar_funcion_de_inicializacion_sensor(void)
{
    TEST_ASSERT_EQUAL(0,aht10_start_measurement(&aht10config));
} 

void test_probar_funcion_empezar_la_medicion_del_sensor(void)
{
    TEST_ASSERT_EQUAL(0,aht10_launch_measurement(&aht10config));
}

void test_probar_funcion_obtener_humedad(void)
{
    uint8_t data=0;
    TEST_ASSERT_EQUAL(0,aht10_get_humedity(&aht10config,&data));
    TEST_ASSERT_EQUAL(0,data);
}

void test_probar_funcion_obtener_temperatura(void)
{
    uint8_t data=0;
    TEST_ASSERT_EQUAL(0,aht10_get_temperature(&aht10config,&data));
    TEST_ASSERT_EQUAL(206,data);
}

void test_probar_funcion_reset(void)
{
    TEST_ASSERT_EQUAL(AHT10_CMD_SOFT_RESET,comando);
}