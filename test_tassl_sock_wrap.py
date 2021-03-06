from py_tassl_sock_wrap import PyTasslSockWrap
from channelpack import ChannelPack
wrap = PyTasslSockWrap();
wrap.set_host_port("127.0.0.1",20800)
wrap.set_ca_files("sdk/gmca.crt","sdk/gmsdk.crt","sdk/gmsdk.key","sdk/gmensdk.crt","sdk/gmensdk.key")
wrap.init()
wrap.try_connect()



import json;
seq = ChannelPack.make_seq32()
data = "{\"jsonrpc\":\"2.0\",\"method\":\"getClientVersion\",\"params\":[],\"id\":1}";
json.loads(data)
pack = ChannelPack(ChannelPack.TYPE_RPC, seq, 0, data)
r = wrap.write(pack.pack())
print("wrap write return ",r)
resultbuffer  = wrap.read()
print("read from lib : ",len(resultbuffer),resultbuffer)
(code, decodelen, responsePack)  = ChannelPack.unpack(resultbuffer)
print("\n->unpack channelpack from server :")
print(responsePack.detail())

import json
j = json.loads(responsePack.data)
print(json.dumps(j,indent=4))
wrap.finish()