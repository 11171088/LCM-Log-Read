// file: read_log.cpp
//
// LCM example program.  Demonstrates how to read and decode messages directly
// from a log file in C++.  It is also possible to use the log file provider --
// see the documentation on the LCM class for details on that method.
//
// compile with:
//  $ g++ -o read_log read_log.cpp -llcm
//
// On a system with pkg-config, you can also use:
//  $ g++ -o read_log read_log.cpp `pkg-config --cflags --libs lcm`

#include <iostream>
#include <fstream>
#include <lcm/lcm-cpp.hpp>
#include "PERCEPTION_LINES_PP.hpp"
#include "INS300C.hpp"

#include <string>

using namespace std;

int main(int argc, char ** argv)
{
    if(argc < 2) {
        fprintf(stderr, "usage: read_log <logfile>\n");
        return 1;
    }

    // Open the log file.
    lcm::LogFile log(argv[1], "r");
    if(!log.good()) {
        perror("LogFile");
        fprintf(stderr, "couldn't open log file %s\n", argv[1]);
        return 1;
    }

    //根据log文件名动态生成保存的txt文件名
    string NameBase = argv[1];    //最原始的log文件的名称
    string LogSaveNameBase = NameBase;   //因为使用string函数中的 substr，append，insert等函数会改变原string值，所以这里另用一个LogSaveNameBase名字
    LogSaveNameBase =  LogSaveNameBase.substr(0,LogSaveNameBase.find('.'));

    //储存的PERCEPTION_LINES_PP通道中的数据
    string LogSaveName =  LogSaveNameBase.append(".txt");              //用于储存除了车道线点以外的其他信息
    string LogSaveNamePoint =  LogSaveNameBase.insert(LogSaveNameBase.find('.'),"_point");   //用于储存车道线点信息


    ofstream ofs;   //ofstream ofs放到while循环外是为了同步存储其他通道的值


    // //在储存PERCEPTION_LINES_PP通道内的数据前增加一行单独放通道名，用于matlab读取时区分各个通道的内容
    // ofs.open(LogSaveName, ios::out | ios::app); 
    // ofs << "PERCEPTION_LINES_PP"<< endl;     //说明是PERCEPTION_LINES_PP通道

    // //储存 PERCEPTION_LINES_PP 中数据
    // while(1) {
    //     // Read a log event.
    //     const lcm::LogEvent *event = log.readNextEvent();
    //     if(!event)
    //         break;

    //     // Only process messages on the EXAMPLE channel.
    //     if(event->channel != "PERCEPTION_LINES_PP")
    //         continue;

    //     // Try to decode the message.
    //     PERCEPTION_LINES_PP msg;
    //     if(msg.decode(event->data, 0, event->datalen) != event->datalen)
    //         continue;

    //     // //将数据以追加（app）的方式写入到test.txt文件中，每个通道的一个event放一行，以tab隔开
    //     // ofstream ofs;

	//     ofs.open(LogSaveName, ios::out | ios::app);   
            
    //     //PERCEPTION_LINES_PP通道里面的信息
    //     //这里强行把lcm-cpp.hpp里面的LogFile类中的curEvent从private改为public
    //     ofs << "nTimeStamp:"<<log.curEvent.timestamp<< '\t'   
    //     <<"nFrameID:"<<(int)msg.stHeader.nFrameID << '\t'
    //     << "nCounter:"<<(int)msg.stHeader.nCounter << '\t'
    //     << "bLeftLeftLine:"<<(int)msg.bLeftLeftLine << '\t'
    //     << "nLeftLeftLineType:"<<(int)msg.nLeftLeftLineType << '\t'
    //     << "nLeftLeftArrayLength:"<<msg.nLeftLeftArrayLength << '\t'
    //     << "bLeftLine:"<<(int)msg.bLeftLine << '\t'
    //     << "nLeftLineType:"<<(int)msg.nLeftLineType << '\t'
    //     << "nLeftArrayLength:"<<msg.nLeftArrayLength << '\t'
    //     << "bRightLine:"<<(int)msg.bRightLine << '\t'
    //     << "nRightLineType:"<<(int)msg.nRightLineType << '\t'
    //     << "nRightArrayLength:"<<msg.nRightArrayLength << '\t'
    //     << "bRightRightLine:"<<(int)msg.bRightRightLine << '\t'
    //     << "nRightRightLineType:"<<(int)msg.nRightRightLineType << '\t'
    //     << "nRightRightArrayLength:"<<msg.nRightRightArrayLength << '\t'
    //     << "rightTurnAreaLength:"<<msg.rightTurnAreaLength << '\t'
    //     << "mapvalid:"<<(int)msg.mapvalid << '\t'
    //     << "RemainLength:"<<msg.RemainLength << '\t'
    //     << "leftTurnAreaLength:"<<msg.leftTurnAreaLength << '\t'
    //     << "rightTurnAreaLength:"<<msg.rightTurnAreaLength << endl;

	//     ofs.close();

    //     // //读取车道线的路径点
	//     // ofs.open(LogSaveNamePoint, ios::out | ios::app);   

    //     // ofs << "nTimeStamp:"<<log.curEvent.timestamp<< endl;

    //     // for ( int i = 0; i < 300; i++)
    //     // {
    //     //     ofs <<msg.gfLeftLineX[i]<< '\t';
    //     //     ofs <<msg.gfLeftLineY[i]<< '\t';
    //     //     ofs <<msg.gfRightLineX[i]<< '\t';
    //     //     ofs <<msg.gfRightLineY[i]<< endl;
    //     // }
        
    //     // ofs.close();


    // }



    //在储存INS300C通道内的数据前增加一行单独放通道名，用于matlab读取时区分各个通道的内容
    ofs.open(LogSaveName, ios::out | ios::app); 
    ofs << "INS300C"<< endl;     //说明是INS300C通道

    //储存 INS300C 中数据
    while(1) {
        // Read a log event.
        const lcm::LogEvent *event = log.readNextEvent();
        if(!event)
            break;

        // Only process messages on the EXAMPLE channel.
        if(event->channel != "INS300C58")
            continue;

        // Try to decode the message.
        INS300C msg;
        if(msg.decode(event->data, 0, event->datalen) != event->datalen)
            continue;

        // //将数据以追加（app）的方式写入到test.txt文件中，每个通道的一个event放一行，以tab隔开
        // ofstream ofs;

	    ofs.open(LogSaveName, ios::out | ios::app);   
            
        //PERCEPTION_LINES_PP通道里面的信息
        //这里强行把lcm-cpp.hpp里面的LogFile类中的curEvent从private改为public
        ofs << "nTimeStamp:"<<log.curEvent.timestamp<<'\t'
        << "Latitude:"<<msg.dLatitude << '\t'
        << "Longitude:"<<msg.dLongitude << '\t'
        << "Altitude:"<<msg.fAltitude << '\t'
        << "VelWest:"<<msg.fVelWest << '\t'
        << "VelUp:"<<msg.fVelUp << '\t'
        <<"VelUp:"<<msg.fHeading << '\t'
        <<"Pitch:"<<msg.fPitch << '\t'
        <<"fRoll:"<<msg.fRoll << endl;


	    ofs.close();

    }


    // Log file is closed automatically when the log variable goes out of
    // scope.

    // printf("done\n");
    std::cout<<"解析完成～"<<std::endl;
    return 0;
}
