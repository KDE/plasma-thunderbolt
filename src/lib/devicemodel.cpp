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

#include "devicemodel.h"
#include "device.h"

using namespace Bolt;

Q_DECLARE_METATYPE(Bolt::Device*)

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(&mManager, &Manager::deviceAdded,
            this, [this](Device *device) {
                beginInsertRows({}, mDevices.count(), mDevices.count());
                mDevices.push_back(device);
                endInsertRows();
            });
    connect(&mManager, &Manager::deviceRemoved,
            this, [this](Device *device) {
                const int idx = mDevices.indexOf(device);
                if (idx == -1) {
                    return;
                }
                beginRemoveRows({}, idx, idx);
                mDevices.removeAt(idx);
                endRemoveRows();
            });
}

QHash<int, QByteArray> DeviceModel::roleNames() const
{
    auto roles = QAbstractListModel::roleNames();
    roles[DeviceRole] = "device";
    return roles;
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return mManager.devices().count();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    const auto devices = mManager.devices();
    if (index.row() >= devices.size()) {
        return {};
    }

    if (role == DeviceRole) {
        return QVariant::fromValue(mDevices.at(index.row()));
    }

    return {};
}


