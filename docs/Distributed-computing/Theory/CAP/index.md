# CAP



## CAP的直观理解

"鱼和熊掌不可兼得":

1) 之前在开发一个application的时候，涉及到在performance和reliability中进行抉择: 

如果实时(同步)地将记录写入到文件，则performance较差，reliability较好，这就是牺牲performance来获取reliability。

如果非实时(异步)地将记录写入到文件，则performance较好，reliability较差，这就是牺牲reliability来获取performance。



