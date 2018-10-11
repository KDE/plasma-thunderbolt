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

#ifndef FAKEDEVICE_H
#define FAKEDEVICE_H

#include <QObject>
#include <QDBusObjectPath>
#include <QJsonObject>

class FakeDevice : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.bolt1.Device")

    Q_PROPERTY(QString Uid READ uid CONSTANT)
    Q_PROPERTY(QString Name READ name CONSTANT)
    Q_PROPERTY(QString Vendor READ vendor CONSTANT)
    Q_PROPERTY(QString Type READ type CONSTANT)
    Q_PROPERTY(QString Status READ status CONSTANT)
    Q_PROPERTY(QString AuthFlags READ authFlags CONSTANT)
    Q_PROPERTY(QString Parent READ parent CONSTANT)
    Q_PROPERTY(QString SysfsPath READ sysfsPath CONSTANT)
    Q_PROPERTY(bool Stored READ stored CONSTANT)
    Q_PROPERTY(QString Policy READ policy CONSTANT)
    Q_PROPERTY(QString Key READ key CONSTANT)
    Q_PROPERTY(QString Label READ label WRITE setLabel)
    Q_PROPERTY(quint64 ConnectTime READ connectTime CONSTANT)
    Q_PROPERTY(quint64 AuthorizeTime READ authorizeTime CONSTANT)
    Q_PROPERTY(quint64 StoreTime READ storeTime CONSTANT)
public:
    explicit FakeDevice(const QJsonObject &json, QObject *parent = nullptr);
    ~FakeDevice() override;

    QDBusObjectPath dbusPath() const;

    QString uid() const;
    QString name() const;
    QString vendor() const;
    QString type() const;
    QString status() const;
    QString authFlags() const;
    QString parent() const;
    QString sysfsPath() const;
    bool stored() const;
    QString policy() const;
    QString key() const;
    QString label() const;
    void setLabel(const QString &label);
    quint64 connectTime() const;
    quint64 authorizeTime() const;
    quint64 storeTime() const;

public Q_SLOTS:
    void Authorize(const QString &flags);

private:
    QDBusObjectPath mDBusPath;

    QString mUid;
    QString mName;
    QString mVendor;
    QString mType;
    QString mStatus;
    QString mAuthFlags;
    QString mParent;
    QString mSysfsPath;
    QString mPolicy;
    QString mKey;
    QString mLabel;
    quint64 mConnectTime = 0;
    quint64 mAuthorizeTime = 0;
    quint64 mStoreTime = 0;
    bool mStored = false;
};

#endif

