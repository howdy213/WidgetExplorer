# WidgetExplorer

## 项目介绍

Qt插件框架

## 框架功能

* 插件属性配置
* 切换主界面插件
* 调用外部程序作为插件
* ...

## 项目组成

1. [src](docs/src.md)
   主程序WidgetExplorer，解析配置文件，完成初始化
2. [deps/WECore](docs/WECore.md)
   生成wecore.dll，WidgetExplorer框架
3. [plugins/LightWidget](docs/LightWidget.md)
   生成LightMain，轻量MainWidget
4. [plugins/ExamplePlugin](docs/ExamplePlugin.md)
   生成ExamplePlugin.dll，示例插件

## 环境依赖

Qt6

## 版本内容更新

###### v1.2.0

1. 导出类到wecore.dll中以改进二进制兼容性
2. 添加一些功能

## 许可证

[APACHE LICENSE, VERSION 2.0](https://www.apache.org/licenses/LICENSE-2.0)

