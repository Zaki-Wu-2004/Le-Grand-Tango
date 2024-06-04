import serial
import time
import numpy as np
import cv2 as cv
import copy
import random
import os
import time



#########################################################################paras demanded-in cm############################################################

K=5 #1:0.2cm
#枪头盒
corh_x=0.5 #最小坐标的角抢横坐标
corh_y=10.7 #最小坐标的角抢纵坐标
d_ch=0.89 #枪头盒距离单位
corh_coords=[] #枪头盒坐标集
for x in range(12):
    for y in range(8):
        corh_coords.append((corh_x+(11-x)*d_ch, corh_y+y*d_ch))

#96孔板
h96_x=10.8 #最小坐标的角孔横坐标
h96_y=1.7 #最小坐标的角孔纵坐标
d_h96=0.89 #96孔板距离单位
h96_coords=[] #96孔板坐标集
for x in range(12):
    for y in range(8):
        h96_coords.append((h96_x+x*d_h96, h96_y+(7-y)*d_h96))

#垃圾桶
tru_lx=11.8
tru_rx=23.5
tru_by=9.8
tru_uy=17.9

#培养皿
pl_cen=(4.4,4.4)
pl_r=4.5

#定位点
comf=(corh_x+11*d_ch,corh_y)
comf_left=(corh_x+10*d_ch,corh_y)
comf_up=(corh_x+11*d_ch,corh_y+d_ch)



#############################################################################函数####################################################################

#图片文件路径
def get_image(directory):
    image_extensions = ['.png']#, '.jpg', '.jpeg', '.bmp', '.tiff', '.gif']
    try:
        while True:
            for filename in os.listdir(directory):
                if any(filename.lower().endswith(ext) for ext in image_extensions):
                    return filename
    except KeyboardInterrupt:
        print("Communication stopped by user.")
    return None
#current_directory = os.getcwd()
file_path = get_image('./')

#元组坐标缩放
def expand(input_tuple):
    # 使用列表推导式进行计算
    new_tuple = tuple(round(x * K) for x in input_tuple)
    return new_tuple


#从图片获取标准点
def fix_image(file):
    a = cv.imread(file)
    a = cv.medianBlur(a, 9)
    b = a.copy()
    a = cv.cvtColor(a, cv.COLOR_BGR2GRAY)
    a = cv.Canny(a, 0, 255)
    H, W = a.shape
    R = 0.0463*H
    Cx = W / 2 - 0.0375*W
    Cy = H / 2 - 0.0972*H
    Y, X = np.ogrid[:H, :W]
    left = Cx - R
    right = Cx + R
    top = Cy - R
    bottom = Cy + R
    mask = (X < left) | (X > right) | (Y < top) | (Y > bottom)
    a[mask] = 0
    a[a > 0.3 * 255] = 255
    a[a <= 0.3 * 255] = 0
    contours, _ = cv.findContours(a, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    C = []
    for cnt in contours:
        M = cv.moments(cnt)
        if M["m00"] != 0:
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])
            cv.circle(b, (cx, cy), radius=5, color=(0, 255, 0), thickness=-1)#
            C.append((cx, cy))
    #cv.imwrite(file+'_fixed.jpg', b)#
    seen = set()
    C = [x for x in C if not (x in seen or seen.add(x))]
    return C
picf, picf_left, picf_up, _=fix_image(file_path)

#图片域和计算域的转换，元组到元组
def from_pic_to_com(Coor):
    px,py=Coor
    KX=(comf[0]-comf_left[0])/(picf[0]-picf_left[0])
    KY=(comf_up[1]-comf[1])/(picf_up[1]-picf[1])
    cx=KX*(px-picf[0])+comf[0]
    cy=KY*(py-picf[1])+comf[1]
    return (cx, cy)

def from_com_to_pic(Coor):
    cx,cy=Coor
    KX=(picf[0]-picf_left[0])/(comf[0]-comf_left[0])
    KY=(picf_up[1]-picf[1])/(comf_up[1]-comf[1])
    px=KX*(cx-comf[0])+picf[0]
    py=KY*(cy-comf[1])+picf[1]
    return (px, py)

#获取【计算域】中的菌落坐标
def get_coords(file):
    a = cv.imread(file)
    a = cv.medianBlur(a, 9)
    b = a.copy()
    a = cv.cvtColor(a, cv.COLOR_BGR2GRAY)
    a = cv.Canny(a, 0, 255)
    H, W = a.shape
    R = 0.21*H
    Cx = 0.255*W
    Cy = 0.78*H
    Y, X = np.ogrid[:H, :W]
    dist_from_center = np.sqrt((X - Cx)**2 + (Y - Cy)**2)
    mask = dist_from_center > R
    a[mask] = 0
    a[a > 0.3 * 255] = 255
    a[a <= 0.3 * 255] = 0
    contours, _ = cv.findContours(a, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    C = []
    for cnt in contours:
        M = cv.moments(cnt)
        if M["m00"] != 0:
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])
            cv.circle(b, (cx, cy), radius=3, color=(0, 0, 255), thickness=-1)#
            C.append((cx, cy))
    #cv.imwrite(file+'_marked.jpg', b)#
    #seen = set()
    #C = [x for x in C if not (x in seen or seen.add(x))]
    C = [from_pic_to_com(x) for x in C]
    return C
tasks=get_coords(file_path)
Lc=len(tasks)

#随机丢垃圾
def litter(x_min, x_max, y_min, y_max):
    random_x = random.uniform(x_min, x_max)
    random_y = random.uniform(y_min, y_max)
    return (random_x, random_y)

#输出函数
def output(a, b, c, d):
    print(a, b, c, d)
    pass


#############################################################################测试####################################################################
def evaluate(C):
    a = cv.imread(file_path)
    a = cv.medianBlur(a, 9)
    for cx,cy in C:
        #print(from_com_to_pic((cx/5,cy/5)))
        #cv.circle(a, tuple(round(x/5) for x in from_com_to_pic((cx,cy))), radius=3, color=(0, 0, 255), thickness=-1)
        cv.circle(a, tuple(round(x) for x in from_com_to_pic((cx,cy))), radius=3, color=(0, 0, 255), thickness=-1)
    cv.imwrite(file_path+'_EVALUATE.jpg', a)
#evaluate(corh_coords)
#evaluate(h96_coords)



#############################################################################任务####################################################################

L=min(Lc,96)
# Set serial parameters
ser = serial.Serial('COM8', 9600, timeout=1)  # Replace with the actual serial port

time.sleep(2)  # Wait for Arduino to reset

def send_coordinates(coordinates):
    # Serialize the coordinate list into a string
    data = ';'.join(["{},{}".format(x, y) for x, y in coordinates])
    ser.write(data.encode())  # Send data
    time.sleep(0.5)
    response = ser.readline().decode('utf-8').strip()  # Read the response from Arduino
    return response

for epoch in range(L):
    time.sleep(30) #睡一会儿
    try:
        while True:
            coordinates = [expand(corh_coords[epoch]), expand(h96_coords[epoch]), expand(tasks[epoch]), expand(litter(tru_lx, tru_rx, tru_by, tru_uy))]
            print(coordinates)
            print(f"Sending coordinates: {coordinates}")
            response = send_coordinates(coordinates)
            print(f"Arduino response: {response}")
            time.sleep(5)  # Send data every 5 seconds
    except KeyboardInterrupt:
        print("Communication stopped by user.")
    finally:
        ser.close()  # Close the serial port
