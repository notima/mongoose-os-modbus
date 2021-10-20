#include "modbus.h"
#include <stdbool.h>

uint16_t mod_rtu_crc(char buf[], int len)
{
  uint16_t crc = 0xFFFF;
  
  for (int pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos];
  
    for (int i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
        crc >>= 1;
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
}

void modbus_get_raw_tcp_request(char* requestBuffer, struct modbus_tcp_request request) {
    requestBuffer[0] = request.mbap.transactionId >> 8;
    requestBuffer[1] = request.mbap.transactionId;
    requestBuffer[2] = request.mbap.protocolId >> 8;
    requestBuffer[3] = request.mbap.protocolId;
    requestBuffer[4] = request.mbap.messageLength >> 8;
    requestBuffer[5] = request.mbap.messageLength;
    requestBuffer[6] = request.mbap.unitId;

    requestBuffer[7] = request.pdu.functionCode;
    requestBuffer[8] = request.pdu.addrBegin >> 8;
    requestBuffer[9] = request.pdu.addrBegin;
    requestBuffer[10] = request.pdu.numOfRegisters >> 8;
    requestBuffer[11] = request.pdu.numOfRegisters;
}

void modbus_get_raw_rtu_request(char* requestBuffer, struct modbus_rtu_request request) {
    requestBuffer[0] = request.slaveAddress;
    requestBuffer[1] = request.pdu.functionCode;
    requestBuffer[2] = request.pdu.addrBegin >> 8;
    requestBuffer[3] = request.pdu.addrBegin;
    requestBuffer[4] = request.pdu.numOfRegisters >> 8;
    requestBuffer[5] = request.pdu.numOfRegisters;

    uint16_t crc = mod_rtu_crc(requestBuffer, 6);

    requestBuffer[6] = crc;
    requestBuffer[7] = crc >> 8;
}

struct modbus_pdu modbus_read_analog_input(uint16_t addrBegin, uint16_t numOfRegisters) {
    struct modbus_pdu pdu;
    pdu.functionCode = 4;
    pdu.addrBegin = addrBegin;
    pdu.numOfRegisters = numOfRegisters;
    return pdu;
}

struct modbus_response modbus_parse_response(char* raw) {
    struct modbus_response response;
    response.slaveAddress = raw[0];
    response.functionCode = raw[1];
    response.dataLength = raw[2];
    response.data = &raw[3];
    return response;
}

bool mgos_modbus_init(void) {
    return true;
}