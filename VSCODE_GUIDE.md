# VSCode 开发调试指南

## 前置要求

1. **Visual Studio Code** - 最新版本
2. **VSCode 扩展**：
   - [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack) (必需)
   - [C/C++ Themes](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-themes) (推荐)
3. **Visual Studio 2022** - 安装了 "使用 C++ 的桌面开发" 工作负载
   - 需要 MSVC 编译器 (v145) 和 Windows SDK

## 快速开始

### 1. 打开项目

在 VSCode 中打开项目根目录 `d:\project\Ling`

### 2. 选择 C++ 配置

- 按下 `Ctrl+Shift+P`
- 输入 `C/C++: Select a Configuration`
- 选择 `x64-Debug`

### 3. 构建项目

**方式一：使用快捷键**

- 按 `Ctrl+Shift+B` 开始构建

**方式二：使用命令面板**

- 按 `Ctrl+Shift+P`
- 输入 `Tasks: Run Build Task`
- 选择 `build-x64-debug`

**方式三：使用终端**

```bash
# 打开 Developer Command Prompt 终端
msbuild Ling.slnx /p:Configuration=Debug /p:Platform=x64 /m
```

### 4. 调试项目

- 按 `F5` 开始调试
- 或在左侧 "运行和调试" 面板 (Ctrl+Shift+D) 中选择配置并点击运行

### 可用的调试配置

| 配置名称                | 说明                          |
| ----------------------- | ----------------------------- |
| 调试 Demo (x64 Debug)   | 调试 Debug 版本的 Demo 程序   |
| 调试 Demo (x64 Release) | 调试 Release 版本的 Demo 程序 |
| 附加到进程              | 附加到正在运行的进程进行调试  |

## 可用的构建任务

| 任务名称          | 说明                    | 快捷键              |
| ----------------- | ----------------------- | ------------------- |
| build-x64-debug   | 构建 x64 Debug 版本     | Ctrl+Shift+B (默认) |
| build-x64-release | 构建 x64 Release 版本   |                     |
| clean-x64-debug   | 清理 x64 Debug 构建产物 |                     |
| rebuild-x64-debug | 重新构建 x64 Debug 版本 |                     |

**运行其他任务：**

- 按 `Ctrl+Shift+P`
- 输入 `Tasks: Run Task`
- 选择要执行的任务

## 调试快捷键

| 快捷键          | 功能                 |
| --------------- | -------------------- |
| `F5`            | 开始调试 / 继续      |
| `F9`            | 切换断点             |
| `F10`           | 单步跳过 (Step Over) |
| `F11`           | 单步进入 (Step Into) |
| `Shift+F11`     | 单步跳出 (Step Out)  |
| `Shift+F5`      | 停止调试             |
| `Ctrl+Shift+F5` | 重启调试             |

## 常见问题

### 1. msbuild 命令找不到

**解决方案：**

- 确保安装了 Visual Studio 2022 和 C++ 工作负载
- 使用 "Developer Command Prompt for VS 2022" 终端
  - 可以在 VSCode 终端中选择此配置文件

**手动设置环境（如果需要）：**

```bash
# 64位环境
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64
```

### 2. IntelliSense 不工作

**解决方案：**

- 确保选择了正确的 C++ 配置 (`x64-Debug`)
- 按 `Ctrl+Shift+P` → `C/C++: Reset IntelliSense Database`
- 重启 VSCode

### 3. 调试时找不到 Demo.exe

**解决方案：**

- 确保先成功构建了项目
- 检查 `x64/Debug/` 目录下是否存在 Demo.exe
- 运行构建任务后再尝试调试

## 项目结构

```
Ling/
├── .vscode/              # VSCode 配置目录
│   ├── tasks.json        # 构建任务配置
│   ├── launch.json       # 调试配置
│   ├── c_cpp_properties.json  # C++ 智能提示配置
│   └── settings.json     # 工作区设置
├── Ling/                 # Ling 框架源代码
├── Yoga/                 # Yoga 布局引擎源代码
├── Demo/                 # Demo 示例程序
└── Ling.slnx             # 解决方案文件
```

## 推荐的 VSCode 扩展

- **C/C++ Extension Pack** - Microsoft 官方 C++ 扩展包
- **GitLens** - Git 增强功能
- **Error Lens** - 行内显示错误信息
- **One Dark Pro** - 暗色主题
- **Material Icon Theme** - 文件图标

## 提示

1. **终端设置**：配置中默认使用 "Developer Command Prompt for VS 2022" 终端，
   这样可以直接运行 `msbuild` 命令而无需手动设置环境。

2. **代码格式化**：保存时自动格式化，使用 VC++ 格式化风格。

3. **构建输出**：构建产物位于 `x64/Debug/` 或 `x64/Release/` 目录。

4. **断点调试**：可以在源代码中直接点击行号左侧设置断点，调试时会自动命中。
