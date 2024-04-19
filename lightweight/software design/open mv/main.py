import sensor
import time
import math
from pyb import UART, LED

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
thresholds = [
    (54, 66, -26, -14, -38, -22), # blue
    (70, 93, -25, -1, 17, 127) # yellow
]

centreX = 135
centreY = 109

debug_cam = True

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

curr_gain = sensor.get_gain_db()
sensor.set_auto_gain(False, gain_db=0)

curr_exposure = sensor.get_exposure_us()
#sensor.set_auto_exposure(False, exposure_us = 3000)

sensor.set_auto_whitebal(False, rgb_gain_db = (-5.55645, -6.52073, -0.504)) #Must remain false for blob tracking
#sensor.set_auto_whitebal(False, rgb_gain_db = (10, 10, 10))

sensor.set_brightness(0)
sensor.set_contrast(3)
clock = time.clock()
uart = UART(1, 1000000)

led = LED(1) # red led
led.on()
print("start")

def mapDist(pixel_dist):
    actual_dist = 0.0000068834 * (pixel_dist ** 4) - 0.0012637153 * (pixel_dist ** 3) + 0.0790691249 * (pixel_dist ** 2) - 1.114138421 * pixel_dist + 10.8643452914
    return actual_dist

def unmapDist(actual_dist):
    pixel_dist = actual_dist
    #do polynomial regression
    return pixel_dist

#def lookForGoals(debug = False):
#    blueX = 0
#    blueY = 0
#    yellowX = 0
#    yellowY = 0
#    clock.tick()
#    img = sensor.snapshot()
#    for i, color_threshold in enumerate(thresholds):
#        max_area = 0
#        largest_blob = None
#        for blob in img.find_blobs([color_threshold], pixels_threshold=200, area_threshold=100, merge=True):
#            img.draw_rectangle(blob.rect())
#            img.draw_cross(blob.cx(), blob.cy())
#            img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
#            color = 'blue' if i == 0 else 'yellow'
#            if color == "blue":
#                blueX = blob.cx()
#                blueY = blob.cy()
#            else :
#                yellowX = blob.cx()
#                yellowY = blob.cy()
#    return [yellowX, yellowY, blueX, blueY]

def lookForGoals():
    yellow_dist = 0.1
    yellow_angle = 0.1
    blue_dist = 0.1
    blue_angle = 0.1
    clock.tick()
    img = sensor.snapshot()
    for i, color_threshold in enumerate(thresholds):
        color = 'blue' if i == 0 else 'yellow'
        draw_color = (0, 0, 255) if i == 0 else (255, 255, 0)
        max_area = 0
        largest_blob = None
        for blob in img.find_blobs([color_threshold], pixels_threshold=200, area_threshold=100, merge=True, margin = 20):
            if blob.area() > max_area:
                max_area = blob.area()
                largest_blob = blob

        if largest_blob != None:
            if color == "blue":
                x = largest_blob.cx() - centreX
                y = centreY - largest_blob.cy()
                blue_dist = mapDist(math.sqrt(x ** 2 + y ** 2))
                blue_angle = (math.degrees(math.atan2(x, y)) + 90)%360
                print("blue:", blue_dist, blue_angle)

            if color == "yellow":
                x = largest_blob.cx() - centreX
                y = centreY - largest_blob.cy()
                yellow_dist = mapDist(math.sqrt(x ** 2 + y ** 2))
                yellow_angle = (math.degrees(math.atan2(x, y)) + 90)%360
                print("yellow:", yellow_dist, yellow_angle)

            if debug_cam:
                img.draw_rectangle(largest_blob.rect(), color = draw_color)
                img.draw_cross(largest_blob.cx(), largest_blob.cy())
                img.draw_arrow(centreX, centreY, largest_blob.cx(), largest_blob.cy(), color = draw_color)

    return [yellow_dist, yellow_angle, blue_dist, blue_angle]

def send(data):
    sendData = [42] #sync byte

    for num in data:
        # convert from 2dp float to int
        num = int(round(num, 2) * 100)
        sendData += list(num.to_bytes(2, 'little'))

    for num in sendData:
        try:
            uart.writechar(num)
        except:
            pass

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. Don't set "merge=True" because that will merge blobs which we don't want here.
while True:
    data = lookForGoals()
    send(data)


