#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "fakeserver.h"
#include "fakemanager.h"
#include "fakedevice.h"

#include "device.h"
#include "manager.h"
#include "exceptions.h"

#include <memory>

class DeviceTest : public QObject
{
    Q_OBJECT
public:
    DeviceTest()
        : QObject()
    {
        FakeServer::enableFakeEnv();
        qRegisterMetaType<QSharedPointer<Bolt::Device>>();
    }

private Q_SLOTS:
    void testAuthorize()
    {
        QScopedPointer<FakeServer> fakeServer;
        try {
            fakeServer.reset(new FakeServer);
            QVERIFY(fakeServer->wait());
        } catch (const Bolt::DBusException &e) {
            qCritical("DBus error: %s", e.what());
            QFAIL("Caught exception");
        }

        auto fakeManager = fakeServer->manager();
        auto fakeDevice = fakeManager->addDevice(
                std::make_unique<FakeDevice>(QStringLiteral("Device1")));
        fakeDevice->setAuthFlags(QStringLiteral("none"));

        QScopedPointer<Bolt::Manager> manager;
        try {
            manager.reset(new Bolt::Manager);
        } catch (const Bolt::DBusException &e) {
            qCritical("DBus error: %s", e.what());
            QFAIL("Failed to connect to DBus server");
        }

        auto device = manager->device(fakeDevice->uid());
        QVERIFY(device);
        QCOMPARE(device->authFlags(), Bolt::Auth::None);
        device->authorize(Bolt::Auth::NoKey | Bolt::Auth::Boot);

        QTRY_COMPARE(fakeDevice->authFlags(), QStringLiteral("nokey | boot"));
    }
};

#include "devicetest.moc"

QTEST_GUILESS_MAIN(DeviceTest)
