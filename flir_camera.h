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
    void OnImageEvent(ImagePtr);

    INodeMap &getINodeMap();
    std::string getModelName();
    std::string getVendorName();
    CameraPtr getCamera();
    int getRealFrameRate();
    int getFixedFrameRate();
    int getExposureTime();
    int getGain();

    void setCamera(CameraPtr newCam);
    void setExposureAuto(bool mode);
    void setGainAuto(bool);

    bool isExposureAuto();
    bool isGainAuto();
    bool isSteaming();
    bool isConnected();
    string getSerial();
public slots:
    void setExposureTime(int exposure);
    void setGain(int gain);
    void updateFixedFrameRate(int framerate);
signals:
    void exposureTimeChanged(int);
    void gainChanged(int);
    void imageRetrieved(ImagePtr, int);
    void streaming(bool);
private:
    CameraPtr cam = nullptr;
    ImageProcessor processor;
    int frameCount = 0;

};

#endif // FLIR_CAMERA_H
