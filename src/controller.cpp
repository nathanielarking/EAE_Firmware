#include "controller.h"

/**
 * @brief Constructor.
 */
ControlManager::ControlManager(float temperatureSetpoint) {
    temperatureSetpoint = temperatureSetpoint;
}

/**
 * @brief Begins the controller.
 */
void ControlManager::initialize() {}

/**
 * @brief Updates the control signals with the new temperature.
 * 
 * @param temperature The current temperature.
 * @param fanPowerPercent Overwritten with the new fan control signal.
 * @param pumpPowerPercent Overwritten with the new pump control signal.
 */
void ControlManager::process(float temperature, int &fanPowerPercent, int &pumpPowerPercent) {
    /** Placeholder for real PID logic. */
    /** In a real application, would call an underlying PID library to run the feedback. */
    fanPowerPercent = 80;
    pumpPowerPercent = 80;
    return;
}
