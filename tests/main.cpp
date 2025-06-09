#include <gtest/gtest.h>
#include <iostream>

#include "fsm.h"
#include "hal.h"
#include "controller.h"

/**
 * @brief Ensures that, after initialization, the FSM enters the STATE_BOOT state.
 */
TEST(FsmTests, EntersBootState)
{
    FsmStates_e state = STATE_MIN;

    /** Arrange. */
    Parameters_t params = {20.0f, 20.0f};
    HardwareManager hal = HardwareManager();
    ControlManager controller = ControlManager(params.temperatureSetpoint);
    StateManager fsm = StateManager(params, &hal, &controller);

    /** Act. */
    fsm.initialize();
    
    /** Assert. */
    state = fsm.getState();
    EXPECT_EQ(state, STATE_BOOT);
}

/**
 * @brief Ensures that, after initialization and boot, the FSM enters the STATE_IDLE state.
 */
TEST(FsmTests, EntersIdleState)
{
    FsmStates_e state = STATE_MIN;

    /** Arrange. */
    Parameters_t params = {20.0f, 20.0f};
    HardwareManager hal = HardwareManager();
    ControlManager controller = ControlManager(params.temperatureSetpoint);
    StateManager fsm = StateManager(params, &hal, &controller);

    /** Act. */
    fsm.initialize();
    fsm.handleCurrentState();

    
    /** Assert. */
    state = fsm.getState();
    EXPECT_EQ(state, STATE_IDLE);
}

/**
 * @brief Ensures that, after initialization and boot, when the ignition is triggered, 
 * the FSM enters the STATE_IGNITION state.
 */
TEST(FsmTests, EntersIgnitionState)
{
    FsmStates_e state = STATE_MIN;
    PlcInputs_t inputs = {};

    /** Arrange. */
    Parameters_t params = {20.0f, 20.0f};
    HardwareManager hal = HardwareManager();
    ControlManager controller = ControlManager(params.temperatureSetpoint);
    StateManager fsm = StateManager(params, &hal, &controller);

    /** Act. */
    fsm.initialize();
    fsm.handleCurrentState();

    hal.retrieveInputs(inputs);
    inputs.ignitionClosed = true;
    hal.setInputs(inputs);
    
    fsm.handleCurrentState();
    
    /** Assert. */
    state = fsm.getState();
    EXPECT_EQ(state, STATE_IGNITION);
}

/**
 * @brief Ensures that, after initialization and boot, when the ignition is triggered, 
 * and the supply voltage is below the minimum, the FSM enters the STATE_IGNITION state
 * and then goes back to the IDLE state.
 */
TEST(FsmTests, ExitsIgnitionStateOnLowSupplyVoltage)
{
    FsmStates_e state = STATE_MIN;
    PlcInputs_t inputs = {};
    float supplyVoltageThreshold = 20.0f;

    /** Arrange. */
    Parameters_t params = {supplyVoltageThreshold, 20.0f};
    HardwareManager hal = HardwareManager();
    ControlManager controller = ControlManager(params.temperatureSetpoint);
    StateManager fsm = StateManager(params, &hal, &controller);

    /** Act. */
    fsm.initialize();
    fsm.handleCurrentState();

    hal.retrieveInputs(inputs);
    inputs.ignitionClosed = true;
    inputs.supplyVoltage = supplyVoltageThreshold - 1;
    hal.setInputs(inputs);
    
    fsm.handleCurrentState();
    fsm.handleCurrentState();
    
    /** Assert. */
    state = fsm.getState();
    EXPECT_EQ(state, STATE_IDLE);
}

/**
 * @brief Ensures that, after initialization and boot, when the ignition is triggered, 
 * and the level switch is not triggered, the FSM enters the STATE_IGNITION state
 * and then goes back to the IDLE state.
 */
TEST(FsmTests, ExitsIgnitionStateOnLevelSwitchOpen)
{
    FsmStates_e state = STATE_MIN;
    PlcInputs_t inputs = {};
    float supplyVoltageThreshold = 20.0f;

    /** Arrange. */
    Parameters_t params = {supplyVoltageThreshold, 20.0f};
    HardwareManager hal = HardwareManager();
    ControlManager controller = ControlManager(params.temperatureSetpoint);
    StateManager fsm = StateManager(params, &hal, &controller);

    /** Act. */
    fsm.initialize();
    fsm.handleCurrentState();

    hal.retrieveInputs(inputs);
    inputs.ignitionClosed = true;
    inputs.supplyVoltage = supplyVoltageThreshold + 1;
    inputs.levelSwitchClosed = false;
    hal.setInputs(inputs);
    
    fsm.handleCurrentState();
    fsm.handleCurrentState();
    
    /** Assert. */
    state = fsm.getState();
    EXPECT_EQ(state, STATE_IDLE);
}

int main(int argc, char **argv) {
    // Initialize the GoogleTest framework with command-line arguments
    ::testing::InitGoogleTest(&argc, argv);

    // Run all the tests that have been defined and return the result
    return RUN_ALL_TESTS();
}