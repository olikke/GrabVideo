#include "log.h"

Log::Log(bool needGUI,QString caption, QObject *parent) :
    QObject(parent),
    NeedGUI(needGUI)
{  
       QString DirName=QDir::currentPath()+QString("/LOG/");
        if (!QDir(DirName).exists())
            if (!QDir().mkdir(DirName))
            {
                DirName=QDir::currentPath();//QDir::currentPath().toLocal8Bit().constData();
                qWarning()<<QString("Невозможно создать папку для записи %1").arg(DirName);
            }
        DirName+="/"+caption+QDateTime::currentDateTime().toString("yyyy_MM_dd hh_mm_ss__zzz");
        LogFile=new QFile(DirName);
        if (!LogFile->open(QFile::Append | QFile::Text))
        {
            LogFile->~QFile();
            LogFile=nullptr;
        }
}

QStringList Log::getLogger()
{
    return Logger;
}

void Log::Message(LoggerMsg lm, QString message)
{
    QString temp=QDateTime::currentDateTime().toString("hh:mm:ss.zzz   ")+toString(lm)+" "+message;
    if (NeedGUI)
    {
        Logger.push_front(temp);
        emit loggerChanged();
    }
    if (LogFile)
    {
        QTextStream out(LogFile);
        out<<temp<<"\n";
        out.flush();
    }
}

QString Log::toString(LoggerMsg ms)
{
    switch (ms) {
    case LoggerMsg::Info: return "INF";
    case LoggerMsg::Warning: return "WRN";
    case LoggerMsg::Fatal: return "FTL";
    }
    return "UNK";
}
