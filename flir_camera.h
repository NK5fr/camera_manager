#ifndef FLIR_CAMERA_H
#define FLIR_CAMERA_H

#include <QImage>
#include <QObject>

#include "SpinGenApi/SpinnakerGenApi.h"
#include "Spinnaker.h"
#include <iostream>
#include <sstream>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace std;

class FlirCamera : public QObject
{
    Q_OBJECT
public:
    FlirCamera(CameraPtr);
    ~FlirCamera();
    void startAquisition() { cam->BeginAcquisition(); }
    void stopAquisition() { cam->EndAcquisition(); }
    ImagePtr getNextImageConverted();
    int getExposureTime();
    bool getExposureAuto();
    void setExposureMode(bool mode);
public slots:
    void setExposureTime(int exposure);
signals:
    void exposureTimeChanged(int);
private:
    CameraPtr cam = nullptr;
    ImagePtr image;
    ImageProcessor processor;
};

#endif // FLIR_CAMERA_H
