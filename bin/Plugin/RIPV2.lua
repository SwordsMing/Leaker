--lua plugin for ripv2 protocal
--create by zc 2014/04/06

registerProtocal("RIPV2",17,520,"rip_analyse")
function rip_analyse(info)
	setProtocal(info,"RIPV2")
	setInfo(info,"RIPV2 ptotocal")
	
	data = getBuffer(info)
	
end