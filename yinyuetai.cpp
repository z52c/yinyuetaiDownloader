#include "yinyuetai.h"
#include <QtDebug>

yinyuetai::yinyuetai(QObject *parent) : QObject(parent)
{
    dPage=new downloader();
    connect(dPage,SIGNAL(finished()),this,SLOT(pageDownloaded()));
    connect(dPage,SIGNAL(downloadError(QString)),this,SLOT(downloadFailed(QString)));
    connect(dPage,SIGNAL(redirected(QString)),this,SLOT(downloadFailed(QString)));

    dInfo=new downloader();
    connect(dInfo,SIGNAL(finished()),this,SLOT(infoDownloaded()));
    connect(dInfo,SIGNAL(downloadError(QString)),this,SLOT(downloadFailed(QString)));
    connect(dInfo,SIGNAL(redirected(QString)),this,SLOT(downloadFailed(QString)));
}

void yinyuetai::init(QString inUrl)
{
    vid=inUrl.split("/").last();
    qDebug()<<vid;
    dPage->init(inUrl,"page.html");
    dPage->setUserAgent(UA);
    dPage->doGet();
}

void yinyuetai::downloadFailed(QString inStr)
{
    emit status(inStr);
    emit finished(false);
}

void yinyuetai::pageDownloaded()
{
    title=QString("");
    QFile file("page.html");
    char tmp[100];
    if(!file.open(QIODevice::ReadOnly))
    {
        emit status("page.html 打开失败");
        emit finished(false);
        return;
    }
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    QString a;
    while(!stream.atEnd())
    {
        a=stream.readLine();
        if(a.contains("og:title"))
        {
            a=stream.readLine();
            getStringBetweenAandB(a.toStdString().c_str(),"content=\"","\"",tmp);
            title=tmp;
            qDebug()<<title;
            break;
        }
    }
    file.close();
    dInfo->init(QString("http://www.yinyuetai.com/api/info/get-video-urls?json=true&videoId=%1").arg(vid),"info.json");
    dInfo->setUserAgent(UA);
    dInfo->doGet();
}

void yinyuetai::infoDownloaded()
{
    QFile file("info.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        emit status("info.json 打开失败");
        emit finished(false);
        return;
    }
    QString line=file.readLine();
    char tmp[300];
    if(-2==getStringBetweenAandB(line.toStdString().c_str(),"hcVideoUrl\":\"","\"",tmp))
        hcLink="";
    else
        hcLink=tmp;
    if(-2==getStringBetweenAandB(line.toStdString().c_str(),"hdVideoUrl\":\"","\"",tmp))
        hdLink="";
    else
        hdLink=tmp;
    if(-2==getStringBetweenAandB(line.toStdString().c_str(),"heVideoUrl\":\"","\"",tmp))
        heLink="";
    else
        heLink=tmp;

    file.close();
    emit finished(true);
}

QString yinyuetai::getHCLink()
{
    return hcLink;
}
QString yinyuetai::getHDLink()
{
    return hdLink;
}
QString yinyuetai::getHELink()
{
    return heLink;
}
QString yinyuetai::getTile()
{
    return title;
}
