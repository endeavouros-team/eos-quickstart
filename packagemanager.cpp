#include "packagemanager.h"

#include <QFile>
#include <QProcessBuilder>

PackageManager::PackageManager(const QString &binaryPath, PackageModel *packageModel, Terminal terminal, QObject *parent)
    : QObject{parent}, m_pacmanBinary{binaryPath}, m_model{packageModel}, m_preferredTerminal{terminal}
{
    populateInstalledPackages();
}

bool PackageManager::installPackages()
{
    Terminal terminal = getTerminal();

    if (terminal.binary.isEmpty()) {
        qWarning() << QStringLiteral("No supported terminal found, cannot install packages");
        return false;
    }

    const QStringList packageList = m_model->getCheckedPackages();

    if (packageList.isEmpty()) {
        return false;
    }

    QProcessBuilder builder;
    builder.setProgram(terminal.binary);
    builder.setArguments({terminal.args, "pkexec", "bash", "-c", m_pacmanBinary + " -Syu " + packageList.join(" ") + " ; read -p '" + tr("Press enter to resume") + "...'"});
    builder.start();

    builder.waitForFinished();

    // Update the list of installed packages and refresh the model and the view
    populateInstalledPackages();
    m_model->refresh(m_installedPackages);

    return builder.exitCode() == 0;
}

bool PackageManager::populateInstalledPackages()
{
    if (m_pacmanBinary.isEmpty()) {
        qWarning() << QStringLiteral("ERROR: Package Manager not set");
        return false;
    }

    m_installedPackages.clear();

    QProcessBuilder builder;
    builder.setProgram(m_pacmanBinary);
    builder.setArguments({"-Qq"});
    builder.start();

    if (!builder.waitForFinished()) {
        qWarning() << QStringLiteral("Failed to start process ") << builder.program() << builder.arguments();
        return false;
    }

    const QString output = builder.readAllStandardOutput();
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

    return Terminal();
}
