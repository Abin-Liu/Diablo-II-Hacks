idbot 008 by niren7   
modified by ustc_tweeg for pbot/pindlebot

1 当保留列中只胜两空列时自动鉴定并存箱子
2 对jhj pbot，在hackit的autoload=后面删掉mover，加上idbot
  对sousou的pindlebot，不用删，直接加上idbot，只要把mover移动周期
  设成非常大的数让它不可实现就行了，删掉行不行我没试过
3 4f和bn的cain位置不同，都可以使用
4 ！！！ 注意 ！！！对于sousou的pindlebot，因为默认是鉴定过的物品
  也要拣，这样drop的物品又会被拣，所以要改相应的pickit设置


idbot.ini
--------------------------------------------------------------
stash_item=1   表示要装箱子功能
id_col_left=0     要鉴定的最左列,注意是鉴定，不是保留
id_col_right=6   要坚定的最右列
pause_cmd="runpbotsor pause"  用来暂停的命令 
callback_cmd="runpbotsor pause"  鉴定完继续的命令
trigger_free_col=4          当身上空余的列数小于或等于这个数，就进行鉴定
time_out           鉴定超时退出，单位是秒
--------------------------------------------------------------
对于pindlebot就是
pause_cmd="pindlebot pause" 
callback_cmd="pindlebot pause" 