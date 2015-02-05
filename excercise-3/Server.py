
import socket

tcpIp = "0.0.0.0"
tcpPort = 34933
udpIp = "0.0.0.0"
udpPort = 30000
buffer = 1024

s_tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s_tcp.bind((tcpIp, tcpPort))
s_tcp.listen(1)
s_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRM)
s_udp.bind((udpIp, udpPort))

conn_tcp, add_tcp = s_tcp.accept()
print 'conntection TCP address:', add_tcp
while 1:
    data_tcp = conn_tcp.recv(buffer)
    data_udp = s_udp.recvfrom(buffer)
    #if not data: break
    if data_tcp:
        print "received TCP message: ", data_tcp
        conn_tcp.send(data_tcp)  # echo
    if data_udp:
        print "received UDP message: ", data_udp
        