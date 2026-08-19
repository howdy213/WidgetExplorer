/**
 * @file lightsystem.cpp
 * @brief Implementation file for the lightweight plugin loading system.
 * @author howdy213
 * @date 2026-05-04
 * @version 2.0.0
 *
 * @copyright Copyright 2025-2026 howdy213
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
#include "lightsystem.h"
#include "WECore/file/wpath.h"
#include "WECore/plugin/wplugin.h"
#include "WECore/plugin/wpluginmanager.h"
#include "WECore/we/we.h"
#include "WECore/we/webase.h"

#include <QCoreApplication>
#include <QDir>
#include <qjsondocument.h>
#include <qjsonobject.h>

using namespace we::Consts;
using namespace we;

/**
 * @class LightSystemPrivate
 * @brief Private implementation class for LightSystem.
 */
class LightSystemPrivate {
public:
    // Currently empty, reserved for future extensions
};

/**
 * @brief Constructs a LightSystem object.
 */
LightSystem::LightSystem() { d = new LightSystemPrivate; }

/**
 * @brief Destroys the LightSystem object.
 */
LightSystem::~LightSystem() {
    delete d;
    d = nullptr;
}

QJsonObject LightSystem::readJsonFile(QString filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QJsonObject();
    }
    QString content = file.readAll();
    file.close();
    return QJsonDocument::fromJson(content.toUtf8()).object();
}

/**
 * @brief Loads all plugins defined in the configuration file.
 */
void LightSystem::loadAllPlugin() {
    QString plugins = WPath().getModuleFolder() + Plugins::ConfigPath;
    QJsonObject jsonObj = readJsonFile(plugins);
    if (jsonObj.isEmpty())
        return;

    if (jsonObj.contains(Plugins::Plugins)) {
        QJsonObject pluginsObj = jsonObj[Plugins::Plugins].toObject();
        for (auto it = pluginsObj.begin(); it != pluginsObj.end(); ++it) {
            loadPluginRecursive(plugins, it.value().toObject());
        }
    }
}

/**
 * @brief Recursively loads plugins from configuration.
 * @param curPath Path to the directory containing the configuration.
 * @param config JSON object containing the configuration.
 */
void LightSystem::loadPluginRecursive(QDir curPath, QJsonObject object) {
    QDir rootPath = curPath;
    QDir filePath = WPath().resolvePath(
        rootPath.path(), object[Plugins::PluginConfigPath].toString());

    QJsonObject jsonObj = readJsonFile(filePath.path());
    if (jsonObj.isEmpty())
        return;

    if (jsonObj.contains(Plugins::Plugins)) {
        QJsonObject pluginsObj = jsonObj[Plugins::Plugins].toObject();
        for (auto it = pluginsObj.begin(); it != pluginsObj.end(); ++it) {
            loadPluginRecursive(filePath, it.value().toObject());
        }
    } else {
        QDir rootPath = filePath;
        loadSinglePlugin(rootPath, jsonObj);
    }
}

/**
 * @brief Loads a plugin using its JSON configuration.
 * @param curPath Path to the directory containing the configuration.
 * @param config JSON configuration string.
 */
void LightSystem::loadPlugin(QDir curPath, QJsonObject object) {
    QDir pluginPath = WPath().resolvePath(
        curPath.path(), object[Plugins::PluginConfigPath].toString());
    QJsonObject pluginObj = readJsonFile(pluginPath.path());
    if (pluginObj.isEmpty())
        return;
    loadSinglePlugin(curPath, pluginObj);
}

/**
 * @brief Loads a single plugin instance.
 * @param curPath Path to the directory containing the plugin configuration.
 * @param pluginObj JSON object containing the plugin configuration.
 */
void LightSystem::loadSinglePlugin(QDir curPath, QJsonObject pluginObj) {
    if (!pluginObj.contains(Plugin::Path))
        return;

    WPlugin *plugin = new WPlugin(WE::inst()->getWEClass()->pluginManager());
    plugin->readConfig(curPath.path(),pluginObj);
    WE::inst()->getWEClass()->pluginManager()->addPlugin(plugin);
}