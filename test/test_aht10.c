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



/**
 * @brief Funcion para simular la lectura de los datos del puerto I2C
 * 
 * @param addr          Direccion del sensor 
 * @param buffer        Referencia al buffer donde se almacenaran los datos leidos 
 * @param amount        Cantidad de datos a leer
 * @return aht10_status_fnc  Resultado de la operacion de lectura 
 */
aht10_status_fnc   read_I2C_STM32L432_port(uint8_t addr, uint8_t* buffer, uint8_t amount);


/**
 * @brief Funcion para simular la escritura de datos del puerto I2C
 * 
 * @param addr           Direccion del sensor  
 * @param buffer         Puntero a la variable que contine los datos a escribir
 * @param amount         Cantidad de datos a escribir 
 * @return aht10_status_fnc     Resultado de la operacion de escritura 
 */
aht10_status_fnc   write_I2C_STM32L432_port(uint8_t addr, uint8_t* buffer, uint8_t amount);

/**
 * @brief Funcion para simular un retardo 
 * 
 * @param delay tiempo del retardo 
 */
void  delay_STM32L432_port(uint8_t delay);

aht10_status_fnc  read_I2C_STM32L432_port(uint8_t addr,uint8_t* buffer, uint8_t amount)
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
}

void delay_STM32L432_port(uint8_t delay){}   

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

    TEST_ASSERT_EQUAL(SENSOR_IDLE,aht10_get_status(&aht10config));
}

/**
 * @test Se inicializa el sensor con el comando AHT110_CMD_INITIALIZE
 * 
 */
void test_probar_funcion_de_inicializacion_sensor(void)
{
    TEST_ASSERT_EQUAL(AHT10_OK,aht10_start_measurement(&aht10config));

    //Verificar si se cargaron los datos al estructura data_callback
    TEST_ASSERT_EQUAL_HEX(AHT10_ADDRESS_SLAVE,data_callback.adrees);
    TEST_ASSERT_EQUAL(AHT10_CMD_INITIALIZE,data_callback.buffer);
    TEST_ASSERT_EQUAL(1,data_callback.amount);

} 

/**
 * @test Se prueba la funcion para empezar la medicion con el comando AHT10_CMD_TRIGGER_MEASUREMENT
 * 
 */
void test_probar_funcion_empezar_la_medicion_del_sensor(void)
{
    TEST_ASSERT_EQUAL(AHT10_OK,aht10_launch_measurement(&aht10config));

    //Verificar si se cargaron los datos al estructura data_callback
    TEST_ASSERT_EQUAL_HEX(AHT10_ADDRESS_SLAVE,data_callback.adrees);
    TEST_ASSERT_EQUAL(AHT10_CMD_TRIGGER_MEASUREMENT,data_callback.buffer);
    TEST_ASSERT_EQUAL(1,data_callback.amount);
}


/**
 * @test Lectura de la humedad 
 * 
 */
void test_probar_funcion_obtener_humedad(void)
{
    uint8_t humedad=0;
    TEST_ASSERT_EQUAL(AHT10_OK,aht10_get_humedity(&aht10config,&humedad));
    //Verificar el dato obtenido de la humedad
    TEST_ASSERT_EQUAL(0,humedad);
}

/**
 * @test Lectura de la temperatura 
 * 
 */
void test_probar_funcion_obtener_temperatura(void)
{
    int8_t temperatura=0;
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
    TEST_ASSERT_EQUAL(AHT10_OK,aht10SoftReset(&aht10config));
    //Verificar si se cargaron los datos al estructura data_callback
    TEST_ASSERT_EQUAL_HEX(AHT10_ADDRESS_SLAVE,data_callback.adrees);
    TEST_ASSERT_EQUAL(AHT10_CMD_SOFT_RESET,data_callback.buffer);
    TEST_ASSERT_EQUAL(1,data_callback.amount);
}