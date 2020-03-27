#include "imageprovider.h"

ImageProvider::ImageProvider(QObject *parent,QSize size) : QObject(parent),QQuickImageProvider(QQuickImageProvider::Image)
{
    image = QImage(size.width(),size.height(),QImage::Format_RGB32);
    image.fill(QColor("#2d2d2d"));
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id)
//    if (size) *size=image.size();
//    //olikke - во-первых хз ппонадобиться ли вообще - можно принудительно запретить
//    //во-вторых, может Image в qml сделает это быстрее и сам?
//    if (requestedSize.width()>0 && requestedSize.height()>0)
//    {
//        qDebug()<<"Attention! need scale";
//        QImage image2=image.scaled(requestedSize.width(),requestedSize.height(),Qt::KeepAspectRatio);
//        return image2;
//    }
    return image;
}

void ImageProvider::updateImage(const QImage im)
{
    if (!image.isNull())
    {
        image=im;
        //not sure is required
       // image=im.copy();
        static int count=0;
        image.save(QString::number(count++)+".bmp");
        emit imageChanged();
    }
}
