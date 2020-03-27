#pragma once

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <iostream>
#include <memory>
#include <QDebug>
#include "opencv2/opencv.hpp"

class SampleQueue : public QObject
{
    Q_OBJECT
public:
    explicit SampleQueue(QObject *parent = nullptr);
    cv::Mat Dequeue();
    int CountQueue();
private:
    QMutex* mMutex;
    QQueue<cv::Mat> mQueue;
    int MaxSampleCount;
    void enqueue(cv::Mat value);
 public slots:
    void Clear();
    void Enqueue(cv::Mat value);
 signals:
    void NewData();
protected:

};


