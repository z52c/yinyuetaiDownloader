#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    yyt=new yinyuetai();
    connect(yyt,SIGNAL(finished(bool)),this,SLOT(yinyuetaiFinished(bool)));
    connect(yyt,SIGNAL(status(QString)),this,SLOT(status(QString)));
    ui->pushButtonHCDownload->setEnabled(false);
    ui->pushButtonHDDownload->setEnabled(false);
    ui->pushButtonHEDownload->setEnabled(false);

    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(videoDownloaded()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(videoDownloadFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(videoLinkRedirected(QString)));
    connect(d,SIGNAL(progress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::downloadProgress(qint64 r, qint64 t)
{
    ui->progressBar->setMaximum(t);
    ui->progressBar->setValue(r);
}

void  MainWindow::videoDownloaded()
{
    ui->pushButtonHCDownload->setEnabled(true);
    ui->pushButtonHDDownload->setEnabled(true);
    ui->pushButtonHEDownload->setEnabled(true);
    ui->textEdit->append("当前下载已经完成");
}

void MainWindow::videoDownloadFailed(QString inStr)
{
    ui->pushButtonHCDownload->setEnabled(true);
    ui->pushButtonHDDownload->setEnabled(true);
    ui->pushButtonHEDownload->setEnabled(true);
    ui->textEdit->append("当前下载失败");
    ui->textEdit->append(inStr);
}

void MainWindow::videoLinkRedirected(QString inStr)
{
    d->init(inStr,fileName);
    d->doGet();
}

void MainWindow::on_pushButtonGetVideoLink_clicked()
{
    ui->progressBar->setValue(0);
    ui->lineEditHC->clear();
    ui->lineEditHD->clear();
    ui->lineEditHE->clear();
    ui->textEdit->clear();
    ui->pushButtonHCDownload->setEnabled(false);
    ui->pushButtonHDDownload->setEnabled(false);
    ui->pushButtonHEDownload->setEnabled(false);
    QString link=ui->lineEditUrl->text();

    if(link.contains("v.yinyuetai.com/video/"))
    {
        if(link.contains("http"))
        {
            yyt->init(link);
        }
        else
        {
            link=QString("http://%1").arg(link);
            yyt->init(link);
        }
    }
    else
        return;

}

void MainWindow::yinyuetaiFinished(bool inS)
{
    if(inS)
    {
        doJob();
    }
}

void MainWindow::doJob()
{
    ui->labelTitle->setText(yyt->getTile());
    if(!yyt->getHCLink().isEmpty())
    {
        ui->lineEditHC->setText(yyt->getHCLink());
        ui->pushButtonHCDownload->setEnabled(true);
    }
    if(!yyt->getHDLink().isEmpty())
    {
        ui->lineEditHD->setText(yyt->getHDLink());
        ui->pushButtonHDDownload->setEnabled(true);
    }
    if(!yyt->getHELink().isEmpty())
    {
        ui->lineEditHE->setText(yyt->getHELink());
        ui->pushButtonHEDownload->setEnabled(true);
    }
    ui->textEdit->append("链接已经获取，可以点击对应画质后的下载按钮进行下载或者复制链接到迅雷等其他下载工具中下载，也可以直接粘贴到浏览器中进行播放");
}

void MainWindow::status(QString inStr)
{
    ui->textEdit->append(inStr);
}

void MainWindow::on_pushButtonHCDownload_clicked()
{
    fileName=QString("%1-标清.mp4").arg(ui->labelTitle->text());
    d->init(ui->lineEditHC->text(),fileName);
    ui->textEdit->append(QString("准备下载%1-标清.mp4").arg(ui->labelTitle->text()));
    d->setUserAgent(UA);
    d->setReferer("http://www.yinyuetai.com/");
    d->doGet();
    ui->pushButtonHDDownload->setEnabled(false);
    ui->pushButtonHEDownload->setEnabled(false);
}

void MainWindow::on_pushButtonHDDownload_clicked()
{
    fileName=QString("%1-高清.mp4").arg(ui->labelTitle->text());
    d->init(ui->lineEditHD->text(),fileName);
    ui->textEdit->append(QString("准备下载%1-高清.mp4").arg(ui->labelTitle->text()));
    d->setUserAgent(UA);
    d->setReferer("http://www.yinyuetai.com/");
    d->doGet();
    ui->pushButtonHCDownload->setEnabled(false);
    ui->pushButtonHEDownload->setEnabled(false);
}
void MainWindow::on_pushButtonHEDownload_clicked()
{
    fileName=QString("%1-超清.mp4").arg(ui->labelTitle->text());
    d->init(ui->lineEditHE->text(),fileName);
    ui->textEdit->append(QString("准备下载%1-超清.mp4").arg(ui->labelTitle->text()));
    d->setUserAgent(UA);
    d->setReferer("http://www.yinyuetai.com/");
    d->doGet();
    ui->pushButtonHDDownload->setEnabled(false);
    ui->pushButtonHCDownload->setEnabled(false);
}
