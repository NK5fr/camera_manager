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

class FlirCamera : public QObject, public ImageEventHandler
{
    Q_OBJECT
public:
    FlirCamera(CameraPtr);
    ~FlirCamera();
    void startAquisition() { cam->BeginAcquisition(); }
    void stopAquisition() { cam->EndAcquisition(); }
    int getExposureTime();
    bool getExposureAuto();
    void setExposureMode(bool mode);
    void OnImageEvent(ImagePtr);
public slots:
    void setExposureTime(int exposure);
signals:
    void exposureTimeChanged(int);
    void imageRetrieved(ImagePtr, int);
private:
    CameraPtr cam = nullptr;
    ImageProcessor processor;
    int count = 0;
};

#endif // FLIR_CAMERA_H
