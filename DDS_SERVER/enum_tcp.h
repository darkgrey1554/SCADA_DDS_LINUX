#pragma once

#include <iostream>

#define NameMutKKSInPut "Global\\mutex_KKSInPut"
#define NameMutKKSOutPut "Global\\mutex_KKSOutPut"
#define NameMemoryKKSInPut "Global\\MemoryListKKSInPut"
#define NameMemoryKKSOutPut "Global\\MemoryListKKSOutPut"

#define NameStatusMemoryGate "Global\\Status_gate_map"
#define NameMutStatusMemoryGate "Global\\Status_gate_mut"

#define NameMutMemoryInfoChannels "Global\\mutex_MemoryInfoChannels"
#define NameMemoryInfoChannels "Global\\MemoryInfoChannels"

#define NameFileListKKSOut "/etc/Gate_EMT_DTS/List_KKS_Out.txt"
#define NameFileListKKSIn "/etc/Gate_EMT_DTS/List_KKS_In.txt"


enum class command
{
	error = -1,
	configgate = 1,
};

enum class status
{
	work,
	err_init,
	err_listening,
	close,
	stop
};

enum class control_signal
{
	start = 1,
	stop = 0,
};

enum class status_cl
{
	start,
	active,
	pasive,
	stop,
	disconnect,
	empty
};

enum class Source
{
	main = 1,
	reserve = 2,
	empty = 0
};

enum class Status_proc_pack
{
	Error=0,
	OK=1,
	WAIT = 2
};

enum class TypeData
{
	Analog = 2,
	Discrete = 1,
	Binar = 0
};

enum class TypeValue
{
	INPUT = 1,
	OUTPUT = 0
};

std::string outenumstr(Source m);

enum class KKSroute
{
	INPUT = 1,
	OUTPUT = 0
};

enum class ComamndEmt
{
	UpdateKKSListOut = 1,
	UpdateKKSListIn = 2,
	UpdateListChannel = 4
};

enum class CommandMemory
{
	UpdateKKSListOut = 1,
	UpdateKKSListIn = 2,
	UpdateListChannel = 4
};

enum class Status_PPD
{
	OK = 0,
	Error = 1
};

enum class TypeVisor
{
	Server = 0,
	Client = 1
};

enum class ListCommandManager
{
	Error = 0,
	GetInfoConnectionChannels = 2,
	GetListMatchingKKSOut = 3,
	GetListMatchingKKSIn = 4,
	GetStatusServer = 5,
	StartServer = 6,
	StopServer = 7,
	GetConfigInfoServer = 8,
	RestartServer = 9,
	RebornServer = 10,
	ShutDownServer = 11,
	SetIPServer = 12,
	SetPortServer = 13,


	StartClient = 50,
	StopClient = 51,
	GetStatusClient = 52,
	GetConfigInfoClient = 53,
	RestartClient = 54,
	ShutDownClient = 55,
	SetIPClient = 56,
	SetPortClient = 57,
	SetTypeClient = 58,
	RebornClient = 59,
	GetListKKSOut = 60,
	GetListKKSIn = 61,
	SetChannelClient = 62,
	SetFrequencyClient = 63
};

enum class ResultRequestManager
{
	Ok = 0,
	Inaccessible = 1,
	Error = 2
};