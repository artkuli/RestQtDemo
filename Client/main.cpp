#include <QCoreApplication>

#include "Client.h"
#include <QNetworkReply>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;
    auto reply = client.sendGetRequest(QUrl("http://httpbin.org/get"));
    QObject::connect(reply, &QNetworkReply::finished, [&client, reply](){
        qDebug() << reply->readAll();
        reply->deleteLater();
        qApp->exit();
    });

    return a.exec();
}

#include "main.moc"
