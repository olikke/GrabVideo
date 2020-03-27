#include "imageprocessing.h"

void Time(std::chrono::high_resolution_clock::time_point &t1)
{
    t1=std::chrono::high_resolution_clock::now();
}

void CalcAndPrintDuration(std::chrono::high_resolution_clock::time_point const &t1,
                          std::chrono::high_resolution_clock::time_point const &t2)
{
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    qDebug()<<duration<<"mcsec";
}

ImageProcessing::ImageProcessing(QObject *parent) : QObject(parent)
{
    TrackingArea=32;
    TrackingAreaDiv2=TrackingArea>>1;
    TrackingAreaDiv4=TrackingArea>>2;
}

void ImageProcessing::setMousePos(QPointF &val) {MousePos=val;}

void ImageProcessing::increaseTrackingArea(bool inc)
{
    int remTrackingArea=TrackingArea;
    if (inc)
    {
        TrackingArea<32? TrackingArea+=8:
                         TrackingArea<64? TrackingArea+=16 : TrackingArea+=0;
    } else
    {
        TrackingArea>32? TrackingArea-=16:
                         TrackingArea>16? TrackingArea-=8 : TrackingArea-=0;
    }
    TrackingAreaDiv2=TrackingArea>>1;
    TrackingAreaDiv4=TrackingArea>>2;
    if (remTrackingArea!=TrackingArea) {
        remTrackingArea=TrackingArea;
        emit trackingAreaChanged(static_cast<qreal>(TrackingArea)/32.);
    }
}

void ImageProcessing::processImage(cv::Mat image)
{
    makeQImage(image);
}


void ImageProcessing::makeQImage(cv::Mat image)
{
    cv::Mat temp; // make the same cv::Mat
    cvtColor(image, temp,CV_BGR2GRAY);
   QImage fullImage= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Grayscale8);
//    cvtColor(image, temp,CV_BGR2RGB);
//    QImage fullImage= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGBA8888);

    //make crop масштаб окна/изображения учесть
    int x=static_cast<int>(MousePos.x());
    int y=static_cast<int>(MousePos.y());


    //check area is visible
    if (x<TrackingAreaDiv2) x=TrackingAreaDiv2;
        else
            if (x+TrackingAreaDiv2+1>image.cols-1) x=image.cols-TrackingAreaDiv2-1;
    if (y<TrackingAreaDiv2) y=TrackingAreaDiv2;
        else
            if (y+TrackingAreaDiv2+1>image.rows-1) y=image.rows-TrackingAreaDiv2-1;

    cv::Mat cropMat,grayMat;

    image(cv::Rect(x-TrackingAreaDiv2,y-TrackingAreaDiv2,TrackingArea+1,TrackingArea+1)).copyTo(cropMat);

    cv::cvtColor(cropMat, grayMat, CV_BGR2GRAY);

    emit newSample(fullImage);

    if (Tracking!=Track::AUTO)
    {
        CrossPos.setX(x);
        CrossPos.setY(y);
        emit targetPosChanged(CrossPos);

        QImage cropImage= QImage((uchar*) grayMat.data,grayMat.cols, grayMat.rows, grayMat.step, QImage::Format_Grayscale8);
        emit littleSample(cropImage);
        return;
    }

    cv::Mat result;
    adaptiveThreshold(grayMat,result,true,x,y);

    QImage cropImage= QImage((uchar*) result.data,result.cols, result.rows, result.step, QImage::Format_Grayscale8);

    emit littleSample(cropImage);

    if (MANUAL_DRAW) draw(fullImage);

}


//F8
void ImageProcessing::adaptiveThreshold(const cv::Mat& image, cv::Mat &result, bool WhiteFlag, int x, int y)
{
    //порог, по которому будет обрезаться гистограмма
    int threshold=255;
    //значение, которое будет применяться в гистограмме
    int maxval=0;
    WhiteFlag? maxval=255 : maxval=0;

    std::chrono::high_resolution_clock::time_point t1,t2;
    Time(t1);
    cv::Mat histogram;
    histo(image,histogram);
    int pixelCount=0;
    if (WhiteFlag)
    for (int i=255; i>=0; i--)
    {
        pixelCount+=histogram.at<float>(i);
        if (pixelCount>=MaxPorog)
        {
            threshold=i;
            break;
        }
    }
    cv::threshold(image,result,threshold,maxval,CV_THRESH_BINARY);

    cv::Moments m = moments(result,true);
    cv::Point p(m.m10/m.m00, m.m01/m.m00);

    CrossPos.setX(x+p.x);
    CrossPos.setY(y+p.y);
    emit targetPosChanged(CrossPos);



//    qDebug()<<p.x<<p.y;


//    Time(t2);
//    CalcAndPrintDuration(t1,t2);
}

void ImageProcessing::histo(const cv::Mat &image, cv::Mat &histogram)
{
    int numBins = 256;
    float range[] = {0, 255};
    const float* lhranges[] = { range};
    cv::calcHist(&image,1,nullptr,cv::Mat(),//Mask-we dont use
                 histogram,1,&numBins,lhranges,true,false);
//    QString temp="";
//    for (int i=0; i<256; i++)
//    {
//        temp+=QString::number(histogram.at<float>(i))+" ";
//        if (i%20==0)
//        {
//            qDebug()<<temp;
//            temp="";
//        }
//    }
//    qDebug()<<temp;
}

void ImageProcessing::draw(QImage &image)
{
    if (Tracking!=Track::NO)
    {
        std::chrono::high_resolution_clock::time_point t1,t2;
        Time(t1);
        QPainter painter ;
        painter.begin(&image);
        painter.setPen(QPen(Qt::green,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        Tracking==Track::AUTO? drawAuto(painter):  drawManual(painter);
        painter.end();
        Time(t2);
        //CalcAndPrintDuration(t1,t2);
    }
    emit targetPosChanged(CrossPos);
}

void ImageProcessing::drawAuto(QPainter &painter)
{
    ImageX=MousePos.x();
    ImageY=MousePos.y();

    int x = static_cast<int>(ImageX);
    int y = static_cast<int>(ImageY);

    painter.drawLine(x-TrackingAreaDiv2,y-TrackingAreaDiv2,x-TrackingAreaDiv4,y-TrackingAreaDiv2);
    painter.drawLine(x-TrackingAreaDiv2,y-TrackingAreaDiv2,x-TrackingAreaDiv2,y-TrackingAreaDiv4);

    painter.drawLine(x+TrackingAreaDiv2,y-TrackingAreaDiv2,x+TrackingAreaDiv4,y-TrackingAreaDiv2);
    painter.drawLine(x+TrackingAreaDiv2,y-TrackingAreaDiv2,x+TrackingAreaDiv2,y-TrackingAreaDiv4);

    painter.drawLine(x-TrackingAreaDiv2,y+TrackingAreaDiv2,x-TrackingAreaDiv4,y+TrackingAreaDiv2);
    painter.drawLine(x-TrackingAreaDiv2,y+TrackingAreaDiv2,x-TrackingAreaDiv2,y+TrackingAreaDiv4);

    painter.drawLine(x+TrackingAreaDiv2,y+TrackingAreaDiv2,x+TrackingAreaDiv4,y+TrackingAreaDiv2);
    painter.drawLine(x+TrackingAreaDiv2,y+TrackingAreaDiv2,x+TrackingAreaDiv2,y+TrackingAreaDiv4);
}

void ImageProcessing::drawManual(QPainter &painter)
{
    ImageX=MousePos.x();
    ImageY=MousePos.y();

    int x = static_cast<int>(ImageX);
    int y = static_cast<int>(ImageY);

    painter.drawLine(x-TrackingAreaDiv2,y,x-TrackingAreaDiv4,y);
    painter.drawLine(x+TrackingAreaDiv2,y,x+TrackingAreaDiv4,y);
    painter.drawLine(x,y-TrackingAreaDiv2,x,y-TrackingAreaDiv4);
    painter.drawLine(x,y+TrackingAreaDiv2,x,y+TrackingAreaDiv4);
}

void ImageProcessing::newData()
{
    SampleQueue* queue=reinterpret_cast<SampleQueue*>(sender());
    if (queue->CountQueue()>0)
    {
        cv::Mat sample=queue->Dequeue();
        if (sample.empty())
        {
           // emit ReadyImage(sample);
            //return empty Image
            return;
        }
        processImage(sample);
    }

}

void ImageProcessing::stop()
{
    //here u can clear all u need
    qDebug()<<"bue";
    this->deleteLater();

}
