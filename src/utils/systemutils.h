/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

// Qt
#include <QLabel>
#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QProcess>
#include <QMessageBox>
#include <QSettings>

#include <QDebug>

// Local

// GNU
#ifdef Q_OS_LINUX
#include <sys/sysinfo.h>
#endif

class SystemUtils {

    public:
        SystemUtils();
        ~SystemUtils();

        static void getTotalMemory(int32_t &totalRAM);

        static void setCPUTypesx86(QComboBox *CPUType);
        static void setGPUTypes(QComboBox *GPUType);
        static void setKeyboardLayout(QComboBox *keyboardLayout);

        static QString getOsIcon(const QString &osVersion);

        static bool createDisk(const QString &diskName, const QString &format,
                               const double size, bool useEncryption);

    private:

};

#endif // SYSTEMUTILS_H
