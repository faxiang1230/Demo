#########################################################################
# File Name: run.sh
# Author: wangjx
# mail: wangjianxing5210@163.com
# Created Time: 2018年08月14日 星期二 11时21分29秒
#########################################################################
#!/bin/bash
function run_ldpreload(){
	LD_PRELOAD=./hook.so ./test 123
}
run_ldpreload
