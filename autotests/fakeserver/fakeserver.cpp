#include "fakeserver.h"
#include "fakemanager.h"
#include "exceptions.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>
#include <QTest>

FakeServer::FakeServer(const QString &filename)
{
    QFile jsonFile(filename);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qFatal("Failed to open file %s: %s", qUtf8Printable(filename), qUtf8Printable(jsonFile.errorString()));
        QFAIL("Failed to open source file");
        return;
    }

    const auto doc = QJsonDocument::fromJson(jsonFile.readAll());

    if (!QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.fakebolt"))) {
        qFatal("Failed to register org.kde.fakebolt service: %s",
               qUtf8Printable(QDBusConnection::sessionBus().lastError().message()));
        QFAIL("Failed to register org.kde.fakebolt service");
        return;
    }

    try {
        mManager.reset(new FakeManager(doc.object()));
    } catch (const DBusException &e) {
        qFatal("DBus error: %s", e.what());
        QFAIL("Failed to setup FakeManager");
        return;
    }
}

FakeServer::FakeServer()
    : mManager(new FakeManager)
{}

FakeServer::~FakeServer()
{
}

FakeManager *FakeServer::manager() const
{
    return mManager.get();
}
