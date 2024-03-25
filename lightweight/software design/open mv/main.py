import sensor
import time
import math

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
thresholds = [
    (30, 74, -22, 14, -57, -30), # blue
    (0, 100, -50, 127, 19, 61) # yellow
]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=1000)

sensor.set_auto_gain(False)  # must be turned off for color tracking
sensor.set_auto_whitebal(False)  # must be turned off for color tracking
clock = time.clock()

blueDistance = 0
blueAngle = 0
yellowDistance = 0
yellowAngle = 0

def lookForGoals():
    clock.tick()
    img = sensor.snapshot()
    for i, color_threshold in enumerate(thresholds):
        for blob in img.find_blobs([color_threshold], pixels_threshold=200, area_threshold=100, merge=True):
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
            # Calculate the distance
            Lm = (blob.w()+blob.h())/2
            # Assuming K is the known constant
            K = 1350
            distance = K/Lm
            #change to polynomial regression
            color = 'blue' if i == 0 else 'yellow'
            if color == "blue":
                blueDistance = distance
                blueAngle = math.pi/2 - math.atan2(blob.cy(), blob.cx())
            else :
                yellowDistance = distance
                yellowAngle = math.pi/2 - math.atan2(blob.cy(), blob.cx())
            print(color)

def localise():
    blueX = blueDistance * math.sin(blueAngle)
    blueY = blueDistance * math.cos(blueAngle)
    yellowX = yellowDistance * math.sin(yellowAngle)
    yellowY = yellowDistance * math.cos(yellowAngle)
    centreX = (blueX + yellowX)/2
    centreY = (blueY + yellowY)/2
    centreAngle = 90 - math.atan2(centreY, centreX)


# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. Don't set "merge=True" because that will merge blobs which we don't want here.
while True:
    lookForGoals()
    localise()


