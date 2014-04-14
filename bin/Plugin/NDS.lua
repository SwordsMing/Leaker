--lua plugin for DNS protocal
--create by zc 2014/04/10

registerProtocal("DNS",17,53,"dns_analyse")
function dns_analyse(info)
	setProtocal(info,"DNS")
	setInfo(info,"DNS ptotocal")
	
	data = getBuffer(info)
	
end