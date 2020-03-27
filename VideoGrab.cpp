#include "VideoGrab.h"

//https://docs.opencv.org/3.4/d8/dfe/classcv_1_1VideoCapture.html#a8c6d8c2d37505b5ca61ffd4bb54e9a7c

VideoGrab::VideoGrab(QObject *parent) : QObject(parent), timer(new QTimer(this))
{
//
    findCamera();
//    start();

}



//ls -la /dev/video*
//v4l2-ctl --all -
//v4l2-ctl --list-devices

void VideoGrab::findCamera()
{
    m_devices.clear();
    m_devices.push_back(QString("file")); //olikke file first
    for (int numb=0; numb<8; numb++)
    {
        try {
            cv::VideoCapture cap(numb);
            if (cap.isOpened())  {
                m_devices.push_back(QString("/dev/video%1").arg(QString::number(numb)));
                                // qDebug()<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<</*cap.get(CV_CAP_PROP_FPS)<<*/cap.get(CV_CAP_PROP_BRIGHTNESS);
            }
        }
        catch(cv::Exception ex) {
            qDebug()<<"no ok"<<ex.what();
        }
    }
    emit devicesChanged();
}

void VideoGrab::grabFrame()
{
    cv::Mat frame;

    *cap>>frame;
    if (frame.empty()) {
       qDebug()<< "ERROR! blank frame grabbed\n";
       return;
    }

    emit Enqueue(frame);
}

bool VideoGrab::setProperty(int PropId, double value)
{
    cap->set(PropId,value);
    return (cap->get(PropId)==value);
}

void VideoGrab::start(int numb)
{
    cap=new cv::VideoCapture(numb);
    if (!cap->isOpened()) {
        qDebug()<<"can't open";
        return;
    }
    double width=720.;
    double height=576.;
    if (!setProperty(CV_CAP_PROP_FRAME_WIDTH,width))   qDebug()<<"Error1";
    if (!setProperty(CV_CAP_PROP_FRAME_HEIGHT,height))   qDebug()<<"Error2";

    checkBrightness();

    checkContrast();

    checkSaturation();

    checkHue();

    connect(timer,&QTimer::timeout,this,&VideoGrab::grabFrame);
    timer->setInterval(static_cast<int>(1000/25.));
    timer->start();
    m_open=true;
    emit openChanged();
}

void VideoGrab::playFile(QString name)
{
     cap=new cv::VideoCapture(name.toStdString());
     if (!cap->isOpened()) {
         qDebug()<<"can't open";
         return;
     }
     connect(timer,&QTimer::timeout,this,&VideoGrab::grabFrame);
     timer->setInterval(static_cast<int>(1000/cap->get(CV_CAP_PROP_FPS)));

     m_videoSize.setWidth(static_cast<qreal>(cap->get(CV_CAP_PROP_FRAME_WIDTH)));
     m_videoSize.setHeight(static_cast<qreal>(cap->get(CV_CAP_PROP_FRAME_HEIGHT)));

     emit videoSizeChanged();

     timer->start();
     m_open=true;
     emit openChanged();
}

void VideoGrab::checkBrightness()
{
    m_brightness=cap->get(CV_CAP_PROP_BRIGHTNESS);
    qDebug()<<"brightness"<<m_brightness;
    emit brightnessChanged();
}

void VideoGrab::checkContrast()
{
    m_contrast=cap->get(CV_CAP_PROP_CONTRAST);
    qDebug()<<"contrast"<<m_contrast;
    emit contrastChanged();
}

void VideoGrab::checkSaturation()
{
    m_saturation=cap->get(CV_CAP_PROP_SATURATION);
    qDebug()<<"saturation"<<m_saturation;
    emit saturationChanged();
}

void VideoGrab::checkHue()
{
    m_hue=cap->get(CV_CAP_PROP_HUE);
    qDebug()<<"hue"<<m_hue;
    emit hueChanged();
}

void VideoGrab::setBrightness(double val)
{
    if (!cap) {
        emit brightnessChanged();
        return;
    }
    cap->set(CV_CAP_PROP_BRIGHTNESS,val);
    checkBrightness();
}

void VideoGrab::setContrast(double val)
{
    if (!cap) {
        emit contrastChanged();
        return;
    }
    cap->set(CV_CAP_PROP_CONTRAST,val);
    checkContrast();
}

void VideoGrab::setSaturation(double val)
{
    if (!cap) {
        emit saturationChanged();
        return;
    }
    cap->set(CV_CAP_PROP_SATURATION,val);
    checkSaturation();
}

void VideoGrab::setHue(double val)
{
    if (!cap) {
        emit hueChanged();
        return;
    }
    cap->set(CV_CAP_PROP_HUE,val);
    checkHue();
}
