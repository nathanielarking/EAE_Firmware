#ifndef FSM_H
#define FSM_H

#include "hal.h"
#include "controller.h"

/**
 * @brief Describes possible finite-state-machine states.
*/
typedef enum FsmStates_e {
    STATE_MIN,

    /** Initialization. */
    STATE_BOOT,
    /** Unrecoverable error. */
    STATE_FATAL_ERROR,
    /** Main idle state. */
    STATE_IDLE,
    /** Startup state. */
    STATE_IGNITION,
    /** Main process state. */
    STATE_ACTIVE,

    STATE_MAX
} FsmStates_e;

/**
 * @brief The parameters passed into the program via command line.
 */
typedef struct Parameters_t {
    /** The minimum supply voltage that will be tolerated. */
    float minVoltage;
    /** The target temperature for the temperature sensor. */
    float temperatureSetpoint;
} Parameters_t;

/**
 * @brief Defines main application routines and transistions between states.
 */
class StateManager {
public:
    /**
     * @brief Constructor.
     */
    StateManager(Parameters_t params, HardwareManager *hal, ControlManager *controller)
        : params(params), hal(hal), controller(controller) {}

    /**
     * @brief Begins the finite state machine.
     */
    void initialize();

    /**
     * @brief Executes the current state.
     */
    void handleCurrentState();

    /**
     * @brief Retrieves the current state.
     * 
     * @return FsmStates_e The current state.
     */
    FsmStates_e getState();
    
private:
    Parameters_t params;

    /** Current state. */
    FsmStates_e state;

    /** Manager classes. */
    HardwareManager *hal;
    ControlManager *controller;

    /**
     * @brief Handler for state STATE_BOOT.
     */
    void boot();

    /**
     * @brief Handler for state STATE_FATAL_ERROR.
     */
    void fatalError();

    /**
     * @brief Handler for state STATE_IDLE.
     */
    void idle();

    /**
     * @brief Handler for state STATE_IGNITION.
     */
    void ignition();

    /**
     * @brief Handler for state STATE_ACTIVE.
     */
    void active();

};

#endif