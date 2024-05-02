#include "flir_camera.h"
#include "QDebug"
FlirCamera::FlirCamera(CameraPtr cam)
{
    this->cam = cam;
    cam->Init();
    cam->RegisterEventHandler(*this);
    cam->AcquisitionFrameRateEnable.SetValue(true);
    cam->AcquisitionFrameRate.SetValue(30.0);
    setExposureAuto(false);
}

FlirCamera::~FlirCamera()
{
    cam->DeInit();
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

void FlirCamera::OnImageEvent(ImagePtr image)
{
    ImagePtr converted_image = processor.Convert(image, PixelFormat_BGR8);
    count++;
    emit imageRetrieved(converted_image, count);
}

int FlirCamera::getExposureTime() {
    return this->cam->ExposureTime.GetValue();
}

bool FlirCamera::isExposureAuto() {
    Spinnaker::ExposureAutoEnums value = cam->ExposureAuto.GetValue();
    return value != Spinnaker::ExposureAuto_Off;
}

std::string FlirCamera::getModelName() {
    CStringPtr ptrDeviceVendorName = getINodeMap().GetNode("DeviceModelName");
    return ptrDeviceVendorName->ToString().c_str();
}

std::string FlirCamera::getVendorName() {
    CStringPtr ptrDeviceVendorName = getINodeMap().GetNode("DeviceVendorName");
    return ptrDeviceVendorName->ToString().c_str();
}

CameraPtr FlirCamera::getCamera()
{
    return this->cam;
}

void FlirCamera::setCamera(CameraPtr newCam)
{
    this->cam = newCam;
}

bool FlirCamera::isSteaming()
{
    return this->cam->IsStreaming();
}

INodeMap &FlirCamera::getINodeMap() {
    return cam->GetTLDeviceNodeMap();
}
