#include "flir_camera.h"
#include "QDebug"
#include "utils.h"
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
}

void FlirCamera::setExposureTime(int exposure) {
    try {
        this->cam->ExposureTime.SetValue(exposure);
        emit exposureTimeChanged(exposure);
    } catch(Spinnaker::Exception exception) {
        utils::showError(exception.what());
    }
}

void FlirCamera::setGain(int gain)
{
    try {
        cam->Gain.SetValue(gain);
        emit gainChanged(gain);
    } catch(Spinnaker::Exception exception) {
        utils::showError(exception.what());
    }
}

void FlirCamera::setFrameRate(int framerate)
{
    this->resetTimer();
    this->timerId = startTimer(1000);
    cam->AcquisitionFrameRate.SetValue(framerate);
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
    frameCount++;
    emit imageRetrieved(converted_image, frameCount);
}

int FlirCamera::getExposureTime() {
    return this->cam->ExposureTime.GetValue();
}

int FlirCamera::getGain()
{
    return cam->Gain.GetValue();
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

bool FlirCamera::isConnected()
{
    try {
        if(isSteaming()){
            stopAquisition();
            startAquisition();
        }else{
            startAquisition();
            stopAquisition();
        }
        return true;
    }catch(Exception e){
        return false;
    }
}

string FlirCamera::getSerial()
{
    CStringPtr ptrStringSerial = cam->GetTLDeviceNodeMap().GetNode("DeviceSerialNumber");
    return ptrStringSerial->GetValue().c_str();
}

INodeMap &FlirCamera::getINodeMap() {
    return cam->GetTLDeviceNodeMap();
}

void FlirCamera::timerEvent(QTimerEvent *event) {
    secondsCount++;
    emit frameRate(this->getFrameRate());
}

void FlirCamera::resetTimer()
{
    this->frameCount = 0;
    this->secondsCount = 0;
    killTimer(timerId);
    emit frameRate(0);
}

int FlirCamera::getFrameRate() {
    return this->frameCount / this->secondsCount;
}

