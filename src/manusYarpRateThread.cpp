#include "../include/iCub/manusYarpRateThread.h"
#include <cstring>
#include <Manus.h>
#include <unistd.h>

using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

#define THRATE 100 //ms

manusYarpRatethread::manusYarpRatethread(const std::string t_recordHandType):RateThread(THRATE) {
    robot = "icub";
    recordHandType = t_recordHandType;
}

manusYarpRatethread::manusYarpRatethread(const int _sampleRate):RateThread(THRATE) {
    robot = "icub";
    sampleRate = _sampleRate;
}

manusYarpRatethread::manusYarpRatethread(const std::string _fileName):RateThread(THRATE) {
    robot = "icub";
    fileName = _fileName;
}

manusYarpRatethread::manusYarpRatethread(string _robot, string _configFile):RateThread(THRATE){
    robot = _robot;
    configFile = _configFile;
}

manusYarpRatethread::~manusYarpRatethread() {
    // do nothing
}

bool manusYarpRatethread::threadInit() {
    // opening the port for direct input
//    if (!inputPort.open(getName("/image:i").c_str())) {
//        yError("unable to open port to receive input");
//        return false;  // unable to open; let RFModule know so that it won't run
//    }
//
//    if (!outputPort.open(getName("/img:o").c_str())) {
//        yError(": unable to open port to send unmasked events ");
//        return false;  // unable to open; let RFModule know so that it won't run
//    }
//
//    yInfo("Initialization of the processing thread correctly ended");
//
//    return true;
}

std::string manusYarpRatethread::getName(const char* p) {
    string str(name);
    str.append(p);
    return str;
}


//std::string manusYarpRatethread::getRecordHandType(const char* _hand) {
//    string rechand(recordHandType);
//    rechand.append(_hand);
//    return rechand;
//}



//std::string manusYarpRatethread::getfileName(const char* _fileName) {
//    string fileNamed(fileName);
//    fileName.append(_fileName);
//    return fileNamed;
//}

void manusYarpRatethread::setName(string str) {
    this->name=str;
}

void manusYarpRatethread::setRecordHandType(string recHandType) {
}

void manusYarpRatethread::setfileName(string filName) {
}

void manusYarpRatethread::setInputPortName(string InpPort) {
}


std::string toString_allFingerDataCSV(double *sensorsArray) {               //function to define format of CSV file
    // Value sensors for each fingers Low, High
    const std::string pinkyValue = std::to_string(sensorsArray[0]) + "," + std::to_string(sensorsArray[1]);
    const std::string thumbValue = std::to_string(sensorsArray[2]) + "," + std::to_string(sensorsArray[3]);
    const std::string indexValue = std::to_string(sensorsArray[4]) + "," + std::to_string(sensorsArray[5]);
    const std::string middleValue = std::to_string(sensorsArray[6]) + "," + std::to_string(sensorsArray[7]);
    const std::string ringValue = std::to_string(sensorsArray[8]) + "," + std::to_string(sensorsArray[9]);

    return thumbValue + "," + indexValue + "," + middleValue + "," + ringValue + "," + pinkyValue;

}

void manusYarpRatethread::run() {

// Initializing session of MANUS-VR
        manus_session_t session = nullptr;

        uint32_t idDONGLE, count;
        manus_hand_t leftHandData{}, rightHandData{};
        ManusInit(&session);
        ManusGetDongleIDs(session, &idDONGLE, count);


        if (session != nullptr) {

            // Defining the saving file
            std::ofstream recordSensorCSV;
            recordSensorCSV.open(fileName + ".csv");
            recordSensorCSV << "thumbLowR,thumbHighR,indexLowR,indexHighR,middleLowR,middleHighR,ringLowR,ringHighR,pinkyLowR,pinkyHighR,"  \
                "thumbLowL,thumbHighL,indexLowL,indexHighL,middleLowL,middleHighL,ringLowL,ringHighL,pinkyLowL,pinkyHighL" << std::endl;

            while (true) {
                if (recordHandType == "left") {
                    ManusGetHand(session, GLOVE_LEFT, &leftHandData);
                    recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + ",";
                } else if (recordHandType == "right") {
                    ManusGetHand(session, GLOVE_RIGHT, &rightHandData);
                    recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + ",";
                } else {
                    ManusGetHand(session, GLOVE_LEFT, &leftHandData);
                    ManusGetHand(session, GLOVE_RIGHT, &rightHandData);
                    recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + ",";
                    recordSensorCSV << toString_allFingerDataCSV(leftHandData.raw.finger_sensor) + "\n";
                }
                if (sampleRate == 100) {
                    usleep(sampleRate);
                } else if (sampleRate == 500) {
                    usleep(sampleRate);
                } else {
                    usleep(100000);
                }
            }

        } else {
            std::cout << " Unable to Connected to Manus-VR gloves" << std::endl;
            ManusExit(session);
        }

}

bool manusYarpRatethread::processing(){
    //processing goes here...
    return true;
}


void manusYarpRatethread::threadRelease() {
//    recordSensorCSV.close();
    inputPort.interrupt();
    outputPort.interrupt();
    inputPort.close();
    outputPort.close();
}

