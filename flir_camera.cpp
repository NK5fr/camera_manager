#include "flir_camera.h"

FlirCamera::FlirCamera(CameraPtr cam)
{
    this->cam = cam;
    cam->Init();
    cam->RegisterEventHandler(dynamic_cast<ImageEventHandler&>(*this));
}

FlirCamera::~FlirCamera(){
    cam->EndAcquisition();
    cam->DeInit();
}

void FlirCamera::OnImageEvent(ImagePtr image){
    ImagePtr convertedImage = processor.Convert(image, PixelFormat_BGR8);
    ++imageCount;
    emit imageArrived(convertedImage, imageCount);
}
