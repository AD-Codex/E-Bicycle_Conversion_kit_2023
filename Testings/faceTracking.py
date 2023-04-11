import cv2 as cv
import numpy as np
import face_recognition

cap = cv.VideoCapture(0)

while True :
    _, frame = cap.read()

    frame = cv.cvtColor( frame, cv.COLOR_BGR2RGB)
    faceLoc = face_recognition.face_locations(frame)
    print( faceLoc[0] )
    if len(faceLoc) >1 :
        print("detect")
        y1, x2, y2, x1 = faceLoc 
        cv.rectangle( frame, (x1,y1), (x2,y2), (0, 255, 255), 2)

    cv.imshow( "frame", frame)

    key = cv.waitKey(5)
    if key == 27:
        break


cap.release()
cv.destroyAllWindows()
