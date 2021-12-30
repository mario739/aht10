
#include "aht10.h"

void aht10Init(aht10_config_t *obj, aht10WriteFcn_t fncWritePort, aht10ReadFcn_t fncReadPort, delay1ms_t fncDelayPort,uint16_t addressSlave)
{
    obj->writeI2C=fncWritePort;
    obj->readI2C=fncReadPort;
    obj->delay_ms_I2C=fncDelayPort;
    obj->addresSlave=addressSlave;
}

aht10_status aht10_get_status(aht10_config_t *obj)
{
    uint8_t buffer;
    obj->readI2C(obj->addresSlave,&buffer,1);
    if (buffer>>7==0)
    {
        return SENSOR_IDLE;
    }
    else if (buffer>>7==1)
    {
        return SENSOR_BUSY;
    }
}


aht10_status_fnc aht10_start_measurement(aht10_config_t *obj)
{ 
  aht10_status_fnc status = obj->writeI2C(obj->addresSlave,AHT10_CMD_INITIALIZE,1);
  if (status==0)
  {
      obj->delay_ms_I2C(AHT10_DELAY_MEASUREMENT);
      return aht10_OK;
  }
  else
  {
      return  status;
  }
}

aht10_status_fnc aht10_launch_measurement(aht10_config_t *obj)
{
  aht10_status_fnc status = obj->writeI2C(obj->addresSlave,AHT10_CMD_TRIGGER_MEASUREMENT ,3);
  if (status==0)
  {
      obj->delay_ms_I2C(AHT10_DELAY_LAUNCH_MEASUREMENT);
      return aht10_OK;
  }
  else
  {
      return  status;
  }
}

aht10_status_fnc aht10_get_humedity(aht10_config_t*obj, uint8_t *data)
{
  uint8_t bufferRead[6]={0,0,0,0,0,0};

  aht10_launch_measurement(obj);
  
  obj->readI2C(obj->addresSlave, bufferRead ,6);

  uint32_t Data_Humedity=(((uint32_t)bufferRead[1]<<16) | ((uint16_t)bufferRead[2]<<8) | (bufferRead[3]))>>4;

  *data= HUMEDITY(Data_Humedity);

  return aht10_OK;
}


aht10_status_fnc aht10_get_temperature(aht10_config_t*obj, uint8_t *data)
{
  uint8_t bufferRead[6]={0,0,0,0,0,0};

  aht10_launch_measurement(obj);

  obj->readI2C(obj->addresSlave, bufferRead ,6);

  uint32_t Data_Temperature=((uint32_t)(bufferRead[3] & 0x0F)<<16) | ((uint16_t) bufferRead[4]<<8)| bufferRead[5];
  
  *data= TEMPERATURE(Data_Temperature);

  return aht10_OK;

}