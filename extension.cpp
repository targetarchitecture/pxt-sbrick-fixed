#include "pxt.h"
#include "SbrickControlService.h"

namespace bluetooth {
    SbrickControlService* _pService = NULL;
    Action _handler;

    void updateTemperature() {
        while (NULL != _pService) {
            // run action that updates the temperature
            pxt::runAction0(_handler);
            // raise event to trigger notification
            MicroBitEvent ev(MICROBIT_ID_SENSOR_TEMPERATURE, MICROBIT_THERMOMETER_EVT_UPDATE);
            // wait period
            fiber_sleep(_pService->getPeriod());            
        }
    }

    /**
    * Starts a custom sensor service. The handler must call ``setSensorTemperature`` 
    * to update the temperature sent to the service.
    */
    //% blockId=sbrick_startSbrickControlService
    //% block="SBrick control service"
    void startSbrickControlService(Action handler) {
        if (NULL != _pService) return;

        _pService = new SbrickControlService(*uBit.ble);
        _handler = handler;
        pxt::incr(_handler);
        create_fiber(updateTemperature);
    }

    /**
    * Sets the current temperature value on the external temperature sensor
    */
    //% blockId=sbrick_setTemperatureSensorValue
    //% block="Dummy block"
    void setTemperatureSensorValue(int temperature) {
        if (NULL == _pService) return;

        _pService->setTemperature(temperature);
    }
}
