 /**
 * @file aht10
 * @author Mario Aguilar (fernando_aguilar731010@gmail.com)
 * @brief Driver para el sensor aht10
 * @version v1
 * @date 2021-12-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */
 #ifndef AHT10_H
 #define AHT10_H

 #include <stdint.h> 
 #include <stddef.h>
 
 /** @defgroup Comandos del driver
 *  @{
 */
#define AHT10_ADDRESS_SLAVE                  0x38U    /*!< Direccion de esclavo 7 bits       */
#define AHT10_CMD_INITIALIZE                 0xE1U   /*!< Comando de inicializacion        */
#define AHT10_CMD_TRIGGER_MEASUREMENT        0xACU    /*!< Comando para iniciar la lectura  */
#define AHT10_CMD_SOFT_RESET                 0xBAU    /*!< Comando para reiniciar el sensor */
#define AHT10_CMD_DATO_0                     0X33U
#define AHT10_CMD_DATO_1                     0X00U

/**
  * @}
  */

/** @defgroup Definicion de retardos 
 * 
 */
#define AHT10_DELAY_POWER_ON                  40U      /*!<Retardo para el encendido        */
#define AHT10_DELAY_RESET                     25U      /*!<Retardo de reset                 */
#define AHT10_DELAY_MEASUREMENT               100U     /*!<Retardo inicializacion           */
#define AHT10_DELAY_LAUNCH_MEASUREMENT        75U      /*!<Retardo para la medicion         */
/**
  * @}
  */

/** @defgroup Macros para la transformacion de a magnitudes reales
 * 
 */
#define TEMPERATURE(A)                       (int8_t) ((A *0.000191)-50)    /*!<Macro con la formula para obtener la temepratura en grados Centigrados */    
#define HUMEDITY(A)                          (uint8_t) (A *0.000095)         /*!<Macro para obtener la humedad en porcentaje                            */
/**
  * @}
  */


//! Tipo de datos para manejar el estado de la funcion 
typedef enum
{
  AHT10_OK      = 0,      //!< La funcion se ejecuto correctamente 
  AHT10_ERROR   = 1,      //!< La funcion produjo un error
} aht10_status_fnc;

//! Tipo de datos para manejar el estado del sensor 
typedef enum
{
    SENSOR_IDLE,        //!< Sensor ya realizo para conversion y los datos ya se pueden leer 
    SENSOR_BUSY,        //!< Sensor ocupado  realizando la convercion  
}aht10_status;


typedef aht10_status_fnc ( *aht10WriteFcn_t )(uint8_t , uint8_t*, uint8_t); /*!<Puntero a funcion  de callback para la escritura     */

typedef aht10_status_fnc ( *aht10ReadFcn_t )(uint8_t , uint8_t*, uint8_t);  /*!<Puntero a funcion de callback para la lectura*/

typedef void (*delay1ms_t)(uint8_t);                                        /*!<Puntero a funcion de callback para el delay*/

//! Estructura para la configuracion del sensor 
typedef struct 
{
  aht10WriteFcn_t  writeI2C;
  aht10ReadFcn_t  readI2C;
  delay1ms_t      delay_ms_I2C;    
  aht10_status_fnc status_fun;
}aht10_config_t;


/*************************************************************************************************
	 *  @brief Funcion para inicializar el driver AHT10
     *
     *  @details
     *   Se asignan las funciones pasadas por parametros a la estructura que tambien se pasa como parametro
     *
	 *  @param		obj	            structura del tipo aht10_config_t donde se asignaran las funciones de callback
   *  @param    fncWritePort    Funcion de escritura por i2c propia del hardware
   *  @param    fncReadPort     Funcion de leer por i2c propia del hardware
   *  @param    fncDelayPor     Retardos
   *  @param    addressSlave    Direccion del esclavo
	 *  @return     None.
	 *  
***************************************************************************************************/

void aht10Init(aht10_config_t *obj, aht10WriteFcn_t fncWritePort, aht10ReadFcn_t fncReadPort, delay1ms_t fncDelayPort);

/**
 * @brief Funcion para obtener el estado del sensor
 * 
 * @param obj Estructura que contiene la configuracion del driver 
 * @return enumerador que devuelve el estado de la funcion 
 */
aht10_status aht10_get_status(aht10_config_t *obj);

/**
 * @brief Funcion que lanza el inicio la conversion
 * 
 *  @details
 *      Se inicializa la medicion de temperatura y humedad 
 * @param obj Estructura que contiene la configuracion del driver 
 * @return aht10_status_fnc enumerador que devuelve el estado de la funcion
 */

 aht10_status_fnc aht10_launch_measurement(aht10_config_t *obj);

/**
 * @brief Funcion para inizializar el sensor 
 * 
 * @param obj Estructura que contiene la configuracion del driver 
 * @return aht10_status_fnc  enumerador que devuelve el estado de la funcion
 */

aht10_status_fnc aht10_start_measurement(aht10_config_t *obj);

/**
 * @brief Funcion para obtener el dato de la humedad actual 
 * 
 * @param obj Estructura que contiene la configuracion del driver 
 * @param data variable donde se asignara el resultado de la humedad 
 * @return aht10_status_fnc enumerador que devuelve el estado de la funcion
 */

aht10_status_fnc aht10_get_humedity(aht10_config_t*obj, uint8_t *data);


/**
 * @brief Funcion para obtener la temperatura actual
 * 
 * 
 * @param obj Estructura que contiene la configuracion del driver
 * @param data variable donde se asignara el resultado de la temperatura
 * @return aht10_status_fnc enumerador que devuelve el estado de la funcion
 */
aht10_status_fnc aht10_get_temperature(aht10_config_t*obj, int8_t *data);

/**
 * @brief Funcion que resetea el modulo 
 * 
 * @param obj Estructura que contiene la configuracion del driver
 * @return aht10_status_fnc enumerador que devuelve el estado de la funcion
 */
aht10_status_fnc aht10SoftReset(aht10_config_t*obj);

 #endif