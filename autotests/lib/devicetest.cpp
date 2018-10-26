#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "fakeserver.h"
#include "fakemanager.h"
#include "fakedevice.h"

#include "device.h"
#include "manager.h"
#include "exceptions.h"

class DeviceTest : public QObject
{
    Q_OBJECT
public:
    DeviceTest()
        : QObject()
    {
        qputenv("BOLT_DEBUG", "1");
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
        auto fakeDevice = new FakeDevice(QStringLiteral("Device1"));
        fakeManager->addDevice(fakeDevice);

        QScopedPointer<Bolt::Manager> manager;
        try {
            manager.reset(new Bolt::Manager);
        } catch (const Bolt::DBusException &e) {
            qCritical("DBus error: %s", e.what());
            QFAIL("Failed to connect to DBus server");
        }


    }
};

#include "devicetest.moc"

QTEST_GUILESS_MAIN(DeviceTest)
