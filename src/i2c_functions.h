/**
 * @file i2c_functions.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef I2C_FUNCTIONS
#define I2C_FUNCTIONS
#include "aht10.h"
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
#endif