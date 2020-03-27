#pragma once

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QSizeF>


#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"

class VideoGrab : public QObject
{
    Q_OBJECT
public:
    explicit VideoGrab(QObject *parent = nullptr);

    Q_PROPERTY(QStringList devices READ getDevices  NOTIFY devicesChanged)
    QStringList getDevices() {return m_devices;}

    Q_PROPERTY(bool open READ getOpen  NOTIFY openChanged)
    bool getOpen() {return m_open;}

    //if u want to see all suported user controls for u driver - use v4l2-ctl --all

    Q_PROPERTY(double brightness READ getBrightness WRITE setBrightness NOTIFY brightnessChanged)
    double getBrightness() {return m_brightness;}
    void setBrightness(double val);

    Q_PROPERTY(double contrast READ getContrast WRITE setContrast NOTIFY contrastChanged)
    double getContrast() {return m_contrast;}
    void setContrast(double val);

    Q_PROPERTY(double saturation READ getSaturation WRITE setSaturation NOTIFY saturationChanged)
    double getSaturation() {return m_saturation;}
    void setSaturation(double val);

    Q_PROPERTY(double hue READ getHue WRITE setHue NOTIFY hueChanged)
    double getHue() {return m_hue;}
    void setHue(double val);

    Q_PROPERTY(QSizeF videoSize READ getVideoSize NOTIFY videoSizeChanged)
    QSizeF getVideoSize() {return m_videoSize;}

    Q_INVOKABLE void refresh() {findCamera();}
    Q_INVOKABLE void openDevice(int numb) {start(numb);}
    Q_INVOKABLE void playFile(QString name);

signals:
     void Enqueue(cv::Mat value);
     void devicesChanged();
     void openChanged();
     void brightnessChanged();
     void contrastChanged();
     void saturationChanged();
     void hueChanged();
     void videoSizeChanged();
     void eof();
public:


private:
    void findCamera();
    void grabFrame();
    bool setProperty(int PropId, double value);
    void start(int numb);
    void stop();
    QTimer* timer;
    cv::VideoCapture* cap;

    QStringList m_devices;
    bool m_open=false;

    double m_brightness=0.;
    double m_contrast=0.;
    double m_saturation=0.;
    double m_hue=0.;

    void checkBrightness();
    void checkContrast();
    void checkSaturation();
    void checkHue();

    QSizeF m_videoSize;

};
