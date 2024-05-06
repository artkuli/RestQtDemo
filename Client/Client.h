#pragma once

#include <QtNetwork/QNetworkAccessManager>

const QString JSONContentTypeHeader = "application/json";

class Client : public QObject {
    Q_OBJECT
public:
    Client(QObject * parent = nullptr) : QObject(parent) {
        m_manager = new QNetworkAccessManager(this);
    }

    QNetworkReply* sendGetRequest(const QUrl &url) {
        QNetworkRequest request(url);
        return m_manager->get(request);
    }

    QNetworkReply* sendPostRequest(const QUrl &url, const QByteArray &data) {
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, JSONContentTypeHeader);
        return m_manager->post(request, data);
    }

    QNetworkReply* sendDeleteRequest(const QUrl &url) {
        QNetworkRequest request(url);
        return m_manager->deleteResource(request);
    }

private:
    QNetworkAccessManager *m_manager;
};
