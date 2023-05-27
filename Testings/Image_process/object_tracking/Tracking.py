
import cv2 as cv
import numpy as np
import imutils
from collections import deque

cap = cv.VideoCapture(0)

while(1):
    _, frame = cap.read()

    # blur image
    # frame2 = cv.GaussianBlur(frame, (5, 5), 0)

    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)

    lower_blue = np.array([ 110, 50, 50])
    upper_blue = np.array([ 130, 255, 150])
    

    mask = cv.inRange( hsv, lower_blue, upper_blue)
    mask = cv.erode( mask, None, iterations=10)
    mask = cv.dilate( mask, None, iterations=10)

    # mask changes
    cnts = cv.findContours(mask.copy(), cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    center = None

    # print(cnts)

    if len(cnts) > 0:
        c = max(cnts, key=cv.contourArea)

        # circle bounds
        ((x, y), radius) = cv.minEnclosingCircle(c)

        # # rectangle bounds
        # x,y,w,h = cv.boundingRect(c)
        # cv.rectangle( frame,(x,y),(x+w,y+h),(0,255,0),2)

        M = cv.moments(c)

        # centroid of the shape from the moment
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
        print(center)

        # only proceed if the radius meets a minimum size
        if radius > 10:
            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            cv.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 2)
            cv.circle(frame, center, 5, (0, 0, 255), -1)


    cv.imshow( 'frame', frame)
    cv.imshow( 'mask', mask)

    k = cv.waitKey(5) & 0xFF
    if k == 27:
        break

cv.destroyAllWindows()

