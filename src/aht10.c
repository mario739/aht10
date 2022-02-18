
/**
 * @file aht10.c
 * @author Mario Aguilar (fernando_aguilar731010@gmail.com)
 * @brief Driver para el sensor aht10
 * @version v1
 * @date 2021-12-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "aht10.h"

void aht10Init(aht10_config_t *obj, aht10WriteFcn_t fncWritePort, aht10ReadFcn_t fncReadPort, delay1ms_t fncDelayPort)
{
  obj->writeI2C=fncWritePort;
  obj->readI2C=fncReadPort;
  obj->delay_ms_I2C=fncDelayPort;
}

aht10_status aht10_get_status(aht10_config_t *obj)
{
  uint8_t buffer[0]={0};
  aht10_status_fnc status=AHT10_ERROR;
  status= obj->readI2C(AHT10_ADDRESS_SLAVE,buffer,1);
  if (status==AHT10_OK)
  {
    if (buffer[0]>>7==0)                     //El estado del sensor esta en la posicion 8 del byte por eso de desplaza 7 posiciones   
      return SENSOR_IDLE;
    else if (buffer[0]>>7==1)
      return SENSOR_BUSY;
  }
  return SENSOR_BUSY;
}

aht10_status_fnc aht10_start_measurement(aht10_config_t *obj)
{ 
  uint8_t cmd[0]={AHT10_CMD_INITIALIZE};
  aht10_status_fnc status = obj->writeI2C(AHT10_ADDRESS_SLAVE,cmd,1);
  if (status==AHT10_OK)
  {
    obj->delay_ms_I2C(AHT10_DELAY_MEASUREMENT);
  }
  return  status;
}

aht10_status_fnc aht10_launch_measurement(aht10_config_t *obj)
{
  uint8_t cmd[2] = {AHT10_CMD_TRIGGER_MEASUREMENT,AHT10_CMD_DATO_0,AHT10_CMD_DATO_1};
  aht10_status_fnc status = obj->writeI2C(AHT10_ADDRESS_SLAVE ,cmd,3);
  if (status==AHT10_OK)
  {
    obj->delay_ms_I2C(AHT10_DELAY_LAUNCH_MEASUREMENT);
    if (aht10_get_status(obj)==SENSOR_IDLE)
    {
      status=obj->writeI2C(AHT10_ADDRESS_SLAVE,cmd,3);
      obj->delay_ms_I2C(AHT10_DELAY_LAUNCH_MEASUREMENT);
      return status;
    }
    else status=AHT10_ERROR;
  }
  return  status;
}

aht10_status_fnc aht10_get_humedity(aht10_config_t*obj, uint8_t *data)
{
  if (obj== NULL)
  {
    return AHT10_ERROR;
  } 
  uint8_t bufferRead[6]={0};
  uint32_t data_humedity=0;
  aht10_status_fnc status=aht10_launch_measurement(obj);
  if (status==AHT10_OK)
  {
    status= obj->readI2C(AHT10_ADDRESS_SLAVE,bufferRead,6);
    if (status==AHT10_OK)
    {
      data_humedity=(((uint32_t)bufferRead[1]<<16) | ((uint16_t)bufferRead[2]<<8) | (bufferRead[3]))>>4;
      *data= HUMEDITY(data_humedity);
    }
  }
  return status;
}

aht10_status_fnc aht10_get_temperature(aht10_config_t*obj, int8_t *data)
{
  if (obj== NULL)
  {
    return AHT10_ERROR;
  } 
  uint8_t buffer_read[6]={0};
  uint32_t data_temperature=0;
  aht10_status_fnc status=AHT10_ERROR;
  status=aht10_launch_measurement(obj);
  if (status==AHT10_OK)
  {
    status=obj->readI2C(AHT10_ADDRESS_SLAVE ,buffer_read,6);
    if (status==AHT10_OK)
    {
      data_temperature=((uint32_t)(buffer_read[3] & 0x0F)<<16) | ((uint16_t) buffer_read[4]<<8)| buffer_read[5];
      *data= TEMPERATURE(data_temperature);
    }
  }
  return status;
}

aht10_status_fnc  aht10SoftReset(aht10_config_t*obj)
{
  uint8_t cmd[0]={AHT10_CMD_SOFT_RESET};
  aht10_status_fnc status=AHT10_ERROR;
  status=obj->writeI2C(AHT10_ADDRESS_SLAVE ,cmd,1);
  if (status==AHT10_OK)
  {
    obj->delay_ms_I2C(AHT10_DELAY_RESET);
  }
  return status;
}

