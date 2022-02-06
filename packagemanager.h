#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <QObject>
#include <QProcess>

#include "packagemodel.h"

struct Terminal {
    QString binary;
    QString args;
};

class PackageManager : public QObject
{
    Q_OBJECT
public:
    explicit PackageManager(const QString &binaryPath, PackageModel *packageModel, Terminal terminal, QObject *parent = nullptr);

    const QStringList installedPackages() const { return m_installedPackages; }

public slots:
    /*
     * Installs all packages that are marked as "checked" in the package model
     *
     * Return true on success, false otherwise
     */
    bool installPackages();

private:
    /*
     * Finds all installed packages on the system and saves them in m_installedPackages
     *
     * Return true on success, false otherwise
     */
    bool populateInstalledPackages();

    /*
     * Finds the best available terminal
     *
     * If m_preferredTerminal is valid, that will be returned.  If not,
     * it iterates over the list of terminals in m_terminalList to find
     * a valid terminal.  If none are available a default constructed
     * Terminal is returned.
     */
    Terminal getTerminal();

    QString m_pacmanBinary;
    Terminal m_preferredTerminal;
    QStringList m_installedPackages;
    PackageModel *m_model;

    const QVector<Terminal> m_terminalList = {
        {"/usr/bin/konsole", "-e"},
        {"/usr/bin/gnome-terminal", "--"},
        {"/usr/bin/xfce4-terminal", "-x"},
        {"/usr/bin/lxterminal", "-e"},
        {"/usr/bin/xterm", "-e"},
        {"/usr/bin/alacritty", "-e"}
    };
};

#endif // PACKAGEMANAGER_H
