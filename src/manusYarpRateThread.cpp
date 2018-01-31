#include "../include/iCub/manusYarpRateThread.h"


using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

#define THRATE 100 //ms

manusYarpRatethread::manusYarpRatethread() : RateThread(THRATE) {
    robot = "icub";
    sampleRate = 100;
}

manusYarpRatethread::manusYarpRatethread(const std::string t_recordHandType, const int t_sampleRate) : RateThread(
        THRATE) {
    robot = "icub";
    sampleRate = t_sampleRate;
    recordHandType = t_recordHandType;
}


manusYarpRatethread::~manusYarpRatethread() {
    // do nothing
}

bool manusYarpRatethread::threadInit() {

    // Initializing session of MANUS-VR
    uint32_t idDONGLE, count;
    ManusInit(&manus_session);
    ManusGetDongleIDs(manus_session, &idDONGLE, count);

    if (manus_session == nullptr) {
        yInfo("Failed to Initialize Manus-VR session");
        return false;
    }


    // Opening file stream to save output
    recordSensorCSV.open(fileNameCSV + ".csv");          // file name of outputted .csv file

    yInfo("Initialization of the processing thread correctly ended");

    return true;
}

std::string manusYarpRatethread::getName(const char *p) {
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
//    string fileNamed(fileNameCSV);
//    fileNameCSV.append(_fileName);
//    return fileNamed;
//}

void manusYarpRatethread::setName(string str) {
    this->name = str;
}

void manusYarpRatethread::setRecordHandType(string recHandType) {
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


    if (manus_session != nullptr) {


        recordSensorCSV
                << "thumbLowR,thumbHighR,indexLowR,indexHighR,middleLowR,middleHighR,ringLowR,ringHighR,pinkyLowR,pinkyHighR,"  \
                "thumbLowL,thumbHighL,indexLowL,indexHighL,middleLowL,middleHighL,ringLowL,ringHighL,pinkyLowL,pinkyHighL"
                << std::endl;

        if (recordHandType == "left") {
            ManusGetHand(manus_session, GLOVE_LEFT, &leftHandData);
            recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + ",";
        } else if (recordHandType == "right") {
            ManusGetHand(manus_session, GLOVE_RIGHT, &rightHandData);
            recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + ",";
        } else {
            ManusGetHand(manus_session, GLOVE_LEFT, &leftHandData);
            ManusGetHand(manus_session, GLOVE_RIGHT, &rightHandData);
            recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + ",";
            recordSensorCSV << toString_allFingerDataCSV(leftHandData.raw.finger_sensor) + "\n";
        }

        usleep(sampleRate);


    } else {
        std::cout << " Unable to Connected to Manus-VR gloves" << std::endl;
        ManusExit(manus_session);
    }

}

bool manusYarpRatethread::processing() {
    //processing goes here...
    return true;
}


void manusYarpRatethread::threadRelease() {

    ManusExit(manus_session);
//    recordSensorCSV.close();
    inputPort.interrupt();
    outputPort.interrupt();
    inputPort.close();
    outputPort.close();
}

void manusYarpRatethread::setFileName(const string &fileName) {
    manusYarpRatethread::fileNameCSV = fileName;
}

