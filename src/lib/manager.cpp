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

#include "manager.h"
#include "device.h"
#include "managerinterface.h"
#include "dbushelper.h"
#include "exceptions.h"

using namespace Bolt;

using ManagerInterface = org::freedesktop::bolt1::Manager;

Manager::Manager(QObject *parent)
    : QObject(parent)
    , mInterface(new ManagerInterface(
        DBusHelper::serviceName(), QStringLiteral("/org/freedesktop/bolt"),
        DBusHelper::connection()))
{
    if (!mInterface->isValid()) {
        throw DBusException(QStringLiteral("Failed to create Manager DBus interface: %1")
                .arg(DBusHelper::connection().lastError().message()));
    }

    connect(mInterface.get(), &ManagerInterface::DeviceAdded,
            this, [this](const QDBusObjectPath &path) {
                auto device = new Device(path, this);
                mDevices.push_back(device);
                Q_EMIT deviceAdded(device);
            });
    connect(mInterface.get(), &ManagerInterface::DeviceRemoved,
            this, [this](const QDBusObjectPath &path) {
                auto device = std::find_if(
                    mDevices.begin(), mDevices.end(),
                    [path](Device *device) { return device->dbusPath() == path; });
                if (device != mDevices.end()) {
                    mDevices.erase(device);
                    Q_EMIT deviceRemoved(*device);
                    delete *device;
                }
            });

    const auto devicePaths = mInterface->ListDevices().argumentAt<0>();
    for (const auto &devicePath : devicePaths) {
        mDevices.push_back(new Device(devicePath, this));
    }
}

Manager::~Manager()
{
}

uint Manager::version() const
{
    return mInterface->version();
}

bool Manager::isProbing() const
{
    return mInterface->probing();
}

Policy Manager::defaultPolicy() const
{
    return policyFromString(mInterface->defaultPolicy());
}

Security Manager::securityLevel() const
{
    return securityFromString(mInterface->securityLevel());
}

AuthMode Manager::authMode() const
{
    return authModeFromString(mInterface->authMode());
}

Device *Manager::device(const QString &uid) const
{
    auto device = std::find_if(mDevices.cbegin(), mDevices.cend(),
            [uid](Device *device) { return device->uid() == uid; });
    if (device == mDevices.cend()) {
        return nullptr;
    }
    return *device;
}

QList<Device *> Manager::devices() const
{
    return mDevices;
}
