#include "samplequeue.h"

SampleQueue::SampleQueue(QObject *parent):
    QObject(parent),
    mMutex(new QMutex()),
    MaxSampleCount(5)
{
}

void SampleQueue::enqueue(cv::Mat value)
{
        QMutexLocker locker(mMutex);
        mQueue.enqueue(value);
}

void SampleQueue::Enqueue(cv::Mat value)
{    
    if (mQueue.size()<MaxSampleCount)
    {
        enqueue(value);
    }
    else
    {
        //если очередь переполняется, тупо выкидываем самый старый обьект в очереди
        qDebug()<<"remove";
        if (!mQueue.empty())
        {
            mQueue.dequeue();
        }
        enqueue(value);
    }
    emit NewData();
}

cv::Mat SampleQueue::Dequeue()
{
    cv::Mat value;
    QMutexLocker locker(mMutex);
    if (!mQueue.empty())
    {
        value=mQueue.dequeue();
    };
    return value;
}

void SampleQueue::Clear()
{
    QMutexLocker locker(mMutex);
    mQueue.clear();
}

int SampleQueue::CountQueue()
{
   QMutexLocker locker(mMutex);
    return mQueue.count();
}
