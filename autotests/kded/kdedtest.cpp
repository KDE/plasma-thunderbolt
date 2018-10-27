#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "fakeserver.h"
#include "fakemanager.h"
#include "fakedevice.h"

#include "lib/device.h"
#include "lib/exceptions.h"

#include "kded_bolt.h"

#include <memory>

class TestableKDEDBolt : public KDEDBolt
{
    Q_OBJECT
public:
    using KDEDBolt::KDEDBolt;

Q_SIGNALS:
    void deviceNotify(const QSharedPointer<Bolt::Device> &device);

protected:
    void notify(const QSharedPointer<Bolt::Device> &device) override
    {
        Q_EMIT deviceNotify(device);
    }
};


class KDEDTest : public QObject
{
    Q_OBJECT
public:
    KDEDTest()
        : QObject()
    {
        qputenv("BOLT_DEBUG", "1");
        qRegisterMetaType<QSharedPointer<Bolt::Device>>();
    }

private Q_SLOTS:
    void testShouldNotify()
    {
        QScopedPointer<FakeServer> fakeServer;
        try {
            fakeServer.reset(new FakeServer);
            QVERIFY(fakeServer->wait());
        } catch (const Bolt::DBusException &e) {
            qCritical("DBus error: %s", e.what());
            QFAIL("Caught exception");
        }

        TestableKDEDBolt kded(nullptr, {});
        QSignalSpy notifySpy(&kded, &TestableKDEDBolt::deviceNotify);
        QVERIFY(notifySpy.isValid());

        // Add unauthorized device
        auto fakeManager = fakeServer->manager();
        {
            auto fakeDevice = std::make_unique<FakeDevice>(QStringLiteral("Device1"));
            fakeDevice->setStatus(QStringLiteral("connected"));
            fakeDevice->setAuthFlags(QStringLiteral("none"));
            fakeManager->addDevice(std::move(fakeDevice));

            QTRY_COMPARE(notifySpy.size(), 1);
            const auto device = notifySpy[0][0].value<QSharedPointer<Bolt::Device>>();
            QCOMPARE(device->uid(), QStringLiteral("Device1"));
            QCOMPARE(device->authFlags(), Bolt::Auth::None);
            QCOMPARE(device->status(), Bolt::Status::Connected);
        }

        // Add authorized device
        notifySpy.clear();
        {
            auto fakeDevice = std::make_unique<FakeDevice>(QStringLiteral("Device2"));
            fakeDevice->setStatus(QStringLiteral("authorized"));
            fakeDevice->setAuthFlags(QStringLiteral("nokey | boot"));
            fakeManager->addDevice(std::move(fakeDevice));

            QTest::qWait(200);
            QVERIFY(notifySpy.empty());
        }
    }
};

QTEST_MAIN(KDEDTest)

#include "kdedtest.moc"
