#include "imageprocessingfront.h"

ImageProcessingFront::ImageProcessingFront(ImageProcessing* _imagePro, QObject *parent) : QObject(parent),
    imagePro(_imagePro),
    thread(new QThread(this))


{
    imagePro->moveToThread(thread);
    QObject::connect(thread,&QThread::finished, imagePro, &ImageProcessing::stop);
    QObject::connect(thread,&QThread::finished, thread, &QThread::deleteLater);           

    QObject::connect(imagePro,&ImageProcessing::trackingAreaChanged,this,&ImageProcessingFront::trackingAreaChanged);
    QObject::connect(imagePro,&ImageProcessing::targetPosChanged,this,&ImageProcessingFront::targetPosChanged);








    thread->start();
}

ImageProcessingFront::~ImageProcessingFront()
{
    thread->quit();
    thread->wait();
}

void ImageProcessingFront::changeTrack(Track tracking)
{
    imagePro->setTracking(tracking);
}

void ImageProcessingFront::mousePos(QPointF mousePos)
{
    imagePro->setMousePos(mousePos);
}

void ImageProcessingFront::changeTrackingArea(bool increase)
{
    imagePro->increaseTrackingArea(increase);
}
