#include "flir_camera.h"
#include "QDebug"
#include "utils.h"
FlirCamera::FlirCamera(CameraPtr cam)
{
    // Init camera with event handler
    this->cam = cam;
    cam->Init();
    cam->RegisterEventHandler(*this);

    // Reduce frame rate to 30 to avoid lags
    cam->AcquisitionFrameRateEnable.SetValue(true);
    cam->AcquisitionFrameRate.SetValue(30.0);

    // Set the gain auto and exposure auto to false
    setGainAuto(false);
    setExposureAuto(false);

    // Set trigger source and trigger mode values in a map
    triggerValues["software"] =  Spinnaker::TriggerSource_Software;
    triggerValues["line0"] =  Spinnaker::TriggerSource_Line0;
    triggerModeValues["off"] = Spinnaker::TriggerMode_Off;
    triggerModeValues["on"] = Spinnaker::TriggerMode_On;
}

FlirCamera::~FlirCamera() {
    cam->DeInit();
}

// Change exposure time
void FlirCamera::setExposureTime(int exposure)
{
    try {
        this->cam->ExposureTime.SetValue(exposure);
        emit exposureTimeChanged(exposure);
    } catch (Spinnaker::Exception exception) { // Catch exception if auto exposure is activate
        utils::showError(exception.what());
    }
}

// Change gain
void FlirCamera::setGain(int gain)
{
    try {
        cam->Gain.SetValue(gain);
        emit gainChanged(gain);
    } catch (Spinnaker::Exception exception) { // Catch exception if auto gain is activate
        utils::showError(exception.what());
    }
}

// Change frame rate value (Frame rate auto is always off)
void FlirCamera::updateFixedFrameRate(int framerate)
{
    cam->AcquisitionFrameRate.SetValue(framerate);
}

// Change exposure auto value
void FlirCamera::setExposureAuto(bool mode)
{
    try {
        if (mode) {
            cam->ExposureAuto.SetValue(Spinnaker::ExposureAuto_Continuous);
            //emit exposureTimeChanged(this->getExposureTime());
        } else {
            cam->ExposureAuto.SetValue(Spinnaker::ExposureAuto_Off);
        }
    } catch (Spinnaker::Exception exception) {
        qInfo() << exception.what();
    }
}

// Change gain auto value
void FlirCamera::setGainAuto(bool mode){
    try{
        if(mode){
            cam->GainAuto.SetValue(Spinnaker::GainAuto_Continuous);
        }else{
            cam->GainAuto.SetValue(Spinnaker::GainAuto_Off);
        }
    }catch(Spinnaker::Exception exception){
        qInfo() << exception.what();
    }
}

// Catch images
void FlirCamera::OnImageEvent(ImagePtr image)
{
    ImagePtr converted_image = processor.Convert(image, PixelFormat_BGR8); // Convert image format
    frameCount++;
    emit imageRetrieved(converted_image, frameCount); // Emit signal with image and number of taken images
}

// Return exposure time value
int FlirCamera::getExposureTime()
{
    return this->cam->ExposureTime.GetValue();
}

// Return gain value
int FlirCamera::getGain()
{
    return cam->Gain.GetValue();
}

// Return exposure auto value
bool FlirCamera::isExposureAuto()
{
    Spinnaker::ExposureAutoEnums value = cam->ExposureAuto.GetValue();
    return value != Spinnaker::ExposureAuto_Off;
}

// Return gain auto value
bool FlirCamera::isGainAuto() {
    Spinnaker::GainAutoEnums value = cam->GainAuto.GetValue();
    return value != Spinnaker::GainAuto_Off;
}

// Return camera device model name
std::string FlirCamera::getModelName()
{
    CStringPtr ptrDeviceVendorName = cam->GetTLDeviceNodeMap().GetNode("DeviceModelName");
    return ptrDeviceVendorName->ToString().c_str();
}

// Return camera vendor name
std::string FlirCamera::getVendorName()
{
    CStringPtr ptrDeviceVendorName = cam->GetTLDeviceNodeMap().GetNode("DeviceVendorName");
    return ptrDeviceVendorName->ToString().c_str();
}

// Check if the camera is streaming
bool FlirCamera::isSteaming()
{
    return this->cam->IsStreaming();
}

// Check if the camera is still connected by switching acquisition
bool FlirCamera::isConnected()
{
    try {
        if (isSteaming()) {
            stopAquisition();
            startAquisition();
        } else {
            startAquisition();
            stopAquisition();
        }
        return true;
    } catch (Exception e) {
        return false;
    }
}

// Return camera serial number
string FlirCamera::getSerial()
{
    CStringPtr ptrStringSerial = cam->GetTLDeviceNodeMap().GetNode("DeviceSerialNumber");
    return ptrStringSerial->GetValue().c_str();
}

// Return fixed frame rate value
int FlirCamera::getFixedFrameRate()
{
    return cam->AcquisitionFrameRate.GetValue();
}

// Chnage trigger source
void FlirCamera::setTrigger(QString v){
    string value = v.toStdString();
    cam->TriggerSource.SetValue(triggerValues.at(value));
    emit triggerChange(v);
}

// Change trigger mode
void FlirCamera::setTriggerMode(QString v){
    string value = v.toStdString();
    cam->TriggerMode.SetValue(triggerModeValues.at(value));
    emit triggerModeChange(v);
}

// Return the maximum frame rate value according to the camera capacity
int FlirCamera::getMaxFps(){
    int oldValue = cam->AcquisitionFrameRate.GetValue(); // Get fixed frame rate value
    cam->AcquisitionFrameRateEnable.SetValue(false); // Enable auto frame rate
    int value = cam->AcquisitionFrameRate.GetValue(); // Get maximum frame rate value
    cam->AcquisitionFrameRateEnable.SetValue(true); // Disable auto frame rate
    cam->AcquisitionFrameRate.SetValue(oldValue); // Set fixed frame rate value
    return value;
}

