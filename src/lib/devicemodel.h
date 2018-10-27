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

#ifndef DEVICEMODEL_H_
#define DEVICEMODEL_H_

#include <QAbstractListModel>

#include "kbolt_export.h"

namespace Bolt
{

class Manager;
class Device;
class KBOLT_EXPORT DeviceModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Bolt::Manager *manager READ manager WRITE setManager NOTIFY managerChanged)

    /** Whether to show only peripherals or display hosts as well */
    Q_PROPERTY(bool showHosts READ showHosts WRITE setShowHosts NOTIFY showHostsChanged)
public:
    enum Role {
        DeviceRole = Qt::UserRole
    };

    using QAbstractListModel::QAbstractListModel;
    ~DeviceModel() override = default;

    Manager *manager() const;
    void setManager(Manager *manager);

    bool showHosts() const;
    void setShowHosts(bool showHosts);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

Q_SIGNALS:
    void managerChanged(Bolt::Manager *manager);
    void showHostsChanged(bool showHosts);

private:
    void populateWithoutReset();

    Manager *mManager = nullptr;
    QList<QSharedPointer<Device>> mDevices;
    bool mShowHosts = true;
};

} // namespace Bolt

#endif
