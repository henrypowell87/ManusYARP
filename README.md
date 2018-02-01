# ManusYARP
A YARP module thar reads from and outputs finger sensor data from the Manus-VR gloves (https://manus-vr.com/) 


# Dependency
- YARP
- MANUS-VR SDK

The header file and library for both should be installed on your computer (/usr/local)

# CSV Format
The format of the CSV file is as follow :

For both hands [finger/sensor/hand]:
thumbLowR,thumbHighR,indexLowR,indexHighR,middleLowR,middleHighR,ringLowR,ringHighR,pinkyLowR,pinkyHighR,thumbLowL,thumbHighL,indexLowL,indexHighL,middleLowL,middleHighL,ringLowL,ringHighL,pinkyLowL,pinkyHighL

For left hand only:
thumbLowL,thumbHighL,indexLowL,indexHighL,middleLowL,middleHighL,ringLowL,ringHighL,pinkyLowL,pinkyHighL

For right hand only:
thumbLowR,thumbHighR,indexLowR,indexHighR,middleLowR,middleHighR,ringLowR,ringHighR,pinkyLowR,pinkyHighR


# Module Parameters:

Module parameters are called in the command line (assuming that a YARP server is already running) in the following way:

./MANUSYARP --from manusYARP.ini --file_name "FILENAME" --hand_to_read "HAND" --sample_rate SAMPLERATE

file_name:

As string. Can be anything you want. Will be output as a .CSV file.

hand_to_read:

As string. Options: "left" (left hand)/ "right" (right hand)/ "both" (both hands)

sample_rate:

As int. Options (in Hz): 100/ 500/ 1000



