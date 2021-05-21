#pragma once


#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "enum_tcp.h"
#include "logger.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <aio.h> 
#include <errno.h>

#define TIMEDEV 25

int conver_IPstr_int(const char* str);

struct KKSConcord
{
	int channel = -1;
	TypeData typedata;
	int indexdts = -1;
	char KKS[10];
};

struct KKS_VALUE
{
	char KKS[10];
	TypeData type;
	int index;
};

class TCPClient
{
	std::string IP;
	int Port;
	int channel;
	Source TypeDevice;
	std::thread thread_transfer;

	LoggerSpace::Logger* log;

	sockaddr_in addr_server;
	int connect_socket=-1;
	aiocb aiobufsend;
	aiocb aiobufrecv;
	sigevent sigrecv;
	sigevent sigsend;
	aiocb* aio_point;
	int last_error = 0;

	float* bufAin = NULL;
	int* bufDin = NULL;
	char* bufBin = NULL;
	float* bufAout = NULL;
	int* bufDout = NULL;
	char* bufBout = NULL;

	std::vector<KKSConcord> VectorKKSIn;
	std::vector<KKSConcord> VectorKKSOut;

	pthread_mutex_t* mutexAin = 0;
	pthread_mutex_t* mutexDin = 0;
	pthread_mutex_t* mutexBin = 0;
	pthread_mutex_t* mutexAout = 0;
	pthread_mutex_t* mutexDout = 0;
	pthread_mutex_t* mutexBout = 0;
	
	int countAin = -1;
	int countDin = -1;
	int countBin = -1;
	int countAout = -1;
	int countDout = -1;
	int countBout = -1;

	char* buf_recv = NULL;
	char* buf_send = NULL;

	int frequency = 0;

	control_signal ctrl_signal = control_signal::stop;
	status current_status = status::close;
	
	char* status_update_data_in = NULL;
	pthread_mutex_t* mutex_status_update_data_in = 0;
	
	int fun_thread_transfer();
	
	
	unsigned int recv();
	unsigned int send();
	unsigned int createpacketconfiginfo();
	unsigned int createpacketKKSOut();
	unsigned int createpacketKKSIn();
	unsigned int createpackettransferdata(TypeData TY);
	unsigned int createpacketrequest(TypeData TY);
	unsigned int readlistKKSOut();
	unsigned int readlistKKSIn();
	unsigned int ConnectWithServer();
	unsigned int parsingpacketreceivedata();
	unsigned int parsingpacketrconfigdata();
	char GetStatusUpDataIn();

public:
	TCPClient(std::string IP, int Port, int Channel, Source type, int fr);
	~TCPClient();
	unsigned int InitBufer(TypeData TD, TypeValue TV, void* buf, int size, pthread_mutex_t* mut);
	void InitValueStatusUpDataIn(char* status, pthread_mutex_t* mut);
	unsigned int start_client();
	unsigned int stop_client();
	status getstatus();
	int getport();
	std::string getIP();
	int getchannel();
	Source gettype();
	int getfrequency();
	void setport(int port);
	void setIP(std::string ip);
	void setchannel(int ch);
	void settype(Source typ);
	void setfrequency(int f_hz);
	void getlistKKSIn(std::vector<KKS_VALUE>* kks_list);
	void getlistKKSOut(std::vector<KKS_VALUE>* kks_list);
};


