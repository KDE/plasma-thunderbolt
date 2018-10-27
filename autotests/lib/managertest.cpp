#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "fakeserver.h"
#include "fakemanager.h"
#include "fakedevice.h"

#include "lib/manager.h"
#include "lib/device.h"
#include "lib/exceptions.h"

#include <QDebug>

Q_DECLARE_METATYPE(QSharedPointer<Bolt::Device>)

class ManagerTest : public QObject
{
    Q_OBJECT
public:
    ManagerTest()
        : QObject()
    {
        qputenv("BOLT_DEBUG", "1");
        qRegisterMetaType<QSharedPointer<Bolt::Device>>();
    }

private Q_SLOTS:
    void testDeviceAddedRemoved()
    {
        QScopedPointer<FakeServer> server;
        try {
            server.reset(new FakeServer);
        } catch (const Bolt::DBusException &e) {
            qCritical("DBus error: %s", e.what());
            QFAIL("Failed to register on DBus");
        }
        QVERIFY(server->wait());

        auto fakeManager = server->manager();

        QScopedPointer<Bolt::Manager> manager;
        try {
            manager.reset(new Bolt::Manager);
        } catch (Bolt::DBusException &e) {
            qCritical("DBus error: %s", e.what());
            QFAIL("Failed to connect to DBus server");
        }
        QSignalSpy addSpy(manager.get(), &Bolt::Manager::deviceAdded);
        QVERIFY(addSpy.isValid());

        auto fakeDevice = fakeManager->addDevice(
                std::make_unique<FakeDevice>(QStringLiteral("device1")));
        QTRY_COMPARE(addSpy.size(), 1);
        auto device = addSpy.first().first().value<QSharedPointer<Bolt::Device>>();
        QCOMPARE(device->uid(), fakeDevice->uid());

        QSignalSpy removeSpy(manager.get(), &Bolt::Manager::deviceRemoved);
        QVERIFY(removeSpy.isValid());
        fakeManager->removeDevice(fakeDevice->uid());
        QTRY_COMPARE(removeSpy.size(), 1);
        QCOMPARE(removeSpy.first().first().value<QSharedPointer<Bolt::Device>>(), device);
    }
};

QTEST_GUILESS_MAIN(ManagerTest)

#include "managertest.moc"
