import cv2 as cv
import numpy as np
import face_recognition
import time

face_encoding = []
person = [ "avi", "lika", "kettu", "kavidu"]

def trainingModel():
    global face_encoding
    global person

    img = face_recognition.load_image_file("avi.jpg")
    img = cv.cvtColor( img, cv.COLOR_BGR2RGB)
    img_encod = face_recognition.face_encodings(img)[0]
    face_encoding.append( img_encod)

    img = face_recognition.load_image_file("lika.jpg")
    img = cv.cvtColor( img, cv.COLOR_BGR2RGB)
    img_encod = face_recognition.face_encodings(img)[0]
    face_encoding.append( img_encod)

    img = face_recognition.load_image_file("kettu.jpg")
    img = cv.cvtColor( img, cv.COLOR_BGR2RGB)
    img_encod = face_recognition.face_encodings(img)[0]
    face_encoding.append( img_encod)

    img = face_recognition.load_image_file("kavindu.jpg")
    img = cv.cvtColor( img, cv.COLOR_BGR2RGB)
    img_encod = face_recognition.face_encodings(img)[0]
    face_encoding.append( img_encod)




def CameraCapture():
    global face_encoding
    global person

    cap = cv.VideoCapture(0)

    while True :
        _, frame0 = cap.read()

        frame = cv.cvtColor( frame0, cv.COLOR_BGR2RGB)

        faceLoc = face_recognition.face_locations(frame)
        face_encode = face_recognition.face_encodings(frame)
        # print( len(faceLoc), faceLoc, face_encode)

        if len(faceLoc) > 0 :
            print("detect")
            y1, x2, y2, x1 = faceLoc[0]
            cv.rectangle( frame0, (x1,y1), (x2,y2), (0, 255, 255), 2)

            Conditions = face_recognition.compare_faces( face_encoding, face_encode[0])
            R_distance =  face_recognition.face_distance( face_encoding, face_encode[0])

            print( Conditions, R_distance)

            for j in range(len(face_encoding)):
                if Conditions[j] == True :
                    if R_distance[j] < 0.5:
                        cv.putText( frame0, person[j], (x1,y1-10), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)


        cv.imshow( "frame", frame0)

        key = cv.waitKey(50)
        if key == 27:
            break


    cap.release()
    cv.destroyAllWindows()




if __name__ == '__main__' : 

    print("training model.......")
    trainingModel()
    print("Model trained........")

    CameraCapture()

