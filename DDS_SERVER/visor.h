#pragma once

#ifdef _WIN32
#include "tcpserver.h"
#include <windows.h>
#endif // 

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <aio.h> 
#include <errno.h>
#include "tcpclient.h"
#include "dts_client.h"
#endif // __linux__


#include <thread>
#include <iostream>
#include "logger.h"
#include "enum_tcp.h"
#include <atomic>

#define NameMutexConcordKKSIn "Global\\Mutex_ConcordKKSIn"
#define NameMutexConcordKKSOut "Global\\Mutex_ConcordKKSOut"
#define NameMemoryConcordKKSIn "Global\\Memory_ConcordKKSIn"
#define NameMemoryConcordKKSOut "Global\\Memory_ConcordKKSOut"

extern volatile std::atomic<short> global_command;
extern volatile short it;


class VISOR
{
	std::string IP; //адрес потока прослушивания
	int Port; // порт потока прослушивания
	char* buf_send = NULL; // буфер отпавки данных
	char* buf_recv = NULL; // буфер приема данных
	LoggerSpace::Logger* log;

	std::atomic<short> end_work;

	TypeVisor t_visor;

    #ifdef _WIN32
	TCPServer* server = NULL;	
	SOCKET sock;
    #endif

    #ifdef __linux__
	int sock;

	aiocb aiobufsend;
	aiocb aiobufrecv;
	sigevent sigrecv;
	sigevent sigsend;
	aiocb* aio_point;
	int last_error = 0;
	TCPClient* client = NULL;
    #endif // __linux__


	std::thread answer;

	void fun_thread_answer();
	unsigned recv();
	unsigned sendanswer();

	unsigned int HandingRequest();
	unsigned int FuncInaccessible();

	//unsigned int ReadInfoListChannels();
	//unsigned int ReadListMatchKKSOut();
	//unsigned int ReadListMatchKKSIn();
	//unsigned int Stop_Server();
	//unsigned int Start_Server();
	//unsigned int GetStatusServer();

	unsigned int GetStatusClient();
	unsigned int Start_Client();
	unsigned int Stop_Client();
	unsigned int GetConfigClient();
	unsigned int RestartClient();
	unsigned int RebornClient();
	unsigned int ShutDownClient();
	unsigned int SetIPClient();
	unsigned int SetPortClient();
	unsigned int SetTypeClient();
	unsigned int SetChannelClient();
	unsigned int SetFrequencyClient();
	unsigned int GetListKKSOut();
	unsigned int GetListKKSIn();

public:

	VISOR(std::string ip, int port, TypeVisor type);
	~VISOR();
	
#ifdef _WIN32
	void InitTCPServer(TCPServer* ser);
#endif // _WIN32

#ifdef __linux__
	void InitTCPClient(TCPClient* cl);
#endif // __linux__


};