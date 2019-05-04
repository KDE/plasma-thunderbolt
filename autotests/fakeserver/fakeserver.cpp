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

#include "fakeserver.h"
#include "fakemanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusError>
#include <QDebug>
#include <QTest>
#include <QEventLoop>
#include <QTimer>

namespace {

static const QString OrgKdeFakebolt = QStringLiteral("org.kde.fakebolt");

}

FakeServer::FakeServer(const QString &filename)
{
    QFile jsonFile(filename);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qCritical("Failed to open file %s: %s", qUtf8Printable(filename), qUtf8Printable(jsonFile.errorString()));
        throw FakeServerException(
                QStringLiteral("Failed to open file %1: %2").arg(filename, jsonFile.errorString()));
    }

    const auto doc = QJsonDocument::fromJson(jsonFile.readAll());

    if (!QDBusConnection::sessionBus().registerService(OrgKdeFakebolt)) {
        throw FakeServerException(
                QStringLiteral("Failed to register org.kde.fakebolt service: %1")
                    .arg(QDBusConnection::sessionBus().lastError().message()));
    }

    try {
        mManager.reset(new FakeManager(doc.object()));
    } catch (const FakeManagerException &e) {
        throw FakeServerException(e.what());
    }
}

FakeServer::FakeServer()
{
    if (!QDBusConnection::sessionBus().registerService(OrgKdeFakebolt)) {
        throw FakeServerException(
                QStringLiteral("Failed to register org.kde.fakebolt service: %1")
                    .arg(QDBusConnection::sessionBus().lastError().message()));
    }

    try {
        mManager.reset(new FakeManager());
    } catch (FakeManagerException &e) {
        throw FakeServerException(e.what());
    }
}

FakeServer::~FakeServer()
{
}

void FakeServer::enableFakeEnv()
{
    qputenv("KBOLT_FAKE", "1");
}

FakeManager *FakeServer::manager() const
{
    return mManager.get();
}
