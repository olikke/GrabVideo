#pragma once

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QQuickImageProvider>



class ImageProvider : public QObject,public QQuickImageProvider
{
    Q_OBJECT
public:
    ImageProvider(QObject *parent = nullptr,QSize size =QSize(720,576));

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;



public slots:
    void updateImage(const QImage im);

signals:
    void imageChanged();
private:
    QImage image;
};
