English | [中文](README.md)

# WidgetExplorer

## Project Introduction

WidgetExplorer is a desktop application built on the Qt plugin framework.

## Framework Features

- Plugin property configuration
- Switch main interface plugin
- Call external programs as plugins
- ...

## Project Composition

1. [src](docs/src.md)  
   Main program WidgetExplorer, parses configuration files and completes initialization.
2. [deps/WECore](https://github.com/howdy213/WECore)  
   Builds `wecore.dll`, the WidgetExplorer framework.
3. [plugins/LightWidget](https://github.com/howdy213/LightWidget)  
   Builds `LightMain`, a lightweight MainWidget.
4. [plugins/ExamplePlugin](https://github.com/howdy213/ExamplePlugin)  
   Builds `ExamplePlugin.dll`, an example plugin.

## Dependencies

- Qt 6 (requires `core`, `gui`, `widgets` modules)

## Quick Start

1. **Get the source code**  

```bash
   git clone https://github.com/howdy213/WidgetExplorer.git
   cd WidgetExplorer
```

2. **Prepare the WECore dependency**  
   Ensure the `deps/WECore` directory contains the WECore source code (available via submodule or manual download).  

```bash
   git submodule update --init --recursive
```

3. **Open `WidgetExplorer.pro` with Qt Creator**  
   Configure the build kit and compile.

4. **Configure plugins**  
   Edit `config/config.json` and the plugin-specific configuration files (see the `plugins/` directory in the release for examples) as needed, ensuring plugin paths are correct.

5. **Run**  
   Run the generated executable after compilation. The program will load all plugins; if multiple main interface plugins exist, a selection dialog will appear.

## Version Updates

###### v1.2.3

1. Fixed an issue where the application would not exit after closing the window.

## License

WidgetExplorer is open-sourced under the Apache License 2.0.  
For details, please refer to the [LICENSE](LICENSE) file in the project root directory.
