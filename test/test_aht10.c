/**
 * @file test_aht10.c
 * @author Mario Aguilar (fernando_aguilar731010@gmail.com)
 * @brief Driver para el sensor aht10
 * @version v1
 * @date 2021-12-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include "unity.h"
#include "mock_i2c_functions.h"
#include "aht10.h"


typedef struct
{
    uint8_t adrees;
    uint8_t amount;
    uint8_t buffer;
}data_callback_st;

//Variables globales 
data_callback_st data_callback;
uint8_t comando;
aht10_config_t aht10config;

/*aht10_status_fnc  read_I2C_STM32L432_port(uint8_t addr,uint8_t* buffer, uint8_t amount)
{
    uint8_t trama[6]={0,0,0,0,0,0};
    uint8_t data=0;

    if (amount==1)
    {
        *buffer=data;
    }
    else if (amount==6)
    {
        for (int i = 0; i<7; i++)
        {
           *buffer=trama[i];
           buffer++;
        }
    }
    return AHT10_OK;
}
aht10_status_fnc   write_I2C_STM32L432_port(uint8_t addr, uint8_t *buffer, uint8_t amount)
{
    data_callback.adrees=addr;
    data_callback.buffer=*buffer;
    data_callback.amount=amount; 
    return AHT10_OK;
}  */

/**
 * @test Para probar la configuracion del driver
 * 
 */
void test_probar_la_funcion_de_init_del_sensor(void)
{
    aht10Init(&aht10config,write_I2C_STM32L432_port,read_I2C_STM32L432_port,delay_STM32L432_port);

    // Verificacion si se cargaron las funciones a la estructura 
    TEST_ASSERT_EQUAL(aht10config.writeI2C,write_I2C_STM32L432_port);
    TEST_ASSERT_EQUAL(aht10config.readI2C,read_I2C_STM32L432_port);
    TEST_ASSERT_EQUAL(aht10config.delay_ms_I2C,delay_STM32L432_port);
}   
/**
 * @test Probar el estado del sensor 
 * 
 */
void test_probar_funcion_obtener_estado_idle_del_sensor(void)
{
    //Verificar si el sensor se encuentra en estado idle
    uint8_t buffer[1]={0};
    uint8_t data=256;
    read_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,buffer,1,AHT10_OK);
    read_I2C_STM32L432_port_ReturnThruPtr_buffer(&data);
    TEST_ASSERT_EQUAL(SENSOR_IDLE,aht10_get_status(&aht10config));
}

/**
 * @test Se inicializa el sensor con el comando AHT110_CMD_INITIALIZE
 * 
 */
void test_probar_funcion_de_inicializacion_sensor(void)
{
    uint8_t cmd[1]={AHT10_CMD_INITIALIZE};
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,1,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_MEASUREMENT);
    TEST_ASSERT_EQUAL(AHT10_OK,aht10_start_measurement(&aht10config));
} 

/**
 * @test Se prueba la funcion para empezar la medicion con el comando AHT10_CMD_TRIGGER_MEASUREMENT
 * 
 */
void test_probar_funcion_empezar_la_medicion_del_sensor(void)
{
    uint8_t cmd[3] = {AHT10_CMD_TRIGGER_MEASUREMENT,AHT10_CMD_DATO_0,AHT10_CMD_DATO_1};
    uint8_t buffer[1]={0};
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_LAUNCH_MEASUREMENT);
    read_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,buffer,1,AHT10_OK);
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_LAUNCH_MEASUREMENT); 
    TEST_ASSERT_EQUAL(AHT10_OK,aht10_launch_measurement(&aht10config));

    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_ERROR); 
    TEST_ASSERT_EQUAL(AHT10_ERROR,aht10_launch_measurement(&aht10config));

    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_LAUNCH_MEASUREMENT);
    read_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,buffer,1,AHT10_ERROR);
    TEST_ASSERT_EQUAL(AHT10_ERROR,aht10_launch_measurement(&aht10config));

}

/**
 * @test Lectura de la humedad 
 * 
 */
void test_probar_funcion_obtener_humedad(void)
{
    uint8_t bufferRead[6]={0};
    uint8_t humedad=0;
    uint8_t cmd[3] = {AHT10_CMD_TRIGGER_MEASUREMENT,AHT10_CMD_DATO_0,AHT10_CMD_DATO_1};
    uint8_t buffer[1]={0};
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_LAUNCH_MEASUREMENT);
    read_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,buffer,1,AHT10_OK);
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_LAUNCH_MEASUREMENT); 
    read_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,bufferRead,6,AHT10_OK);
    TEST_ASSERT_EQUAL(AHT10_OK,aht10_get_humedity(&aht10config,&humedad));
    TEST_ASSERT_EQUAL(0,humedad);
}

/**
 * @test Lectura de la temperatura 
 * 
 */
void test_probar_funcion_obtener_temperatura(void)
{
    int8_t temperatura=0;

    uint8_t bufferRead[6]={0};
    uint8_t humedad=0;
    uint8_t cmd[3] = {AHT10_CMD_TRIGGER_MEASUREMENT,AHT10_CMD_DATO_0,AHT10_CMD_DATO_1};
    uint8_t buffer[1]={0};
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_LAUNCH_MEASUREMENT);
    read_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,buffer,1,AHT10_OK);
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,cmd,3,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_LAUNCH_MEASUREMENT); 
    read_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE,bufferRead,6,AHT10_OK);    
    TEST_ASSERT_EQUAL(AHT10_OK,aht10_get_temperature(&aht10config,&temperatura));
    //Verificar el dato obtenido de la temperatura 
    TEST_ASSERT_EQUAL(-50,temperatura);
}


/**
 *@test Resetear el sensor por software
 * 
 */
void test_probar_funcion_reset(void)
{
    uint8_t cmd[1]={AHT10_CMD_SOFT_RESET};
    write_I2C_STM32L432_port_ExpectAndReturn(AHT10_ADDRESS_SLAVE ,cmd,1,AHT10_OK);
    delay_STM32L432_port_Expect(AHT10_DELAY_RESET);
    TEST_ASSERT_EQUAL(AHT10_OK,aht10SoftReset(&aht10config));

}