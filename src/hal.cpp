#include <stdio.h>
#include <iostream>

#include "hal.h"

/** Input pins. */
#define IGNITION_INPUT IN_0
#define LEVEL_INPUT IN_1
#define TEMP_INPUT IN_2
#define SUPPLY_VOLTAGE_INPUT IN_3

/** Output pins. */
#define PUMP_ENABLE_OUTPUT OUT_0
#define PUMP_IGNITION_OUTPUT OUT_1
#define FAN_ENABLE_OUTPUT OUT_2
#define FAN_PWM_OUTPUT OUT_3
#define DISPLAY_IGNITION_OUTPUT OUT_4

/**
 * @brief Constructor
 */
HardwareManager::HardwareManager() {
    _inputs.supplyVoltage = 0.0f;
    _inputs.ignitionClosed = false;
    _inputs.levelSwitchClosed = false;
    _inputs.temperature = 0.0f;

    _outputs.fanEnable = false;
    _outputs.fanPowerPercent = 0;
    _outputs.pumpEnable = false;
    _outputs.pumpIgnition = false;
    _outputs.pumpPowerPercent = 0;
    _outputs.displayState = {DRY, ""};
}

/**
 * @brief Initializes the finite state machine.
 */
void HardwareManager::initialize() {
    readPlcRegisters();
}

/**
 * @brief Retrieves the current status of the PLC inputs.
 * 
 * @param inputs Overwritten with the current PLC input status.
 */
void HardwareManager::retrieveInputs(PlcInputs_t &inputs) {
    readPlcRegisters();
    inputs = _inputs;
}

/**
 * @brief Set the given inputs to the internal variable
 * storing the PLC input registers
 * 
 * @note This function is only used for testing.
 * To improve: use a macro that removes this function definition
 * when not compiling for the test build, or a macro that removes
 * the "private" identifier while testing, so the _inputs variable
 * can be accessed directly.
 * 
 * @param inputs The inputs to set.
 */
void HardwareManager::setInputs(PlcInputs_t inputs) {
    _inputs = inputs;
}

/**
 * @brief Retrieves the current status of the PLC outputs.
 * 
 * @param inputs Overwritten with the current PLC output status.
 */
void HardwareManager::retrieveOutputs(PlcOutputs_t &outputs) {
    outputs = _outputs;
}

/**
 * @brief Sets the given outputs to the PLC output registers.
 * 
 * @param outputs The outputs to set.
 */
void HardwareManager::setOutputs(PlcOutputs_t outputs) {
    _outputs = outputs;
    writePlcRegisters();
}

/** 
 * @brief Forces the PLC to update the output signals
 * based on the current output registers.
 */
void HardwareManager::flushOutputs() {
    /** Call the underlying PLC driver code to set the output registers. */
}

/** 
 * @brief Receives a pointer to the next CAN message on the reception queue.
 * 
 * In a real application, there would be a seperate thread receiving all CAN
 * messages and putting them in a queue to be popped by the main thread.
 * This function would be called on a loop until a nullptr is returned. 
 * 
 * @param message Overwritten with a pointer to the message.
 */
void HardwareManager::receiveNextCanMessage(char *&message) {
    message = nullptr;
}

/**
 * @brief Reads the input register values from the 
 * underlying PLC driver, convers the values to the 
 * expected format, and populates the inputs variable.
 */
void HardwareManager::readPlcRegisters() {
    /**
     * This code is disabled to allow mocking of the input values in testing through setInputs.
     * In a real application, a code mocking framework would be used to mock
     * the return values we want in the test returned by the readRegister functions.
     * However, in this example I've disabled this so I can mock the inputs
     * easily by calling setInputs().
     */
    /**
     _inputs.supplyVoltage = readFloatPlcRegister(SUPPLY_VOLTAGE_INPUT);
     _inputs.ignitionClosed = readBooleanPlcRegister(IGNITION_INPUT);
     _inputs.levelSwitchClosed = readBooleanPlcRegister(LEVEL_INPUT);
     _inputs.temperature = readFloatPlcRegister(TEMP_INPUT);
     */
}

/**
 * @brief Converts the outputs variable to values
 * expected by the underlying PLC driver and
 * populates the output registers.
 */
void HardwareManager::writePlcRegisters() {
    /** 
     * Define mock data to write to the register.
     * In a real application, this would be converted from the
     * PlcOutputs_t variable into a type the PLC output expects.
     */
    char *mockData = nullptr;

    writePlcRegister(PUMP_ENABLE_OUTPUT, mockData);
    writePlcRegister(PUMP_IGNITION_OUTPUT, mockData);
    writePlcRegister(FAN_ENABLE_OUTPUT, mockData);
    writePlcRegister(FAN_PWM_OUTPUT, mockData);
    writePlcRegister(DISPLAY_IGNITION_OUTPUT, mockData);
}

/**
 * @brief Reads an input register via the underlying PLC driver. 
 * The register is expected to be a boolean.
 * 
 * @param address The register address. Can be IN0, IN1, ... IN11.
 * @returns The register value.
 */
bool HardwareManager::readBooleanPlcRegister(PlcInputRegisters_e address) {
    return false;
}

/**
 * @brief Reads an input register via the underlying PLC driver. 
 * The register is expected to be a float.
 * 
 * @param address The register address. Can be IN0, IN1, ... IN11.
 * @returns The register value.
 */
float HardwareManager::readFloatPlcRegister(PlcInputRegisters_e address) {
    return 0;
}

/**
 * @brief Writes to the output register via the underlying PLC driver.
 * 
 * @param address The register address. Can be OUT0, OUT1, ... OUT11.
 * @param value The register value.
 */
void HardwareManager::writePlcRegister(PlcOutputRegisters_e address, char* value) {
    /** Call the underlying PLC driver to set the output register. */

    /** Send any CAN messages necessary to update the pump and display state. */
    sendCanMessage(0U, 0U, nullptr);
}

/**
 * @brief Pushes a CAN message to the outgoing message queue.
 * 
 * In a real application, this function would be further abstracted behind
 * individual functions for sending each unique CAN message, eg. sendDisplayMessage().
 * Each message has its own different signals within the 8 bytes of data,
 * and each signal has its own offset and scale factor.
 * 
 * The code to pack these signals into the data is not included here.
 * 
 * @param id The ID of the CAN frame.
 * @param dlc The data length code.
 * @param message The message to send.
 */
void HardwareManager::sendCanMessage(uint16_t id, uint8_t dlc, char message[CAN_MESSAGE_LEN]) {
    /** Push the message onto a RTOS queue, to be sent in a seperate thread. */
}

