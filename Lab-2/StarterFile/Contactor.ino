#include <stdlib.h>
#include <stdbool.h>
#include "Contactor.h"


void updateContactor(float* contactorStatus, const byte* contactorPin) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    digitalWrite(*contactorPin, (int)(*contactorStatus));
    Serial.println(*contactorStatus);
    return;
}

void contactorTask(void* cData) {
    /****************
        Function name:
        Function inputs:
        Function outputs:
        Function description:
        Author(s):
    *****************/
    ContactorData* data = (ContactorData*) cData;

    // Update all sensors
    updateContactor(data->contactorStatus, data->contactorPin);

    return;
}
