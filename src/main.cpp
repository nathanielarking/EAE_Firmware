#include <cstdlib>
#include <iostream>

#include "fsm.h"
#include "hal.h"
#include "controller.h"

extern "C" {
    extern float MIN_VOLTAGE;
    extern float TEMP_SETPOINT;
}

int main(int argc, char* argv[]) {
    /** Ensure the arguments were supplied. */
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <MIN_VOLTAGE> <TEMP_SETPOINT>" << std::endl;
        return 1;
    }

    /** Convert arguments to correct variable. */
    float minVoltage = atof(argv[1]);
    float tempSetpoint = atof(argv[2]);

    std::cout << "Minimum Voltage: " << minVoltage << std::endl;
    std::cout << "Temperature Setpoint: " << tempSetpoint << std::endl;

    /** Initialize classes. */
    Parameters_t params = {minVoltage, tempSetpoint};
    HardwareManager hal = HardwareManager();
    ControlManager controller = ControlManager(tempSetpoint);
    StateManager fsm = StateManager(params, &hal, &controller);

    /** Run the code. */
    fsm.initialize();
    while (true) {
        fsm.handleCurrentState();
    }

    return 0;
}