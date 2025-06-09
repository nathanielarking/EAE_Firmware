#ifndef HAL_H
#define HAL_H

#include <cstdint>

/** Maximum number of characters to support in a display message. */
#define DISPLAY_MESSAGE_SIZE 255

/** Eight bytes is the size of the data in a CAN frame. */
#define CAN_MESSAGE_LEN 8

/**
 * @brief Describes the current status of the coolant in the system,
 * as read by the level sensor.
 */
typedef enum CoolantStatus_e {
    /** There is enough coolant in the system to run. */
    SUFFICIENT,
    /** There is not enough coolant in the system to run. */
    DRY
} CoolantStatus_e;

/**
 * @brief The information that is shown on the display.
 */
typedef struct DisplayState_t {
    /** The coolant status. */
    CoolantStatus_e coolantStatus;
    /** Optional message. */
    char message[DISPLAY_MESSAGE_SIZE];
} DisplayState_t;

/**
 * @brief Stores the relevant information about the PLC input state
 * after being read from the registers and converted into process variables. 
 */
typedef struct PlcInputs_t {
    /** The voltage the PLC is being supplied with. */
    float supplyVoltage;
    /** If true, the ignition switch connected to the PLC is closed. */
    bool ignitionClosed;
    /** If true, the level switch connected to the PLC is closed. */
    bool levelSwitchClosed;
    /** The temperature read by the temperature sensor connected to the PLC. */
    float temperature;
} PlcInputs_t;

/**
 * @brief Stores the relevant information about the PLC output state
 * before being used to update the output registers.
 */
typedef struct PlcOutputs_t {
    /** Whether power is provided to the fan. */
    bool fanEnable;
    /** The percent of the fan's maximum power to target, ie. the duty cycle. */
    int fanPowerPercent;
    /** Whether power is provided to the pump. */
    bool pumpEnable;
    /** Whether the pump ignition input is pulled high. */
    bool pumpIgnition;
    /** The percent of the pump's maximum power to target, ie. the duty cycle. */
    int pumpPowerPercent;
    /** The information used to update the display. */
    DisplayState_t displayState;
} PlcOutputs_t;

/** 
 * @brief Describes the possible input registers in the PLC.
 */
typedef enum PlcInputRegisters_e {
    IN_0,
    IN_1,
    IN_2,
    IN_3,
    IN_4,
    IN_5,
    IN_6,
    IN_7,
    IN_8,
    IN_9,
    IN_10,
    IN_11,
} PlcInputRegisters_e;

/** 
 * @brief Describes the possible output registers in the PLC.
 */
typedef enum PlcOutputRegisters_e {
    OUT_0,
    OUT_1,
    OUT_2,
    OUT_3,
    OUT_4,
    OUT_5,
    OUT_6,
    OUT_7,
    OUT_8,
    OUT_9,
    OUT_10,
    OUT_11,
} PlcOutputRegisters_e;

/**
 * @brief Defines an abstract interface for reading/writing to the PLCs input/output registers.
 */
class HardwareManager {
public:
    /**
     * @brief Constructor.
     */
    HardwareManager();

    /**
     * @brief Begins the HAL.
     */
    void initialize();

    /**
     * @brief Retrieves the current status of the PLC inputs.
     * 
     * @param inputs Overwritten with the current PLC input status.
     */
    void retrieveInputs(PlcInputs_t &inputs);

    /**
     * @brief Set the given inputs to the internal variable
     * storing the PLC input registers
     * 
     * @note This function is only used for testing.
     * To improve: use a macro that removes this function definition
     * when not compiling for the test build.
     * 
     * @param inputs The inputs to set.
     */
    void setInputs(PlcInputs_t inputs);

    /**
     * @brief Retrieves the current status of the PLC outputs.
     * 
     * @param inputs Overwritten with the current PLC output status.
     */
    void retrieveOutputs(PlcOutputs_t &outputs);

    /**
     * @brief Sets the given outputs to the PLC output registers.
     * 
     * @param outputs The outputs to set.
     */
    void setOutputs(PlcOutputs_t outputs);

    /** 
     * @brief Forces the PLC to update the output signals
     * based on the current output registers.
     */
    void flushOutputs();

    /** 
     * @brief Receives a pointer to the next CAN message on the reception queue.
     * 
     * In a real application, there would be a seperate thread receiving all CAN
     * messages and putting them in a queue to be popped by the main thread.
     * This function would be called on a loop until a nullptr is returned. 
     * 
     * @param message Overwritten with a pointer to the message.
     */
    void receiveNextCanMessage(char *&message);
    
private:
    /** Current state. */
    PlcInputs_t _inputs;
    PlcOutputs_t _outputs;

    /**
     * @brief Reads the input register values from the 
     * underlying PLC driver, convers the values to the 
     * expected format, and populates the inputs variable.
     */
    void readPlcRegisters();

    /**
     * @brief Converts the outputs variable to values
     * expected by the underlying PLC driver and
     * populates the output registers.
     */
    void writePlcRegisters();

    /**
     * @brief Reads an input register via the underlying PLC driver. 
     * The register is expected to be a boolean.
     * 
     * @param address The register address. Can be IN0, IN1, ... IN11.
     * @returns The register value.
     */
    bool readBooleanPlcRegister(PlcInputRegisters_e address);

    /**
     * @brief Reads an input register via the underlying PLC driver. 
     * The register is expected to be a float.
     * 
     * @param address The register address. Can be IN0, IN1, ... IN11.
     * @returns The register value.
     */
    float readFloatPlcRegister(PlcInputRegisters_e address);

    /**
     * @brief Writes to the output register via the underlying PLC driver.
     * 
     * @param address The register address. Can be OUT0, OUT1, ... OUT11.
     * @param value The register value.
     */
    void writePlcRegister(PlcOutputRegisters_e address, char* value);

    /**
     * @brief Pushes a CAN message to the outgoing message queue.
     * 
     * In a real application, this function would be further abstracted behind
     * individual functions for sending each unique CAN message.
     * Each message has its own different signals within the 8 bytes of data,
     * and each signal has its own offset and scale factor.
     * 
     * The code to pack these signals into the data is not included here.
     * 
     * @param id The ID of the CAN frame.
     * @param dlc The data length code.
     * @param message The message to send.
     */
    void sendCanMessage(uint16_t id, uint8_t dlc, char message[CAN_MESSAGE_LEN]);
};

#endif