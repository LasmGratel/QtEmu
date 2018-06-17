/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
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

// Qt

// Local
#include "machineconfiggeneral.h"

MachineConfigGeneral::MachineConfigGeneral(Machine *machine,
                                           QWidget *parent) : QWidget(parent) {

    m_generalTabWidget = new QTabWidget();
    m_generalTabWidget -> setSizePolicy(QSizePolicy::MinimumExpanding,
                                            QSizePolicy::MinimumExpanding);
    m_generalTabWidget -> addTab(new BasicTab(machine, this), tr("Basic Details"));
    m_generalTabWidget -> addTab(new DescriptionTab(machine, this), tr("Description"));

    m_generalPageLayout = new QVBoxLayout();
    m_generalPageLayout -> setAlignment(Qt::AlignCenter);
    m_generalPageLayout -> addWidget(m_generalTabWidget);

    m_generalPageWidget = new QWidget(this);
    m_generalPageWidget -> setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_generalPageWidget -> setLayout(m_generalPageLayout);

    qDebug() << "MachineConfigGeneral created";
}

MachineConfigGeneral::~MachineConfigGeneral() {
    qDebug() << "MachineConfigGeneral destroyed";
}

BasicTab::BasicTab(Machine *machine,
                   QWidget *parent) : QWidget(parent) {

    m_machineNameLineEdit = new QLineEdit();

    m_OSType = new QComboBox();
    m_OSType -> setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::MinimumExpanding);

    m_OSType -> addItem("GNU/Linux");
    m_OSType -> addItem("Microsoft Windows");
    m_OSType -> addItem("BSD");
    m_OSType -> addItem(tr("Other"));

    connect(m_OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
           this, &BasicTab::selectOS);

    m_OSVersion = new QComboBox();
    m_OSVersion -> setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::MinimumExpanding);

    this -> selectOS(0);

    m_machineUuidLabel = new QLabel();
    m_machineStatusLabel = new QLabel();

    m_basicTabFormLayout = new QFormLayout();
    m_basicTabFormLayout -> setAlignment(Qt::AlignTop);
    m_basicTabFormLayout -> setLabelAlignment(Qt::AlignLeft);
    m_basicTabFormLayout -> setHorizontalSpacing(20);
    m_basicTabFormLayout -> setVerticalSpacing(10);
    m_basicTabFormLayout -> addRow(tr("Name") + ":", m_machineNameLineEdit);
    m_basicTabFormLayout -> addRow(tr("Type") + ":", m_OSType);
    m_basicTabFormLayout -> addRow(tr("Version") + ":", m_OSVersion);
    m_basicTabFormLayout -> addRow(tr("UUID") + ":", m_machineUuidLabel);
    m_basicTabFormLayout -> addRow(tr("Status") + ":", m_machineStatusLabel);

    m_basicTabLayout = new QVBoxLayout();
    m_basicTabLayout -> addItem(m_basicTabFormLayout);

    this -> setLayout(m_basicTabLayout);

    qDebug() << "BasicTab created";
}

BasicTab::~BasicTab() {
    qDebug() << "BasicTab destroyed";
}

void BasicTab::selectOS(int OSSelected){

    this -> m_OSVersion -> clear();

    if (OSSelected == 0) {
        this -> m_OSVersion -> addItem(tr("Debian"));
        this -> m_OSVersion -> addItem(tr("Ubuntu"));
        this -> m_OSVersion -> addItem(tr("Fedora"));
        this -> m_OSVersion -> addItem(tr("OpenSuse"));
        this -> m_OSVersion -> addItem(tr("Mageia"));
        this -> m_OSVersion -> addItem(tr("Gentoo"));
        this -> m_OSVersion -> addItem(tr("Arch Linux"));
        this -> m_OSVersion -> addItem(tr("Linux"));
    } else if (OSSelected == 1) {
        this -> m_OSVersion -> addItem(tr("Microsoft 95"));
        this -> m_OSVersion -> addItem(tr("Microsoft 98"));
        this -> m_OSVersion -> addItem(tr("Microsoft 2000"));
        this -> m_OSVersion -> addItem(tr("Microsoft XP"));
        this -> m_OSVersion -> addItem(tr("Microsoft Vista"));
        this -> m_OSVersion -> addItem(tr("Microsoft 7"));
        this -> m_OSVersion -> addItem(tr("Microsoft 8"));
        this -> m_OSVersion -> addItem(tr("Microsoft 10"));
    } else if (OSSelected == 2) {
        this -> m_OSVersion -> addItem(tr("FreeBSD"));
        this -> m_OSVersion -> addItem(tr("OpenBSD"));
        this -> m_OSVersion -> addItem(tr("NetBSD"));
    } else if (OSSelected == 3) {
        this -> m_OSVersion -> addItem(tr("Debian GNU Hurd"));
        this -> m_OSVersion -> addItem(tr("Arch Hurd"));
        this -> m_OSVersion -> addItem(tr("Redox"));
        this -> m_OSVersion -> addItem(tr("ReactOS"));
    }
}

DescriptionTab::DescriptionTab(Machine *machine,
                               QWidget *parent) : QWidget(parent) {

    m_machineDescLabel = new QLabel(tr("Description") + ":");
    m_machineDescTextEdit = new QPlainTextEdit();

    m_descriptionLayout = new QVBoxLayout();
    m_descriptionLayout -> setAlignment(Qt::AlignVCenter);
    m_descriptionLayout -> addWidget(m_machineDescLabel);
    m_descriptionLayout -> addWidget(m_machineDescTextEdit);

    this -> setLayout(m_descriptionLayout);

    qDebug() << "DescriptionTab created";
}

DescriptionTab::~DescriptionTab() {
    qDebug() << "DescriptionTab destroyed";
}
