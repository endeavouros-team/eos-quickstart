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

    const QStringList installedPackages() const;

public slots:
    bool installPackages();

private:
    bool populateInstalledPackages();

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
