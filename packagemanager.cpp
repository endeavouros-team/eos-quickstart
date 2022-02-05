#include "packagemanager.h"

#include <QFile>

PackageManager::PackageManager(const QString &binaryPath, PackageModel *packageModel, Terminal terminal, QObject *parent)
    : QObject{parent}
{
    m_preferredTerminal = terminal;
    m_pacmanBinary = binaryPath;
    m_model = packageModel;
    populateInstalledPackages();
}

const QStringList PackageManager::installedPackages() const
{
    return m_installedPackages;
}

bool PackageManager::installPackages()
{
    Terminal terminal = getTerminal();

    if (terminal.binary.isEmpty()) {
        return false;
    }

    QList<PackageData> *packages = m_model->model();

    QStringList packageList;
    for (const PackageData &package : *packages) {
        if (package.isChecked) {
            packageList.append(package.packageName);
        }
    }

    if (packageList.isEmpty()) {
        return false;
    }

    QProcess proc;

    proc.setProgram(terminal.binary);
    proc.setArguments(QStringList() << terminal.args << "pkexec" << "bash" << "-c" << m_pacmanBinary + " -Syu " + packageList.join(" ") + " ; read -p '" + tr("Press enter to resume") + "...'");

    proc.start();

    proc.waitForFinished(1000 * 60);

    if( proc.exitCode() == 0) {
        return true;
    } else {
        return false;
    }
}

bool PackageManager::populateInstalledPackages()
{
    if (m_pacmanBinary.isEmpty()) {
        qWarning() << tr("ERROR: Package Manager not set");
        return false;
    }

    m_installedPackages.clear();

    QProcess proc;
    proc.setProgram(m_pacmanBinary);
    proc.setArguments({"-Qq"});
    proc.start();

    if (!proc.waitForFinished()) {
        qWarning() << tr("Failed to start process ") << proc.program() << proc.arguments();
        return false;
    }

    QString output = proc.readAllStandardOutput();
    const QStringList lines = output.split('\n');

    for (const QString &line : lines) {
        if (!line.trimmed().isEmpty()) {
            m_installedPackages.append(line.trimmed());
        }
    }

    return true;
}

Terminal PackageManager::getTerminal()
{
    if (QFile::exists(m_preferredTerminal.binary)) {
        return m_preferredTerminal;
    }

    // Iterate over the list until we find an installed terminal
    for (const Terminal &terminal : m_terminalList) {
        if (QFile::exists(terminal.binary)) {
            return terminal;
        }
    }
    qWarning() << tr("Preferred terminal not available and no terminal found on system");
    return Terminal();
}


