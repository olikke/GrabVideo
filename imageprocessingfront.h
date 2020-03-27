#ifndef IMAGEPROCESSINGFRONT_H
#define IMAGEPROCESSINGFRONT_H

#include <QObject>
#include <QThread>

#include "imageprocessing.h"

class ImageProcessingFront : public QObject
{
    Q_OBJECT
public:
    ImageProcessingFront(ImageProcessing* _imagePro, QObject *parent = nullptr);
    ~ImageProcessingFront();

    Q_INVOKABLE void changeTrack(Track tracking);

    Q_INVOKABLE void mousePos(QPointF mousePos);

    Q_INVOKABLE void changeTrackingArea(bool increase);

//    Q_INVOKABLE void useFilter(int filter);

//    Q_INVOKABLE void changeFilterParam(int param);

signals:

    void targetPosChanged(QPointF targetPos);

//    void eof();

//    void targetLost(); //not sure - maybe property better - show color shape

    void trackingAreaChanged(qreal scal);

    //for thread
    void stopAll();

private:
    ImageProcessing* imagePro;
    QThread* thread;
};

#endif // IMAGEPROCESSINGFRONT_H
