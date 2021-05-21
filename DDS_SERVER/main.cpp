#include <cstdio>
#include "logger.h"
#include <pthread.h>
#include <mutex>
#include <iostream>
#include "config_struct.h"
#include "tcpclient.h"
#include "visor.h"
#include "daemon_init.h"
#include <sys/file.h>


#define CURRENT_DIR "/usr/local/bin/Gate_EMT_DDS/"
#define block_file_name "/usr/local/bin/Gate_EMT_DDS/pid.txt"
#define PATH_REBORN "./usr/local/bin/Gate_EMT_DDS/Gate_EMT_DDS.out"


pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t* mut2 = &mut;


int main(int argc, char* argv[])
{

    /// --- блоктровка повторного запуска канала --- /// 

    global_command.store(0, std::memory_order::memory_order_relaxed);

    int block_file = 0;
    int result_pid = 0;
    char error_block_file = 0;
    int sys_error_block_file = 0;
    block_file = open(block_file_name, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (block_file == -1)
    {
        error_block_file = 1;
        sys_error_block_file = errno;
    }

    if (block_file != -1)
    {
        result_pid = flock(block_file, LOCK_EX | LOCK_NB);
        if (result_pid != 0)
        {
            error_block_file = 2;
            sys_error_block_file = errno;
        }
    }

    ////////////////////////////////////////////////////


    unsigned int result_daemon = 0;
    int result_exec = 0;

    result_daemon = InitDaemon();

    chdir(CURRENT_DIR);


    /*float bufAin[100]; for (int i = 0; i < 100; i++) bufAin[i] = i;
    int bufDin[100]; for (int i = 0; i < 100; i++) bufDin[i] = i;
    char bufBin[100]; for (int i = 0; i < 100; i++) bufBin[i] = 0;

    float bufAout[100]; for (int i = 0; i < 100; i++) bufAout[i] = 0;
    int bufDout[100]; for (int i = 0; i < 100; i++) bufDout[i] = 0;
    char bufBout[100]; for (int i = 0; i < 100; i++) bufBout[i] = 0;

    char statusin = 0;

    pthread_mutex_t mutex_stat = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_t mutex_Ain = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_Din = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_Bin = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_Aout = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_Dout = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex_Bout = PTHREAD_MUTEX_INITIALIZER;*/

    ConfigTCPClient conf_client;
    ConfigLoger conf_log;
    ConfigVISOR conf_visor;
    unsigned int result = 0;
    std::string messeng;
    PointBuf pbuf;
    int size_buf;
    int res2 = 0;
    std::string ficks_log_name;

    conf_client.ReadConfig();
    conf_log.ReadConfig();
    conf_visor.ReadConfig();

    ficks_log_name.clear();
    ficks_log_name += "log/" + conf_log.DefLogName;

    LoggerSpace::Logger* log = LoggerSpace::Logger::getpointcontact();
    log->SetLogMode(conf_log.DefLogMode);
    log->SetSysLogMode(conf_log.DefLogMode);
    log->SetNameLog(ficks_log_name.c_str());//log->SetNameLog(conf_log.DefLogName.c_str());
    log->SetNameSysLog(conf_log.DefSysLogName.c_str());
    log->SetSizeFile(conf_log.DefSizeLogFile * 1024 * 1024);

    if (conf_log.DefStatusLog == LoggerSpace::Status::ON) { log->TurnOnLog(); }
    else if (conf_log.DefStatusLog == LoggerSpace::Status::OFF) { log->TurnOffLog(); }
    if (conf_log.DefStatusSysLog == LoggerSpace::Status::ON) { log->TurnOnSysLog(); }
    else if (conf_log.DefStatusSysLog == LoggerSpace::Status::OFF) { log->TurnOffSysLog(); }

    if (error_block_file != 0)
    {
        messeng.clear();
        messeng += std::to_string(error_block_file) + '\t';
        messeng += std::to_string(sys_error_block_file) + '\t';
        messeng += "ERROR LOCK BLOCK FILE";
        log->WriteLogERR(messeng.c_str());
        sleep(3);
        return 0;
    }

    if (conf_client.result != 0)
    {
        for (int i = 0; i < 7; i++)
        {
            if (conf_client.result & (1 << i) > 0)
            {
                messeng.clear();
                messeng += std::to_string(conf_client.result & (1 << i)) + '\t';
                messeng += "NULL\t";
                messeng += "ERROR CONFIG FILE OF TCP_CLIENT";
                log->WriteLogWARNING(messeng.c_str());
            }
        }
    }

    if (conf_log.result != 0)
    {
        for (int i = 0; i < 8; i++)
        {
            if (conf_log.result & (1 << i) > 0)
            {
                messeng.clear();
                messeng += std::to_string(conf_log.result & (1 << i)) + '\t';
                messeng += "NULL\t";
                messeng += "ERROR CONFIG FILE OF LOGGER";
                log->WriteLogWARNING(messeng.c_str());
            }
        }
    }

    if (conf_visor.result != 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (conf_visor.result & (1 << i) > 0)
            {
                messeng.clear();
                messeng += std::to_string(conf_visor.result & (1 << i)) + '\t';
                messeng += "NULL\t";
                messeng += "ERROR CONFIG FILE OF MANAGER";
                log->WriteLogWARNING(messeng.c_str());
            }
        }
    }


    DDS_CLIENT* DDS = new DDS_CLIENT(conf_client.channel, conf_client.f_hz);

    for (;;)
    {
        if (DDS->InitPPD() == Status_PPD::OK) break;
        if (get_sign() == 1) break;
        sleep(5);
    }

    result = DDS->ReadListKKSInput();
    if (result != 0)
    {
        messeng.clear();
        messeng += std::to_string(result) + '\t';
        messeng += "NULL\t";
        messeng += "ERROR READING LIST KKS OUTPUT DATA";
        log->WriteLogERR(messeng.c_str());
    }

    result = DDS->InitMemory();
    if (result != 0)
    {
        for (int i = 0; i < 6; i++)
        {
            if ((result & (1 << i)) > 0)
            {
                messeng.clear();
                messeng += std::to_string(result & (1 << i)) + '\t';
                messeng += "NULL\t";
                messeng += "ERROR INITIALIZATION MEMORY DDS_CLIENT (" + std::to_string(DDS->GetLastError()) + ')';
                log->WriteLogERR(messeng.c_str());
            }
        }

    }

    TCPClient* tcp_unit = new TCPClient(conf_client.IP, conf_client.Port, conf_client.channel, conf_client.typesource, conf_client.f_hz);
    log->WriteLogERR(std::to_string(conf_client.f_hz).c_str());

    pbuf = DDS->GetPointBuf(TypeData::Analog, TypeValue::OUTPUT);
    size_buf = DDS->GetSizeBuf(TypeData::Analog, TypeValue::OUTPUT);
    tcp_unit->InitBufer(TypeData::Analog, TypeValue::INPUT, pbuf.buf, size_buf, pbuf.mutex);

    pbuf = DDS->GetPointBuf(TypeData::Analog, TypeValue::INPUT);
    size_buf = DDS->GetSizeBuf(TypeData::Analog, TypeValue::INPUT);
    tcp_unit->InitBufer(TypeData::Analog, TypeValue::OUTPUT, pbuf.buf, size_buf, pbuf.mutex);

    pbuf = DDS->GetPointBuf(TypeData::Discrete, TypeValue::OUTPUT);
    size_buf = DDS->GetSizeBuf(TypeData::Discrete, TypeValue::OUTPUT);
    tcp_unit->InitBufer(TypeData::Discrete, TypeValue::INPUT, pbuf.buf, size_buf, pbuf.mutex);

    pbuf = DDS->GetPointBuf(TypeData::Discrete, TypeValue::INPUT);
    size_buf = DDS->GetSizeBuf(TypeData::Discrete, TypeValue::INPUT);
    tcp_unit->InitBufer(TypeData::Discrete, TypeValue::OUTPUT, pbuf.buf, size_buf, pbuf.mutex);

    pbuf = DDS->GetPointBuf(TypeData::Binar, TypeValue::OUTPUT);
    size_buf = DDS->GetSizeBuf(TypeData::Binar, TypeValue::OUTPUT);
    tcp_unit->InitBufer(TypeData::Binar, TypeValue::INPUT, pbuf.buf, size_buf, pbuf.mutex);

    pbuf = DDS->GetPointBuf(TypeData::Binar, TypeValue::INPUT);
    size_buf = DDS->GetSizeBuf(TypeData::Binar, TypeValue::INPUT);
    tcp_unit->InitBufer(TypeData::Binar, TypeValue::OUTPUT, pbuf.buf, size_buf, pbuf.mutex);

    pbuf = DDS->GetPointStatusOutData();
    tcp_unit->InitValueStatusUpDataIn((char*)pbuf.buf, pbuf.mutex);
    tcp_unit->start_client();

    /*tcp_unit->InitBufer(TypeData::Analog, TypeValue::OUTPUT, bufAout, 100, &mutex_Aout);
    tcp_unit->InitBufer(TypeData::Discrete, TypeValue::OUTPUT, bufDout, 100, &mutex_Dout);
    tcp_unit->InitBufer(TypeData::Binar, TypeValue::OUTPUT, bufBout, 100, &mutex_Bout);
    tcp_unit->InitBufer(TypeData::Analog, TypeValue::INPUT, bufAin, 100, &mutex_Ain);
    tcp_unit->InitBufer(TypeData::Discrete, TypeValue::INPUT, bufDin, 100, &mutex_Din);
    tcp_unit->InitBufer(TypeData::Binar, TypeValue::INPUT, bufBin, 100, &mutex_Bin);
    tcp_unit->InitValueStatusUpDataIn(&statusin, &mutex_stat);
    tcp_unit->start_client();*/


    VISOR* vis = new VISOR(conf_visor.IP, conf_visor.Port, TypeVisor::Client);
    vis->InitTCPClient(tcp_unit);



    /*int stt = 0;
    std::cout << "\x1B[s" << std::endl;
    while (1)
    {
        //std::cout << "\x1B[u" << std::endl;
        //std::cout << "\x1B[J";
        usleep(500000);
        //std::system("clear");
        std::cout << "\x1B[2J";
        std::cout << "ANALOG - " << bufAout[1] <<std::endl;
        std::cout << "DISCRETE - " << bufDout[1] << std::endl;
        std::cout << "BINAR - " << (int)bufBout[1] << std::endl;

        stt++;
        if (stt >= 4)
        {
            stt = 0;
            for (int i = 0; i < 10; i++)
            {
                bufAin[i]+=0.1;
                bufDin[i]++;
                bufBin[i] = (~bufBin[i]) & 1;
            }
            pthread_mutex_lock(&mutex_stat);
            statusin |= 4;
            statusin |= 2;
            statusin |= 1;
            pthread_mutex_unlock(&mutex_stat);
        }

    }*/

    short gl_com = 0;
    while (1)
    {
        sleep(2);

        if (get_sign() != 0)
        {
            if (get_sign() == 1) { log->WriteLogINFO("NULL\tNULL\tSIGTERM"); break; }
            if (get_sign() == 2) { log->WriteLogINFO("NULL\tNULL\tSIGTHUB"); break; }
            if (get_sign() == 3) { log->WriteLogINFO("NULL\tNULL\tSIGTSTOP"); break; }
            set_sign(0);
        }

        gl_com = global_command.load();
        if (gl_com != 0)
        {
            messeng.clear();
            messeng += "NULL\tNULL\t";
            if (gl_com == 1) messeng += "REBORN CLIENT ON COMMAND";
            if (gl_com == 2) messeng += "SHUTDOWN CLIENT ON COMMAND";
            log->WriteLogINFO(messeng.c_str());
            break;
        }

        /*stt++;
        if ((stt%5) == 0)
        {
            for (int i = 0; i < 10; i++)
            {
                bufAin[i] += 0.1;
                bufDin[i]++;
                bufBin[i] = (~bufBin[i]) & 1;
            }
            pthread_mutex_lock(&mutex_stat);
            statusin |= 4;
            statusin |= 2;
            statusin |= 1;
            pthread_mutex_unlock(&mutex_stat);
        }
        if (stt >= 150)
        {
            stt = 0;
            messeng.clear();
            messeng += "ANALOG[1] - " + std::to_string(bufAout[1]);
            log->WriteLogINFO(messeng.c_str());
        }*/

    }


    tcp_unit->~TCPClient();
    vis->~VISOR();
    DDS->~DDS_CLIENT();
    flock(block_file, LOCK_UN);
    close(block_file);

    sleep(1);
    if (gl_com == 1)
    {
        //sleep(3);
        //result_exec = execv(argv[0], argv);
    }

    //exit(0);
    return 0;
}









