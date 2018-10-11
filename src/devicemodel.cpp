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

#include "devicemodel.h"

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(&mClient, &Client::deviceAdded,
            this, [this](Device *device) {
                beginInsertRows({}, mDevices.count(), mDevices.count());
                mDevices.push_back(device);
                endInsertRows();
            });
    connect(&mClient, &Client::deviceRemoved,
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
    return { {Role::Device, "device"} };
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return mClient.devices().count();
}

QVariant DeviceModel(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    const auto devices = mClient.devices();
    if (index.row() >= devices.size()) {
        return {};
    }

    if (role == Role::Device) {
        return devices.at(index.row());
    }

    return QAbstractListModel::data(index, role);
}


