#pragma once
#include <string.h>

#define MAX_PACKET_SIZE 1000000
#define PACKET_DATA_LEN 1024

enum PacketTypes {

	INIT_CONNECTION = 0,

	ACTION_EVENT = 1,

	COMMUNICATION = 2,

	JOIN_GAME = 3,
};

struct Packet {

	unsigned int packet_type;
	unsigned int id;
	char packet_data[PACKET_DATA_LEN];

	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};