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

#include "fakedevice.h"
#include "fakedeviceadaptor.h"
#include "exceptions.h"

#include <QDBusConnection>
#include <QDBusError>

FakeDevice::FakeDevice(const QJsonObject &json, QObject *parent)
    : QObject(parent)
    , mUid(json[QStringLiteral("Uid")].toString())
    , mName(json[QStringLiteral("Name")].toString())
    , mVendor(json[QStringLiteral("Vendor")].toString())
    , mType(json[QStringLiteral("Type")].toString())
    , mStatus(json[QStringLiteral("Status")].toString())
    , mAuthFlags(json[QStringLiteral("AuthFlags")].toString())
    , mParent(json[QStringLiteral("Parent")].toString())
    , mSysfsPath(json[QStringLiteral("SysfsPath")].toString())
    , mPolicy(json[QStringLiteral("Policy")].toString())
    , mKey(json[QStringLiteral("Key")].toString())
    , mLabel(json[QStringLiteral("Label")].toString())
    , mConnectTime(json[QStringLiteral("ConnectTime")].toVariant().value<quint64>())
    , mAuthorizeTime(json[QStringLiteral("AuthorizeTime")].toVariant().value<quint64>())
    , mStoreTime(json[QStringLiteral("StoreTime")].toVariant().value<quint64>())
    , mStored(json[QStringLiteral("Stored")].toBool())
{
    new FakeDeviceAdaptor(this);
    auto bus = QDBusConnection::sessionBus();
    if (!bus.registerObject(dbusPath().path(), this)) {
        throw DBusException(QStringLiteral("Failed to register device %1 to DBus: %2")
                .arg(mUid, bus.lastError().message()));
    }
}

FakeDevice::~FakeDevice()
{
    QDBusConnection::sessionBus().unregisterObject(dbusPath().path());
}

QDBusObjectPath FakeDevice::dbusPath() const
{
    return QDBusObjectPath(QStringLiteral("/org/freedesktop/bolt/devices/%1")
            .arg(QString(mUid).replace(QLatin1Char('-'), QLatin1Char('_'))));
}

QString FakeDevice::uid() const
{
    return mUid;
}

QString FakeDevice::name() const
{
    return mName;
}

QString FakeDevice::vendor() const
{
    return mVendor;
}

QString FakeDevice::type() const
{
    return mType;
}

QString FakeDevice::status() const
{
    return mStatus;
}

QString FakeDevice::authFlags() const
{
    return mAuthFlags;
}

QString FakeDevice::parent() const
{
    return mParent;
}

QString FakeDevice::sysfsPath() const
{
    return mSysfsPath;
}

bool FakeDevice::stored() const
{
    return mStored;
}

QString FakeDevice::policy() const
{
    return mPolicy;
}

QString FakeDevice::key() const
{
    return mKey;
}

QString FakeDevice::label() const
{
    return mLabel;
}

void FakeDevice::setLabel(const QString &label)
{
    mLabel = label;
}

quint64 FakeDevice::connectTime() const
{
    return mConnectTime;
}

quint64 FakeDevice::authorizeTime() const
{
    return mAuthorizeTime;
}

quint64 FakeDevice::storeTime() const
{
    return mStoreTime;
}

void FakeDevice::Authorize(const QString &flags)
{
    Q_UNUSED(flags);

    // TODO
}
