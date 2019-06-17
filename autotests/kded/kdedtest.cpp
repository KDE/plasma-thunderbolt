/*
 * Copyright (c) 2018 - 2019  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <QTest>
#include <QSignalSpy>

#include "fakeserver.h"
#include "fakemanager.h"
#include "fakedevice.h"

#include "device.h"

#include "kded_bolt.h"

#include <memory>

class TestableKDEDBolt : public KDEDBolt
{
    Q_OBJECT
public:
    using KDEDBolt::KDEDBolt;

Q_SIGNALS:
    void deviceNotify(const QVector<QSharedPointer<Bolt::Device>> &device);

protected:
    void notify() override
    {
        Q_EMIT deviceNotify(sortDevices(mPendingDevices));
    }
};


class KDEDTest : public QObject
{
    Q_OBJECT
public:
    KDEDTest()
        : QObject()
    {
        FakeServer::enableFakeEnv();
        qRegisterMetaType<QSharedPointer<Bolt::Device>>();
    }

private Q_SLOTS:
    void testShouldNotify()
    {
        std::unique_ptr<FakeServer> fakeServer;
        try {
            fakeServer = std::make_unique<FakeServer>();
        } catch (const FakeServerException &e) {
            qWarning("Fake server exception: %s", e.what());
           QFAIL("Caught server exception");
        }

        TestableKDEDBolt kded(nullptr, {});
        QSignalSpy notifySpy(&kded, &TestableKDEDBolt::deviceNotify);
        QVERIFY(notifySpy.isValid());

        // Add unauthorized device
        auto fakeManager = fakeServer->manager();
        try {
            auto fakeDevice = std::make_unique<FakeDevice>(QStringLiteral("Device1"));
            fakeDevice->setStatus(QStringLiteral("connected"));
            fakeDevice->setAuthFlags(QStringLiteral("none"));
            fakeManager->addDevice(std::move(fakeDevice));

            QTRY_COMPARE(notifySpy.size(), 1);
            const auto devices = notifySpy[0][0].value<QVector<QSharedPointer<Bolt::Device>>>();
            QCOMPARE(devices.size(), 1);
            const auto device = devices.front();
            QCOMPARE(device->uid(), QStringLiteral("Device1"));
            QCOMPARE(device->authFlags(), Bolt::Auth::None);
            QCOMPARE(device->status(), Bolt::Status::Connected);
        } catch (const FakeDeviceException &e) {
            qWarning("Fake device exception: %s", e.what());
            QFAIL("Caught device exception");
        }

        // Add authorized device
        notifySpy.clear();
        try {
            auto fakeDevice = std::make_unique<FakeDevice>(QStringLiteral("Device2"));
            fakeDevice->setStatus(QStringLiteral("authorized"));
            fakeDevice->setAuthFlags(QStringLiteral("nokey | boot"));
            fakeManager->addDevice(std::move(fakeDevice));

            QTest::qWait(200);
            QVERIFY(notifySpy.empty());
        } catch (const FakeDeviceException &e) {
            qWarning("Fake device exception: %s", e.what());
            QFAIL("Caught device exception");
        }
    }
};

QTEST_MAIN(KDEDTest)

#include "kdedtest.moc"
