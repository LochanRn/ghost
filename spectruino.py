
import serial
import socket
sa = serial.Serial('/dev/ttyUSB0',115200)
UDP_IP_ADDRESS="192.168.1.125"
UDP_PORT_NO=23911
clientSock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
while True:
	message = sa.read()
#	print(message)
	clientSock.sendto((message),(UDP_IP_ADDRESS,UDP_PORT_NO))
