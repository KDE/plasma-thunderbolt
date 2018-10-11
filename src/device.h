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

#ifndef DEVICE_H_
#define DEVICE_H_

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QDBusObjectPath>

#include "enum.h"

#include <memory>
#include <functional>

class OrgFreedesktopBolt1DeviceInterface;
namespace Bolt {

class Domain;
class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString uid READ uid CONSTANT STORED false)
    Q_PROPERTY(QString name READ name CONSTANT STORED false)
    Q_PROPERTY(QString vendor READ vendor CONSTANT STORED false)
    Q_PROPERTY(Type type READ type CONSTANT STORED false)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged STORED false)
    Q_PROPERTY(AuthFlags authFlags READ authFlags CONSTANT STORED false)
    Q_PROPERTY(QString parent READ parent CONSTANT STORED false)
    Q_PROPERTY(QString sysfsPath READ sysfsPath CONSTANT STORED false)
    Q_PROPERTY(QDateTime connectTime READ connectTime CONSTANT STORED false)
    Q_PROPERTY(QDateTime authorizeTime READ authorizeTime CONSTANT STORED false)
    Q_PROPERTY(bool stored READ stored CONSTANT STORED false)
    Q_PROPERTY(Policy policy READ policy CONSTANT STORED false)
    Q_PROPERTY(KeyState keyState READ keyState CONSTANT STORED false)
    Q_PROPERTY(QDateTime storeTime READ storeTime CONSTANT STORED false)
    Q_PROPERTY(QString label READ label CONSTANT STORED false)

    Q_ENUM(Status)
    Q_ENUM(Auth)
    Q_ENUM(KeyState)
    Q_ENUM(Security)
    Q_ENUM(Policy)
    Q_ENUM(Type)
    Q_ENUM(AuthMode)
    Q_FLAGS(AuthFlags)
public:
    explicit Device(const QDBusObjectPath &path, QObject *parent = nullptr);
    explicit Device(QObject *parent = nullptr);
    ~Device() override;

    QString uid() const;
    QString name() const;
    QString vendor() const;
    Type type() const;
    Status status() const;
    AuthFlags authFlags() const;
    QString parent() const;
    QString sysfsPath() const;
    QDateTime connectTime() const;
    QDateTime authorizeTime() const;
    bool stored() const;
    Policy policy() const;
    KeyState keyState() const;
    QDateTime storeTime() const;
    QString label() const;

    QDBusObjectPath dbusPath() const;

public Q_SLOTS:
    void authorize();

Q_SIGNALS:
    void statusChanged(Status);

private:
    QScopedPointer<OrgFreedesktopBolt1DeviceInterface> mInterface;
    QDBusObjectPath mDbusPath;
};

} // namespace

#endif
