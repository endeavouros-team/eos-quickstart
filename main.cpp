#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QIcon>
#include <QLocale>
#include <QThread>
#include <QTranslator>
#include <QScopedPointer>

#include "PackageManager.h"
#include "PackageModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Load translator
    QScopedPointer<QTranslator> translator(new QTranslator());
    const auto uiLanguages = QLocale::system().uiLanguages();
    for (const auto &locale : uiLanguages) {
        const QString baseName = "eosquickstart_" + QLocale(locale).name();
        if (translator->load(":/i18n/" + baseName)) {
            app.installTranslator(translator.data());
            break;
        }
    }

    // Load settings
    QScopedPointer<QSettings> settings(new QSettings("/etc/eos-quickstart.conf", QSettings::NativeFormat));

    // Configure preferred terminal
    const Terminal preferredTerminal = {
        settings->value("PreferredTerminalPath").toString(),
        settings->value("PreferredTerminalArgs").toString()
    };

    // Load package manager
    QScopedPointer<PackageManager> packageManager(new PackageManager(
        settings->value("PackageManager", "/usr/bin/pacman").toString(),
        preferredTerminal
    ));

    // Load installed packages
    const auto packageList = packageManager->installedPackages();

    // Load package model
    QScopedPointer<PackageModel> model(new PackageModel());
    model->loadModel(*settings, packageList);

    // Configure QML context properties
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("packageModel", model.data());
    engine.rootContext()->setContextProperty("packageManager", packageManager.data());

    // Load QML UI
    const QUrl url(u"qrc:/eosquickstart/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // Set window icon and start event loop
    app.setWindowIcon(QIcon::fromTheme("system-software-install"));
    return app.exec();
}
