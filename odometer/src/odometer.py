#!/usr/bin/env python

import rospy
from std_msgs.msg import TimerEvent
from odometer.msg import speed_msg
import RPi.GPIO as GPIO

SENSOR_PIN = 23
PPR = 32
g_count = 0

class Odometer:
    def __init__(self, nh):
        self.speed = 0.0
        self.total_count = 0
        self.speed_pub = nh.Publisher('/speed', speed_msg, queue_size=10)
        self.timer = rospy.Timer(rospy.Duration(0.5), self.publish_speed)

    def calculate_speed(self):
        global g_count
        self.total_count += g_count
        self.speed = (g_count * 1.527 / PPR) * 3.6 * 2  # Count to KPH
        g_count = 0
        rospy.loginfo("KPH: %f km/h", self.speed)
        return self.speed

    def publish_speed(self, event):
        msg = speed_msg()
        msg.kph = self.calculate_speed()
        msg.odo = self.total_count / PPR * 1.527
        self.speed_pub.publish(msg)

def count_up(channel):
    global g_count
    g_count += 1

def main():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(SENSOR_PIN, GPIO.IN)
    rospy.init_node('odo_node')
    nh = rospy.NodeHandle()
    odometer = Odometer(nh)
    GPIO.add_event_detect(SENSOR_PIN, GPIO.BOTH, callback=count_up)
    rospy.spin()

if __name__ == '__main__':
    main()
