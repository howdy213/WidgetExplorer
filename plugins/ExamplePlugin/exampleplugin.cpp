/**
 * @file exampleplugin.cpp
 * @brief 示例插件实现文件
 * @author howdy213
 * @date 2026-1-30
 * @version 1.1.0
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
#include "exampleplugin.h"
#include "exampledialog.h"
#include "examplewidget.h"

#include "WECore/WPlugin/wplugin.h"
#include "WECore/WPlugin/wplugindata.h"
#include "WECore/WPlugin/wwidgetmanager.h"

#include <QWidget>
#include <Windows.h>
using namespace we::Consts;

///
/// \brief ExamplePlugin::ExamplePlugin
///
ExamplePlugin::ExamplePlugin() {}
///
/// \brief ExamplePlugin::~ExamplePlugin
///
ExamplePlugin::~ExamplePlugin() {}
///
/// \brief ExamplePlugin::init
/// \param msg
/// \return
///
bool ExamplePlugin::init(WMetaData &msg) {
    PluginData::setData(qvariant_cast<WEBase *>(msg.map[Data::WEBase]));
    PluginData::setPlugin(qvariant_cast<WPlugin *>(msg.map[Data::Plugin]));

    auto widgetManager = PClass->widgetManager();
    widget = new ExampleWidget(PData);
    widgetManager->addWidget(QUuid::createUuid(), widget);

    WPlugin *plugin = qvariant_cast<WPlugin *>(msg.map[Data::Plugin]);
    plugin->setMetaData(Plugin::Name, "Example");
    return true;
}
///
/// \brief ExamplePlugin::recMsg
/// \param msg
///
void ExamplePlugin::recMsg(WMetaData &msg) { widget->sendMessageCallback(msg); }
///
/// \brief ExamplePlugin::deinit
/// \param msg
/// \return
///
bool ExamplePlugin::deinit(WMetaData &msg) {
    Q_UNUSED(msg);
    return true;
}
