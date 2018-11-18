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

// Local
#include "machine.h"

Machine::Machine(QObject *parent) : QObject(parent) {
    this -> m_machineProcess = new QProcess(this);

    connect(m_machineProcess, &QProcess::readyReadStandardOutput,
            this, &Machine::readMachineStandardOut);

    connect(m_machineProcess, &QProcess::readyReadStandardError,
            this, &Machine::readMachineErrorOut);

    connect(m_machineProcess, &QProcess::started,
            this, &Machine::machineStarted);

    connect(m_machineProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Machine::machineFinished);

    qDebug() << "Machine object created";
}

Machine::~Machine() {
    qDebug() << "Machine object destroyed";
}

/**
 * @brief Get the name of the machine
 *
 * Get the name of the machine
 */
QString Machine::getName() const {
    return name;
}

/**
 * @brief Set the name of the machine
 *
 * Set the name of the machine
 */
void Machine::setName(const QString &value) {
    name = value;
}

/**
 * @brief Get the operating system type of the machine
 *
 * Get the operating system type of the machine.
 * Ex: GNU/Linux, Windows...
 */
QString Machine::getOSType() const {
    return OSType;
}

/**
 * @brief Set the operating system type of the machine
 *
 * Set the operating system type of the machine.
 * Ex: GNU/Linux, Windows...
 */
void Machine::setOSType(const QString &value) {
    OSType = value;
}

/**
 * @brief Get the operating system version of the machine
 *
 * Get the operating system version of the machine.
 * Ex: Debian, Mageia, Arch Linux...
 */
QString Machine::getOSVersion() const {
    return OSVersion;
}

/**
 * @brief Set the operating system version of the machine
 *
 * Set the operating system version of the machine.
 * Ex: Debian, Mageia, Arch Linux...
 */
void Machine::setOSVersion(const QString &value) {
    OSVersion = value;
}

/**
 * @brief Get the machine path
 *
 * Get the machine path
 * Ex: /home/xexio/Vms/Debian
 */
QString Machine::getPath() const {
    return path;
}

/**
 * @brief Set the machine path
 *
 * Set the machine path
 * Ex: /home/xexio/Vms/Debian
 */
void Machine::setPath(const QString &value) {
    path = value;
}

/**
 * @brief Get the configuration path
 *
 * Get the configuration path
 */
QString Machine::getConfigPath() const {
    return configPath;
}

/**
 * @brief Set the configuration path
 *
 * Set the configuration path
 */
void Machine::setConfigPath(const QString &value) {
    configPath = value;
}

/**
 * @brief Get the machine uuid
 *
 * get the machine uuid with a unique key generated
 * with QUuid
 * Ex: {fc6a2dd5-3c31-401f-a9c7-86ad6190a77f}
 */
QString Machine::getUuid() const {
    return uuid;
}

/**
 * @brief Set the machine uuid
 *
 * Set the machine uuid with a unique key generated
 * with QUuid
 * Ex: {fc6a2dd5-3c31-401f-a9c7-86ad6190a77f}
 */
void Machine::setUuid(const QString &value) {
    uuid = value;
}

/**
 * @brief Get the machine state
 *
 * Get the machine state
 * Ex: Started, Stopped or Rebooted
 */
Machine::States Machine::getState() const {
    return state;
}

/**
 * @brief Set the machine state
 *
 * Set the machine state
 * Ex: Started, Stopped or Rebooted
 */
void Machine::setState(const States &value) {
    state = value;
}

QString Machine::getDescription() const {
    return description;
}

void Machine::setDescription(const QString &value) {
    description = value;
}

/**
 * @brief Get the CPU Type of the machine
 *
 * Get the CPU Type of the machine
 * Ex: core2duo, pentium, Opteron_G5...
 */
QString Machine::getCPUType() const {
    return CPUType;
}

/**
 * @brief Set the CPU Type of the machine
 *
 * Set the CPU Type of the machine
 * Ex: core2duo, pentium, Opteron_G5...
 */
void Machine::setCPUType(const QString &value) {
    CPUType = value;
}

/**
 * @brief Get the CPU Count of the machine
 *
 * Get the CPU Count of the machine
 */
qint64 Machine::getCPUCount() const {
    return CPUCount;
}

/**
 * @brief Set the CPU Count of the machine
 *
 * Set the CPU Count of the machine
 */
void Machine::setCPUCount(const qint64 &value) {
    CPUCount = value;
}

/**
 * @brief Get the CPU Socket Count of the machine
 *
 * Get the CPU Socket Count of the machine
 */
qint64 Machine::getSocketCount() const {
    return socketCount;
}

/**
 * @brief Set the CPU Socket Count of the machine
 *
 * Set the CPU Socket Count of the machine
 */
void Machine::setSocketCount(const qint64 &value) {
    socketCount = value;
}

/**
 * @brief Get the CPU Cores per Socket of the machine
 *
 * Get the CPU Cores per Socket of the machine
 */
qint64 Machine::getCoresSocket() const {
    return coresSocket;
}

/**
 * @brief Set the CPU Cores per Socket of the machine
 *
 * Set the CPU Cores per Socket of the machine
 */
void Machine::setCoresSocket(const qint64 &value) {
    coresSocket = value;
}

/**
 * @brief Get the CPU Threads per Core of the machine
 *
 * Get the CPU Threads per Core of the machine
 */
qint64 Machine::getThreadsCore() const {
    return threadsCore;
}

/**
 * @brief Set the CPU Threads per Core of the machine
 *
 * Set the CPU Threads per Core of the machine
 */
void Machine::setThreadsCore(const qint64 &value) {
    threadsCore = value;
}

/**
 * @brief Get the max hot CPUs of the machine
 *
 * Get the max hot CPUs of the machine
 */
qint64 Machine::getMaxHotCPU() const {
    return maxHotCPU;
}

/**
 * @brief Set the max hot CPUs of the machine
 *
 * Set the max hot CPUs of the machine
 */
void Machine::setMaxHotCPU(const qint64 &value) {
    maxHotCPU = value;
}

/**
 * @brief Get the GPU of the machine
 *
 * Get the GPU of the machine
 * Ex: std, vmware, cg3...
 */
QString Machine::getGPUType() const {
    return GPUType;
}

/**
 * @brief Set the GPU of the machine
 *
 * Set the GPU of the machine
 * Ex: std, vmware, cg3...
 */
void Machine::setGPUType(const QString &value) {
    GPUType = value;
}

/**
 * @brief Get the keyboard of the machine
 *
 * Get the keyboard of the machine
 * Ex: ar, es, hr...
 */
QString Machine::getKeyboard() const {
    return keyboard;
}

/**
 * @brief Set the keyboard of the machine
 *
 * Set the keyboard of the machine
 * Ex: ar, es, hr...
 */
void Machine::setKeyboard(const QString &value) {
    keyboard = value;
}

/**
 * @brief Get the RAM of the machine
 *
 * Get the RAM of the machine.
 * There's a limit in the RAM that the machine can use.
 * You can't use more RAM that your system have.
 *
 * Ex: If your computer have 8GiB of RAM, your machine
 * are limited to that amount of RAM
 */
qlonglong Machine::getRAM() const {
    return RAM;
}

/**
 * @brief Set the RAM of the machine
 *
 * Set the RAM of the machine.
 * There's a limit in the RAM that the machine can use.
 * You can't use more RAM that your system have.
 *
 * Ex: If your computer have 8GiB of RAM, your machine
 * are limited to that amount of RAM
 */
void Machine::setRAM(const qlonglong &value) {
    RAM = value;
}

/**
 * @brief Get the audio cards of the machine
 *
 * Get the audio cards of the machine
 * Ex: ac97, es1370, hda...
 */
QStringList Machine::getAudio() const {
    return audio;
}

/**
 * @brief Set the audio cards of the machine
 *
 * Set the audio cards of the machine
 * Ex: ac97, es1370, hda...
 */
void Machine::setAudio(const QStringList &value) {
    audio = value;
}

/**
 * @brief Get if the machine have network
 *
 * Get if the machine have network
 */
bool Machine::getUseNetwork() const {
    return useNetwork;
}

/**
 * @brief Set if the machine have network
 *
 * Set if the machine have network
 */
void Machine::setUseNetwork(bool value) {
    useNetwork = value;
}

/**
 * @brief Get the list of media
 * @return media list
 *
 * Get the list of media of the machine
 */
QList<Media> Machine::getMedia() const {
    return media;
}

/**
 * @brief Add media to the media list
 * @param value, new media
 *
 * Add media to the media list
 */
void Machine::addMedia(const Media value) {
    this -> media.append(value);
}

/**
 * @brief Get the accelerator machine
 *
 * Get the accelerator machine
 * Ex: kvm, xen...
 */
QStringList Machine::getAccelerator() const {
    return accelerator;
}

/**
 * @brief Set the accelerator machine
 *
 * Set the accelerator machine
 * Ex: kvm, xen...
 */
void Machine::setAccelerator(const QStringList &value) {
    accelerator = value;
}

Boot Machine::getBoot() const {
    return m_boot;
}

void Machine::setBoot(const Boot &boot) {
    m_boot = boot;
}

// Methods
/**
 * @brief Add the audio card to the list
 * @param audio, audio card to be added
 *
 * Add the audio card to the list
 */
void Machine::addAudio(const QString audio) {
    if( ! this -> audio.contains(audio)){
        this -> audio.append(audio);
    }
}

/**
 * @brief Remove the audio card from the list
 * @param audio, audio card to be removed
 *
 * Remove the audio card from the list
 */
void Machine::removeAudio(const QString audio) {
    if(this -> audio.contains(audio)){
        this -> audio.removeOne(audio);
    }
}

void Machine::removeAllAudioCards() {
    this->audio.clear();
}

/**
 * @brief Add an accelerator to the list
 * @param accel code of the accelerator
 *
 * Add an accelerator to the list
 */
void Machine::addAccelerator(const QString accel) {
    if ( ! this->accelerator.contains(accel)){
        this -> accelerator.append(accel);
    }
}

/**
 * @brief Remove an accelerator from the list
 * @param accel code of the accelerator
 *
 * Remove an accelerator from the list if exists
 */
void Machine::removeAccelerator(const QString accel) {
    if(this -> accelerator.contains(accel)){
        this -> accelerator.removeOne(accel);
    }
}

void Machine::removeAllAccelerators() {
    this->accelerator.clear();
}

/**
 * @brief Get all the audio cards separated by commas
 * @return Audio cards separated by commas
 *
 * Get all the audio cards separated by commas
 * Ex: ac97, es1370, hda
 */
QString Machine::getAudioLabel() {

    QHash<QString, QString> soundCardsHash = SystemUtils::getSoundCards();

    QStringList audioCards = this -> audio;
    for(int i = 0; i < audioCards.size(); ++i) {
        audioCards.replace(i, soundCardsHash.value(audioCards.at(i)));
    }

    QString audioLabel = audioCards.join(",");

    return audioLabel;
}

/**
 * @brief Get all the accelerators separated by commas
 * @return Accelerators separated by commas
 *
 * Get all the accelerators separated by commas
 * Ex: kvm,tcg
 */
QString Machine::getAcceleratorLabel() {

    QHash<QString, QString> acceleratorsHash = SystemUtils::getAccelerators();

    QStringList accel = this -> accelerator;
    for(int i = 0; i < accel.size(); ++i) {
        accel.replace(i, acceleratorsHash.value(accel.at(i)));
    }

    QString acceleratorLabel = accel.join(",");

    return acceleratorLabel;
}

void Machine::runMachine() {

    QStringList args = this->generateMachineCommand();

    QString program;

    #ifdef Q_OS_LINUX
    program = "qemu-system-x86_64";
    #endif
    #ifdef Q_OS_WIN
    // TODO: Control windows execution
    #endif
    #ifdef Q_OS_MACOS
    // TODO: Control MacOS execution
    #endif
    #ifdef Q_OS_FREEBSD
    // TODO: Control FreeBSD execution
    #endif

    // TODO
    //m_machineProcess -> setProcessEnvironment(buildEnvironment());

    m_machineProcess -> start(program, args);
}

void Machine::stopMachine() {
    this -> m_machineProcess -> write(qPrintable("system_powerdown\n"));
    this -> state = Machine::Stopped;

    emit(machineStateChangedSignal(Machine::Stopped));
}

void Machine::resetMachine() {
    this -> m_machineProcess -> write(qPrintable("system_reset\n"));
}

void Machine::pauseMachine() {

    if (state == Machine::Started) {
        this -> m_machineProcess -> write(qPrintable("stop\n"));
        this -> state = Machine::Paused;

        emit(machineStateChangedSignal(Machine::Paused));

    } else if (state == Machine::Paused) {
        this -> m_machineProcess -> write(qPrintable("cont\n"));
        this -> state = Machine::Started;

        emit(machineStateChangedSignal(Machine::Started));
    }
}

void Machine::readMachineStandardOut() {
    // TODO: Show in a window
    qDebug() << "Standard Out: " << this -> m_machineProcess -> readAllStandardOutput();
}

void Machine::readMachineErrorOut() {
    // TODO: Show in a window
    qDebug() << "Error Out: " <<  m_machineProcess -> readAllStandardError();
}

void Machine::machineStarted() {
    this -> state = Machine::Started;

    emit(machineStateChangedSignal(Machine::Started));
}

void Machine::machineFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    qDebug() << "Exit code: " << exitCode << " exit status: " << exitStatus;
    this -> state = Machine::Stopped;
    emit(machineStateChangedSignal(Machine::Stopped));
}

QString Machine::getHostSoundSystem() const
{
    return hostSoundSystem;
}

void Machine::setHostSoundSystem(const QString &value)
{
    hostSoundSystem = value;
}

QProcessEnvironment Machine::buildEnvironment() {
    
    // TODO: Implement Windows and MacOS
    QProcessEnvironment env = m_machineProcess -> processEnvironment();
    env.insert("QEMU_AUDIO_DRV", "alsa");

    return env;
}

QStringList Machine::generateMachineCommand() {

    // TODO: Add qemu before commands

    QStringList qemuCommand;

    qemuCommand << "-monitor" << "stdio";

    qemuCommand << "-name";
    qemuCommand << this->name;

    qemuCommand << "-uuid";
    qemuCommand << this->uuid.remove("{").remove("}");

    QString accelerators;
    bool firstAccel = true;
    QStringListIterator accelIterator(this->accelerator);
    while (accelIterator.hasNext()) {
        if(firstAccel) {
            firstAccel = false;
        } else {
            accelerators.append(", ");
        }
        accelerators.append(accelIterator.next());
    }

    qemuCommand << "-accel";
    qemuCommand << accelerators;

    QString audioCards;
    bool firstAudio = true;
    QStringListIterator audioIterator(this->audio);
    while (audioIterator.hasNext()) {
        if(firstAudio) {
            firstAudio = false;
        } else {
            audioCards.append(", ");
        }
        audioCards.append(audioIterator.next());
    }

    qemuCommand << "-soundhw";
    qemuCommand << audioCards;

    qemuCommand << "-m";
    qemuCommand << QString::number(this->RAM);

    qemuCommand << "-k";
    qemuCommand << this->keyboard;

    qemuCommand << "-vga";
    qemuCommand << this->GPUType;

    // CPU
    qemuCommand << "-cpu";
    qemuCommand << this->CPUType;

    QString cpuArgs(QString::number(this->CPUCount));

    cpuArgs.append(",cores=");
    cpuArgs.append(QString::number(this->coresSocket));

    cpuArgs.append(",threads=");
    cpuArgs.append(QString::number(this->threadsCore));


    cpuArgs.append(",sockets=");
    cpuArgs.append(QString::number(this->socketCount));

    cpuArgs.append(",maxcpus=");
    cpuArgs.append(QString::number(this->maxHotCPU));

    qemuCommand << "-smp";
    qemuCommand << cpuArgs;

    QString pipe = this->path.append(QDir::toNativeSeparators("/")).append(this->name);

    qemuCommand << "-pidfile";
    qemuCommand << pipe.append(".pid");

    // Network
    if (this->useNetwork) {
        qemuCommand << "-net";
        qemuCommand << "nic";

        qemuCommand << "-net";
        qemuCommand << "user";
    } else {
        qemuCommand << "-net";
        qemuCommand << "none";
    }

    // HDD
    //qemuCommand << "-drive";
    //qemuCommand << QString("file=").append(diskObject["path"].toString()).append(",index=0,media=disk");

    // CDROM TODO, for test not implemented yet :'(
    qemuCommand << "-cdrom";
    qemuCommand << "/home/xexio/Downloads/torrent/archlinux-2018.10.01-x86_64.iso";

    qDebug() << "Command " << qemuCommand;

    return qemuCommand;
}

void Machine::saveMachine() {
    QFile machineFile(this->configPath);
    machineFile.open(QIODevice::WriteOnly); // TODO: Check if open the file fails

    QJsonObject machineJSONObject;
    machineJSONObject["name"]        = this->name;
    machineJSONObject["OSType"]      = this->OSType;
    machineJSONObject["OSVersion"]   = this->OSVersion;
    machineJSONObject["description"] = this->description;
    machineJSONObject["RAM"]         = this->RAM;
    machineJSONObject["network"]     = this->useNetwork;
    machineJSONObject["path"]        = this->path;
    machineJSONObject["uuid"]        = this->uuid;
    machineJSONObject["hostsoundsystem"] = this->hostSoundSystem;
    // TODO: Implement another types
    machineJSONObject["binary"] = "qemu-system-x86_64";

    QJsonObject cpu;
    cpu["CPUType"]     = this->CPUType;
    cpu["CPUCount"]    = this->CPUCount;
    cpu["socketCount"] = this->socketCount;
    cpu["coresSocket"] = this->coresSocket;
    cpu["threadsCore"] = this->threadsCore;
    cpu["maxHotCPU"]   = this->maxHotCPU;
    machineJSONObject["cpu"] = cpu;

    QJsonObject gpu;
    gpu["GPUType"]  = this->GPUType;
    gpu["keyboard"] = this->keyboard;
    machineJSONObject["gpu"] = gpu;

    QJsonArray media;
    for (int i = 0; i < this->media.size(); ++i) {
        QJsonObject disk;
        disk["name"] = this->media.at(i).name();
        disk["path"] = this->media.at(i).path();
        disk["type"] = this->media.at(i).type();
        disk["interface"] = this->media.at(i).interface();
        disk["uuid"] = QUuid::createUuid().toString();

        media.append(disk);
    }

    machineJSONObject["media"] = media;

    QJsonObject kernelBoot;
    kernelBoot["enabled"] = this->m_boot.kernelBootEnabled();
    kernelBoot["kernelPath"] = this->m_boot.kernelPath();
    kernelBoot["initrdPath"] = this->m_boot.initrdPath();
    kernelBoot["kernelArgs"] = this->m_boot.kernelArgs();

    QJsonObject boot;
    boot["bootMenu"] = this->m_boot.bootMenu();
    boot["kernelBoot"] = kernelBoot;
    boot["bootOrder"] = QJsonArray::fromStringList(this->m_boot.bootOrder());

    machineJSONObject["boot"] = boot;

    machineJSONObject["accelerator"] = QJsonArray::fromStringList(this->accelerator);
    machineJSONObject["audio"] = QJsonArray::fromStringList(this->audio);

    QJsonDocument machineJSONDocument(machineJSONObject);

    machineFile.write(machineJSONDocument.toJson());

    qDebug() << "Machine saved";
}

/**
 * @brief Insert the new machine in the machines file
 *
 * Insert the new machine in the machines file.
 * At the bottom of the file.
 */
void Machine::insertMachineConfigFile(){

    // TODO: Get the data directory path from QSettings
    // Open the file
    QString dataDirectoryPath = QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/");

    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    machinesFile.open(QIODevice::ReadWrite); // TODO: Check if open the file fails

    // Read all data included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonObject machinesObject;

    // Read other machines
    QJsonArray machines = machinesDocument["machines"].toArray();

    // Create the new machine
    QJsonObject machine;
    machine["uuid"]       = this->uuid;
    machine["name"]       = this->name;
    machine["path"]       = this->path;
    machine["configpath"] = this->configPath;
    machine["icon"]       = this->OSVersion.toLower().replace(" ", "_");

    machines.append(machine);
    machinesObject["machines"] = machines;

    QJsonDocument machinesJSONDocument(machinesObject);

    machinesFile.seek(0);
    machinesFile.write(machinesJSONDocument.toJson());
    machinesFile.close();
}

Media::Media() {
    qDebug() << "Media object created";
}

Media::~Media() {
    qDebug() << "Media object destroyed";
}

/**
 * @brief Get the media name
 * @return media name
 *
 * Get the media names
 */
QString Media::name() const {
    return m_name;
}

/**
 * @brief Set the media name
 * @param mediaName, new name for the media
 *
 * Set the media name
 */
void Media::setName(const QString &name) {
    m_name = name;
}

/**
 * @brief Get the media path
 * @return media path
 *
 * Get the media path
 */
QString Media::path() const {
    return m_path;
}

/**
 * @brief Set the new media path
 * @param mediaPath, new media path
 *
 * Set the new media path
 */
void Media::setPath(const QString &path) {
    m_path = path;
}

/**
 * @brief Get the media size
 * @return media size
 *
 * Get the media size
 */
qlonglong Media::size() const {
    return m_size;
}

/**
 * @brief Set the media size
 * @param mediaSize, media size
 *
 * Set the media size
 */
void Media::setSize(const qlonglong &size) {
    m_size = size;
}

/**
 * @brief Get the media type
 * @return media type
 *
 * Get the media type
 */
QString Media::type() const {
    return m_type;
}

/**
 * @brief Set the media type
 * @param mediaType, new media type
 *
 * Set the media type
 * Ex: ide, scsi...
 */
void Media::setType(const QString &type) {
    m_type = type;
}

/**
 * @brief Get the media format
 * @return media format
 *
 * Get the media format
 * Ex: qcow, qcow2, raw...
 */
QString Media::format() const {
    return m_format;
}

/**
 * @brief Set the media format
 * @param mediaFormat, new media format
 *
 * Set the media format
 */
void Media::setFormat(const QString &format) {
    m_format = format;
}

/**
 * @brief Get the media interface
 * @return media interface
 *
 * Get the media interface
 * Ex: ide, scsi...
 */
QString Media::interface() const {
    return m_interface;
}

/**
 * @brief Set the new media interface
 * @param mediaInterface, media interface
 *
 * Set the new media interface
 */
void Media::setInterface(const QString &interface) {
    m_interface = interface;
}

/**
 * @brief Get the media cache
 * @return the media cache
 *
 * Get the media cache
 * Ex: none, writeback...
 */
QString Media::cache() const {
    return m_cache;
}

/**
 * @brief Set the media cache
 * @param mediaCache, new media cache
 *
 * Set the media cache
 */
void Media::setCache(const QString &cache) {
    m_cache = cache;
}

/**
 * @brief Get the media IO
 * @return media IO
 *
 * Get the media IO
 * Ex: threads, native...
 */
QString Media::IO() const {
    return m_IO;
}

/**
 * @brief Set the media IO
 * @param mediaIO, new media IO
 *
 * Set the media IO
 */
void Media::setIO(const QString &IO) {
    m_IO = IO;
}

QUuid Media::uuid() const {
    return m_uuid;
}

void Media::setUuid(const QUuid &uuid) {
    m_uuid = uuid;
}

Boot::Boot() {
    qDebug() << "Boot object created";
}

Boot::~Boot() {
    qDebug() << "Boot object destroyed";
}

bool Boot::bootMenu() const {
    return m_bootMenu;
}

void Boot::setBootMenu(bool bootMenu) {
    m_bootMenu = bootMenu;
}

bool Boot::kernelBootEnabled() const {
    return m_kernelBootEnabled;
}

void Boot::setKernelBootEnabled(bool kernelBootEnabled) {
    m_kernelBootEnabled = kernelBootEnabled;
}

QString Boot::kernelPath() const {
    return m_kernelPath;
}

void Boot::setKernelPath(const QString &kernelPath) {
    m_kernelPath = kernelPath;
}

QString Boot::initrdPath() const {
    return m_initrdPath;
}

void Boot::setInitrdPath(const QString &initrdPath) {
    m_initrdPath = initrdPath;
}

QString Boot::kernelArgs() const {
    return m_kernelArgs;
}

void Boot::setKernelArgs(const QString &kernelArgs) {
    m_kernelArgs = kernelArgs;
}

QStringList Boot::bootOrder() const {
    return m_bootOrder;
}

void Boot::setBootOrder(const QStringList &bootOrder) {
    m_bootOrder = bootOrder;
}

void Boot::addBootOrder(const QString bootOrder) {
    if( ! this->m_bootOrder.contains(bootOrder)){
        this->m_bootOrder.append(bootOrder);
    }
}

void Boot::removeBootOrder(const QString bootOrder) {
    if(this->m_bootOrder.contains(bootOrder)){
        this->m_bootOrder.removeOne(bootOrder);
    }
}

void Boot::removeAllBootOrder() {
    this->m_bootOrder.clear();
}
