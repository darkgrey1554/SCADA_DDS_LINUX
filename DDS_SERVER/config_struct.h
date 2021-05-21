#pragma once

#include <iostream>
#include "logger.h"
#include "enum_tcp.h"

#define NAMECONFIG "Config.txt"
#define NAMECONFIGCLIENT "/etc/Gate_EMT_DTS/Config.txt"

struct ConfigTCPServer
{
	std::string IP = "192.168.128.50";
	int Port = 38000;
	
	char result = 0;

	int ReadConfig();
};

struct ConfigLoger
{

	std::string DefLogName = "logGate";
	std::string DefSysLogName = "logGate";
	LoggerSpace::LogMode DefLogMode = LoggerSpace::LogMode::INFO;
	LoggerSpace::Status DefStatusLog = LoggerSpace::Status::ON;
	LoggerSpace::Status DefStatusSysLog = LoggerSpace::Status::OFF;
	unsigned int  DefSizeLogFile = 10240;

	char result = 0;

	int ReadConfig();

};

struct ConfigTCPClient
{
	std::string IP = "192.168.12.50";
	int Port = 22000;
	int channel = -1;
	Source typesource = Source::main;
	int f_hz = -1;

	char result = 0;

	int ReadConfig();
};

struct ConfigVISOR
{
	std::string IP = "192.168.128.50";
	int Port = 40001;

	char result = 0;

	int ReadConfig();
};