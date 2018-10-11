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

#ifndef MANAGER_H_
#define MANAGER_H_

#include <QScopedPointer>
#include <QObject>

#include <functional>
#include <memory>

#include "enum.h"
#include "kbolt_export.h"

class OrgFreedesktopBolt1ManagerInterface;
namespace Bolt
{

class Device;
class KBOLT_EXPORT Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint version READ version CONSTANT STORED false)
    Q_PROPERTY(bool isProbing READ isProbing CONSTANT STORED false)
    Q_PROPERTY(Policy defaultPolicy READ defaultPolicy CONSTANT STORED false)
    Q_PROPERTY(Security securityLevel READ securityLevel CONSTANT STORED false)
    Q_PROPERTY(AuthMode authMode READ authMode CONSTANT STORED false)

public:
    explicit Manager(QObject *parent = nullptr);
    ~Manager() override;

    uint version() const;
    bool isProbing() const;
    Policy defaultPolicy() const;
    Security securityLevel() const;
    AuthMode authMode() const;

public Q_SLOTS:
    Device *device(const QString &uid) const;
    QList<Device *> devices() const;

Q_SIGNALS:
    void deviceAdded(Device *);
    void deviceRemoved(Device *);

private:
    QScopedPointer<OrgFreedesktopBolt1ManagerInterface> mInterface;

    uint mVersion = 0;
    Policy mPolicy = Policy::Unknown;
    Security mSecurity = Security::Unknown;
    AuthMode mAuthMode = AuthMode::Disabled;
    bool mIsProbing = false;

    QList<Device *> mDevices;
};

} // namespace

#endif
