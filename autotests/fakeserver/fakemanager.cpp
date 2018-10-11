/*
    Copyright (c) 2018 Daniel Vrátil <dvratil@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "fakemanager.h"
#include "fakemanageradaptor.h"
#include "fakedevice.h"
#include "exceptions.h"

#include <QDBusConnection>
#include <QDBusError>
#include <QJsonArray>

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
        throw DBusException(QStringLiteral("Failed to register FakeManager to DBus: %1")
                .arg(QDBusConnection::sessionBus().lastError().message()));
    }

    const auto jsonDevices = json[QStringLiteral("Devices")].toArray();
    for (const auto &jsonDevice : jsonDevices) {
        auto device = new FakeDevice(jsonDevice.toObject(), this);
        mDevices.insert(device->uid(), device);
    }
}

FakeManager::~FakeManager()
{
    QDBusConnection::sessionBus().unregisterObject(kManagerDBusPath);
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
    Q_UNUSED(uid);
    Q_UNUSED(policy);
    Q_UNUSED(flags);
    // TODO
    return {};
}

void FakeManager::ForgetDevice(const QString &uid)
{
    // TODO? I don't think this is correct...
    auto device = mDevices.find(uid);
    if (device != mDevices.end()) {
        mDevices.erase(device);
        delete *device;
    }
}

