#include "fakeserver.h"
#include "fakemanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusError>
#include <QDebug>
#include <QTest>
#include <QEventLoop>
#include <QTimer>

namespace {

static const QString OrgKdeFakebolt = QStringLiteral("org.kde.fakebolt");

}

FakeServer::FakeServer(const QString &filename)
{
    QFile jsonFile(filename);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qCritical("Failed to open file %s: %s", qUtf8Printable(filename), qUtf8Printable(jsonFile.errorString()));
        throw FakeServerException(
                QStringLiteral("Failed to open file %1: %2").arg(filename, jsonFile.errorString()));
    }

    const auto doc = QJsonDocument::fromJson(jsonFile.readAll());

    if (!QDBusConnection::sessionBus().registerService(OrgKdeFakebolt)) {
        throw FakeServerException(
                QStringLiteral("Failed to register org.kde.fakebolt service: %1")
                    .arg(QDBusConnection::sessionBus().lastError().message()));
    }

    try {
        mManager.reset(new FakeManager(doc.object()));
    } catch (const FakeManagerException &e) {
        throw FakeServerException(e.what());
    }
}

FakeServer::FakeServer()
{
    if (!QDBusConnection::sessionBus().registerService(OrgKdeFakebolt)) {
        throw FakeServerException(
                QStringLiteral("Failed to register org.kde.fakebolt service: %1")
                    .arg(QDBusConnection::sessionBus().lastError().message()));
    }

    try {
        mManager.reset(new FakeManager());
    } catch (FakeManagerException &e) {
        throw FakeServerException(e.what());
    }
}

FakeServer::~FakeServer()
{
}

void FakeServer::enableFakeEnv()
{
    qputenv("KBOLT_FAKE", "1");
}

FakeManager *FakeServer::manager() const
{
    return mManager.get();
}
