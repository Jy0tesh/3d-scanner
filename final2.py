import cv2
import numpy as np
img= cv2.imread('bottle.jpg')
img=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
img = cv2.GaussianBlur(img,(5,5),0)
ret,th3 = cv2.threshold(img,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
kernel = np.ones((5,5),np.uint8)
img = cv2.erode(th3,kernel,iterations = 3)
final=np.zeros((2204,3920,1), np.uint8)
m=0
n=0
for i in range (2201):
	c=0
	for j in range(3917):
		if img[i,j]==img[i,j+1]:
			continue
		if c==0:
			m=j
			c=c+1
			continue

		if c==1:
			n=j+1
			print i,(m+n)//2
			final[i,(m+n)//2]=255


cv2.namedWindow('initial',cv2.WINDOW_NORMAL)
cv2.imshow('initial', img)
cv2.namedWindow('final',cv2.WINDOW_NORMAL)
cv2.imshow('final', final)
cv2.waitKey()
cv2.destroyALLWindows()


		     	

