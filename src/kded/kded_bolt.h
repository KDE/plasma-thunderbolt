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

#ifndef KDED_BOLT_H
#define KDED_BOLT_H

#include <kdedmodule.h>

#include <QScopedPointer>
#include <QSharedPointer>
#include <QVector>
#include <QTimer>
#include <QMap>

class KNotification;
namespace Bolt {
class Manager;
class Device;
}

class Q_DECL_EXPORT KDEDBolt : public KDEDModule
{
    Q_OBJECT

public:
    KDEDBolt(QObject *parent, const QVariantList &args);
    ~KDEDBolt() override;

protected:
    virtual void notify();

    QVector<QSharedPointer<Bolt::Device>> sortDevices(const QVector<QSharedPointer<Bolt::Device>> &devices);

private:
    enum AuthMode {
        Enroll,
        Authorize
    };
    void authorizeDevices(QVector<QSharedPointer<Bolt::Device>> devices, AuthMode mode);

protected:
    QScopedPointer<Bolt::Manager> mManager;
    QVector<QSharedPointer<Bolt::Device>> mPendingDevices;
    QMap<KNotification*, QVector<QSharedPointer<Bolt::Device>>> mNotifiedDevices;
    QTimer mPendingDeviceTimer;
};

#endif // KDED_BOLT_H
