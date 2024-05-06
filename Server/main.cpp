#include <QCoreApplication>

#include <QHttpServer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList users = {"testUser"};

    QHttpServer server;
    server.route("/users", QHttpServerRequest::Method::Get, [&users] () {
        return QHttpServerResponse(QJsonArray::fromStringList(users), QHttpServerResponse::StatusCode::Ok);
    });

    server.route("/users", QHttpServerRequest::Method::Post, [&users](const QHttpServerRequest &request) {
        QJsonDocument doc = QJsonDocument::fromJson(request.body());
        const QJsonObject obj = doc.object();
        const QString name = obj["name"].toString();
        if (name.isEmpty()) {
            return QHttpServerResponse("Invalid data", QHttpServerResponse::StatusCode::BadRequest);
        }
        if (users.contains(name)) {
            return QHttpServerResponse("User exists", QHttpServerResponse::StatusCode::BadRequest);
        }
        users.append(name);
        return QHttpServerResponse("User added", QHttpServerResponse::StatusCode::Ok);
    });

    server.route("/users/<arg>", QHttpServerRequest::Method::Delete, [&users] (const QString& name) {
        if (name.isEmpty() || !users.contains(name)) {
            return QHttpServerResponse("Invalid data", QHttpServerResponse::StatusCode::BadRequest);
        }
        users.removeAll(name);
        return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
    });

    const int port = 8080;
    if (server.listen(QHostAddress::Any, port)) {
        qDebug() << "Start listening on port:" << port;
    } else {
        qDebug() << "Cannot start listening on port:" << port;
        exit(1);
    }
    return a.exec();
}
