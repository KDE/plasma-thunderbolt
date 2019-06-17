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
#include "manager.h"

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
        std::unique_ptr<FakeServer> fakeServer;
        try {
            fakeServer = std::make_unique<FakeServer>();
        } catch (const FakeServerException &e) {
            qWarning("Fake server exception: %s", e.what());
            QFAIL("Caught server exception");
        }

        auto fakeManager = fakeServer->manager();
        FakeDevice *fakeDevice = nullptr;
        try {
            fakeDevice = fakeManager->addDevice(
                std::make_unique<FakeDevice>(QStringLiteral("Device1")));
        } catch (const FakeDeviceException &e) {
            qWarning("Fake device exception: %s", e.what());
            QFAIL("Caught device exception");
        }
        fakeDevice->setAuthFlags(QStringLiteral("none"));

        Bolt::Manager manager;
        QVERIFY(manager.isAvailable());

        auto device = manager.device(fakeDevice->uid());
        QVERIFY(device);
        QCOMPARE(device->authFlags(), Bolt::Auth::None);
        device->authorize(Bolt::Auth::NoKey | Bolt::Auth::Boot);

        QTRY_COMPARE(fakeDevice->authFlags(), QStringLiteral("nokey | boot"));
    }
};

#include "devicetest.moc"

QTEST_GUILESS_MAIN(DeviceTest)
