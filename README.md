中文 | [English](README_en.md)
# WidgetExplorer

## 项目介绍

WidgetExplorer 是一个基于 Qt 插件框架的桌面应用程序。

## 框架功能

* 插件属性配置
* 切换主界面插件
* 调用外部程序作为插件
* ...

## 项目组成

1. [src](docs/src.md)
   主程序WidgetExplorer，解析配置文件，完成初始化
2. [deps/WECore](https://github.com/howdy213/WECore)
   生成wecore.dll，WidgetExplorer框架
3. [plugins/LightWidget](https://github.com/howdy213/LightWidget)
   生成LightMain，轻量的MainWidget
4. [plugins/ExamplePlugin](https://github.com/howdy213/ExamplePlugin)
   生成ExamplePlugin.dll，示例插件

## 环境依赖

- Qt 6（需包含 `core`、`gui`、`widgets` 模块）

## 快速开始

1. **获取源码 **  
   
   ```bash
   git clone https://github.com/howdy213/WidgetExplorer.git
   cd WidgetExplorer
   ```
   
2. **准备 WECore 依赖**  
   确保 `deps/WECore` 目录下包含 WECore 源码（可通过子模块或手动下载）。  
   
   ```bash
   git submodule init
   git submodule update --init --recursive
   ```
   
3. **使用 Qt Creator 打开 `WidgetExplorer.pro`**  
   配置构建套件（Kit），并进行编译。

4. **配置插件**  
   根据需要编辑 `config/config.json` 和插件自身的配置文件（参见Release中 `plugins/` 下的示例），确保插件路径正确。

5. **运行**  
   编译后运行生成的可执行文件。程序会加载所有插件，若存在多个主界面插件，则弹出选择对话框。

## 版本内容更新

###### v1.2.2

1. 整理项目框架

## 许可证

WidgetExplorer 使用 Apache License 2.0 开源许可证
详细信息请参阅项目根目录下的 [LICENSE](LICENSE) 文件。
