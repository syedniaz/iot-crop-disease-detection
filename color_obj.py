import cv2
import cvlib as cv
import urllib.request
import numpy as np
from cvlib.object_detection import draw_bbox
import concurrent.futures

url = 'http://192.168.10.162/cam-hi.jpg'
im = None

def run_detection():
    cv2.namedWindow("detection", cv2.WINDOW_AUTOSIZE)
    while True:
        img_resp = urllib.request.urlopen(url)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        im = cv2.imdecode(imgnp, -1)

        bbox, label, conf = cv.detect_common_objects(im)
        im = draw_bbox(im, bbox, label, conf)

        for i in range(len(bbox)):
            x, y, w, h = bbox[i]
            object_roi = im[y:y+h, x:x+w]
            hsv = cv2.cvtColor(object_roi, cv2.COLOR_BGR2HSV)

            l_h, l_s, l_v = 0, 0, 0  # Lower HSV threshold for black
            u_h, u_s, u_v = 179, 255, 40  # Upper HSV threshold for black
            mask = cv2.inRange(hsv, (l_h, l_s, l_v), (u_h, u_s, u_v))
            black_pixels = cv2.countNonZero(mask)
            total_pixels = object_roi.shape[0] * object_roi.shape[1]
            black_percentage = (black_pixels / total_pixels) * 100

            label_text = f"{label[i]} ({black_percentage:.2f}%)"
            cv2.putText(im, label_text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 1)

        cv2.imshow('detection', im)
        key = cv2.waitKey(5)
        if key == ord('q'):
            break

    cv2.destroyAllWindows()

if __name__ == '__main__':
    print("started")
    run_detection()
