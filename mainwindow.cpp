#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // テスト用にアスキージャパンのRSS.XMLファイルURLを初期値に設定
    ui->lineEdit->setText("http://rss.rssad.jp/rss/ascii/rss.xml");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMenuFileExit_triggered()
{
    // 終了処理
    this->close();
}

void MainWindow::on_actionMenuFileOpen_triggered()
{
    QMessageBox mesbox;
    mesbox.setText(tr("処理ないです。"));
    mesbox.exec();
}

void MainWindow::DomParser()
{

    // DOMによるXmlファイルの解析処理
    while (!xmlstream.atEnd()) {
        xmlstream.readNext();
        if(xmlstream.isStartElement()){
            // エレメントの開始
            if(xmlstream.name() == "item"){
                if(titleStr != ""){
                    // 取得したタイトルとリンクURLを設定
                    ui->listWidget->addItem(titleStr);
                    linkList.append(linkStr);
                }
                titleStr.clear();
                linkStr.clear();
            }
            // カレントのタグ
            currentTag = xmlstream.name().toString();
        } else if(xmlstream.isEndElement()){
            //エレメントの終了
            if(xmlstream.name() == "item"){
                // 取得したタイトルとリンクURLを設定
                ui->listWidget->addItem(titleStr);
                linkList.append(linkStr);

                titleStr.clear();
                linkStr.clear();

            }
        } else if(xmlstream.isCharacters() && !xmlstream.isWhitespace()){
            // 文字の取得
            if (currentTag == "title")
                   titleStr += xmlstream.text().toString(); //タイトルの文字を追加
            else if (currentTag == "link")
                   linkStr += xmlstream.text().toString();  //リンクURLの文字を追加

        }
    }

}

void MainWindow::on_rssDownButton_clicked()
{
    // ダウンロードのURL設定
    QString urlStr = ui->lineEdit->text();

    this->downloadFile(urlStr);
}

// HTTPによるファイルダウンロード
void MainWindow::downloadFile(QString url)
{
    // ネッットワークマネージャー生成
    manager = new QNetworkAccessManager(this);
    // ネットワークマネージャーリクエスト実行後のメソッド設定
    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
    // GETリクエスト実行
    manager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::replyFinished (QNetworkReply *reply)
{
    // リクエストのエラー処理
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {
        // デバッグ リクエストに成功した場合の受信
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        // Xmlストリームバッファーへ受信内容を追加
        xmlstream.addData(reply->readAll());
        this->DomParser();


        /*
        QFile *file = new QFile(outfilename);
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;
        */
    }

    reply->deleteLater();
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    // リストのアクティブタイトルをダブルクリックした時の処理
    //qDebug() << linkList[index.row()];
    QUrl url = linkList[index.row()]; // リンク文字列をURLに設定
    ui->webView->setUrl(url); // WebKitブラウザーに設定する。
}
