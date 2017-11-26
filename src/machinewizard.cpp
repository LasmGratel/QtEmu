/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include "machinewizard.h"

MachineWizard::MachineWizard(QWidget *parent) : QWizard(parent) {

    setWindowTitle(tr("Create a new Machine"));

    setPage(Page_Name, new MachineNamePage(this));
    setPage(Page_Hardware, new MachineHardwarePage(this));
    setPage(Page_Accelerator, new MachineAcceleratorPage(this));
    setPage(Page_Memory, new MachineMemoryPage(this));
    setPage(Page_Disk, new MachineDiskPage(this));
    setPage(Page_New_Disk, new MachineNewDiskPage(this));
    setPage(Page_Conclusion, new MachineConclusionPage(this));

    setStartId(Page_Name);

    #ifndef Q_OS_MAC
        setWizardStyle(ClassicStyle);
    #endif

    #ifdef Q_OS_MAC
        setWizardStyle(MacStyle);
    #endif

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    setMinimumSize(700, 400);

    qDebug() << "MachineWizard created";
}

MachineWizard::~MachineWizard() {
    qDebug() << "MachineWizard destroyed";
}

MachineNamePage::MachineNamePage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine name and operating system"));

    descriptionNameLabel = new QLabel(tr("Select name and operating system for your new machine."));
    descriptionNameLabel -> setWordWrap(true);

    machineNameLabel = new QLabel(tr("Name") + ":");
    machineNameLineEdit = new QLineEdit();

    OSTypeLabel = new QLabel(tr("Type") + ":");
    OSType = new QComboBox();

    OSType -> addItem("GNU/Linux");
    OSType -> addItem("Microsoft Windows");
    OSType -> addItem("BSD");
    OSType -> addItem(tr("Other"));

    connect(OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MachineNamePage::selectOS);

    OSVersionLabel = new QLabel(tr("Version") + ":");
    OSVersion = new QComboBox();

    selectOS(0);

    registerField("machine.name*", machineNameLineEdit);

    machineFolderCreated = QString();

    mainLayout = new QGridLayout();

    mainLayout -> setColumnStretch(1, 10);
    mainLayout -> setColumnStretch(2, 10);
    mainLayout -> setColumnStretch(3, 10);

    mainLayout -> addWidget(descriptionNameLabel, 0, 0, 1, 4);
    mainLayout -> addWidget(machineNameLabel,     1, 0, 1, 1);
    mainLayout -> addWidget(machineNameLineEdit,  1, 1, 1, 3);
    mainLayout -> addWidget(OSTypeLabel,          2, 0, 1, 1);
    mainLayout -> addWidget(OSType,               2, 1, 1, 3);
    mainLayout -> addWidget(OSVersionLabel,       3, 0, 1, 1);
    mainLayout -> addWidget(OSVersion,            3, 1, 1, 3);

    setLayout(mainLayout);

    qDebug() << "MachineNamePage created";
}

MachineNamePage::~MachineNamePage() {
    qDebug() << "MachineNamePage destroyed";
}

bool MachineNamePage::validatePage() {

    if ( ! machineFolderCreated.isEmpty() ) {
        return true;
    }

    QSettings settings;
    settings.beginGroup("Configuration");

    QString strMachinePath = settings.value("machinePath", QDir::homePath()).toString();

    settings.endGroup();

    QString strMachineName = field("machine.name").toString();
    QString strMachinePathMsg = strMachinePath;
    QString strFullMachinePath = strMachinePath.append("/").append(strMachineName);

    if ( QDir(strFullMachinePath).exists() ) {
        qDebug() << "The folder alredy exists" << strFullMachinePath;

        createMachineMessageBox = new QMessageBox();
        createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        createMachineMessageBox -> setIcon(QMessageBox::Critical);
        createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                              "in the parent folder <strong>%2<strong></p>"
                                              "<p>This folder alredy exists and possibly belongs to another machine.</p>")
                        .arg(strMachineName).arg(strMachinePathMsg));
        createMachineMessageBox -> exec();

        return false;
    }

    if ( ! QDir().mkpath(strFullMachinePath) ) {
        qDebug() << "Machine folder created" << strFullMachinePath;

        createMachineMessageBox = new QMessageBox();
        createMachineMessageBox -> setWindowTitle(tr("Qtemu - Critical error"));
        createMachineMessageBox -> setIcon(QMessageBox::Critical);
        createMachineMessageBox -> setText(tr("<p>Cannot create the machine folder <strong>%1</strong> "
                                              "in the parent folder <strong>%2<strong></p>"
                                              "<p>Please check that the parent really exists and that "
                                              "you have permissions to create the machine folder.</p>")
                        .arg(strMachineName).arg(strMachinePathMsg));
        createMachineMessageBox->exec();

        return false;
    }

    machineFolderCreated = strFullMachinePath;

    return true;
}

void MachineNamePage::cleanupPage() {

    if ( ! machineFolderCreated.isEmpty() ) {
        QDir().rmpath(machineFolderCreated);
    }

}

MachineHardwarePage::MachineHardwarePage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine hardware"));

    hardwareTabWidget = new QTabWidget();
    hardwareTabWidget -> addTab(new ProcessorTab(), tr("Processor"));
    hardwareTabWidget -> addTab(new GraphicsTab(), tr("Graphics"));
    hardwareTabWidget -> addTab(new AudioTab(), tr("Audio"));
    hardwareTabWidget -> addTab(new NetworkTab(), tr("Network"));

    hardwareLayout = new QVBoxLayout();
    hardwareLayout -> setAlignment(Qt::AlignCenter);
    hardwareLayout -> addWidget(hardwareTabWidget);

    setLayout(hardwareLayout);

    qDebug() << "MachineHardwarePage created";
}

ProcessorTab::ProcessorTab(QWidget *parent) : QWidget(parent) {

    CPUTypeLabel = new QLabel(tr("CPU Type") + ":");
    CPUTypeLabel -> setWordWrap(true);

    CPUType = new QComboBox();
    SystemUtils::setCPUTypesx86(CPUType);

    CPUTypeLayout = new QHBoxLayout();
    CPUTypeLayout -> setAlignment(Qt::AlignVCenter);
    CPUTypeLayout -> setSpacing(5);
    CPUTypeLayout -> addWidget(CPUTypeLabel);
    CPUTypeLayout -> addWidget(CPUType);

    CPUCountLabel = new QLabel(tr("CPU Count") + ":");
    CPUCountLabel -> setWordWrap(true);
    CPUCountSpinBox = new QSpinBox();
    CPUCountSpinBox -> setMinimum(1);
    CPUCountSpinBox -> setMaximum(255);

    coresSocketLabel = new QLabel(tr("Cores per socket") + ":");
    coresSocketLabel -> setWordWrap(true);
    coresSocketSpinBox = new QSpinBox();
    coresSocketSpinBox -> setMinimum(0);
    coresSocketSpinBox -> setMaximum(255);

    CPUCountLayout = new QHBoxLayout();
    CPUCountLayout -> setAlignment(Qt::AlignVCenter);
    CPUCountLayout -> setSpacing(5);
    CPUCountLayout -> addWidget(CPUCountLabel);
    CPUCountLayout -> addWidget(CPUCountSpinBox);
    CPUCountLayout -> addWidget(coresSocketLabel);
    CPUCountLayout -> addWidget(coresSocketSpinBox);

    socketCountLabel = new QLabel(tr("Socket count") + ":");
    socketCountLabel -> setWordWrap(true);
    socketCountSpinBox = new QSpinBox();
    socketCountSpinBox -> setMinimum(0);
    socketCountSpinBox -> setMaximum(255);

    threadsCoreLabel = new QLabel(tr("Threads per core") + ":");
    threadsCoreLabel -> setWordWrap(true);
    threadsCoreSpinBox = new QSpinBox();
    threadsCoreSpinBox -> setMinimum(0);
    threadsCoreSpinBox -> setMaximum(255);

    socketLayout = new QHBoxLayout();
    socketLayout -> setAlignment(Qt::AlignVCenter);
    socketLayout -> setSpacing(5);
    socketLayout -> addWidget(socketCountLabel);
    socketLayout -> addWidget(socketCountSpinBox);
    socketLayout -> addWidget(threadsCoreLabel);
    socketLayout -> addWidget(threadsCoreSpinBox);

    maxHotCPULabel = new QLabel(tr("Maximum number of hotpluggable CPUs") + ":");
    maxHotCPULabel -> setWordWrap(false);
    maxHotCPUSpinBox = new QSpinBox();
    maxHotCPUSpinBox -> setMinimum(0);
    maxHotCPUSpinBox -> setMaximum(255);

    maxHotCPUsLayout = new QHBoxLayout();
    maxHotCPUsLayout -> setAlignment(Qt::AlignVCenter);
    maxHotCPUsLayout -> setSpacing(5);
    maxHotCPUsLayout -> addWidget(maxHotCPULabel);
    maxHotCPUsLayout -> addWidget(maxHotCPUSpinBox);

    CPUSettingsLayout = new QVBoxLayout();
    CPUSettingsLayout -> setAlignment(Qt::AlignVCenter);
    CPUSettingsLayout -> addItem(CPUCountLayout);
    CPUSettingsLayout -> addItem(socketLayout);
    CPUSettingsLayout -> addItem(maxHotCPUsLayout);

    CPUSettings = new QGroupBox(tr("CPU Settings"));
    CPUSettings -> setLayout(CPUSettingsLayout);

    processorLayout = new QVBoxLayout();
    processorLayout -> setAlignment(Qt::AlignVCenter);
    processorLayout -> addItem(CPUTypeLayout);
    processorLayout -> addWidget(CPUSettings);

    setLayout(processorLayout);

    qDebug() << "ProcessorTab created";
}

ProcessorTab::~ProcessorTab() {
    qDebug() << "ProcessorTab destroyed";
}

GraphicsTab::GraphicsTab(QWidget *parent) : QWidget(parent) {

    GPUTypeLabel = new QLabel(tr("GPU Type") + ":");
    GPUTypeLabel -> setWordWrap(true);
    GPUType = new QComboBox();
    SystemUtils::setGPUTypes(GPUType);

    GPUType -> setCurrentIndex(1);

    keyboardLabel = new QLabel(tr("Keyboard layout") + ":");
    keyboardLabel -> setWordWrap(true);
    keyboard = new QComboBox();
    SystemUtils::setKeyboardLayout(keyboard);

    keyboard -> setCurrentIndex(5);

    graphicsLayout = new QGridLayout();

    graphicsLayout -> setColumnStretch(1, 10);
    graphicsLayout -> setColumnStretch(2, 10);

    graphicsLayout -> addWidget(GPUTypeLabel,  1, 0, 1, 1);
    graphicsLayout -> addWidget(GPUType,       1, 1, 1, 3);
    graphicsLayout -> addWidget(keyboardLabel, 2, 0, 1, 1);
    graphicsLayout -> addWidget(keyboard,      2, 1, 1, 3);

    setLayout(graphicsLayout);

    qDebug() << "GraphicsTab created";
}

GraphicsTab::~GraphicsTab() {
    qDebug() << "GraphicsTab destroyed";
}

AudioTab::AudioTab(QWidget *parent) : QWidget(parent) {

    creativeCheck = new QCheckBox("Creative Sound Blaster 16");

    ensoniqCheck = new QCheckBox("ENSONIQ AudioPCI ES1370");

    intelAC97Check = new QCheckBox("Intel AC97(82801AA)");
    intelAC97Check -> setChecked(true);

    yamahaCheck = new QCheckBox("Yamaha YM3812");

    gravisCheck = new QCheckBox("Gravis Ultrasound GF1");

    CS4231ACheck = new QCheckBox("CS4231A");

    intelHDACheck = new QCheckBox("Intel HD Audio");

    pcSpeakerCheck = new QCheckBox(tr("PC Speaker"));

    audioLayout = new QGridLayout();
    audioLayout -> addWidget(creativeCheck,  0, 0, 1, 1);
    audioLayout -> addWidget(ensoniqCheck,   0, 1, 1, 1);

    audioLayout -> addWidget(intelAC97Check, 1, 0, 1, 1);
    audioLayout -> addWidget(yamahaCheck,    1, 1, 1, 1);

    audioLayout -> addWidget(gravisCheck,    2, 0, 1, 1);
    audioLayout -> addWidget(CS4231ACheck,   2, 1, 1, 1);

    audioLayout -> addWidget(intelHDACheck,  3, 0, 1, 1);
    audioLayout -> addWidget(pcSpeakerCheck, 3, 1, 1, 1);

    setLayout(audioLayout);

    qDebug() << "AudioTab created";
}

AudioTab::~AudioTab() {
    qDebug() << "AudioTab destroyed";
}

NetworkTab::NetworkTab(QWidget *parent) : QWidget(parent) {

    withNetworkRadio = new QRadioButton(tr("User Mode Network Connection (Uses the user mode network stack)"));
    withNetworkRadio -> setChecked(true);

    withoutNetworkRadio = new QRadioButton(tr("No network (No network cards installed on this machine"));

    networkLayout = new QVBoxLayout();
    networkLayout -> addWidget(withNetworkRadio);
    networkLayout -> addWidget(withoutNetworkRadio);

    setLayout(networkLayout);

    qDebug() << "NetworkTab created";
}

NetworkTab::~NetworkTab() {
    qDebug() << "NetworkTab destroyed";
}

MachineHardwarePage::~MachineHardwarePage() {
    qDebug() << "MachineHardwarePage destroyed";
}

MachineAcceleratorPage::MachineAcceleratorPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine accelerator"));

    acceleratorTabWidget = new QTabWidget();
    acceleratorTabWidget -> addTab(new KVMTab(), tr("KVM"));
    acceleratorTabWidget -> addTab(new TCGTab(), tr("TCG"));
    acceleratorTabWidget -> addTab(new HAXMTab(), tr("HAXM"));

    acceleratorLayout = new QVBoxLayout();
    acceleratorLayout -> setAlignment(Qt::AlignCenter);
    acceleratorLayout -> addWidget(acceleratorTabWidget);

    setLayout(acceleratorLayout);

    qDebug() << "MachineAcceleratorPage created";
}

MachineAcceleratorPage::~MachineAcceleratorPage() {
    qDebug() << "MachineAcceleratorPage destroyed";
}

KVMTab::KVMTab(QWidget *parent) : QWidget(parent) {

    kvmRadioButton = new QRadioButton("Kernel-based Virtual Machine (KVM)");

    kvmDescriptionLabel = new QLabel("KVM (for Kernel-based Virtual Machine) is a full virtualization solution"
                                     "for GNU/Linux on x86 hardware containing virtualization extensions (Intel VT or AMD-V).");
    kvmDescriptionLabel -> setWordWrap(true);

    kvmURLLabel = new QLabel("<a href=\"https://www.linux-kvm.org\">www.linux-kvm.org</a>");

    kvmLayout = new QVBoxLayout();
    kvmLayout -> addWidget(kvmRadioButton);
    kvmLayout -> addWidget(kvmDescriptionLabel);
    kvmLayout -> addWidget(kvmURLLabel, 0, Qt::AlignCenter);

    setLayout(kvmLayout);

    qDebug() << "KVMTab created";
}

KVMTab::~KVMTab() {
    qDebug() << "KVMTab destroyed";
}

TCGTab::TCGTab(QWidget *parent) : QWidget(parent) {

    tcgRadioButton = new QRadioButton("Tiny Code Generator (TCG)");

    tcgDescriptionLabel = new QLabel("The Tiny Code Generator (TCG) exists to transform"
                                      "target insns (the processor being emulated) via the"
                                      "TCG frontend to TCG ops which are then transformed"
                                      "into host insns (the processor executing QEMU itself)"
                                      "via the TCG backend.");
    tcgDescriptionLabel -> setWordWrap(true);

    tcgURLLabel = new QLabel("<a href=\"https://https://wiki.qemu.org/Documentation/TCG\">wiki.qemu.org</a>");

    tcgLayout = new QVBoxLayout();
    tcgLayout -> addWidget(tcgRadioButton);
    tcgLayout -> addWidget(tcgDescriptionLabel);
    tcgLayout -> addWidget(tcgURLLabel, 0, Qt::AlignCenter);

    setLayout(tcgLayout);

    qDebug() << "TCGTab created";
}

TCGTab::~TCGTab() {
    qDebug() << "TCGTab destroyed";
}

HAXMTab::HAXMTab(QWidget *parent) : QWidget(parent) {

    haxmRadioButton = new QRadioButton("Hardware Accelerated Execution Manager (HAXM)");

    haxmDescriptionLabel = new QLabel("Intel® Hardware Accelerated Execution Manager"
                                       "(Intel® HAXM) is a hardware-assisted virtualization"
                                       "engine (hypervisor) that uses Intel® Virtualization Technology"
                                       "(Intel® VT) to speed up Android* app emulation on a host machine."
                                      );
    haxmDescriptionLabel -> setWordWrap(true);

    haxmURLLabel = new QLabel("<a href=\"https://software.intel.com/en-us/articles/intel-hardware-accelerated-execution-manager-intel-haxm\">software.intel.com</a>");

    haxmLayout = new QVBoxLayout();
    haxmLayout -> addWidget(haxmRadioButton);
    haxmLayout -> addWidget(haxmDescriptionLabel);
    haxmLayout -> addWidget(haxmURLLabel, 0, Qt::AlignCenter);

    setLayout(haxmLayout);

    qDebug() << "HAXMTab created";
}

HAXMTab::~HAXMTab() {
    qDebug() << "HAXMTab destroyed";
}

MachineMemoryPage::MachineMemoryPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine memory"));

    descriptionMemoryLabel = new QLabel(
                tr("Select the amount of base memory (RAM) in megabytes for virtual machine allocating."));
    descriptionMemoryLabel -> setWordWrap(true);

    int32_t totalRAM = 0;

    SystemUtils::getTotalMemory(totalRAM);

    spinBoxMemoryLabel = new QLabel("MiB");

    memorySpinBox = new QSpinBox();
    memorySpinBox -> setMinimum(1);
    memorySpinBox -> setMaximum(totalRAM);

    memorySlider = new QSlider(Qt::Horizontal);
    memorySlider -> setTickPosition(QSlider::TicksBelow);
    memorySlider -> setTickInterval(500);
    memorySlider -> setMinimum(1);
    memorySlider -> setMaximum(totalRAM);

    connect(memorySlider, &QSlider::valueChanged,
            memorySpinBox, &QSpinBox::setValue);

    minMemoryLabel = new QLabel("1 MiB");
    maxMemorylabel = new QLabel(QString("%1 MiB").arg(totalRAM));

    machineMemoryLayout = new QGridLayout();

    machineMemoryLayout -> setColumnStretch(1, 50);

    machineMemoryLayout -> addWidget(descriptionMemoryLabel, 0, 0, 1, 5);
    machineMemoryLayout -> addWidget(memorySlider,           1, 0, 1, 3);
    machineMemoryLayout -> addWidget(memorySpinBox,          1, 3, 1, 1);
    machineMemoryLayout -> addWidget(spinBoxMemoryLabel,     1, 4, 1, 1);
    machineMemoryLayout -> addWidget(minMemoryLabel,         2, 0, 1, 1);
    machineMemoryLayout -> addWidget(maxMemorylabel,         2, 2, 1, 1);

    setLayout(machineMemoryLayout);

    qDebug() << "MachineMemoryPage created";
}

MachineMemoryPage::~MachineMemoryPage() {
    qDebug() << "MachineMemoryPage destroyed";
}

MachineDiskPage::MachineDiskPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine virtual hard disk"));

    machineDiskLabel = new QLabel(
                tr("Select a virtual hard disk to the new machine."
                   "You can either create a new hard disk or select an existing one."));
    machineDiskLabel -> setWordWrap(true);

    machineDiskInfoLabel = new QLabel(tr("If you need a more complex storage set-up"
                                         "you can skip this step and make the changes"
                                         "to the machine settings once the machine is created."));
    machineDiskInfoLabel -> setWordWrap(true);

    machineDiskSizeLabel = new QLabel(tr("The recommended size of the hard disk is"));
    machineDiskSizeLabel -> setWordWrap(true);

    noDiskRadio = new QRadioButton(tr("Do not add a virtual hard disk"));

    createDiskRadio = new QRadioButton(tr("Create a new virtual hard disk"));
    createDiskRadio -> setChecked(true);

    useExistingDiskRadio = new QRadioButton(tr("Use an existing virtual hard disk"));

    connect(useExistingDiskRadio, &QAbstractButton::toggled,
            this, &MachineDiskPage::useExistingDiskToggle);

    hardDiskPathLineEdit = new QLineEdit();
    hardDiskPathLineEdit -> setEnabled(false);

    pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                        "",
                                        this);

    this -> useExistingDiskToggle(false);

    useOldDiskLayout = new QHBoxLayout();
    useOldDiskLayout -> setAlignment(Qt::AlignVCenter);
    useOldDiskLayout -> addWidget(hardDiskPathLineEdit);
    useOldDiskLayout -> addWidget(pathNewDiskPushButton);

    machineDiskLayout = new QVBoxLayout();
    machineDiskLayout -> setSpacing(5);
    machineDiskLayout -> addWidget(machineDiskLabel);
    machineDiskLayout -> addWidget(machineDiskInfoLabel);
    machineDiskLayout -> addWidget(machineDiskSizeLabel);
    machineDiskLayout -> addWidget(noDiskRadio);
    machineDiskLayout -> addWidget(createDiskRadio);
    machineDiskLayout -> addWidget(useExistingDiskRadio);
    machineDiskLayout -> addItem(useOldDiskLayout);

    setLayout(machineDiskLayout);

    qDebug() << "MachineDiskPage created";
}

MachineDiskPage::~MachineDiskPage() {
    qDebug() << "MachineDiskPage destroyed";
}

MachineNewDiskPage::MachineNewDiskPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("New virtual hard disk"));

    ////////////////////////////   DISK NAME   ///////////////////////////////////
    fileLocationGroupBox = new QGroupBox(tr("Disk name"));

    fileName = new QLineEdit();

    pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                        "",
                                        this);

    fileLocationLayout = new QHBoxLayout();
    fileLocationLayout -> addWidget(fileName);
    fileLocationLayout -> addWidget(pathNewDiskPushButton);

    fileLocationGroupBox -> setLayout(fileLocationLayout);

    //////////////////////////   END DISK NAME   /////////////////////////////////

    ////////////////////////////   DISK SIZE   ///////////////////////////////////
    fileSizeGroupBox = new QGroupBox(tr("Disk size"));

    fileSizeLayout = new QHBoxLayout();

    fileSizeGroupBox -> setLayout(fileSizeLayout);

    //////////////////////////   END DISK SIZE   /////////////////////////////////

    fileTypeGroupBox = new QGroupBox(tr("Disk type"));

    rawRadioButton = new QRadioButton(tr("raw ()"));
    qcowRadioButton = new QRadioButton(tr("qcow (QEMU Copy-on-write)"));
    qcow2RadioButton = new QRadioButton(tr("qcow2 (QEMU Copy-on-write 2)"));
    qedRadioButton = new QRadioButton(tr("qed (QEMU enhanced disk)"));
    vmdkRadioButton = new QRadioButton(tr("vmdk (Virtual Machine Disk)"));
    vpcRadioButton = new QRadioButton(tr("vpc ()"));
    vdiRadioButton = new QRadioButton(tr("vdi (VirtualBox Disk Image)"));
    cloopRadioButton = new QRadioButton(tr("cloop ()"));

    diskTypeLayout = new QVBoxLayout();
    diskTypeLayout -> addWidget(rawRadioButton);
    diskTypeLayout -> addWidget(qcowRadioButton);
    diskTypeLayout -> addWidget(qcow2RadioButton);
    diskTypeLayout -> addWidget(qedRadioButton);
    diskTypeLayout -> addWidget(vmdkRadioButton);
    diskTypeLayout -> addWidget(vpcRadioButton);
    diskTypeLayout -> addWidget(vdiRadioButton);
    diskTypeLayout -> addWidget(cloopRadioButton);

    fileTypeGroupBox -> setLayout(diskTypeLayout);

    storageTypeGroupBox = new QGroupBox(tr("Aditional options"));

    newDiskLayout = new QVBoxLayout();
    newDiskLayout -> addWidget(fileLocationGroupBox);
    newDiskLayout -> addWidget(fileSizeGroupBox);
    newDiskLayout -> addWidget(fileTypeGroupBox);
    newDiskLayout -> addWidget(storageTypeGroupBox);

    setLayout(newDiskLayout);

    qDebug() << "MachineNewDiskPage created";
}

MachineNewDiskPage::~MachineNewDiskPage() {
    qDebug() << "MachineNewDiskPage destroyed";
}

MachineConclusionPage::MachineConclusionPage(QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine Summary"));

    conclusionLayout = new QFormLayout();
    conclusionLayout -> addRow(tr("Name") + ":", new QLabel("MACHINE_NAME"));
    conclusionLayout -> addRow(tr("Processor") + ":", new QLabel("MACHINE_PROCESSOR"));
    conclusionLayout -> addRow(tr("Graphics") + ":", new QLabel("MACHINE_GPU"));
    conclusionLayout -> addRow(tr("Audio") + ":", new QLabel("MACHINE_AUDIO"));
    conclusionLayout -> addRow(tr("Network") + ":", new QLabel("MACHINE_NET"));
    conclusionLayout -> addRow(tr("RAM") + ":", new QLabel("MACHINE_RAM"));
    conclusionLayout -> addRow(tr("Accelerator") + ":", new QLabel("MACHINE_Accelerator"));
    conclusionLayout -> addRow(tr("Disk") + ":", new QLabel("MACHINE_DISK"));

    setLayout(conclusionLayout);

    qDebug() << "MachineConclusionPage created";
}

MachineConclusionPage::~MachineConclusionPage() {
    qDebug() << "MachineConclusionPage destroyed";
}

void MachineNamePage::selectOS(int OSSelected){

    this -> OSVersion -> clear();

    if (OSSelected == 0) {
        this -> OSVersion -> addItem(tr("Debian"));
        this -> OSVersion -> addItem(tr("Ubuntu"));
        this -> OSVersion -> addItem(tr("Fedora"));
        this -> OSVersion -> addItem(tr("OpenSuse"));
        this -> OSVersion -> addItem(tr("Mageia"));
        this -> OSVersion -> addItem(tr("Gentoo"));
        this -> OSVersion -> addItem(tr("Arch Linux"));
        this -> OSVersion -> addItem(tr("Linux 4.x"));
    } else if (OSSelected == 1) {
        this -> OSVersion -> addItem(tr("Microsoft 95"));
        this -> OSVersion -> addItem(tr("Microsoft 98"));
        this -> OSVersion -> addItem(tr("Microsoft 2000"));
        this -> OSVersion -> addItem(tr("Microsoft XP"));
        this -> OSVersion -> addItem(tr("Microsoft Vista"));
        this -> OSVersion -> addItem(tr("Microsoft 7"));
        this -> OSVersion -> addItem(tr("Microsoft 8"));
        this -> OSVersion -> addItem(tr("Microsoft 10"));
    } else if (OSSelected == 2) {
        this -> OSVersion -> addItem(tr("FreeBSD"));
        this -> OSVersion -> addItem(tr("OpenBSD"));
        this -> OSVersion -> addItem(tr("NetBSD"));
    } else if (OSSelected == 3) {
        this -> OSVersion -> addItem(tr("Debian GNU Hurd"));
        this -> OSVersion -> addItem(tr("Arch Hurd"));
        this -> OSVersion -> addItem(tr("Minix"));
        this -> OSVersion -> addItem(tr("ReactOS"));
    }
}

void MachineDiskPage::useExistingDiskToggle(bool toggled) {

    this -> pathNewDiskPushButton -> setEnabled(toggled);
}

