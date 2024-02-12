import cv2
import urllib.request
import numpy as np

def nothing(x):
    pass

url = 'http://192.168.137.252/cam-mid.jpg'  # Change IP address here
cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)

l_h, l_s, l_v = 0, 0, 0  # Lower HSV threshold for black
u_h, u_s, u_v = 180, 255, 52  # Upper HSV threshold for black

while True:
    img_resp = urllib.request.urlopen(url)
    imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
    frame = cv2.imdecode(imgnp, -1)

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    l_b = np.array([l_h, l_s, l_v])
    u_b = np.array([u_h, u_s, u_v])

    mask = cv2.inRange(hsv, l_b, u_b)

    cnts, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for c in cnts:
        area = cv2.contourArea(c)
        if area > 2000:
            cv2.drawContours(frame, [c], -1, (255, 0, 0), 3)
            M = cv2.moments(c)
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])

            cv2.circle(frame, (cx, cy), 7, (255, 255, 255), -1)
            cv2.putText(frame, "black", (cx - 20, cy - 20), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
            
            # Calculate percentage of black in the contour
            contour_mask = np.zeros_like(mask)
            cv2.drawContours(contour_mask, [c], -1, 255, -1)
            black_pixels = cv2.countNonZero(mask & contour_mask)
            total_pixels = cv2.contourArea(c)
            black_percentage = (black_pixels / total_pixels) * 100
            cv2.putText(frame, f"Black: {black_percentage:.2f}%", (cx - 20, cy + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)
            
            # Display warning if black percentage is above 80
            if black_percentage > 80:
                cv2.putText(frame, "Possibly Diseased, Please Check", (cx - 50, cy + 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1)

    res = cv2.bitwise_and(frame, frame, mask=mask)

    cv2.imshow("live transmission", frame)
    cv2.imshow("mask", mask)
    cv2.imshow("res", res)
    key = cv2.waitKey(5)
    if key == ord('q'):
        break

cv2.destroyAllWindows()