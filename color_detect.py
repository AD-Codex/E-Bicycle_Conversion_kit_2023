import cv2 as cv
import numpy as np

# video capture
cap = cv.VideoCapture(0)


while(1):
	# video read, take each frame
	_, frame = cap.read()

	# color masking for red
	hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

	# filter using color map
	lower_red = np.array([0, 100, 20])
	upper_red = np.array([179, 255, 255])
	
	# lower_blue = np.array([110, 50, 50])
	# upper_blue = np.array([130, 255, 255])

	mask = cv.inRange(hsv, lower_red, upper_red)
	mask = cv.erode(mask, None, iterations=10)
	mask = cv.dilate(mask, None, iterations=10)

	res = cv.bitwise_and( frame, frame, mask= mask)

	cv.imshow('frame', frame)
	cv.imshow('mask', mask)
	cv.imshow('res', res)

	k = cv.waitKey(5) & 0xFF
	if k == 27:
		break


cv.destroyAllWindows()

print("End")

