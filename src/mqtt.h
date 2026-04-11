#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>

// Each MQTT Control Packet contains a fixed header.
#define MQTT_HEADER_LEN = 2 // 1 byte for the fixed header and 1 byte for the remaining length
#define MQTT_ACK_LEN 4

#define CONNACK_BYTE 0x20  // 0010 0000
#define PUBLISH_BYTE 0x30  // 0011 0000
#define PUBACK_BYTE 0x40   // 0100 0000
#define PUBREC_BYTE 0x50   // 0101 0000
#define PUBREL_BYTE 0x60   // 0110 0000
#define PUBCOMP_BYTE 0x70  // 0111 0000
#define SUBACK_BYTE 0x90   // 1001 0000
#define UNSUBACK_BYTE 0xB0 // 1011 0000
#define PINGRESP_BYTE 0xD0 // 1101 0000

/*  Fixed Header Format:
                (7- 4)
    +--------+---------------+-------------+
    |        | MQTT Control  | Flags (3-0) |
    | Byte 1 | Packet Type   |             |
    |        +---------------+-------------+
    | Byte 2 | Remaining Length (1-4 bytes)|
    |--------+---------------+-------------+

*/

/*


*/

enum packet_type {
  // 4 bit unsigned value
  // Binary (0000 to 1111)  Decimal 0 to 15

  // 0 is reserved

  CONNECT = 1,      // Client request to connect to server
  CONNACK = 2,      // Connect acknowledgment from server to client
  PUBLISH = 3,      // Publish message (two ways: client to server or server to client)
  PUBACK = 4,       // Publish acknowledgment (two ways: client to server or
                    // server to client)
  PUBREC = 5,       // Publish received (two ways: client to server or server to client)
  PUBREL = 6,       // Publish release (two ways: client to server or server to client)
  PUBCOMP = 7,      // Publish complete (two ways: client to server or server to client)
  SUBSCRIBE = 8,    // Client subscribe to topics request
  SUBACK = 9,       // Subscribe acknowledgment from server to client
  UNSUBSCRIBE = 10, // Client unsubscribe request
  UNSUBACK = 11,    // Unsubscribe acknowledgment
  PINGREQ = 12,     // Client ping request
  PINGRESP = 13,    // Ping response from server to client
  DISCONNECT = 14,  // Client is disconnecting

  // 15 is reserved
};

enum qos_level {
  // 2 bit unsigned value
  // Binary (00 to 11) Decimal 0 to 3

  // 0 is reserved

  AT_MOST_ONCE = 1,  // QoS level 0: At most once delivery
  AT_LEAST_ONCE = 2, // QoS level 1: At least once delivery
  EXACTLY_ONCE = 3,  // QoS level 2: Exactly once delivery

};

union mqtt_header {
  unsigned char byte;

  struct {
    unsigned retain : 1;
    unsigned qos : 2;
    unsigned dup : 1;
    unsigned type : 4;
  } bits;
};

#endif MQTT_H