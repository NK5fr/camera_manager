#include "flir_camera.h"
#include "QDebug"
FlirCamera::FlirCamera(CameraPtr cam)
{
    this->cam = cam;
    cam->Init();
    cam->RegisterEventHandler(*this);
    cam->AcquisitionFrameRateEnable.SetValue(true);
    cam->AcquisitionFrameRate.SetValue(60.0);
}

FlirCamera::~FlirCamera()
{
    cam->DeInit();
}

void FlirCamera::setExposureTime(int exposure) {
    try {
        this->cam->ExposureTime.SetValue(exposure);
    } catch(Spinnaker::Exception exception) {
        qInfo() << exception.what();
    }
}
void FlirCamera::setExposureMode(bool mode)
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

void FlirCamera::OnImageEvent(ImagePtr image)
{
    ImagePtr converted_image = processor.Convert(image, PixelFormat_BGR8);
    count++;
    emit imageRetrieved(converted_image, count);
}

int FlirCamera::getExposureTime() {
    return this->cam->ExposureTime.GetValue();
}

bool FlirCamera::getExposureAuto() {
    Spinnaker::ExposureAutoEnums value = cam->ExposureAuto.GetValue();
    return value == Spinnaker::ExposureAuto_Continuous;
}
