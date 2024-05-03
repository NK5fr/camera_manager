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
    void startAquisition() { cam->BeginAcquisition(); timerId = startTimer(1000); emit streaming(true); }
    void stopAquisition() { cam->EndAcquisition(); resetTimer(); emit streaming(false); }
    void changeAcquisition(bool streaming) { (streaming) ? stopAquisition() : startAquisition();}
    void OnImageEvent(ImagePtr);

    INodeMap &getINodeMap();
    std::string getModelName();
    std::string getVendorName();
    CameraPtr getCamera();
    int getFrameRate();
    int getExposureTime();
    int getGain();

    void setCamera(CameraPtr newCam);
    void setExposureAuto(bool mode);

    bool isExposureAuto();
    bool isSteaming();
    bool isConnected();
    string getSerial();
public slots:
    void setExposureTime(int exposure);
    void setGain(int gain);
    void setFrameRate(int framerate);
signals:
    void exposureTimeChanged(int);
    void gainChanged(int);
    void imageRetrieved(ImagePtr, int);
    void streaming(bool);
    void frameRate(int);
private:
    CameraPtr cam = nullptr;
    ImageProcessor processor;
    int frameCount = 0;
    int secondsCount = 0;
    int timerId;

    void timerEvent(QTimerEvent *event);
    void resetTimer();

};

#endif // FLIR_CAMERA_H
