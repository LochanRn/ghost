import cv2
from datetime import datetime

fname=str(datetime.now())+".avi"
fourcc=cv2.VideoWriter_fourcc(*'XVID')
out=cv2.VideoWriter(fname,fourcc,10.0,(640,480))
cap=cv2.VideoCapture(1)

while (1):
	ret,frame =cap.read() 
	out.write(frame)
cap.release()