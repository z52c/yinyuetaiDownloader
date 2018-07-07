#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "yinyuetai.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void yinyuetaiFinished(bool inS);
    void status(QString inStr);
    void on_pushButtonGetVideoLink_clicked();
    void downloadProgress(qint64 r,qint64 t);
    void videoDownloaded();
    void videoDownloadFailed(QString inStr);
    void videoLinkRedirected(QString inStr);
    void on_pushButtonHCDownload_clicked();

    void on_pushButtonHDDownload_clicked();

    void on_pushButtonHEDownload_clicked();

private:
    Ui::MainWindow *ui;
    yinyuetai *yyt;
    downloader *d;
    QString fileName;

    void doJob();
};

#endif // MAINWINDOW_H
