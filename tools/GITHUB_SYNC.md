# GitHub 同步操作记录

## 2026-09-30 顶部参数等间距

本机号继续右对齐x372，状态和分站按实际文字宽度向右收拢，两段间距统一8px。
最大uint16地址仍可完整显示；主页Logo、数据网格和第二页保持原样。
WCH GCC12构建和HEX生成成功，无警告或错误，FLASH342232字节、RAM66040字节。
前后各67个离线用例通过，21265项文字布局检查无失败；43个保留页面及5个第二页
用例逐像素一致。预览和日志位于忽略目录`tools/ui_preview/home_spacing_output/`。
本次仅同步UI源码、相关测试和说明文档，烧录及实屏效果尚未验证。

## 2026-09-30 主页两页统一放大

本次改动限定于主页绘图、Logo资源及生成器、相关离线验证和说明文档。
第一页面状态置左、分站居中、本机号置右；第二页统计置顶，两页Logo和数据行大小一致。
当前实际远程为`git@github.com:vasicambree-png/ttk.git`，分支`main`；fetch后本地与远程一致。
WCH GCC12构建及HEX生成成功：FLASH342200字节，RAM66040字节，无固件警告或错误。
前后各67个绘图用例、21246项文字及布局检查通过，43个保留页面逐像素一致。
`.mrs/`快照与预览/构建产物保持本地；烧录和实屏观察尚未验证。

目的：保留本项目的同步记录。跨项目流程已安装为全局 `github-sync` skill，并由全局 `AGENTS.md` 引用；通用经验优先读取该 skill，项目配置仍需现场确认。

## 项目和已验证的现象

- 项目：`D:\蓝牙模块优化`。
- 当前远程：`https://github.com/vasicambree-png/ttk.git`，分支 `main`。每次操作仍需读取实际配置，不硬编码旧提交 SHA。
- 2026-09-29 同步时，命令行 `git fetch origin` 返回 `Repository not found`。
- `git credential-manager github list` 当时只列出 `1tjk1`；GitHub 连接的用户为 `vasicambree-png`，连接确认目标仓库存在、为私有仓库且具有 push 权限。证据指向命令行登录账号不匹配。
- 同日已用 GCM 2.7.3 的 `github login --username vasicambree-png --browser --force` 登录成功，并设置 GitHub.com 专属全局默认用户名。原生 Git fetch 和 ls-remote 已成功；保留旧账号凭据，不影响其他 Git 服务。device 登录曾不返回授权码，当前优先 browser 流程。
- 已通过 GitHub 连接的 Git Data 工具上传，并核对远程代码树与本地 `git write-tree` 的 SHA 一致。原提交历史保留，本地分支也对应同一个提交。
- `Repository not found` 不足以证明仓库不存在；先确认账号和访问权限，不创建替代仓库、不修改 origin。

## 每次同步的最短检查

在项目目录执行：

```powershell
git status --short --branch
git remote -v
git diff --stat
git diff --cached --stat
git ls-files --others --exclude-standard
```

1. 按本次任务检查改动并完成相应验证。单纯上传既有代码时不声称重新编译通过。
2. 明确选择源码、依赖资源、生成器和必要文档。现有 `tools/ui_preview/.gitignore` 已忽略预览输出和宿主编译产物；`CH584_V1_0_1/.mrs/` 快照不上传。
3. 使用 `git add -- <明确的路径>`，避免无差别 `git add .`。
4. 执行 `git diff --cached --check` 并复核暂存范围。同步期间如果文件变化，重新核对快照。

## 路线 A：命令行认证可用时

```powershell
git fetch origin
git rev-list --left-right --count HEAD...origin/main
git commit -m "描述本次实际改动"
git push origin main
git ls-remote origin refs/heads/main
git rev-parse HEAD
git status --short --branch
```

每条命令成功后才执行依赖它的下一条。提交前确认远程没有尚未整合的新增提交；推送必须为正常快进。远程分支 SHA 与本地 HEAD 一致才算成功。

当前优先路线 A。以后若认证再次异常，不要反复执行失败的 fetch/push；核对账号与访问权限，必要时使用路线 B。需要重新登录时使用 Git Credential Manager 正式 browser 流程；不要读取、打印令牌或自行把凭据写入文件。

## 路线 B：通过已登录的 GitHub 连接

通过 `ALL_TOOLS` 查找以下工具并阅读完整描述中的参数声明：`github_get_profile`、`github_get_repo`、`github_fetch`、`github_create_blob`、`github_create_tree`、`github_create_commit`、`github_update_ref`。不要猜参数名。

1. 用连接确认身份、目标仓库及写入权限。用 `github_fetch` 读取 `/repos/{owner}/{repo}/git/ref/heads/{branch}` 的当前 SHA。
2. 对比远程 SHA 与本地 HEAD。不同则先检查新增提交，不能直接覆盖远程。
3. 获取本地父树：`git rev-parse 'HEAD^{tree}'`；获取暂存目标树：`git write-tree`。PowerShell 中带花括号的 Git 参数必须加引号。
4. 从 **Git 暂存区 blob** 读取内容，不直接读取可能继续变化的工作区文件。可用 `git ls-files --stage -z` 取得路径、mode、SHA，再用 `git cat-file blob <SHA>` 读取原始字节。用 base64 传二进制，保留编码及换行；按 blob SHA 去重。
5. 用 `github_create_blob` 上传新增内容，每个返回 SHA 必须等于本地 blob SHA。工具返回结果只输出 SHA 和进度，不打印完整源码、base64 或整个大型提交 diff。
6. 用 `github_create_tree` 基于父树增加任务文件，提供 path/mode/type/blob SHA；返回树 SHA 必须与 `git write-tree` 一致。删除文件只有任务明确需要时才列入。
7. 用 `github_create_commit` 创建以当前远程 SHA 为父提交、以上树为内容的提交。通过 `github_fetch` 读取 `/git/commits/{new_sha}` 获取 Git 元数据，避免 `github_fetch_commit` 返回大型 diff。
8. 本地先导入该提交对象并验证 SHA，再用 `github_update_ref` 更新远程分支，`force=false`。重新读取远程 ref 核对 SHA。远程若已变化，停止更新并检查差异。
9. 用带旧 SHA 的 `git update-ref` 将本地分支和对应 origin 跟踪引用推进到已验证的同一个提交。只推进引用，不重置、覆盖工作区；再次核对暂存树、工作区、远程与本地提交。

### 本地提交对象的重建要点

通过 API 创建的提交与另行执行 `git commit` 的提交通常不具有相同 SHA，不能只因为文件相同就认为本地历史同步了。

未签名的 Git 提交原始格式为：`tree` 行、所有 `parent` 行、`author` 行、`committer` 行、空行、原始 message 字节。身份行含 name、email、Unix 秒和 `±HHMM` 时区。

API 将日期归一化为 UTC，会丢失原始时区偏移。上次实际提交的 author/committer 使用 `-0400`，message 没有末尾换行；不能固定假设 `+0000` 或自动补换行。可先尝试当前客户端时区，再尝试常见偏移和末尾换行组合；仅当 `SHA1("commit " + 字节长度 + NUL + 内容)` 与远程 SHA **完全一致**时，才通过 `git hash-object -t commit -w --stdin` 导入。未匹配时不要移动本地引用，报告未完成的本地对齐。

大文件内容在 `exec_command` 输出中可能被截断。需要在 `functions.exec` 内分块读取、检查每块长度后拼接并传工具，不把数据输出给模型。上次使用每块 200000 个 ASCII 字符；不要照搬整个文件列表或上传已在父树中的不变文件。

## 完成报告

返回实际提交链接、分支、同步一致性结果；说明未上传的本地缓存。源码检查、构建、测试、烧录、实屏观察分别表述，未进行的项目标为“尚未验证”。

网页版分析应指定最新分支和提交 SHA；推送成功不能保证另一个聊天已经刷新或重新索引仓库。

## 2026-09-30 统一字体和等待数据页面

本轮同步目标经现场检查为`origin/main`，地址为`git@github.com:vasicambree-png/ttk.git`。
原生`git fetch origin`成功，同步前HEAD与远程分支一致。
提交范围限于UI绘图、菜单字模及生成器、相关预览检查和说明；
`.mrs/` IDE快照保留且不暂存，`unified_text_output/`预览及日志留在本地忽略目录。
验证为WCH固件重新编译/链接/HEX生成成功（无编译警告和错误），
279个离线绘图用例和151项文字布局检查通过。烧录和实屏效果尚未验证。
