#include "flir_camera.h"

FlirCamera::FlirCamera(CameraPtr cam)
{
    this->cam = cam;
    cam->Init();

    cam->AcquisitionFrameRateEnable.SetValue(true);
    cam->AcquisitionFrameRate.SetValue(15.0);
}

FlirCamera::~FlirCamera(){
    image->Release();
    cam->EndAcquisition();
    cam->DeInit();
}

ImagePtr FlirCamera::getNextImageConverted()
{
    image = cam->GetNextImage(1000);
    return processor.Convert(image, PixelFormat_BGR8);
}


