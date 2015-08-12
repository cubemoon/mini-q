#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TEMPLATE_ROOT=`dirname "$DIR"`/template
php "$DIR/lib/create_project.php" -t "$TEMPLATE_ROOT/PROJECT_TEMPLATE_01" $*

if [[ $? != 0 ]]; then
	echo "create project error."
    exit $?
fi
# 默认保留工程的轻量性, 把 qmm 的库目录链接到用户工程路径, 方便性有:
# 1. 快速切换 qmm 版本
# 2. 拷贝 qmm/lib 过来就可以保留项目的完整性
#
## link to QMM_ROOT/lib
# ln -sv ${QMM_ROOT}/lib HOW_TO_GET_-o_PATH
