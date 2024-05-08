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
    void startAquisition()
    {
        cam->BeginAcquisition();
        emit streaming(true);
    }
    void stopAquisition()
    {
        cam->EndAcquisition();
        emit streaming(false);
    }
    void changeAcquisition(bool streaming) { (streaming) ? stopAquisition() : startAquisition(); }
    void OnImageEvent(ImagePtr);

    INodeMap &getINodeMap();
    std::string getModelName();
    std::string getVendorName();
    int getRealFrameRate();
    int getFixedFrameRate();
    int getExposureTime();
    int getGain();

    void setExposureAuto(bool mode);
    void setGainAuto(bool);

    bool isExposureAuto();
    bool isGainAuto();
    bool isSteaming();
    bool isConnected();
    string getSerial();
    int getMaxFps();
    void setExposureTime(int exposure);
    void setGain(int gain);
    void updateFixedFrameRate(int framerate);
    void setTrigger(QString value);
    void setTriggerMode(QString value);
    CameraPtr getCam() const;

signals:
    void exposureTimeChanged(int);
    void gainChanged(int);
    void imageRetrieved(ImagePtr, int);
    void streaming(bool);
    void triggerChange(QString);
    void triggerModeChange(QString);
private:
    CameraPtr cam = nullptr;
    ImageProcessor processor;
    int frameCount = 0;
    map<string, Spinnaker::TriggerSourceEnums> triggerValues;
    map<string, Spinnaker::TriggerModeEnums> triggerModeValues;
};

#endif // FLIR_CAMERA_H
