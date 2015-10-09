#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QMainWindow>
#endif

#include <QMessageBox>
#include <QWebView>
#include <QXmlStreamReader>
#include <QFile>
#include <QTreeWidget>
#include <QDomDocument>
#include <QtNetwork>
#include <QDebug>
#include <QList>


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
    void on_actionMenuFileExit_triggered();

    void on_actionMenuFileOpen_triggered();

    void on_rssDownButton_clicked();

    void replyFinished (QNetworkReply *reply);

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QDomDocument domDoc;
    QXmlStreamReader xmlstream;
    QTreeWidgetItem tree;
    QList<QString> linkList;
    QString currentTag;
    QString linkStr;
    QString titleStr;
    void DomParser();
    void downloadFile(QString url);
};

#endif // MAINWINDOW_H
