#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QIcon>
#include <QLocale>
#include <QThread>
#include <QTranslator>

#include "packagemanager.h"
#include "packagemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "eosquickstart_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QSettings settings("/etc/eos-quickstart.conf", QSettings::NativeFormat);

    const Terminal preferredTerminal = {settings.value("PreferredTerminalPath").toString(), settings.value("PreferredTerminalArgs").toString()};

    PackageModel model;

    PackageManager packageManager(settings.value("PackageManager", "/usr/bin/pacman").toString(), &model, preferredTerminal);

    const QStringList packageList = packageManager.installedPackages();

    model.loadModel(settings, packageList);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("packageModel"), &model);
    engine.rootContext()->setContextProperty(QStringLiteral("packageManager"), &packageManager);
    const QUrl url(u"qrc:/eosquickstart/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);
    app.setWindowIcon(QIcon::fromTheme("system-software-install"));
    return app.exec();
}
