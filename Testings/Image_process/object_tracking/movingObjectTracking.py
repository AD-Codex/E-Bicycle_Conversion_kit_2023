import cv2 as cv

cap = cv.VideoCapture("video1.mp4")

# Object detect from stable camera
object_detector = cv.createBackgroundSubtractorMOG2()


while True :
    ret, frame = cap.read()
    h, w, _ = frame.shape
    # print(h,w)

    # Extract region
    roi = frame[ 340:720, 160:960]

    # object detect
    mask = object_detector.apply(roi)
    _, mask = cv.threshold( mask, 254, 255, cv.THRESH_BINARY)
    contours, _ = cv.findContours( mask, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        # calculate area
        area = cv.contourArea(cnt)
        if area > 500:
            # cv.drawContours( roi, [cnt], -1, (0, 255, 0), 2)
            x, y, w, h = cv.boundingRect(cnt)
            cv.rectangle( roi, (x,y), (x+w, y+h), (0, 255,0), 3)


    cv.imshow( "frame", frame)
    # cv.imshow( "mask", mask)

    key = cv.waitKey(5)
    if key == 27:
        break


cap.release()
cv.destroyAllWindows()
