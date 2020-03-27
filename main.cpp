#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include "log.h"
#include "VideoGrab.h"
#include "samplequeue.h"
#include "imageprocessing.h"
#include "imageprovider.h"
#include "imageprocessingfront.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<LoggerMsg>("LoggerMsg");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<Track>("Track");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Some organization");

    QApplication app(argc, argv);

    QQmlApplicationEngine * engine=new QQmlApplicationEngine(&app);
    QQmlContext *context = engine->rootContext();


    VideoGrab* grab=new VideoGrab(&app);
    SampleQueue* sampleQueue = new SampleQueue(&app);

    QObject::connect(grab,&VideoGrab::Enqueue,sampleQueue,&SampleQueue::Enqueue,Qt::QueuedConnection);
    ImageProcessing* imageProcessing=new ImageProcessing();

    QObject::connect(sampleQueue,&SampleQueue::NewData,imageProcessing,&ImageProcessing::newData,Qt::QueuedConnection);
    ImageProvider* imageProvider = new ImageProvider(&app,QSize(1440,1152));
    QObject::connect(imageProcessing,&ImageProcessing::newSample,imageProvider,&ImageProvider::updateImage,Qt::QueuedConnection);
    ImageProvider* littleProvider = new ImageProvider(&app,QSize(33,33));
    QObject::connect(imageProcessing,&ImageProcessing::littleSample,littleProvider,&ImageProvider::updateImage,Qt::QueuedConnection);

    ImageProcessingFront* imageFront=new ImageProcessingFront(imageProcessing,&app);
    context->setContextProperty("imageFront",imageFront);


    context->setContextProperty("videoGrab",grab);
    context->setContextProperty("iProvider",imageProvider);
    context->setContextProperty("littleProvider",littleProvider);

    engine->addImageProvider("live",imageProvider);
    engine->addImageProvider("little",littleProvider);

    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine->rootObjects().isEmpty())  return -1;


    return app.exec();
}



