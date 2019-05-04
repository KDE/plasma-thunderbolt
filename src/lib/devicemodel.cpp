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

#include "devicemodel.h"
#include "device.h"
#include "manager.h"

using namespace Bolt;

Q_DECLARE_METATYPE(QSharedPointer<Bolt::Device>)

void DeviceModel::setManager(Manager *manager)
{
    if (mManager == manager) {
        return;
    }

    if (mManager) {
        mManager->disconnect(this);
    }

    beginResetModel();
    mManager = manager;
    mDevices.clear();
    if (!mManager) {
        endResetModel();
        return;
    }

    connect(mManager, &Manager::deviceAdded,
            this, [this](const QSharedPointer<Device> &device) {
                if (mShowHosts || device->type() == Type::Peripheral) {
                    beginInsertRows({}, mDevices.count(), mDevices.count());
                    mDevices.push_back(device);
                    endInsertRows();
                }
            });
    connect(mManager, &Manager::deviceRemoved,
            this, [this](const QSharedPointer<Device> &device) {
                const int idx = mDevices.indexOf(device);
                if (idx == -1) {
                    return;
                }
                beginRemoveRows({}, idx, idx);
                mDevices.removeAt(idx);
                endRemoveRows();
            });

    populateWithoutReset();
    endResetModel();

    Q_EMIT managerChanged(mManager);
}

Manager *DeviceModel::manager() const
{
    return mManager;
}

bool DeviceModel::showHosts() const
{
    return mShowHosts;
}

void DeviceModel::setShowHosts(bool showHosts)
{
    if (mShowHosts != showHosts) {
        mShowHosts = showHosts;
        Q_EMIT showHostsChanged(mShowHosts);
        if (mManager) {
            beginResetModel();
            populateWithoutReset();
            endResetModel();
        }
    }
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

    return mDevices.count();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    if (index.row() >= mDevices.size()) {
        return {};
    }

    if (role == DeviceRole) {
        return QVariant::fromValue(mDevices.at(index.row()).data());
    }

    return {};
}

void DeviceModel::populateWithoutReset()
{
    Q_ASSERT(mManager);

    mDevices.clear();
    const auto all = mManager->devices();
    std::copy_if(all.cbegin(), all.cend(), std::back_inserter(mDevices),
            [this](const QSharedPointer<Device> &device) {
                return mShowHosts || device->type() == Type::Peripheral;
            });
}
