/**
 * @file main.cpp
 * @brief Program entry point: application initialization and main flow control.
 * @author howdy213
 * @date 2026-08-20
 * @version 2.0.0
 *
 * Copyright 2025-2026 howdy213
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "lightwidget.h"

#include "WECore/plugin/wpluginconfigwidget.h"
#include "WECore/utils/wpath.h"
#include "WECore/metadata/wmetadocument.h"
#include "WECore/style/wstyle.h"
#include "WECore/we/we.h"

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QLabel>
#include <QLockFile>
#include <QMenu>
#include <QMessageBox>
#include <QStyleFactory>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <QVBoxLayout>
#include <QVariant>
#include <WECore/utils/wshellexecute.h>

using namespace we::Consts;
using namespace we;

// Forward declarations
QStringList processParams(int argc, char *argv[], bool &pluginManagerMode);
WMetaDocument *initConfigManager(WEBase *base);
bool handleQtEnvironment(WMetaDocument *config);
bool installTranslation(WMetaDocument *config);
bool applyStyle(WMetaDocument *config);
int handlePluginConfigManager(WPluginConfigManager *configManager);
int initMainPlugin(LightWidget *base, QStringList params,
                   WMetaDocument *config);

/**
 * @brief Application entry point.
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 * @return Exit code: 0 for a normal exit, non-zero on error or when the user
 *         quit early.
 *
 * Performs the following work:
 * 1. Takes the single instance lock so the program cannot run twice.
 * 2. Parses the command line to detect the plugin configuration manager mode.
 * 3. Initializes the core system (the WE framework and LightWidget).
 * 4. Loads the global configuration and sets the Qt environment variables.
 * 5. Runs the flow that matches the selected mode.
 */
int main(int argc, char *argv[]) {
    // Single instance lock: keeps the program from being started twice.
    QLockFile lockfile(WPath().getModuleFolder() + "we.lock");
    if (!lockfile.tryLock(0))
        return 1;

    bool pluginManagerMode = false;
    QStringList params = processParams(argc, argv, pluginManagerMode);

    // Create the LightWidget instance and initialize the core system.
    auto lw = new LightWidget;
    if (!WE::init(lw))
        return 0;
    if (!lw->init())
        return 0;

    // Load the global configuration file.
    auto config = initConfigManager(lw);

    // Set the Qt environment variables from the configuration; this has to be
    // done before QApplication is created.
    handleQtEnvironment(config);

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/icon/we.png"));

    // Install the UI translation; this has to be done after QApplication is
    // created and before any plugin is loaded.
    installTranslation(config);

    // Apply the UI style, which likewise has to be done after QApplication is
    // created.
    applyStyle(config);

    // Plugin configuration manager mode.
    if (pluginManagerMode)
        return handlePluginConfigManager(lw->pluginConfigManager());

    // Normal startup.
    if (!initMainPlugin(lw, params, config))
        return 1; // No main plugin is available, so there is nothing to run.

    return a.exec();
}

/**
 * @brief Parses the command line into the run mode and the remaining arguments.
 * @param argc Number of arguments.
 * @param argv Argument array.
 * @param pluginManagerMode Output: whether the plugin configuration manager mode
 *        was requested.
 * @return The arguments left after the mode flag was removed, normally the ones
 *         the main plugin needs.
 *
 * Recognizes `-pluginmanager` and `--pluginmanager`. Every other argument is
 * collected and returned for the main plugin to use.
 */
QStringList processParams(int argc, char *argv[], bool &pluginManagerMode) {
    QStringList params;
    for (int i = 1; i < argc; ++i) {
        QString arg = QString::fromLocal8Bit(argv[i]);
        if (arg == "-pluginmanager" || arg == "--pluginmanager") {
            pluginManagerMode = true;
            continue;
        }
        params.append(arg);
    }
    return params;
}

/**
 * @brief Initializes the global configuration document.
 * @param base The WEBase instance.
 * @return The initialized configuration document.
 *
 * Creates the configuration file when it does not exist yet, so the document can
 * always be loaded from disk.
 */
WMetaDocument *initConfigManager(WEBase *base) {
    QString path = WPath().getModuleFolder() + Config::ConfigPath;
    QDir().mkpath(QFileInfo(path).absolutePath());
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        file.close();
    base->getWEClass()->configManager()->load(path, true);
    return base->getWEClass()->configManager();
}

/**
 * @brief Sets the Qt environment variables from the configuration.
 * @param config The global configuration.
 * @return Always true.
 *
 * Covers the font DPI and the scale factor. Has to run before QApplication is
 * created, because Qt reads these variables during startup.
 */
bool handleQtEnvironment(WMetaDocument *config) {
    if (/*qEnvironmentVariableIsEmpty("QT_FONT_DPI")*/ true) {
        if (config->hasArg(Config::Font))
            qputenv("QT_FONT_DPI",
                    qvariant_cast<QByteArray>(config->get(Config::Font)));
        if (config->hasArg(Config::Scale))
            qputenv("QT_SCALE_FACTOR",
                    qvariant_cast<QByteArray>(config->get(Config::Scale)));
    }
    return true;
}

/**
 * @brief Installs the UI translation selected by the Language entry.
 * @param config The global configuration.
 * @return Always true.
 *
 * Has to be called after QApplication is created. The value is zh_CN or en_US and
 * defaults to zh_CN; en_US is the language the sources are written in, so it
 * needs no translation file. The translation files are embedded as
 * `:/i18n/WECore_<locale>.qm` when WECore is built.
 */
bool installTranslation(WMetaDocument *config) {
    static QTranslator translator;
    QString language = config->hasArg(Config::Language)
                           ? config->get(Config::Language).toString()
                           : QString();
    if (language.isEmpty())
        language = "zh_CN";
    if (language == "en_US")
        return true; // English is the source language, so the source is used.
    if (!translator.load(":/i18n/WECore_" + language))
        qWarning() << "Failed to load the translation file:" << language;
    else
        QCoreApplication::installTranslator(&translator);
    return true;
}

/**
 * @brief Applies the UI style described by the Style section.
 * @param config The global configuration.
 * @return Whether the style was applied; an unknown style name falls back to the
 *         default style.
 *
 * Has to be called after QApplication is created. The selection is stored in the
 * `Style` section of the configuration (StyleName / Theme / ColorTheme /
 * AccentColor / StyleFile / ThemeFile); a settings dialog edits that section
 * through the sub-config WStyle contributes, while this function only reads the
 * whole section at startup and hands it to WStyle.
 *
 * The section is read as a whole map so that WStyle can resolve the entry names
 * itself.
 */
bool applyStyle(WMetaDocument *config) {
    if (!config)
        return false;
    const QVariantMap values = config->get(Config::Style).toMap();
    const QString name =
        values
            .value(QString::fromLatin1(style::KeyStyleName),
                   QString::fromLatin1(style::StyleDefault))
            .toString();
    style::WStyle *style = style::WStyle::create(name);
    // The configured style is unavailable, for instance because the style plugin
    // it comes from is not installed on this machine.
    if (!style)
        style = style::WStyle::create(QString::fromLatin1(style::StyleDefault));
    return style ? style->applyFromValues(values) : false;
}

/**
 * @brief Handles the plugin configuration manager mode.
 * @param configManager The plugin configuration manager.
 * @return The process exit code.
 *
 * This mode edits the plugin configuration files on their own and loads no plugin
 * at all. The editor dialog is shown and the process exits once editing is done.
 */
int handlePluginConfigManager(WPluginConfigManager *configManager) {
    QDialog dlg;
    dlg.setWindowTitle("Plugin Configuration Manager");
    dlg.resize(800, 600);

    // Create the configuration editor and point it at the root configuration.
    WPluginConfigWidget *configWidget =
        new WPluginConfigWidget(configManager, &dlg);
    QString rootConfigPath = WPath().getModuleFolder() + Plugins::ConfigPath;
    configWidget->setRootJsonPath(rootConfigPath);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->addWidget(configWidget);

    QLabel *tipLabel = new QLabel("Safe mode: no plugin is loaded.", &dlg);
    layout->addWidget(tipLabel);

    dlg.exec();

    // Ask whether the application should be restarted once editing is done.
    QMessageBox::StandardButton ret = QMessageBox::question(
        nullptr, QStringLiteral("Plugin Configuration Manager"),
        QStringLiteral("Configuration editing finished. Restart the application?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (ret != QMessageBox::Yes) {
        return 0;
    }

    WShellExecute::asyncExecute(
        WPath().getModuleFolder() + "tools/WELauncher.exe", "open", "-t 500");
    return 0;
}

/**
 * @brief Initializes and starts the main plugin.
 * @param base The LightWidget instance.
 * @param params The arguments parsed from the command line, passed on to the main
 *        plugin.
 * @param config The global configuration.
 * @return 1 on success, 0 on failure.
 *
 * Reads the LocalUuid of the default main plugin from the configuration (empty
 * when unset) and calls LightWidget::initMainPlugin to load and initialize the
 * main plugin. A message box is shown and 0 is returned when that fails.
 */
int initMainPlugin(LightWidget *base, QStringList params,
                   WMetaDocument *config) {
    QUuid defaultMain = QUuid();
    if (config->hasArg(Config::DefaultMain))
        defaultMain = QUuid(base->getWEClass()
                                ->configManager()
                                ->get(Config::DefaultMain)
                                .toString());

    if (!base->initMainPlugin(params, defaultMain)) {
        QMessageBox::StandardButton ret = QMessageBox::question(
            nullptr, QStringLiteral("Notice"),
            QStringLiteral("No main widget is available. Open the plugin "
                           "configuration manager?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (ret == QMessageBox::Yes) {
            return handlePluginConfigManager(base->pluginConfigManager());
        } else {
            return 0;
        }
    }
    return 1;
}
