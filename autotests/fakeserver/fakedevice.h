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

#ifndef FAKEDEVICE_H
#define FAKEDEVICE_H

#include <QObject>
#include <QDBusObjectPath>
#include <QJsonObject>

class FakeDeviceException : public std::runtime_error
{
public:
    FakeDeviceException(const QString &what)
        : std::runtime_error(what.toStdString()) {}
};

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
    Q_PROPERTY(QString Label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(quint64 ConnectTime READ connectTime CONSTANT)
    Q_PROPERTY(quint64 AuthorizeTime READ authorizeTime CONSTANT)
    Q_PROPERTY(quint64 StoreTime READ storeTime CONSTANT)
public:
    explicit FakeDevice(const QString &uid, QObject *parent = nullptr);
    explicit FakeDevice(const QJsonObject &json, QObject *parent = nullptr);
    ~FakeDevice() override;

    QDBusObjectPath dbusPath() const;

    QString uid() const;
    QString name() const;
    void setName(const QString &name);
    QString vendor() const;
    void setVendor(const QString &vendor);
    QString type() const;
    void setType(const QString &type);
    QString status() const;
    void setStatus(const QString &status);
    QString authFlags() const;
    void setAuthFlags(const QString &authFlags);
    QString parent() const;
    void setParent(const QString &parent);
    QString sysfsPath() const;
    void setSysfsPath(const QString &sysfsPath);
    bool stored() const;
    void setStored(bool stored);
    QString policy() const;
    void setPolicy(const QString &policy);
    QString key() const;
    void setKey(const QString &key);
    QString label() const;
    void setLabel(const QString &label);
    quint64 connectTime() const;
    void setConnectTime(quint64 connectTime);
    quint64 authorizeTime() const;
    void setAuthorizeTime(quint64 authorizeTime);
    quint64 storeTime() const;
    void setStoreTime(quint64 storeTime);

public Q_SLOTS:
    void Authorize(const QString &flags);

Q_SIGNALS:
    void labelChanged(const QString &label);

private:
    QDBusObjectPath mDBusPath;

    QString mUid;
    QString mName;
    QString mVendor;
    QString mType;
    QString mStatus = QStringLiteral("unknown");
    QString mAuthFlags = QStringLiteral("none");
    QString mParent;
    QString mSysfsPath;
    QString mPolicy = QStringLiteral("unknown");
    QString mKey;
    QString mLabel;
    quint64 mConnectTime = 0;
    quint64 mAuthorizeTime = 0;
    quint64 mStoreTime = 0;
    bool mStored = false;
};

#endif

