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

#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

#include "fakedevice.h"
#include "fakemanager.h"
#include "fakeserver.h"

#include "device.h"
#include "manager.h"

#include <memory>

Q_DECLARE_METATYPE(QSharedPointer<Bolt::Device>)

class ManagerTest : public QObject
{
    Q_OBJECT
public:
    ManagerTest()
        : QObject()
    {
        FakeServer::enableFakeEnv();
        qRegisterMetaType<QSharedPointer<Bolt::Device>>();
    }

private Q_SLOTS:
    void testDeviceAddedRemoved()
    {
        std::unique_ptr<FakeServer> server;
        try {
            server = std::make_unique<FakeServer>();
        } catch (const FakeServerException &e) {
            qWarning("Fake server exception: %s", e.what());
            QFAIL("Exception server caught");
        }

        auto fakeManager = server->manager();

        Bolt::Manager manager;
        QVERIFY(manager.isAvailable());

        QSignalSpy addSpy(&manager, &Bolt::Manager::deviceAdded);
        QVERIFY(addSpy.isValid());

        FakeDevice *fakeDevice = nullptr;
        try {
            fakeDevice = fakeManager->addDevice(std::make_unique<FakeDevice>(QStringLiteral("device1")));
        } catch (const FakeDeviceException &e) {
            qWarning("Fake device exception: %s", e.what());
            QFAIL("Caught device exception");
        }
        QTRY_COMPARE(addSpy.size(), 1);
        auto device = addSpy.first().first().value<QSharedPointer<Bolt::Device>>();
        QCOMPARE(device->uid(), fakeDevice->uid());

        QSignalSpy removeSpy(&manager, &Bolt::Manager::deviceRemoved);
        QVERIFY(removeSpy.isValid());
        fakeManager->removeDevice(fakeDevice->uid());
        QTRY_COMPARE(removeSpy.size(), 1);
        QCOMPARE(removeSpy.first().first().value<QSharedPointer<Bolt::Device>>(), device);
    }
};

QTEST_GUILESS_MAIN(ManagerTest)

#include "managertest.moc"
