#include "fakeserver.h"
#include "fakemanager.h"
#include "exceptions.h"

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
        QFAIL("Failed to open source file");
        return;
    }

    const auto doc = QJsonDocument::fromJson(jsonFile.readAll());

    if (!QDBusConnection::sessionBus().registerService(OrgKdeFakebolt)) {
        qCritical("Failed to register org.kde.fakebolt service: %s",
               qUtf8Printable(QDBusConnection::sessionBus().lastError().message()));
        QFAIL("Failed to register org.kde.fakebolt service");
        return;
    }

    mManager.reset(new FakeManager(doc.object()));
}

FakeServer::FakeServer()
{
    if (!QDBusConnection::sessionBus().registerService(OrgKdeFakebolt)) {
        qCritical("Failed to register org.kde.fakebolt service :%s",
                qUtf8Printable(QDBusConnection::sessionBus().lastError().message()));
        QFAIL("Failed to register org.kde.fakebolt service.");
        return;
    }

    mManager.reset(new FakeManager());
}

FakeServer::~FakeServer()
{
}

void FakeServer::enableFakeEnv()
{
    qputenv("KBOLT_FAKE", "1");
}

bool FakeServer::wait() const
{
    auto bus = QDBusConnection::sessionBus();
    if (bus.interface()->isServiceRegistered(OrgKdeFakebolt)) {
        return true;
    }
    QEventLoop ev;
    QObject::connect(bus.interface(), &QDBusConnectionInterface::serviceRegistered,
            [&ev](const QString &service) {
                if (service == OrgKdeFakebolt) {
                    ev.exit(0);
                }
            });
    QTimer::singleShot(5000, [&ev]() {
                qCritical("Timeout waiting for %s", qUtf8Printable(OrgKdeFakebolt));
                ev.exit(1);
                QFAIL("Timeout waiting for DBus service.");
            });
    return ev.exec() == 0;
}

FakeManager *FakeServer::manager() const
{
    return mManager.get();
}
