
#include <stdlib.h>
#include <stdint.h>
#pragma once


typedef struct
{
	uint64_t Time_Code;
	uint64_t RunTime_Code;
	uint32_t AsicID;
	uint32_t Pack_Id;
	uint32_t *Charge;
	uint32_t *CoarseTime;
	uint32_t *FineTime;
	uint32_t *Hit;
} t_ASIC_packet;


typedef struct
{
	t_ASIC_packet *packets;
	int allocated_packets;
	int valid_packets;
} t_ASIC_packet_collection;


typedef enum
{
	LOW_LEVEL_TCP = 0
} SOCKET_TYPE;


typedef enum
{
	REG_ACCESS = 0,
	STREAMING_BLOCKING = 1,
	STREAMING_NONBLOCKING = 2
} BUS_MODE;




typedef struct
{
	void *zmq_context;
	void *zmq_pullsocket;
	int zmq_connected;
	int recv_blocking;
}tZMQEndpoint;

typedef struct {
	int Csocket;
	int connected;
	uint32_t __IICBASEADDRESS;
	uint32_t __IICBASEADDRESS_STATUS;
	SOCKET_TYPE socketType;
	tZMQEndpoint *zmq;
} tR5560_Handle;
