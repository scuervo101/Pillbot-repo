#!/usr/bin/python

"""
Script for serial communication between raspi and msp430
"""

import rospy
from raspi_serial.msg import received_data
from raspi_serial.msg import transmit_data
import serial, time


class SerialController():
    def __init__(self):
        rospy.loginfo("Setting up serial Node...")
        rospy.init_node('serial_node')

        # Open serial communication on ttyAMA0
        self.ser = serial.Serial ("/dev/ttyAMA0", 115200)
        rospy.loginfo("Openned serial com on /dev/ttyAMA0 with baud of 115200")

        self.rx_data = 0

        # Create publisher for /rx_data topic
        self.ros_pub_received_data = rospy.Publisher("/rx_data", received_data, queue_size=1)
        rospy.loginfo("Established publisher for /rx_data topic")

        # Create subscriber for /tx_data topic
        self.ros_sub_trasmit_data = rospy.Subscriber("/tx_data", transmit_data, self.transmiting_data)
        rospy.loginfo("Established subscriber for /tx_data topic")

        rospy.loginfo("Initialization complete")
    
    def transmiting_data(self, message):
        # Callback function to transmit data on tx_data topic

        self.ser.write(message.tx_string)
        rospy.loginfo("Transmitting tx_data" + hex(ord(message.tx_string)))

    def run(self):
        # Run function that will continuously poll serial read

        while not rospy.is_shutdown():
            self.rx_data = self.ser.read()              #read serial port
            time.sleep(0.03)
            data_left = self.ser.inWaiting()             #check for remaining byte
            self.rx_data  += self.ser.read(data_left)
            if self.rx_data: 
                rospy.loginfo("Received data: " + self.rx_data)
                self.ros_pub_received_data.publish(self.rx_data)

if __name__ == "__main__":
    raspi_sc = SerialController()
    raspi_sc.run()