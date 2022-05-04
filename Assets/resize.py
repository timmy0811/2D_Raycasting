import cv2 as cv
import glob, os, time
from PIL import Image

ch = input('Resize Folder? (y/n) ')

images = []
out_images = []

if(ch == 'n'):
    img_input = input("Enter filename: ")
    img = cv.imread(img_input, cv.IMREAD_UNCHANGED)

    print('Dimensions: ', img.shape)
else:
    end = input('Enter file extension: ')
    for file in glob.iglob('*' + end, recursive=True):
        images.append(file)

resize_width = int(input("New width: "))
resize_height = int(input("New height: "))

dimensions = (resize_width, resize_height)

if(ch == 'n'):
    img_output = cv.resize(img, dimensions, interpolation= cv.INTER_AREA)
    cv.imshow("Resized image", img_output)
    cv.imwrite(img_input, img_output)
else:
    for img_file in images:
        img = cv.imread(img_file, cv.IMREAD_UNCHANGED)
        img_out = cv.resize(img, dimensions, interpolation= cv.INTER_AREA)
        cv.imwrite(str(img_file), img_out)

cv.waitKey(0)
cv.destroyAllWindows()