#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 * @brief Responsible for updating the pump and fan's control signals with a PID loop.
 */
class ControlManager {
public:
    /**
     * @brief Constructor.
     */
    ControlManager(float temperatureSetpoint);

    /**
     * @brief Begins the controller.
     */
    void initialize();

    /**
     * @brief Updates the control signals given the current temperature.
     * 
     * @param temperature The current temperature.
     * @param fanPowerPercent Overwritten with the new fan control signal.
     * @param pumpPowerPercent Overwritten with the new pump control signal.
     */
    void process(float temperature, int &fanPowerPercent, int &pumpPowerPercent);

private:
    /** The setpoint temperature to be returned by the temp sensor. */
    float temperatureSetpoint;
};

#endif
