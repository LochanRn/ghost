import cv2
import socket
import time
import math
#ADDRESS='192.168.43.237'
ADDRESS='127.0.0.1'
#ADDRESS='192.168.1.231'
port=12000

s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
print("connected")
#cap=cv2.VideoCapture(0)
cap1=cv2.VideoCapture(1)
width=int(cap1.get(3))
height=int(cap1.get(4))
print(width)
print(" ")
print(height)
j=1
time.sleep(1)
def send(capt):
	ret,frame =capt.read() 
	#frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	#frame=cv2.resize(frame,(352,288))
	data = cv2.imencode('.jpg', frame)[1].tostring()
	if len(data)>=65000:
		frame=cv2.resize(frame,(320,240))
		data=cv2.imencode('.jpg',frame)[1].tostring()
	frn="fr_"+str(j)+".jpg"
	print(len(data))
	s.sendto(data,(ADDRESS,port))
	s.sendto(b"masala",(ADDRESS,port))

while (1):

	send(cap1)

cap.release()
cv2.destroyAllWindows()
s.close()
