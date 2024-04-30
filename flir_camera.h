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


class FlirCamera : public QObject, public ImageEventHandler
{
    Q_OBJECT
public:
    FlirCamera(CameraPtr);
    ~FlirCamera();

    void OnImageEvent(Spinnaker::ImagePtr image);
    int getImageCount() const { return this->imageCount; }
    void startAquisition() {cam->BeginAcquisition();}

signals:
    void imageArrived(ImagePtr, int const);

private:
    int imageCount = 0;
    CameraPtr cam = nullptr;
    ImageProcessor processor;
};

#endif // FLIR_CAMERA_H
