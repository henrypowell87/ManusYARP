#include "../include/iCub/manusYarpRateThread.h"


using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;
using namespace std;

#define THRATE 2000//ms

manusYarpRatethread::manusYarpRatethread() : RateThread(THRATE) {
    robot = "icub";
}

manusYarpRatethread::manusYarpRatethread(const std::string t_recordHandType, const int t_sampleRate) : RateThread(
        t_sampleRate) {
    robot = "icub";
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


    // Opening file stream to save output and labeling columns
    recordSensorCSV.open(fileNameCSV + ".csv");          // file name of outputted .csv file


    if (recordHandType == "left") {
        recordSensorCSV
                << "thumbLowL,thumbHighL,indexLowL,indexHighL,middleLowL,middleHighL,ringLowL,ringHighL,pinkyLowL,pinkyHighL"
                << std::endl;
    }
    else if (recordHandType == "right") {
        recordSensorCSV
                << "thumbLowR,thumbHighR,indexLowR,indexHighR,middleLowR,middleHighR,ringLowR,ringHighR,pinkyLowR,pinkyHighR,"
                << std::endl;
    }
    else {
        recordSensorCSV
                << "thumbLowR,thumbHighR,indexLowR,indexHighR,middleLowR,middleHighR,ringLowR,ringHighR,pinkyLowR,pinkyHighR,"  \
                "thumbLowL,thumbHighL,indexLowL,indexHighL,middleLowL,middleHighL,ringLowL,ringHighL,pinkyLowL,pinkyHighL"
                << std::endl;
    }

    yInfo("Initialization of the processing thread ended correctly.");

    return true;
}

std::string manusYarpRatethread::getName(const char *p) {
    string str(name);
    str.append(p);
    return str;
}

void manusYarpRatethread::setName(string str) {
    this->name = str;
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

        if (recordHandType == "left") {
            ManusGetHand(manus_session, GLOVE_LEFT, &leftHandData);
            recordSensorCSV << toString_allFingerDataCSV(leftHandData.raw.finger_sensor) + "," + "\n";
        } else if (recordHandType == "right") {
            ManusGetHand(manus_session, GLOVE_RIGHT, &rightHandData);
            recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + "," + "\n";
        } else {
            ManusGetHand(manus_session, GLOVE_LEFT, &leftHandData);
            ManusGetHand(manus_session, GLOVE_RIGHT, &rightHandData);
            recordSensorCSV << toString_allFingerDataCSV(rightHandData.raw.finger_sensor) + ",";
            recordSensorCSV << toString_allFingerDataCSV(leftHandData.raw.finger_sensor) + "\n";
        }


    } else {
        std::cout << " Unable to connect to Manus-VR gloves" << std::endl;
        ManusExit(manus_session);
    }

}

bool manusYarpRatethread::processing() {
    //processing goes here...
    return true;
}


void manusYarpRatethread::threadRelease() {

    ManusExit(manus_session);
    recordSensorCSV.close();
    inputPort.interrupt();
    outputPort.interrupt();
    inputPort.close();
    outputPort.close();
}

void manusYarpRatethread::setFileName(const string &fileName) {
    manusYarpRatethread::fileNameCSV = fileName;
}

