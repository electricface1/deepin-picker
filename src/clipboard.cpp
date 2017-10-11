/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include "clipboard.h"
#include "settings.h"
#include "utils.h"
#include <QApplication>
#include <QClipboard>
#include <QtDBus>
#include <QDebug>

Clipboard::Clipboard(QObject *parent) : QObject(parent)
{

}

Clipboard::~Clipboard()
{
    
}

void Clipboard::copyToClipboard(QColor color, QString colorType)
{
    // Save color type to config file.
    QString colorString;
    if (colorType == "HEX") {
        colorString = Utils::colorToHex(color);
    } else if (colorType == "RGB") {
        colorString = Utils::colorToRGB(color);
    } else if (colorType == "RGBA") {
        colorString = Utils::colorToRGBA(color);
    } else if (colorType == "Float_RGB") {
        colorString = Utils::colorToFloatRGB(color);
    } else if (colorType == "Float_RGBA") {
        colorString = Utils::colorToFloatRGBA(color);
    }
    
    Settings *settings = new Settings();
    settings->setOption("color_type", colorType);
    
    // Copy to clipbard.
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(colorString);
    
    // Popup notify.
    QDBusInterface notification("org.freedesktop.Notifications",
                                "/org/freedesktop/Notifications",
                                "org.freedesktop.Notifications",
                                QDBusConnection::sessionBus());

    QStringList actions;
    QVariantMap hints;
    
    QList<QVariant> arg;
    arg << (QCoreApplication::applicationName())                             // appname
        << ((unsigned int) 0)                                                // id
        << QString("deepin-picker")                                          // icon
        << tr("Deepin Picker")                                               // summary
        << QString(tr("Copy color %1 to system clipboard")).arg(colorString) // body
        << actions                                                           // actions
        << hints                                                             // hints
        << (int) -1;                                                         // timeout
    notification.callWithArgumentList(QDBus::AutoDetect, "Notify", arg);
    
    // Quit application.
    QApplication::quit();
}
