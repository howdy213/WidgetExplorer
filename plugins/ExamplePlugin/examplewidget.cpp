/**
 * @file examplewidget.cpp
 * @brief 示例窗口实现文件
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
#include "examplewidget.h"
#include "exampledialog.h"

#include "WECore/WPlugin/wplugin.h"
#include "WECore/WPlugin/wplugindata.h"

#include <QWidget>
#include <windows.h>
using namespace we::Consts;

ExampleWidget::ExampleWidget(WEBase *parent) { Q_UNUSED(parent) }
ExampleWidget::~ExampleWidget() {}
void ExampleWidget::receiveMessage(WMetaData &msg) {
    if (msg.map[Data::Command] == "start") {
        if (widget.isHidden())
            widget.show();
        else
            widget.activateWindow();
    }
    widget.appCommand((QString) "\n--" +
                      qvariant_cast<QString>(PPlugin->getMetaData(Plugin::Name)) +
                      "@" +
                      qvariant_cast<QString>(PPlugin->getMetaData(Plugin::Path)) +
                      "--\nfrom:" + msg.from + "\ndest:" + msg.dest + "\nmsg:\n");
    widget.appCommand(msg.map);
    if (msg.map[Data::Command] == "clear") {
        widget.setCommand(data);
        if (widget.isHidden())
            widget.show();
        else
            widget.activateWindow();
    }
}
void ExampleWidget::initWidget() {
    this->setAttr(Widget::Name, "ExampleWidget");
}
