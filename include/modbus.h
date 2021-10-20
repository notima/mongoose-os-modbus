#include "stdint.h"
#include "stdbool.h"

/**
 * Protocol Data Unit
 * This structure contains the data of the 
 * modbus request.
 */
struct modbus_pdu {
    uint8_t functionCode;
    uint16_t addrBegin;
    uint16_t numOfRegisters;
};

/**
 * Modbus application header for modbus tcp
 * This structure conatains data that is 
 * specific to modbus tcp requests.
 */
struct modbus_tcp_mbap {
    uint16_t transactionId;
    uint16_t protocolId;
    uint16_t messageLength;
    uint8_t unitId;
};

/**
 * A complete request structure for modbus tcp
 */
struct modbus_tcp_request {
    struct modbus_tcp_mbap mbap;
    struct modbus_pdu pdu;
};

/**
 * A complete request structure for modbus rtu
 */
struct modbus_rtu_request {
    uint8_t slaveAddress;
    struct modbus_pdu pdu;
};

/**
 * A response structure compatible with both
 * modbus rtu and modbus tcp.
 */
struct modbus_response {
    bool valid;
    struct modbus_tcp_mbap mbap;
    uint8_t slaveAddress;
    uint8_t functionCode;
    uint8_t dataLength;
    char* data;
};

/**
 * Get the raw bytes of a modbus tcp request
 * @param requestBuffer Where to write the bytes.
 * Make sure enough memory is allocated at this 
 * address!
 * @param request The request
 */
void modbus_get_raw_tcp_request(char* requestBuffer, struct modbus_tcp_request request);

/**
 * Get the raw bytes of a modbus rtu request
 * @param requestBuffer Where to write the bytes.
 * Make sure enough memory is allocated at this 
 * address!
 * @param request The request
 */
void modbus_get_raw_rtu_request(char* requestBuffer, struct modbus_rtu_request request);

struct modbus_pdu modbus_read_discrete_output(uint16_t addrBegin, uint16_t numOfRegisters);

struct modbus_pdu modbus_read_discrete_input(uint16_t addrBegin, uint16_t numOfRegisters);

struct modbus_pdu modbus_read_analog_output(uint16_t addrBegin, uint16_t numOfRegisters);

struct modbus_pdu modbus_read_analog_input(uint16_t addrBegin, uint16_t numOfRegisters);

struct modbus_response modbus_parse_response(char* raw);