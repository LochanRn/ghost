import socket
import base64
import cv2
import numpy
import time
import sys

#serverName = '192.168.1.78'
serverName='127.0.0.1'
serverPort=12000

s= socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

s.bind((serverName, serverPort))
def rcv():
	data =b''
	while(1):
		try:
			r,serverName=s.recvfrom(75000)
			# if(len(r)==0):
			# 	break
			print("r = ",len(r))
			a=r.find(b"masala")
			if(a!=-1):
				
				data+=r[:a]
				#print(len(data)," ------------------------------------------------------------------------------------------------------------------------------------------------------- --")
				if(len(data)<=0):
			#		print("gadbad")
					data=b''
					return b'1'
					break
				return data
				break
			else:
				data+=r
			
		except Exception as e:
			print("lassan")
			print(e)
			continue
j=0
while 1:
	data=b''
	while(data is b''):
		data=rcv()
	nparr = numpy.fromstring(data, numpy.uint8)
	frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
	if type(frame) is type(None):
		pass
	else:
		try:
			frame=cv2.resize(frame,(1280,720))
			cv2.imshow("hello there",frame)
			if cv2.waitKey(10) == ord('q'):
				s.close()
				cv2.destroyAllWindows()
				sys.exit()
		except:
			s.close()
			cv2.destroyAllWindows()
			exit(0)
	j+=1
s.close()
