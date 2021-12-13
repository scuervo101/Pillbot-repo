#!/usr/bin/python

"""
Central master controller node which will handle all transactions and logic
"""

import rospy
from central_master_controller.msg import command
from raspi_serial.msg import transmit_data, received_data
from fiducial_msgs.msg import Fiducial, FiducialArray
from geometry_msgs.msg import PoseStamped, Pose, Point, Quaternion
from nav_msgs.msg import Path
from tf.transformations import quaternion_from_euler, euler_from_quaternion, quaternion_multiply
import time

class MasterController():
    def __init__(self):
        rospy.loginfo("Setting up master controller Node...")
        rospy.init_node('cmc_node')

        # Delivery target initialized
        self.target = 0

        # Scan variables
        self.time_last_scan = time.time()
        self.scan_rate = 15
        self.caught_flag = 0

        # Deliver variables
        self.enter_deliver = 0
        self.avg_left_side = 0
        self.avg_right_side = 0
        self.target_in_frame = 0
        self.center_tolerance = 128

        # Released variables
        self.prev_mode = 0
        self.new_explore_path = 0

        # Yaw variables
        self.bot_yaw = 0 # In radians
        self.explore_yaw = 0 # In radians
        self.yaw_tolerance = .314

        # Rx data
        self.prev_rx_data = 0
        self.ack_flag = 0
        self.ack_rx_data = 0

        # Tx data
        self.prev_tx_sent = 0

        # Mode control variable
        self.mode = 5
        """
        Modes:

        0 = Idle mode

        1 = Run mode 
            (rotate until bot_yaw and explore_yaw are the same and then move forward, transfer to scan mode at the rate defined in scan_rate)
        2 = Scan mode
            (rotate until target fiducial is found, then enter drop off mode)
        3 = Drop off mode
            (move towards fiducial and sound alarm)
        4 = Pause mode
            (stop motor functions)
        5 = Waiting for handshake
            ()
        6 = Killed mode
            (Wait for restart)
        7 = Released mode
            (Release control to msp430 until it flags and until we receive a new exploration path)
        8 = Waiting mode
            (Waiting for new explore path mode)
        9 = Pickup mode
            (Waiting for pickup)
        """

        # Create publisher for /tx_data topic
        self.ros_pub_tx_data = rospy.Publisher("/tx_data", transmit_data, queue_size=1)
        rospy.loginfo("Established publisher for /tx_data topic")

        time.sleep(3)

        # Establish Connection with MSP430
        self.ros_pub_tx_data.publish([0xff])
        self.prev_tx_sent = 0xff
        self.handshake_flag = 0

        # Create subscriber for /rx_data topic
        self.ros_sub_rx_data = rospy.Subscriber("/rx_data", received_data, self.rx_data_cb)
        rospy.loginfo("Established subscriber for /rx_data topic")

        # Creater subscriber for /cmd topic
        self.ros_sub_cmd = rospy.Subscriber("/cmd", command, self.cmd_cb)
        rospy.loginfo("Established subscriber for /cmd topic")

        # Create subscriber for /fiducial_vertices topic
        self.ros_sub_fiducial_vertices = rospy.Subscriber("/fiducial_vertices", FiducialArray, self.fiducial_vertices_cb)
        rospy.loginfo("Established subscriber for /fiducial_vertices topic")

        # Creater subscriber for /exploration_path topic
        self.ros_sub_exploration_path = rospy.Subscriber("/exploration_path", Path, self.exploration_path_cb)
        rospy.loginfo("Established subscriber for /exploration_path topic")

        # Creater subscriber for /slam_out_pose topic
        self.ros_sub_slam_out_pose = rospy.Subscriber("/slam_out_pose", PoseStamped, self.slam_out_pose_cb)
        rospy.loginfo("Established subscriber for /slam_out_pose topic")

        rospy.loginfo("Initialization complete")

    def rx_data_cb(self, message): #_________________________________________________________________________________________________________________
        # Callback function to handle rx data

        # Store message
        self.prev_rx_data = ord(message.rx_string[0])

        # Debugging log, comment out once finalized
        rospy.loginfo("Rx data received: " + hex(self.prev_rx_data))

        # Start of communication
        if (self.prev_rx_data == 0xff):
            self.handshake_flag = 1
            self.mode = 0
            rospy.loginfo("Handshake established connection with msp430")

        elif (self.prev_rx_data == 0x07):
            self.mode = 6

            # Send kill command to msp430
            self.ros_pub_tx_data.publish([0x1f])
            self.prev_tx_sent = 0x1f

            rospy.loginfo("Killing bot due to fault from msp430")

        elif ((self.prev_rx_data == 0x20) or (self.prev_rx_data == 0x21)):
            # Save previous mode and set to Released mode
            self.prev_mode = self.mode 
            self.mode = 7

            # Send ack_stop to msp430
            self.ros_pub_tx_data.publish([0x20])
            self.prev_tx_sent = 0x20

            # If previously in delivery mode
            if (self.prev_mode == 3):
                time.sleep(0.1)
                # Send ack play speaker to msp430
                self.ros_pub_tx_data.publish([0x22])
                self.prev_tx_sent = 0x22          

            rospy.loginfo("Releasing control to raspi")

        elif ((self.prev_rx_data >= 0x10) and (self.prev_rx_data <= 0x16)):
            self.ack_flag = 1
            self.ack_rx_data = self.prev_rx_data

    def cmd_cb(self, message): #_____________________________________________________________________________________________________________________
        # Callback function to handle cmd

        # Debugging log, comment out once finalized
        rospy.loginfo("Cmd received")

        if (message.cmd > 0x80):            # Deliver Command
            if (self.target != 0):
                rospy.loginfo("Target has already been selected")
            elif (self.mode == 6):
                rospy.loginfo("Bot is dead")
            elif (self.mode != 5):
                self.target = message.cmd - 128
                # Enter deliver mode
                self.mode = 1
                self.time_last_scan = time.time()

                rospy.loginfo("Set target to: " + str(self.target))
            else:
                rospy.loginfo("Never established connection with msp430")

        elif (message.cmd == 0x70):         # Drop command
            if (self.mode == 5):
                rospy.loginfo("Cannot drop due to connection issue")
            elif (self.mode == 6):
                rospy.loginfo("Bot is dead")
            else:
                self.mode = 0
                self.target = 0
                self.ros_pub_tx_data.publish([0x10])
                self.prev_tx_sent = 0x10

        elif (message.cmd == 0x30):         # Kill command
            rospy.loginfo("Killing bot")

            # Send kill command to msp430
            self.ros_pub_tx_data.publish([0x1f])
            self.prev_tx_sent = 0x1f

            # Enter killed mode
            self.mode = 6

        elif (message.cmd == 0x20):         # Pause command
            if (self.mode == 4):
                rospy.loginfo("Unpausing motor functions")

                # Send pause command to msp430
                self.ros_pub_tx_data.publish([0x0f])
                self.prev_tx_sent = 0x0f
                
                time.sleep(0.3)
                while(self.prev_rx_data != 0x10):            
                    if (self.prev_rx_data == 0x01):
                        rospy.loginfo("msp430 did not unpause")
                        
                    self.ros_pub_tx_data.publish([0x0f])
                    self.prev_tx_sent = 0x0f
                    time.sleep(0.3)

                # Enter deliver mode if target
                if (self.target):
                    self.mode = 1
                else:
                    self.mode = 0
            elif (self.mode == 5):
                rospy.loginfo("Never established connection with msp430")
            elif (self.mode == 6):
                rospy.loginfo("Bot is dead")

            else:
                rospy.loginfo("Pausing motor functions")

                # Send pause command to msp430 and set to pause mode
                self.ros_pub_tx_data.publish([0x0f])
                self.prev_tx_sent = 0x0f
                self.mode = 4

        elif (message.cmd == 0x10):         # State command
            rospy.loginfo("Outputting state")

            rospy.loginfo("____________________________")
            if (self.mode == 0):
                rospy.loginfo("Current state: Idle mode")
            elif (self.mode == 1):
                rospy.loginfo("Current state: Delivery mode")
            elif (self.mode == 2):
                rospy.loginfo("Current state: Scan mode")   
            elif (self.mode == 3):
                rospy.loginfo("Current state: Drop off mode")
            elif (self.mode == 4):
                rospy.loginfo("Current state: Pause mode")
            elif (self.mode == 5):
                rospy.loginfo("Currently waiting for handshake to establish connection to msp430")
            elif (self.mode == 6):
                rospy.loginfo("CMC is dead")
            elif (self.mode == 7):
                rospy.loginfo("Current state: Released mode")
            elif (self.mode == 8):
                rospy.loginfo("Current state: Waiting mode")
            elif (self.mode == 9):
                rospy.loginfo("Current state: Pickup mode")

    def fiducial_vertices_cb(self, message):    #____________________________________________________________________________________________________
        # Callback function to handle fiducial vertices

        # Debugging log, comment out once finalized
        rospy.loginfo("Fiducial array received")

        # # Only check fiducials if in scan mode
        # Search for fiducial whenever

        # Clear in frame and caught flag incase target is not found
        self.target_in_frame = 0
        self.caught_flag = 0

        # Search through caught fiducials for target fiducial
        if ((self.mode != 5) or (self.mode != 6)):
            for fids in message.fiducials:
                if (fids.fiducial_id == self.target):
                    if (self.mode == 3):
                        rospy.loginfo("Target is in frame")
                        self.target_in_frame = 1

                        self.avg_left_side = (fids.x0 + fids.x1)/2
                        fix_right_side = (fids.x2 + fids.x3)/2
                        self.avg_right_side = 1280 - fix_right_side
                    else:
                        rospy.loginfo("Target has be found")

                        # Stopped moving
                        self.caught_flag = 1
                        
                        # Send pause command to stop moving
                        self.ros_pub_tx_data.publish([0x0f])
                        self.prev_tx_sent = 0x0f

                        # Check for acknowledge 
                        tx_expect = 0x01 
                        time.sleep(0.3)
                        if (self.prev_rx_data != tx_expect):
                            rospy.loginfo("Error: Issue with msp430 acknowledging "+str(tx_expect)+" command")
                            continue

                        # Enter drop off mode and set enter flag
                        self.mode = 3
                        self.enter_deliver = 1

    def exploration_path_cb(self, message): #________________________________________________________________________________________________________
        # Callback function to handle the exploration path

        # Debugging log, comment out once finalized
        rospy.loginfo("Exploration path received")

        # Set new explore path variable
        self.new_explore_path = 1

        if (not message.poses[0]):
            rospy.loginfo("Must wait for new exploration path")
            self.mode = 8

        # Save quaternion values
        quaternion = (
            message.poses[0].pose.orientation.x,
            message.poses[0].pose.orientation.y,
            message.poses[0].pose.orientation.z,
            message.poses[0].pose.orientation.w)

        # Convert quaternion to euler
        euler = euler_from_quaternion(quaternion)
        self.explore_yaw = euler[2]

    def slam_out_pose_cb(self, message): #___________________________________________________________________________________________________________
        # Callback function to handle the slam out pose

        # # Debugging log, comment out once finalized
        # rospy.loginfo("Slam out pose received")

        # Save quaternion values
        quaternion = (
            message.pose.orientation.x,
            message.pose.orientation.y,
            message.pose.orientation.z,
            message.pose.orientation.w)

        quaternion_rot = quaternion_from_euler(0, 0, -1.57079)

        final_quaternion = quaternion_multiply(quaternion_rot, quaternion)

        # Convert quaternion to euler
        euler = euler_from_quaternion(final_quaternion)
        self.bot_yaw = euler[2]
 
    def run(self): #_________________________________________________________________________________________________________________________________
        # Run function for controller

        rate = rospy.Rate(10)

        while not rospy.is_shutdown():

            if (self.mode == 1):                                        # Delivery state
                tx_send = 0x10
                self.ros_pub_tx_data.publish([tx_send])
                self.prev_tx_sent = tx_send

                # Acknowledge handle
                time.sleep(0.3)
                if (self.ack_flag):
                    if (self.ack_rx_data != tx_send):
                        rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                        continue
                    self.ack_flag = 0
                else:
                    rospy.loginfo("Error: No acknowledgement received")
                    continue

                # Calculate delta for bot correction
                correction = self.bot_yaw - self.explore_yaw

                rospy.loginfo("Delta between yaws: "+str(correction))

                if ((correction <= self.yaw_tolerance) and (correction >= -self.yaw_tolerance)):
                    # Correct direction move forward
                    tx_send = 0x11
                    self.ros_pub_tx_data.publish([tx_send])
                    self.prev_tx_sent = tx_send

                    # Acknowledge handle
                    time.sleep(0.3)
                    if (self.ack_flag):
                        if (self.ack_rx_data != tx_send):
                            rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                            continue
                        self.ack_flag = 0
                    else:
                        rospy.loginfo("Error: No acknowledgement received")
                        continue

                    #time.sleep(5)

                elif(correction < 0):
                    # Rotate right
                    tx_send = 0x14
                    self.ros_pub_tx_data.publish([tx_send])
                    self.prev_tx_sent = tx_send

                    # Acknowledge handle
                    time.sleep(0.3)
                    if (self.ack_flag):
                        if (self.ack_rx_data != tx_send):
                            rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                            continue
                        self.ack_flag = 0
                    else:
                        rospy.loginfo("Error: No acknowledgement received")
                        continue

                elif(correction > 0):
                    # Rotate left
                    tx_send = 0x13
                    self.ros_pub_tx_data.publish([tx_send])
                    self.prev_tx_sent = tx_send

                    # Acknowledge handle
                    time.sleep(0.3)
                    if (self.ack_flag):
                        if (self.ack_rx_data != tx_send):
                            rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                            continue
                        self.ack_flag = 0
                    else:
                        rospy.loginfo("Error: No acknowledgement received")
                        continue

                # # Scan the environment for fiducials
                # if (time.time() - self.time_last_scan > self.scan_rate):    # Scan state
                #     # Enter scan mode
                #     self.mode = 2

                #     rospy.loginfo("Performing scan operation")

                #     if (self.caught_flag != 1):
                #         # Tell msp430 to scan for fiducials
                #         tx_send = 0x15
                #         self.ros_pub_tx_data.publish([tx_send])
                #         self.prev_tx_sent = tx_send

                #         # Acknowledge handle
                #         time.sleep(0.3)
                #         if (self.ack_flag):
                #             if (self.ack_rx_data != tx_send):
                #                 rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                #                 continue
                #             self.ack_flag = 0
                #         else:
                #             rospy.loginfo("Error: No acknowledgement received")
                #             continue

                #     # Scan (spin) for 10 seconds
                #     time.sleep(10)

                #     # Save scan time
                #     self.time_last_scan = time.time()
                    
                #     # Exit scan mode if fiducial was not caught
                #     if (self.caught_flag != 1):
                #         self.mode = 1

            elif (self.mode == 2):                                      # Fault scan state
                rospy.loginfo("Bot should not be in scan mode at this time, returning to correct mode")
                if (self.target != 0):
                    self.mode = 1
                else:
                    self.mode = 0

            elif (self.mode == 3):                                      # Drop off state
                if (self.enter_deliver):
                    # Unpause moving
                    tx_send = 0x0f
                    self.ros_pub_tx_data.publish([0x0f])
                    self.prev_tx_sent = 0x0f
                    # Expect ackstop to acknowledge unpause
                    tx_send = 0x10

                    # Acknowledge handle
                    time.sleep(0.3)
                    if (self.ack_flag):
                        if (self.ack_rx_data != tx_send):
                            rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                            continue
                        self.ack_flag = 0
                    else:
                        rospy.loginfo("Error: No acknowledgement received")
                        continue
                    
                    # Clear enter deliver flag
                    self.enter_deliver = 0
                
                if (self.target_in_frame):
                    image_offset = self.avg_left_side - self.avg_right_side

                    rospy.loginfo("Delta between images: "+str(image_offset))

                    if ((image_offset <= self.center_tolerance) and (image_offset >= -self.center_tolerance)):
                        # Correct direction move forward
                        tx_send = 0x11
                        self.ros_pub_tx_data.publish([tx_send])
                        self.prev_tx_sent = tx_send

                        # Acknowledge handle
                        time.sleep(0.3)
                        if (self.ack_flag):
                            if (self.ack_rx_data != tx_send):
                                rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                                continue
                            self.ack_flag = 0
                        else:
                            rospy.loginfo("Error: No acknowledgement received")
                            continue

                    elif (image_offset < 0):
                        # Rotate right
                        tx_send = 0x14
                        self.ros_pub_tx_data.publish([tx_send])
                        self.prev_tx_sent = tx_send

                        # Acknowledge handle
                        time.sleep(0.3)
                        if (self.ack_flag):
                            if (self.ack_rx_data != tx_send):
                                rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                                continue
                            self.ack_flag = 0
                        else:
                            rospy.loginfo("Error: No acknowledgement received")
                            continue

                    elif (image_offset > 0):
                        # Rotate left
                        tx_send = 0x13
                        self.ros_pub_tx_data.publish([tx_send])
                        self.prev_tx_sent = tx_send

                        # Acknowledge handle
                        time.sleep(0.3)
                        if (self.ack_flag):
                            if (self.ack_rx_data != tx_send):
                                rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                                continue
                            self.ack_flag = 0
                        else:
                            rospy.loginfo("Error: No acknowledgement received")
                            continue
                else:
                    tx_send = 0x10
                    self.ros_pub_tx_data.publish([tx_send])
                    self.prev_tx_sent = tx_send

                    # Acknowledge handle
                    time.sleep(0.3)
                    if (self.ack_flag):
                        if (self.ack_rx_data != tx_send):
                            rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                            continue
                        self.ack_flag = 0
                    else:
                        rospy.loginfo("Error: No acknowledgement received")
                        continue

                    # Play speaker sound until acknowledged
                    tx_send = 0x16
                    self.ros_pub_tx_data.publish([tx_send])
                    self.prev_tx_sent = tx_send

                    # Acknowledge handle
                    time.sleep(0.3)
                    if (self.ack_flag):
                        if (self.ack_rx_data != tx_send):
                            rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                            continue
                        self.ack_flag = 0
                    else:
                        rospy.loginfo("Error: No acknowledgement received")
                        continue

                    self.mode = 9

            elif (self.mode == 4):                                      # Pause state
                if (self.prev_tx_sent != 0x0f):
                    self.ros_pub_tx_data.publish([0x0f])
                    self.prev_tx_sent = 0x0f
                time.sleep(1)

            elif (self.mode == 0):                                      # Idle state
                if (self.prev_tx_sent != 0x10):
                    self.ros_pub_tx_data.publish([0x10])
                    self.prev_tx_sent = 0x10
                time.sleep(1)

            elif (self.mode == 7):                                      # Released state
                # Wait until control is released
                if (self.prev_rx_data == 0x22):
                    # If delivery mode, wait until new explore is received
                    if (self.prev_mode == 1):
                        self.new_explore_path = 0
                        self.mode = 8
                        
                    elif (self.prev_mode == 3):
                        self.mode = 9
                        
                    # Else return to previous mode
                    else:
                        self.mode = self.prev_mode

                    rospy.loginfo("Returning to state: " + str(self.mode))
                    
                else:
                    rospy.loginfo("Waiting for msp430 to release control")

            elif (self.mode == 8):                                      # Waiting state
                if (self.prev_tx_sent != 0x10):
                    self.ros_pub_tx_data.publish([0x10])
                    self.prev_tx_sent = 0x10

                if (self.new_explore_path) and (self.target):
                    rospy.loginfo("New exploration path received")
                    self.mode = 1
                else:
                    rospy.loginfo("Waiting for new exploration path")
                    time.sleep(1)

            elif (self.mode == 9):                                      # Pickup state
                 # Wait for pill received msg
                while (self.prev_rx_data != 0x30):
                    rospy.loginfo("Waiting for pills to be acquired")
                    time.sleep(3)

                # Move backwards for a bit
                tx_send = 0x12
                self.ros_pub_tx_data.publish([tx_send])
                self.prev_tx_sent = tx_send

                # Acknowledge handle
                time.sleep(0.3)
                if (self.ack_flag):
                    if (self.ack_rx_data != tx_send):
                        rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                        continue
                    self.ack_flag = 0
                else:
                    rospy.loginfo("Error: No acknowledgement received")
                    continue

                time.sleep(1)

                # Return to idle state
                self.target = 0
                self.caught_flag = 0

                tx_send = 0x10
                self.ros_pub_tx_data.publish([tx_send])
                self.prev_tx_sent = tx_send

                # Acknowledge handle
                time.sleep(0.3)
                if (self.ack_flag):
                    if (self.ack_rx_data != tx_send):
                        rospy.loginfo("Error: Issue with msp430 acknowledging "+tx_send+" command")
                        continue
                    self.ack_flag = 0
                else:
                    rospy.loginfo("Error: No acknowledgement received")
                    continue

                self.mode = 0

            rate.sleep()


if __name__ == "__main__":
    cmc_cont = MasterController()
    cmc_cont.run()