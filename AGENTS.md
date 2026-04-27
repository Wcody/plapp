# AGENTS.md

本文件面向在本仓库内工作的智能编码代理，帮助快速理解项目结构、构建方式与协作约束。

## 项目概况

- 项目类型：`uni-app x` 应用，当前以 UTS + `.uvue` 页面 / 组件为主。
- HBuilderX：基于 5.07 环境验证，CLI 路径为 `H:\000AppDevTool\HBuilderX\cli.exe`。
- 当前入口页：`pages.json` 只启用了 `pages/test/test`，其余页面配置暂时被注释。
- 不要手动编辑 `unpackage/` 下的编译产物；这是构建缓存目录，且已被 `.gitignore` 忽略。

## 仓库结构

### 根目录关键文件

- `main.uts`：应用入口，创建 `SSRApp`。
- `App.uvue`：全局生命周期、全局事件监听、公共样式。
- `pages.json`：页面注册与全局导航配置。
- `manifest.json`：App / Web / 小程序平台配置、权限、模块声明。
- `platformConfig.json`：平台相关配置。
- `uni.scss`：全局样式变量入口，当前内容较少。
- `index.html`：Web 端入口模板。

### 源码目录

- `pages/`：页面级 `.uvue` 文件。
  - `pages/test/test.uvue`：当前启用的 BLE 调试页。
  - `pages/index/`、`pages/m1/`、`pages/duo/`：历史或候选业务页面。
- `components/`：可复用 UI / 业务组件，目录名使用 kebab-case。
  - `m1-*`：M1 模块相关组件。
  - `duo-*`：Duo 模块相关组件。
  - `info-msg/`：通用消息提示组件。
- `utils/`：UTS 工具与协议逻辑。
  - `utils/bleProtocol.uts`：BLE 协议组包、解析、轮询。
  - `utils/common.uts`：公共方法，如音效、格式化。
- `data/`：静态业务数据与类型初始化。
  - `data/duo-config.uts`
  - `data/m1-programs.uts`

### 资源与产物

- `static/`：静态资源。
  - `static/logo.png`
  - `static/sounds/click.mp3`
- `unpackage/`：编译输出与缓存，不纳入源码维护。
- `.hbuilderx/`：本地 IDE 配置，不作为协作规则来源。

## 测试与规则文件现状

- 未发现自动化测试目录，如 `tests/`、`__tests__/`、`spec/`。
- 未发现 `package.json`，说明当前仓库不依赖 npm 脚本驱动构建 / 测试。
- 未发现以下额外代理规则文件，因此本文件即当前仓库的主要代理约束来源：
  - `.cursor/rules/`
  - `.cursorrules`
  - `.github/copilot-instructions.md`
  - `CLAUDE.md`
- 未发现既有 `AGENTS.md`，因此本次为首次建立仓库级代理说明。

## 常用命令

以下命令已在当前仓库路径下验证可用。

### 编译 / 运行

- Web 编译检查：

```powershell
"H:\000AppDevTool\HBuilderX\cli.exe" launch web --project "D:\Wcke\000SourceCode\HBuilderX\plapp" --compile true --continue-on-error false
```

- Android 编译检查：

```powershell
"H:\000AppDevTool\HBuilderX\cli.exe" launch app-android --project "D:\Wcke\000SourceCode\HBuilderX\plapp" --compile true --cleanCache false --continue-on-error false
```

- Android 真机运行指定页面：

```powershell
"H:\000AppDevTool\HBuilderX\cli.exe" launch app-android --project "D:\Wcke\000SourceCode\HBuilderX\plapp" --pagePath pages/test/test
```

- iOS 运行可参考 `.hbuilderx/launch.json` 中的 `uni-app:app-ios` 配置。

### 诊断 / lint

- 单文件语义诊断（推荐作为“lint”替代手段）：

```powershell
"H:\000AppDevTool\HBuilderX\cli.exe" lsp lint --project "D:\Wcke\000SourceCode\HBuilderX\plapp" --file "D:\Wcke\000SourceCode\HBuilderX\plapp\pages\test\test.uvue" --platform app-android,web
```

- 若要检查其他文件，只替换 `--file` 的绝对路径即可。

### 发布

- Web 发布：

```powershell
"H:\000AppDevTool\HBuilderX\cli.exe" publish web --project "D:\Wcke\000SourceCode\HBuilderX\plapp"
```

- Android 本地打包资源：

```powershell
"H:\000AppDevTool\HBuilderX\cli.exe" publish app-android --project "D:\Wcke\000SourceCode\HBuilderX\plapp" --type appResource
```

## 如何运行“单个测试”

本仓库目前没有 Jest / Vitest / uni-app 自动化测试脚本，因此“单个测试”按以下方式理解：

1. 单文件检查：对目标 `.uvue` / `.uts` 文件运行一次 `lsp lint`。
2. 单页面验证：运行指定页面，例如 `pages/test/test`，用于 BLE 相关人工验证。
3. 最小编译验证：执行一次 `launch web --compile true` 或 `launch app-android --compile true`。

如果后续引入自动化测试框架，请第一时间更新本文件，并补充“运行单测”的精确命令格式。

## 代码风格

### Imports

- 优先顺序：框架 / 外部依赖 -> `@/` 别名模块 -> 同目录相对路径。
- 保持导入最小化，避免未使用 import。
- 现有代码同时存在带分号与不带分号风格；修改文件时优先遵循所在文件原有风格，不要做无关格式化。

### UTS / 类型约束

- 新增对象结构、联合字面量时优先使用 `type`，不要默认写 `interface`。
- 所有变量先声明再使用；不要依赖 `var` 或隐式提升。
- 不使用 `undefined` 表示空值，统一使用 `null`。
- 条件表达式必须是布尔值，不依赖 JS 的真值 / 假值隐式转换。
- 对复杂响应式状态、回调入参、导出函数补充显式类型。

### 命名

- 目录名、组件目录名：kebab-case，如 `m1-slider-control`。
- 组件文件：通常与目录同名，如 `components/m1-item/m1-item.uvue`。
- 类型名、类名：PascalCase，如 `SendParam`、`BleProtocol`。
- 函数、变量：camelCase，如 `handleDisconnect`、`channelAParams`。
- 布尔值：优先 `is*`、`has*`、`show*` 前缀。
- 事件名：kebab-case，如 `update:modelValue`、`param-click`、`msg-close`。

### 样式与布局

- `uni-app x` 中凡是 `display: flex`，必须显式写 `flex-direction`。
- 需要滚动的页面 / 容器，在 App 侧必须用 `#ifdef APP` + `<scroll-view>` 包裹。
- 仅使用 class 选择器；不要写标签选择器、子选择器、通配选择器。
- 新增样式时优先延续现有页面的视觉语言，不做全局大改。

### 错误处理与日志

- 外设、协议、平台能力相关逻辑优先“可降级”：失败时给出 `uni.showToast` 或 `console.error`，不要静默吞错。
- 对运行时不可用的能力，参考 `pages/test/test.uvue` 中的兜底写法，提供 stub 与用户提示。
- 保留必要日志，尤其是 BLE 收发、连接状态、协议解析、轮询生命周期。

### 响应式与组件协作

- 优先使用 `script setup`。
- 组件之间通过 `props + emits` 通信，事件名保持语义化。
- 修改数组项时，仓库现有代码偏向“原地修改 + 保持引用”，处理列表时先遵循这一模式。
- 公共格式化、协议逻辑应沉淀到 `utils/`，不要在页面里重复实现。

## 协作规则

- 先看 `pages.json` 和目标页面，再决定改动范围；避免无意启用或删除其他页面。
- 不要提交 `unpackage/`、`.hbuilderx/`、本地缓存或设备相关产物。
- 若只改文档，可不触发整仓重构；若改 `.uvue` / `.uts`，至少做一次相关文件 `lsp lint` 或最小编译验证。
- 若改 BLE 协议或设备交互逻辑，优先验证 `pages/test/test.uvue` 的链路是否仍可人工调试。
- 若新增页面，记得同步更新 `pages.json`、必要资源引用以及本文件的结构说明。

## 提交前最小检查

1. 目标文件已完成 `lsp lint` 或对应平台编译检查。
2. 没有误改 `unpackage/`、缓存文件或 IDE 配置。
3. `display: flex` 均显式声明 `flex-direction`。
4. App 侧可滚动区域已按需使用 `<scroll-view>`。
5. 新增 UTS 类型、空值、条件判断符合本文件约束。
