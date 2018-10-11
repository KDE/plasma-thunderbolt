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

#include <QCoreApplication>
#include <QDBusConnection>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QJsonDocument>
#include <QFile>
#include <QTimer>

#include <iostream>

#include "fakemanager.h"
#include "exceptions.h"

int main(int argc, char **argv)
{
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationName(QStringLiteral("fakeserver"));

    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption cfgOption(QStringLiteral("cfg"), QStringLiteral("Config file"), QStringLiteral("FILE"));
    parser.addOption(cfgOption);
    parser.addHelpOption();
    parser.process(app);
    if (!parser.isSet(cfgOption)) {
        std::cout << "Missing option --cfg" << std::endl;
        parser.showHelp();
        return 0;
    }

    const QString filename = parser.value(cfgOption);
    QFile jsonFile(filename);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open file " << filename.toStdString()
            << ": " << jsonFile.errorString().toStdString() << std::endl;
        return -1;
    }
    const auto doc = QJsonDocument::fromJson(jsonFile.readAll());

    if (!QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.fakebolt"))) {
        std::cerr << "Failed to register org.kde.fakebolt service" << std::endl;
        app.exit(-1);
    }

    try {
        FakeManager manager(doc.object());
        return app.exec();
    } catch (const DBusException &e) {
        std::cerr << "DBus error: " << e.what();
        return -1;
    }
}
