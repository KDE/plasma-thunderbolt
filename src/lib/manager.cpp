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

#include "manager.h"
#include "device.h"
#include "managerinterface.h"
#include "dbushelper.h"
#include "libkbolt_debug.h"

using namespace Bolt;

using ManagerInterface = org::freedesktop::bolt1::Manager;

Manager::Manager(QObject *parent)
    : QObject(parent)
    , mInterface(new ManagerInterface(
        DBusHelper::serviceName(), QStringLiteral("/org/freedesktop/bolt"),
        DBusHelper::connection()))
{
    if (!mInterface->isValid()) {
        qCWarning(log_libkbolt, "Failed to connect to Bolt manager DBus interface: %s",
                    qUtf8Printable(mInterface->lastError().message()));
        return;
    }

    connect(mInterface.get(), &ManagerInterface::DeviceAdded,
            this, [this](const QDBusObjectPath &path) {
                auto device = Device::create(path, this);
                if (device) {
                    mDevices.push_back(device);
                    qCDebug(log_libkbolt, "New device %s (%s) added",
                            qUtf8Printable(device->uid()), qUtf8Printable(device->name()));
                    Q_EMIT deviceAdded(device);
                }
            });
    connect(mInterface.get(), &ManagerInterface::DeviceRemoved,
            this, [this](const QDBusObjectPath &path) {
                if (auto device = this->device(path)) {
                    mDevices.removeOne(device);
                    qCDebug(log_libkbolt, "Device %s (%s) removed",
                            qUtf8Printable(device->uid()), qUtf8Printable(device->name()));
                    Q_EMIT deviceRemoved(device);
                }
            });

    const auto devicePaths = mInterface->ListDevices().argumentAt<0>();
    for (const auto &devicePath : devicePaths) {
        auto device = Device::create(devicePath, this);
        if (device) {
            mDevices.push_back(device);
        }
    }
}

Manager::~Manager()
{
}

bool Manager::isAvailable() const
{
    return mInterface.get() && mInterface->isValid();
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
    const auto policy = mInterface->defaultPolicy();
    if (!mInterface->isValid() || policy.isEmpty()) {
        return Policy::Unknown;
    }
    return policyFromString(policy);
}

Security Manager::securityLevel() const
{
    const auto level = mInterface->securityLevel();
    if (!mInterface->isValid() || level.isEmpty()) {
        return Security::Unknown;
    }
    return securityFromString(level);
}

AuthMode Manager::authMode() const
{
    const auto mode = mInterface->authMode();
    if (!mInterface->isValid() || mode.isEmpty()) {
        return AuthMode::Disabled;
    }
    return authModeFromString(mode);
}

void Manager::setAuthMode(AuthMode mode)
{
    mInterface->setAuthMode(authModeToString(mode));
}

QSharedPointer<Device> Manager::device(std::function<bool(const QSharedPointer<Device> &)> &&match) const
{
    auto device = std::find_if(mDevices.cbegin(), mDevices.cend(), std::move(match));
    return device == mDevices.cend() ? QSharedPointer<Device>() : *device;
}

QSharedPointer<Device> Manager::device(const QString &uid) const
{
    return device([uid](const QSharedPointer<Device> &device) {
        return device->uid() == uid;
    });
}

QSharedPointer<Device> Manager::device(const QDBusObjectPath &path) const
{
    return device([path](const QSharedPointer<Device> &device) {
        return device->dbusPath() == path;
    });
}

QList<QSharedPointer<Device>> Manager::devices() const
{
    return mDevices;
}

void Manager::enrollDevice(const QString &uid, Policy policy, AuthFlags authFlags)
{
    qCDebug(log_libkbolt, "Enrolling device %s with policy %s and flags %s",
            qUtf8Printable(uid), qUtf8Printable(policyToString(policy)),
            qUtf8Printable(authFlagsToString(authFlags)));

    mInterface->EnrollDevice(uid, policyToString(policy), authFlagsToString(authFlags));
}

void Manager::forgetDevice(const QString &uid)
{
    qCDebug(log_libkbolt, "Forgetting device %s", qUtf8Printable(uid));
    mInterface->ForgetDevice(uid);
}
