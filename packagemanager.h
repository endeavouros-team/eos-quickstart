#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <QObject>
#include <QProcess>

#include "packagemodel.h"

struct Terminal {
    QString binary;
    QString args;
};

class PackageManager : public QObject {
    Q_OBJECT
public:
    explicit PackageManager(const QString &binaryPath, PackageModel *packageModel, const Terminal &terminal, QObject *parent = nullptr);

    const QStringList& installedPackages() const { return m_installedPackages; }

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
    Terminal getTerminal() const;

    const QString m_pacmanBinary;
    const Terminal m_preferredTerminal;
    QStringList m_installedPackages;
    PackageModel *m_model;

    const auto m_terminalList = QVector<Terminal>{
        {"/usr/bin/konsole", "-e"},
        {"/usr/bin/gnome-terminal", "--"},
        {"/usr/bin/xfce4-terminal", "-x"},
        {"/usr/bin/lxterminal", "-e"},
        {"/usr/bin/xterm", "-e"},
        {"/usr/bin/alacritty", "-e"}
    };
};

#endif // PACKAGEMANAGER_H
