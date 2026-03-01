English | [中文](README.md)

# WidgetExplorer

## Introduction

WidgetExplorer is a desktop application based on the Qt plugin framework.

## Framework Features

* Plugin property configuration
* Switch main interface plugins
* Invoke external programs as plugins
* ...

## Project Components

1. [src](docs/src.md)  
   Main program WidgetExplorer, parses configuration files and completes initialization
2. [deps/WECore](https://github.com/howdy213/WECore)  
   Generates wecore.dll, the WidgetExplorer framework
3. [plugins/LightWidget](https://github.com/howdy213/LightWidget)  
   Generates LightMain, a lightweight MainWidget
4. [plugins/ExamplePlugin](https://github.com/howdy213/ExamplePlugin)  
   Generates ExamplePlugin.dll, an example plugin

## Environment Dependencies

- Qt 6 (requires `core`, `gui`, `widgets` modules)

## Quick Start

1. **Get the source code**  

   ```bash
   git clone https://github.com/howdy213/WidgetExplorer.git
   cd WidgetExplorer
   ```

2. **Prepare WECore dependency**  
   Ensure the `deps/WECore` directory contains the WECore source code (can be added via submodule or manual download).  

   ```bash
   git submodule init
   git submodule update --init --recursive
   ```

3. **Open `WidgetExplorer.pro` with Qt Creator**  
   Configure the kit and compile.

4. **Configure plugins**  
   Edit `config/config.json` and the respective plugin configuration files as needed (refer to the examples in the `plugins/` directory of the release), ensuring the plugin paths are correct.

5. **Run**  
   Run the generated executable after compilation. The program will load all plugins; if there are multiple main interface plugins, a selection dialog will appear.

## Changelog

###### v1.2.2

1. Project structure reorganization

## License

WidgetExplorer is open-sourced under the Apache License 2.0.  
For more details, please refer to the [LICENSE](LICENSE) file in the project root directory.
