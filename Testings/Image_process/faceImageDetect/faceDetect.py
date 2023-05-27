import cv2 as cv
import face_recognition

face_encoding = []
person = [ "avi", "lika", "kettu", "kavidu"]

def trainingModel():
    global face_encoding
    global person

    # train model using image
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


if __name__ == '__main__' :    
    print("training model.......")
    trainingModel()
    print("Model trained........")


    img_0 = cv.imread("2.jpg")
    x, y, _ = img_0.shape

    # resolution filter
    img_0 = cv.resize( img_0, ( int(y/4), int(x/4)))

    img_1 = cv.cvtColor( img_0, cv.COLOR_BGR2RGB)

    # detect faces in image
    face_L = face_recognition.face_locations( img_1)
    img_1_encode = face_recognition.face_encodings( img_1)

    for i in range(len(face_L)):
        y1, x2, y2, x1  = face_L[i]
        cv.rectangle( img_0, (x1,y1), (x2,y2), (0, 255, 255), 2)
        
        Conditions = face_recognition.compare_faces( face_encoding, img_1_encode[i])
        R_distance =  face_recognition.face_distance( face_encoding, img_1_encode[i])

        # select best options
        for j in range(len(face_encoding)):
            if Conditions[j] == True :
                if R_distance[j] < 0.5:
                    cv.putText( img_0, person[j], (x1,y1-10), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
                    # print(person[j])


    cv.imshow("img_1", img_0)

    cv.waitKey(0)




