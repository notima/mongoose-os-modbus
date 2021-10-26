# MODBUS library for Mongoose OS

This library contains data structures and helper functions for creating modbus requests and parsing responses. The library does not take care of communication with modbus devices. It simply serializes and parses data.

## Example usage

Create a modbus rtu request:

    struct modbus_rtu_request request;
    request.slaveAddress = 0x01;        // Request a value from slave 1
    request.pdu.functionCode = 0x04;    // Read Input Registers (FC=04)
    request.pdu.addrBegin = 0x56;       // Start reading at address 86
    request.pdu.numOfRegisters = 0x02;  // Read 2 registers (4 bytes)

Alternatively, The `modbus_read_analog_input` function can be used to create the pdu (protocol data unit):

    struct modbus_rtu_request request;
    request.slaveAddress = 0x01;        // Request a value from slave 1
    request.pdu = modbus_read_analog_input(0x56, 0x02);

The following pdu creation methods exists:

    struct modbus_pdu modbus_read_discrete_output(uint16_t addrBegin, uint16_t numOfRegisters); // FC = 01
    struct modbus_pdu modbus_read_discrete_input(uint16_t addrBegin, uint16_t numOfRegisters);  // FC = 02
    struct modbus_pdu modbus_read_analog_output(uint16_t addrBegin, uint16_t numOfRegisters);   // FC = 03
    struct modbus_pdu modbus_read_analog_input(uint16_t addrBegin, uint16_t numOfRegisters);    // FC = 04

Get the bytes to transmit to a modbus device:

    char requestBuffer[8];
    modbus_get_raw_rtu_request(requestBuffer, request);

The content of `requestBuffer` will be `01 04 00 56 00 02 91 DB`.

 - `01`: Slave address (`0x01` = 1)
 - `04`: Function code (`0x04` = 4)
 - `0056`: Data address of the first register to read (`0x0056` = 86)
 - `0002`: How many registers to read (`0x0002` = 2)
 - `91DB`: Cyclic redundancy checksum (generated automatically)

