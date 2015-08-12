@echo off
set DIR=%~dp0
set TEMPLATE_ROOT=%DIR%..\template
%DIR%win32\php.exe "%DIR%lib\create_project.php" -t "%TEMPLATE_ROOT%\PROJECT_TEMPLATE_01" %*

"默认保留工程的轻量性, 把 qmm 的库目录链接到用户工程路径, 方便性有:
" 1. 快速切换 qmm 版本
" 2. 拷贝 qmm/lib 过来就可以保留项目的完整性
"
"" link to QMM_ROOT/lib
" 使用命令行创建一个链接
