# ExamplePlugin

## 项目介绍

ExamplePlugin 是 WidgetExplorer 框架的一个示例插件，旨在演示如何基于 WECore 开发一个简单的 Qt 插件。
该插件包含一个对话框窗口，可以接收来自框架的消息，并将消息内容显示在文本框中。

## 插件功能

* 注册一个名为 `ExampleWidget` 的窗口部件到 WidgetExplorer 的部件管理器。
* 接收框架发送的 `WMetaData` 消息，解析命令并执行相应操作：

  * 当命令为 `"start"` 时，显示对话框窗口
  * 当命令为 `"clear"` 时，清空对话框内容

* 提供一个简单的 UI 界面（基于 `exampledialog.ui`），包含一个只读的 `QTextEdit` 控件，用于展示接收到的消息。

## 项目组成

1. **插件入口类 (`ExamplePlugin`)**  
   继承自 `WPluginInterface`，负责插件的初始化、消息接收和反初始化。
2. **窗口部件类 (`ExampleWidget`)**  
   继承自 `WWidget`，实现 `receiveMessage` 和 `initWidget`。
3. **对话框类 (`ExampleDialog`)**  
   继承自 `QWidget`，封装了 UI 界面。
4. **资源文件**  
   `exampledialog.ui`：对话框界面。
5. **项目配置文件 (`ExamplePlugin.pro`)**  
   配置 Qt 模块、编译选项、输出路径、依赖库（WECore）等。

## 版本内容更新

###### v1.1.0

* 初始版本，实现基本消息显示功能。
* 支持 `start` 和 `clear` 命令。
* 完成插件框架集成。

## 许可证

本项目使用[APACHE LICENSE, VERSION 2.0](https://www.apache.org/licenses/LICENSE-2.0)许可证，详见LICENSE文件

