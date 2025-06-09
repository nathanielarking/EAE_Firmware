#include <iostream>
#include <stdio.h>

#include "fsm.h"

/**
 * @brief Initializes the finite state machine.
 */
void StateManager::initialize() {
    hal->initialize();
    controller->initialize();

    state = STATE_BOOT;
}

/**
 * @brief Executes the current state.
 */
void StateManager::handleCurrentState() {
    switch (state) {
        case STATE_BOOT:
            boot();
            break;
        case STATE_FATAL_ERROR:
            fatalError();
            break;
        case STATE_IDLE:
            idle();
            break;
        case STATE_IGNITION:
            ignition();
            break;
        case STATE_ACTIVE:
            active();
            break;
        default:
            std::cout << "State machine set to invalid state." << std::endl;
            state = STATE_FATAL_ERROR;
            break;
    }
}

/**
 * @brief Retrieves the current state.
 * 
 * @return FsmStates_e The current state.
 */
FsmStates_e StateManager::getState() {
    return state;
}

/**
 * @brief Handler for state STATE_BOOT.
 */
void StateManager::boot() {
    bool selfTestsOk = false;
    
    /** Perform any self-tests, checks, etc. */
    selfTestsOk = true;
    if (selfTestsOk == false) {
        std::cout << "Self-tests failed." << std::endl;
        state = STATE_FATAL_ERROR;
        return;
    }

    /** Go to idle state. */
    state = STATE_IDLE;
    std::cout << "Entering idle state." << std::endl;
    return;
}

/**
 * @brief Handler for state STATE_FATAL_ERROR.
 */
void StateManager::fatalError() {
    /** Log errors to console, attempt to output to display, ETC. */
}

/**
 * @brief Handler for state STATE_IDLE.
 */
void StateManager::idle() {
    PlcInputs_t inputs = {};
    PlcOutputs_t outputs = {};
    char *canMessage = nullptr;

    /** Retrieve PLC inputs and outputs. */
    hal->retrieveInputs(inputs);
    hal->retrieveOutputs(outputs);

    /** Handle new received CAN messages. */
    hal->receiveNextCanMessage(canMessage);
    while (canMessage != nullptr) {
        /** Handle message. */

        hal->receiveNextCanMessage(canMessage);
    }

    /** Enter ignition state upon ignition. */
    if (inputs.ignitionClosed == true) {
        std::cout << "Entering ignition state." << std::endl;
        state = STATE_IGNITION;
        return;
    }

    /** Update display state. */
    if (inputs.levelSwitchClosed == true) {
        outputs.displayState.coolantStatus = SUFFICIENT;
        snprintf(outputs.displayState.message, DISPLAY_MESSAGE_SIZE, "Ready for ignition.");
        
    } else {
        outputs.displayState.coolantStatus = DRY;
        snprintf(outputs.displayState.message, DISPLAY_MESSAGE_SIZE, "Coolant refill required.");
    }

    /** Set outputs. */
    hal->setOutputs(outputs);
    hal->flushOutputs();
}

/**
 * @brief Handler for state STATE_IGNITION.
 */
void StateManager::ignition() {
    PlcInputs_t inputs = {};
    PlcOutputs_t outputs = {};

    /** Retrieve PLC inputs and outputs. */
    hal->retrieveInputs(inputs);
    hal->retrieveOutputs(outputs);

    /** Pre-ignition guards. */
    /** Under-voltage. */
    if (inputs.supplyVoltage < params.minVoltage) {
        std::cout << "Supply voltage is less than the minimum of: " << params.minVoltage << std::endl;
        goto exit;
    }

    /** Coolant level. */
    if (inputs.levelSwitchClosed == false) {
        std::cout << "Coolant levels are not sufficient." << std::endl;
        goto exit;
    }

    /** Ignition switch. */
    if (inputs.ignitionClosed == false) {
        std::cout << "Ignition disabled." << std::endl;
        goto exit;
    }

    /** Activate the pump. Begin at 20%. */
    outputs.pumpEnable = true;
    outputs.pumpIgnition = true;
    outputs.pumpPowerPercent = 20;
    hal->setOutputs(outputs);
    hal->flushOutputs();
    /** 
     * Implement a delay here, to avoid stacking the pump
     * and the fan's inrush current.
     */


    /** Activate the fan. Begin at 20%. */
    outputs.fanEnable = true;
    outputs.fanPowerPercent = 20;
    hal->setOutputs(outputs);
    hal->flushOutputs();

    std::cout << "Entering active state." << std::endl;
    state = STATE_ACTIVE;
    return;

exit:
    /** Disable the equipment. */
    outputs.fanPowerPercent = 0;
    outputs.pumpPowerPercent = 0;
    hal->setOutputs(outputs);
    hal->flushOutputs();

    std::cout << "Entering idle state." << std::endl;
    state = STATE_IDLE;
    return;
}

/**
 * @brief Handler for state STATE_ACTIVE.
 */
void StateManager::active() {
    PlcInputs_t inputs = {};
    PlcOutputs_t outputs = {};
    char *canMessage = nullptr;

    /** Retrieve PLC inputs and outputs. */
    hal->retrieveInputs(inputs);
    hal->retrieveOutputs(outputs);

    /** Handle new received CAN messages. */
    hal->receiveNextCanMessage(canMessage);
    while (canMessage != nullptr) {
        /** Handle message. */

        hal->receiveNextCanMessage(canMessage);
    }

    /** Guards. */
    /** Under-voltage. */
    if (inputs.supplyVoltage < params.minVoltage) {
        std::cout << "Supply voltage is less than the minimum of: " << params.minVoltage << std::endl;
        goto exit;
    }

    /** Coolant level. */
    if (inputs.levelSwitchClosed == false) {
        std::cout << "Coolant levels are not sufficient." << std::endl;
        outputs.displayState.coolantStatus = DRY;
        goto exit;
    }

    /** Ignition switch. */
    if (inputs.ignitionClosed == false) {
        std::cout << "Ignition disabled." << std::endl;
        goto exit;
    }

    /** Update the controller. */
    controller->process(inputs.temperature, outputs.fanPowerPercent, outputs.pumpPowerPercent);

    /** Update the outputs. */
    hal->setOutputs(outputs);
    hal->flushOutputs();
    
    return;

exit:
    /** Disable the equipment. */
    outputs.fanPowerPercent = 0;
    outputs.pumpPowerPercent = 0;
    hal->setOutputs(outputs);
    hal->flushOutputs();

    std::cout << "Entering idle state." << std::endl;
    state = STATE_IDLE;
    return;
}