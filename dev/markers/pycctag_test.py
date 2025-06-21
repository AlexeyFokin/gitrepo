import cv2
import pycctag

import numpy as np


#img = cv2.imread('1.png', cv2.IMREAD_COLOR)


dictionary = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_50)
parameters =  cv2.aruco.DetectorParameters()
detector = cv2.aruco.ArucoDetector(dictionary, parameters)

vid = cv2.VideoCapture(0)

# Check if camera opened successfully
if (vid.isOpened()== False): 
  print("Error opening video stream or file")

while(vid.isOpened()):

    ret, frame = vid.read() 
 

    gray_img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # (corners, ids, rejected) = detector.detectMarkers(frame)

    # # verify *at least* one ArUco marker was detected
    # if len(corners) > 0:
    #     # flatten the ArUco IDs list
    #     ids = ids.flatten()
    #     # loop over the detected ArUCo corners
    #     for (markerCorner, markerID) in zip(corners, ids):
    #         # extract the marker corners (which are always returned in
    #         # top-left, top-right, bottom-right, and bottom-left order)
    #         corners = markerCorner.reshape((4, 2))
    #         (topLeft, topRight, bottomRight, bottomLeft) = corners
    #         # convert each of the (x, y)-coordinate pairs to integers
    #         topRight = (int(topRight[0]), int(topRight[1]))
    #         bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
    #         bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
    #         topLeft = (int(topLeft[0]), int(topLeft[1]))
    #         # draw the bounding box of the ArUCo detection
    #         cv2.line(frame, topLeft, topRight, (0, 255, 0), 2)
    #         cv2.line(frame, topRight, bottomRight, (0, 255, 0), 2)
    #         cv2.line(frame, bottomRight, bottomLeft, (0, 255, 0), 2)
    #         cv2.line(frame, bottomLeft, topLeft, (0, 255, 0), 2)
    #         # compute and draw the center (x, y)-coordinates of the ArUco
    #         # marker
    #         cX = int((topLeft[0] + bottomRight[0]) / 2.0)
    #         cY = int((topLeft[1] + bottomRight[1]) / 2.0)
    #         cv2.circle(frame, (cX, cY), 4, (0, 0, 255), -1)
    #         # draw the ArUco marker ID on the image
    #         cv2.putText(frame, str(markerID),
    #             (topLeft[0], topLeft[1] - 15), cv2.FONT_HERSHEY_SIMPLEX,
    #             0.5, (0, 255, 0), 2)
    #         #print("[INFO] ArUco marker ID: {}".format(markerID))

    markervector = pycctag.detect_from_img(gray_img)
    markerlist = list(markervector)

    for i in markerlist:
        if i.status == 1:
            cv2.circle(frame, (int(i.x), int(i.y)), 10, (255,0,0), 5)
        #print(f"id={i.id} status={i.status} x={i.x} y={i.y}")

    cv2.imshow('Frame',frame)

    if cv2.waitKey(1) & 0xFF == ord('q'): 
        break

vid.release() 
cv2.destroyAllWindows() 


# p0 = my_lib.Point(13, 17)
# my_lib.moveTo(p0)

# cv_img = my_lib.getImage()
# cv_img2 = my_lib.Mat.from_array(gray_img)
# print(type(cv_img2))
# #markervector = pycctag.detect_from_img(cv_img2)

# np_arr = cv2.blur(np.asarray(cv_img), (3, 3))

# cv_img2 = my_lib.Mat.from_array(np_arr)

# i = my_lib.Mat().from_array(gray_img)

# #markervector = pycctag.detect_from_img(i)


# a = np.array([[[0.606, 0.091, 0.098], [0.751, 0.639, 0.956]],
#                 [[0.721, 0.163, 0.543], [0.454, 0.281, 0.909]],
#                 [[0.806, 0.348, 0.279], [0.462, 0.656, 0.388]]],
#                 dtype=np.float64)
# a = mat.Mat.from_array(a)
# print(type(a))
# #markervector = pycctag.detect_from_img(a)
