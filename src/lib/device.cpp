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

#include "device.h"
#include "deviceinterface.h"
#include "dbushelper.h"
#include "exceptions.h"

#include <QDBusObjectPath>

using namespace Bolt;

using DeviceInterface = org::freedesktop::bolt1::Device;

Device::Device(QObject *parent)
    : QObject(parent)
{}

Device::Device(const QDBusObjectPath &path, QObject *parent)
    : QObject(parent)
    , mInterface(new DeviceInterface(
        DBusHelper::serviceName(), path.path(), DBusHelper::connection()))
    , mDBusPath(path)
{
    if (!mInterface->isValid()) {
        throw DBusException(QStringLiteral("Failed to obtain DBus interface for device %1: %2")
                .arg(path.path(), DBusHelper::connection().lastError().message()));
    }
}

Device::~Device()
{}

QDBusObjectPath Device::dbusPath() const
{
    return mDBusPath;
}

QString Device::uid() const
{
    return mInterface ? mInterface->uid() : QString();
}

QString Device::name() const
{
    return mInterface ? mInterface->name() : QString();
}

QString Device::vendor() const
{
    return mInterface ? mInterface->vendor() : QString();
}

Type Device::type() const
{
    return mInterface ? typeFromString(mInterface->type()) : Type::Unknown;
}

Status Device::status() const
{
    return mInterface ? statusFromString(mInterface->status()) : Status::Unknown;
}

AuthFlags Device::authFlags() const
{
    return mInterface ? authFlagsFromString(mInterface->authFlags()) : Auth::None;
}

QString Device::parent() const
{
    return mInterface ? mInterface->parentUid() : QString();
}

QString Device::sysfsPath() const
{
    return mInterface ? mInterface->sysfsPath() : QString();
}

QDateTime Device::connectTime() const
{
    return mInterface ? QDateTime::fromTime_t(mInterface->connectTime()) : QDateTime();
}

QDateTime Device::authorizeTime() const
{
    return mInterface ? QDateTime::fromTime_t(mInterface->authorizeTime()) : QDateTime();
}

bool Device::stored() const
{
    return mInterface ? mInterface->stored() : false;
}

Policy Device::policy() const
{
    return mInterface ? policyFromString(mInterface->policy()) : Policy::Unknown;
}

KeyState Device::keyState() const
{
    return mInterface ? keyStateFromString(mInterface->key()) : KeyState::Unknown;
}

QDateTime Device::storeTime() const
{
    return mInterface ? QDateTime::fromTime_t(mInterface->storeTime()) : QDateTime();
}

QString Device::label() const
{
    return mInterface ? mInterface->label() : QString();
}

void Device::authorize(AuthFlags authFlags)
{
    mInterface->Authorize(authFlagsToString(authFlags));
}

