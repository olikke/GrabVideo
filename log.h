#pragma once

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>

enum class LoggerMsg: quint8
{
    Info,
    Warning,
    Fatal
};
//dont foreget qRegisterMetaType<LoggerMsg>("LoggerMsg"); in main function


class Log : public QObject
{
    Q_OBJECT
public:
    Log(bool needGUI, QString caption = "", QObject *parent = nullptr);

    Q_PROPERTY(QStringList logger READ getLogger NOTIFY loggerChanged)
    QStringList getLogger();

signals:
    void loggerChanged();
public slots:
    void Message(LoggerMsg lm,  QString message);
private:
    QString toString(LoggerMsg ms);

    QFile* LogFile;
    QStringList Logger;
    bool NeedGUI;
};


