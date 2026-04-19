#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>

// Each MQTT Control Packet contains a fixed header.
#define MQTT_HEADER_LEN 2 // 1 byte for the fixed header and 1 byte for the remaining length
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

struct mqtt_connect {
  union mqtt_header header;

  union {
    unsigned char byte;
    struct {
      int reserved : 1;
      unsigned clean_session : 1;
      unsigned will : 1;
      unsigned will_qos : 2;
      unsigned will_retain : 1;
      unsigned password : 1;
      unsigned username : 1;
    } bits;
  };

  struct {
    unsigned short keepalive;
    unsigned char *client_id;
    unsigned char *username;
    unsigned char *password;
    unsigned char *will_topic;
    unsigned char *will_message;
  } payload;
};

struct mqtt_connack {
  union mqtt_header header;

  union {
    unsigned char byte;
    struct {
      unsigned session_present : 1;
      unsigned reserved : 7;
    } bits;
  };
  unsigned char rc;
};

struct mqtt_subscribe {
  union mqtt_header header;
  unsigned short pkt_id;
  unsigned short tuples_len;
  struct {
    unsigned short topic_len;
    unsigned char *topic;
    unsigned qos;
  } *tuples;
};

struct mqtt_unsubscribe {
  union mqtt_header header;
  unsigned short pkt_id;
  unsigned short tuples_len;
  struct {
    unsigned short topic_len;
    unsigned char *topic;
  } *tuples;
};

struct mqtt_suback {
  union mqtt_header header;
  unsigned short pkt_id;
  unsigned short rcslen;
  unsigned char *rcs;
};

struct mqtt_publish {
  union mqtt_header header;
  unsigned short pkt_id;
  unsigned short topiclen;
  unsigned char *topic;
  unsigned short payloadlen;
  unsigned char *payload;
};

struct mqtt_ack {
  union mqtt_header header;
  unsigned short pkt_id;
};

typedef struct mqtt_ack mqtt_puback;
typedef struct mqtt_ack mqtt_pubrec;
typedef struct mqtt_ack mqtt_pubrel;
typedef struct mqtt_ack mqtt_pubcomp;
typedef struct mqtt_ack mqtt_unsuback;
typedef union mqtt_header mqtt_pingreq;
typedef union mqtt_header mqtt_pingres;
typedef union mqtt_header mqtt_disconnect;

union mqtt_packet {

  struct mqtt_ack ack;
  union mqtt_header header;

  struct mqtt_connect connect;
  struct mqtt_connack connack;

  struct mqtt_suback suback;
  struct mqtt_publish publish;

  struct mqtt_subscribe subscribe;
  struct mqtt_unsubscribe unsubscribe;
};

int mqtt_encode_length(unsigned char *, size_t);

unsigned long long mqtt_decode_length(const unsigned char **);

int unpack_mqtt_packet(const unsigned char *, union mqtt_packet *);

unsigned char *pack_mqtt_packet(const union mqtt_packet *, unsigned);

union mqtt_header *mqtt_packet_header(unsigned char);

struct mqtt_ack *mqtt_packet_ack(unsigned char, unsigned short);

struct mqtt_conack *mqtt_packet_connack(unsigned char, unsigned char, unsigned char);

struct mqtt_suback *mqtt_packet_suback(unsigned char, unsigned short, unsigned char *, unsigned short);

struct mqtt_publish *mqtt_packet_pubish(unsigned char, unsigned short, size_t, unsigned char *, size_t, unsigned char *);

void mqtt_packet_release(union mqtt_packet *, unsigned);

#endif