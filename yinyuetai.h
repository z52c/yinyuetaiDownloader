#ifndef YINYUETAI_H
#define YINYUETAI_H

#include <QObject>
#include "downloader.h"
#include "daye.h"

class yinyuetai : public QObject
{
    Q_OBJECT
public:
    explicit yinyuetai(QObject *parent = nullptr);
    void init(QString inUrl);
    QString getTile();
    QString getHCLink();
    QString getHDLink();
    QString getHELink();

signals:
    void finished(bool);
    void status(QString);
private slots:
    void pageDownloaded();
    void downloadFailed(QString inStr);
    void infoDownloaded();

private:
    downloader *dPage;
    downloader *dInfo;
    QString vid;
    QString title;
    QString hcLink;
    QString hdLink;
    QString heLink;
};

#endif // YINYUETAI_H
