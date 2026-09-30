# 验证记录

执行日期：2026-09-29（用户时区 America/New_York）。

## 已实测

- 使用用户提供的密钥访问 DeepSeek `/models`，HTTP 200，返回 `deepseek-flash` 和 `deepseek-v4-pro`。
- 密钥已保存到 Windows 凭据管理器并成功读回；代码及配置没有明文密钥。
- Codex CLI 0.156.1 使用 DeepSeek Responses，真实保存本机会话。
- DeepSeek 使用 `apply_patch` 创建 `smoke_value.txt`，随后运行 Python 断言精确内容 `b'BRIDGE_SMOKE_OK\n'`；工具退出 0。根代理独立检查文件也通过，实际为 16 字节。模型总结中的“17 字节”计数不准确，以文件读回为准。
- 将 DeepSeek 可读历史导出后交给官方 `gpt-6-sol` 新会话，GPT 正确指出此前文件名、文本和失败原因；官方模型调用退出 0。
- Codex 桌面 read_thread 能读取 DeepSeek 会话。
- 任务 baseline 不匹配时，启动器在调用模型前拒绝执行。
- Python 源码语法、PowerShell 启动器语法、JSON 模板及模型目录结构检查通过。
- `auth.json` 和 `relay.config.toml` SHA256 前后相同。`config.toml` 由 Codex 自动新增临时 `work/bridge-smoke` 目录的 trusted 项；从内存中扣除该项后 SHA256 与测试前一致，核实没有其他变更，默认模型与认证配置保持原值。

## 已发现并处理

隔离测试使用 `--ignore-user-config` 时，丢失已有 `[windows].sandbox="elevated"`，导致 Windows CLI 将 workspace-write 降为 read-only。隔离测试现显式保留已有后端配置，写入与 shell 已实际成功。正常入口继续继承用户配置。

DS 原生历史直接 fork 给 GPT 时返回 HTTP 400，包含 `input[5].content array_above_max_length`。跨 provider 的推理历史未自动正规化，因此本工具推荐 `--handoff` 导出可读记录后启动新会话。这条替代路径已实际验证。直接跨 provider 的 resume/fork 尚未验证可用。

首次真实仓库发布时，启动器固定 workspace-write，导致 `.git` 写入被拒绝，Git HTTPS 凭据也不可用。正常入口现改为继承本机既有 sandbox_mode / approval_policy，并显式传给 CLI；不会通过改全局配置扩大权限。真实 GitHub 发布结果需以之后核对的提交为准。

本机默认 `gpt-6.1-sol` 在此次 CLI 测试中被官方接口拒绝；实时账号模型列表中 `gpt-6-sol` 已实测调用成功。本工具未改变默认配置。

## 尚未验证

- 新任务在实际嵌入式项目的构建、烧录或硬件行为。本轮工具验证没有更改固件。
- 网页版 GPT 此刻读取私有仓库的权限、索引刷新速度与人工任务单往返。
- DeepSeek 在真实项目修改后的 GitHub 推送，需要对选定仓库运行真实任务并核对远程 SHA。
- 直接在桌面模型菜单切换 DeepSeek/GPT。同一 Codex Home 不代表供应商全部历史格式自动兼容。

## 实测会话

- DeepSeek 编辑与测试成功：`01a0f06d-8e4e-7a63-a370-878d7baf55fa`
- 官方 GPT 读取先前 DeepSeek 记录成功：`01a0f06b-e112-7640-8321-41ae3b16d5d4`

完整记录保存在本机 `work/bridge-runs/`，不应上传仓库。退出码 0 表示模型一轮执行结束，仍需检查任务要求的具体测试和最终 Git 提交。
