#!/usr/bin/env python3
"""
交互式动态生成Qt插件工程，基于ExamplePlugin模板。
自动适应模板目录文件，替换以下内容：
- 文件名中的 "example" -> 插件名小写
- 文件内容中的 "EXAMPLE" -> 插件名大写
- 文件内容中的 "Example" -> 插件名Pascal
- 文件内容中的 "example" -> 插件名小写
- 特定注释字段：作者、版权、日期、版本

用法：直接运行脚本，按提示输入信息。
"""

import os
import sys
import shutil
import datetime
import re
from pathlib import Path

# ----------------------------------------------------------------------
# 参数输入
# ----------------------------------------------------------------------
def get_input(prompt, default=None, required=False):
    while True:
        if default:
            value = input(f"{prompt} (默认: {default}): ").strip()
            if not value:
                return default
        else:
            value = input(f"{prompt}: ").strip()
        if required and not value:
            print("此项不能为空，请重新输入。")
            continue
        return value

def get_date(default):
    """获取日期，格式 YYYY-M-D"""
    while True:
        date_str = input(f"文档日期 (默认: {default}): ").strip()
        if not date_str:
            return default
        parts = date_str.split('-')
        if len(parts) == 3 and all(p.isdigit() for p in parts):
            return date_str
        print("日期格式应为 YYYY-M-D，例如 2026-2-20")

def get_version(default):
    while True:
        ver = input(f"版本号 (默认: {default}): ").strip()
        if not ver:
            return default
        if ver.count('.') in (1,2) and all(p.replace('.','').isdigit() for p in ver.split('.')):
            return ver
        print("版本号格式应为 x.y 或 x.y.z，例如 1.2.0")

# ----------------------------------------------------------------------
# 路径处理
# ----------------------------------------------------------------------
def get_root_dir():
    """脚本所在目录的父目录视为ROOT"""
    return Path(__file__).parent.absolute().parent

# ----------------------------------------------------------------------
# 名称转换
# ----------------------------------------------------------------------
def to_pascal(name):
    return name[0].upper() + name[1:] if name else name

def to_upper(name):
    return name.upper()

def to_lower(name):
    return name.lower()

# ----------------------------------------------------------------------
# 主逻辑
# ----------------------------------------------------------------------
def main():
    print("=" * 50)
    print("       从 ExamplePlugin 生成新插件")
    print("=" * 50)

    base = get_input("插件基础名 (如 Test)", required=True)
    author = get_input("作者名", required=True)
    now = datetime.datetime.now()
    default_copyright = str(now.year)
    copyright_year = get_input("版权年份 (如 2026 或 2025-2026)", default=default_copyright)
    default_date = f"{now.year}-{now.month}-{now.day}"
    date = get_date(default_date)
    default_version = "1.2.0"
    version = get_version(default_version)

    base_pascal = to_pascal(base)
    base_upper = to_upper(base)
    base_lower = to_lower(base)

    plugin_class = base_pascal + "Plugin"
    widget_class = base_pascal + "Widget"
    dialog_class = base_pascal + "Dialog"
    plugin_lower = base_lower + "plugin"
    widget_lower = base_lower + "widget"
    dialog_lower = base_lower + "dialog"

    root_dir = get_root_dir()
    src_dir = root_dir / "plugins" / "ExamplePlugin"
    if not src_dir.is_dir():
        print(f"错误：源模板目录不存在：{src_dir}")
        sys.exit(1)

    dst_dir = root_dir / "plugins" / plugin_class
    if dst_dir.exists():
        answer = input(f"目标目录 {dst_dir} 已存在，是否覆盖？(y/n): ").strip().lower()
        if answer not in ('y', 'yes'):
            print("操作取消。")
            return
        shutil.rmtree(dst_dir)
    dst_dir.mkdir(parents=True, exist_ok=True)

    # 正则替换规则（用于注释字段）
    regex_replacements = [
        (r'@author\s+howdy213', f'@author {author}'),
        (r'Copyright\s+\d{4}(?:-\d{4})?\s+howdy213', f'Copyright {copyright_year} {author}'),
        (r'@date\s+\d{4}-\d{1,2}-\d{1,2}', f'@date {date}'),
        (r'@version\s+\d+\.\d+\.\d+', f'@version {version}'),
    ]

    # 全局字符串替换（按长度降序，避免子串误伤）
    replacements = [
        ("EXAMPLE", base_upper),
        ("Example", base_pascal),
        ("example", base_lower),
    ]
    replacements.sort(key=lambda x: len(x[0]), reverse=True)

    file_count = 0

    for src_file in src_dir.rglob('*'):
        if not src_file.is_file():
            continue

        rel_path = src_file.relative_to(src_dir)
        # 替换文件名中的 example 和 Example
        new_name = rel_path.name.replace("example", base_lower).replace("Example", base_pascal)
        dst_file = dst_dir / rel_path.parent / new_name
        dst_file.parent.mkdir(parents=True, exist_ok=True)

        try:
            with open(src_file, 'r', encoding='utf-8') as f:
                content = f.read()
        except UnicodeDecodeError:
            shutil.copy2(src_file, dst_file)
            print(f"已复制二进制文件: {rel_path} -> {dst_file.name}")
            file_count += 1
            continue

        # 先正则替换特定字段
        for pattern, repl in regex_replacements:
            content = re.sub(pattern, repl, content)

        # 再全局字符串替换
        for old, new in replacements:
            content = content.replace(old, new)

        with open(dst_file, 'w', encoding='utf-8') as f:
            f.write(content)

        print(f"已生成: {dst_file.relative_to(root_dir)}")
        file_count += 1

    print(f"\n✅ 插件生成完成！共处理 {file_count} 个文件。")
    print(f"   目录: {dst_dir}")
    print("   如有特殊字符串未正确替换，请手动调整")

if __name__ == "__main__":
    main()
    os.system("pause")
