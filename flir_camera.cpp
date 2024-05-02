#include "flir_camera.h"
#include "QDebug"
FlirCamera::FlirCamera(CameraPtr cam)
{
    this->cam = cam;
    cam->Init();
    cam->AcquisitionFrameRateEnable.SetValue(true);
    cam->AcquisitionFrameRate.SetValue(15.0);
    setExposureAuto(false);
}

FlirCamera::~FlirCamera()
{
    image->Release();
    stopAquisition();
    cam->DeInit();
}

ImagePtr FlirCamera::getNextImageConverted()
{
    image = cam->GetNextImage(1000);
    return processor.Convert(image, PixelFormat_BGR8);
}

void FlirCamera::setExposureTime(int exposure) {
    try {
        this->cam->ExposureTime.SetValue(exposure);
        emit exposureTimeChanged(exposure);
    } catch(Spinnaker::Exception exception) {
        qInfo() << exception.what();
    }
}
void FlirCamera::setExposureAuto(bool mode)
{
    try {
        if (mode) {
            cam->ExposureAuto.SetValue(Spinnaker::ExposureAuto_Continuous);
            //emit exposureTimeChanged(this->getExposureTime());
        } else {
            cam->ExposureAuto.SetValue(Spinnaker::ExposureAuto_Off);
        }
    } catch(Spinnaker::Exception exception) {
        qInfo() << exception.what();
    }
}

int FlirCamera::getExposureTime() {
    return this->cam->ExposureTime.GetValue();
}

bool FlirCamera::isExposureAuto() {
    Spinnaker::ExposureAutoEnums value = cam->ExposureAuto.GetValue();
    return value != Spinnaker::ExposureAuto_Off;
}
