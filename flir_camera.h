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
    void startAquisition() { cam->BeginAcquisition(); emit streaming(true); }
    void stopAquisition() { cam->EndAcquisition(); emit streaming(false); }
    void changeAcquisition(bool streaming) { (streaming) ? stopAquisition() : startAquisition();}
    int getExposureTime();
    void OnImageEvent(ImagePtr);
    bool isExposureAuto();
    void setExposureAuto(bool mode);
    INodeMap &getINodeMap();
    std::string getModelName();
    std::string getVendorName();
    CameraPtr getCamera();
    void setCamera(CameraPtr newCam);
    bool isSteaming();
    void setOpen(bool);
    bool isOpen();
    string getSerial();
public slots:
    void setExposureTime(int exposure);
signals:
    void exposureTimeChanged(int);
    void imageRetrieved(ImagePtr, int);
    void streaming(bool);
private:
    CameraPtr cam = nullptr;
    ImageProcessor processor;
    int count = 0;
    bool open = false;
};

#endif // FLIR_CAMERA_H
