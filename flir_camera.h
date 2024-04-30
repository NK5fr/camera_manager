#ifndef FLIR_CAMERA_H
#define FLIR_CAMERA_H

#include <QObject>
#include <QImage>

#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include <iostream>
#include <sstream>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;


class FlirCamera
{

public:
    FlirCamera(CameraPtr);
    ~FlirCamera();
    void startAquisition() {cam->BeginAcquisition();}
    ImagePtr getNextImageConverted();

private:
    CameraPtr cam = nullptr;
    ImagePtr image;
    ImageProcessor processor;
};

#endif // FLIR_CAMERA_H
