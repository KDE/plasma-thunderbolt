/*
 * Copyright (c) 2018  Daniel Vrátil <dvratil@kde.org>
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

#include "fakemanager.h"
#include "fakemanageradaptor.h"
#include "fakedevice.h"

#include <QDBusConnection>
#include <QDBusError>
#include <QJsonArray>

#include <QDebug>

namespace {
static const QString kManagerDBusPath = QStringLiteral("/org/freedesktop/bolt");
}

FakeManager::FakeManager(const QJsonObject &json, QObject *parent)
    : QObject(parent)
    , mProbing(json[QStringLiteral("Probing")].toBool())
    , mDefaultPolicy(json[QStringLiteral("DefaultPolicy")].toString())
    , mSecurityLevel(json[QStringLiteral("SecurityLevel")].toString())
    , mAuthMode(json[QStringLiteral("AuthMode")].toString())
{
    new FakeManagerAdaptor(this);
    if (!QDBusConnection::sessionBus().registerObject(kManagerDBusPath, this)) {
        throw FakeManagerException(QStringLiteral("Failed to register FakeManager to DBus: %1")
                    .arg(QDBusConnection::sessionBus().lastError().message()));
    }

    const auto jsonDevices = json[QStringLiteral("Devices")].toArray();
    for (const auto &jsonDevice : jsonDevices) {
        auto device = new FakeDevice(jsonDevice.toObject(), this);
        mDevices.insert(device->uid(), device);
    }
}

FakeManager::FakeManager(QObject *parent)
    : QObject(parent)
{
    new FakeManagerAdaptor(this);
    if (!QDBusConnection::sessionBus().registerObject(kManagerDBusPath, this)) {
        throw FakeManagerException(QStringLiteral("Failed to register FakeManager to DBus: %1")
                .arg(QDBusConnection::sessionBus().lastError().message()));
    }
}

FakeManager::~FakeManager()
{
    QDBusConnection::sessionBus().unregisterObject(kManagerDBusPath);
    qDeleteAll(mDevices);
}

FakeDevice *FakeManager::addDevice(std::unique_ptr<FakeDevice> device)
{
    auto ptr = device.release();
    mDevices.insert(ptr->uid(), ptr);
    Q_EMIT DeviceAdded(ptr->dbusPath());
    return ptr;
}

void FakeManager::removeDevice(const QString &uid)
{
    auto deviceIt = mDevices.find(uid);
    if (deviceIt == mDevices.end()) {
        return;
    }
    auto device = *deviceIt;
    mDevices.erase(deviceIt);
    Q_EMIT DeviceRemoved(device->dbusPath());
    device->deleteLater();
}

QList<FakeDevice*> FakeManager::devices() const
{
    return mDevices.values();
}

unsigned int FakeManager::version() const
{
    return 1;
}

bool FakeManager::isProbing() const
{
    return mProbing;
}

QString  FakeManager::defaultPolicy() const
{
    return mDefaultPolicy;
}

QString FakeManager::securityLevel() const
{
    return mSecurityLevel;
}

QString FakeManager::authMode() const
{
    return mAuthMode;
}

void FakeManager::setAuthMode(const QString &authMode)
{
    qDebug("Manager: authMode changed to %s", qUtf8Printable(authMode));
    mAuthMode = authMode;
}

QList<QDBusObjectPath> FakeManager::ListDevices() const
{
    QList<QDBusObjectPath> rv;
    rv.reserve(mDevices.size());
    for (const auto device : mDevices) {
        rv.push_back(device->dbusPath());
    }
    return rv;
}

QDBusObjectPath FakeManager::DeviceByUid(const QString &uid) const
{
    auto device = mDevices.value(uid);
    return device ? device->dbusPath() : QDBusObjectPath();
}

QDBusObjectPath FakeManager::EnrollDevice(const QString &uid,
                                          const QString &policy,
                                          const QString &flags)
{
    auto device = mDevices.value(uid);
    if (policy == QLatin1Literal("default")) {
        device->setPolicy(defaultPolicy());
    } else {
        device->setPolicy(policy);
    }
    device->setAuthFlags(flags);
    device->setStored(true);
    device->setStatus(QLatin1Literal("authorized"));

    return device->dbusPath();
}

void FakeManager::ForgetDevice(const QString &uid)
{
    auto device = mDevices.value(uid);
    device->setStored(false);
    device->setStatus(QLatin1Literal("connected"));
}

