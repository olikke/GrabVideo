#pragma once

#include <QObject>
#include <QImage>
#include <QPainter>
#include <chrono>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "samplequeue.h"

enum Track {
    NO,
    AUTO,
    MANUAL
};

//переключение рисования вручную / рисования GUI
const bool MANUAL_DRAW = false;

//olikke разделить интерфейсную часть и непосредственно thread!!!

class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessing(QObject *parent = nullptr);

    void setTracking(Track &val) {Tracking=val;}

    void setMousePos(QPointF &val);

//    Q_PROPERTY(QPointF crossPos READ getCrossPos NOTIFY crossPosChanged)
//    QPointF getCrossPos() {return CrossPos;}

    //изменение площади зоны сопровождения
    Q_INVOKABLE void increaseTrackingArea(bool inc);
//    Q_INVOKABLE void changeFilterParameter(int val) {MaxPorog=val*10;}

signals:
    void newSample(QImage image);
    void trackingAreaChanged(qreal scal);
    void targetPosChanged(QPointF point);
    //сделать обратную связь с автоматом и темное на светлом для Ф8
    void littleSample(QImage image);
public slots:
    void newData();
    void stop();
private:
    void processImage(cv::Mat image);
    void makeQImage(cv::Mat image);

    //Положение курсора на экране (не != изображению при масштабе)
    QPointF MousePos;

    //размеры экрана
    QSizeF VideoSize;

    //положение метки сопровождения на изображении (верхний левый угол)
    QPointF CrossPos;

    //признак сопровождения
    Track Tracking = Track::NO;

    double ImageX, ImageY;
    //сторона зоны сопровождения
    int TrackingArea, TrackingAreaDiv2, TrackingAreaDiv4;

    //кол-во пикселей, по которому определяется порог в гистограмме для F8
    //зависит от размеров выборки. Желательно завести этот параметр фильтра снаружи
    int MaxPorog=50;



    //раскраски
    void draw(QImage &image);
    void drawAuto(QPainter& painter);
    void drawManual(QPainter& painter);

    //сопровождение
    void adaptiveThreshold(const cv::Mat& image, cv::Mat &result, bool WhiteFlag,int x, int y); //whiteFlag - сопровождение белого обьекта на темном фоне

    void histo(const cv::Mat &image, cv::Mat &histogram);

};
