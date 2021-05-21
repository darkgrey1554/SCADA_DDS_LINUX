#pragma once
#include "enum_tcp.h"
#include <thread>

#define SIZEPACKINFOCLINET 38 
struct InfoClient
{
	//SOCKET socket;
	std::thread::id id;
	unsigned int IP;
	int port;

	int channel = -1;
	Source typesourse = Source::empty;
	int countAin = -1;
	int countDin = -1;
	int countBin = -1;
	int countAout = -1;
	int countDout = -1;
	int countBout = -1;
	int frequency = -1;
	status_cl status = status_cl::empty;

	void clear()
	{
		//socket = INVALID_SOCKET;
		IP = 0;
		port = 0;
		typesourse = Source::empty;
		channel = -1;
	}
};

/*struct KKSConcord
{
	int channel = -1;
	TypeData typedata;
	int indexdts = -1;
	char KKS[10];

};*/


struct InfoChannelForMemory
{
	int channel = -1;

	int countAin = 0;
	int countDin = 0;
	int countBin = 0;
	int countAout = 0;
	int countDout = 0;
	int countBout = 0;

	char status = 0;
};


struct PointBuf
{
	pthread_mutex_t* mutex;
	void* buf;
};

