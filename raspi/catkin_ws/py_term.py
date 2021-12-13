#!/usr/bin/python

import time, os
import readline

"""
Python Robot Control terminal
"""

class Terminal():
    def __init__(self):
        print("=========================================")
        print("Welcome to the pill-bot terminal control:")
        print("=========================================")

    def kill_bot(self):
        print("Stoping bot, killing all nodes")
        
        # Debug print
        #print("rostopic pub /cmd central_master_controller/command \"cmd: 0x30\"") 
        os.system("rostopic pub /cmd central_master_controller/command \"cmd: 0x30\"")
        time.sleep(3)
        os.system("rosnode kill -a")

    def run(self):
        try:
            while True:
                # Raw cmd input
                cmd_raw = raw_input("py_term@pill-bot:$ ")
                # cmd input split between spaces
                cmd = cmd_raw.split()
                # cmd keyword
                cmd_key = cmd[0]

                if cmd_key == "pause":
                    print("Pausing bots movements")

                    # Debug print
                    #print("rostopic pub /cmd central_master_controller/command \"cmd: 0x20\"") 
                    os.system("rostopic pub /cmd central_master_controller/command \"cmd: 0x20\"")
                
                elif cmd_key == "stop":
                    self.kill_bot()

                elif cmd_key == "exit":
                    exit()

                elif cmd_key == "deliver":
                    val = int(cmd[1])
                    if val > 112 or val < 100:
                        print("*We only support deliver for room numbers 112 - 100")
                    else:
                        val = 0x80 + int(cmd[1])
                        # Debug print
                        #print("rostopic pub /cmd central_master_controller/command \"cmd: " + str(val) + "\"") 
                        os.system("rostopic pub /cmd central_master_controller/command \"cmd: " + str(val) + "\"")

                elif cmd_key == "drop":
                    print("Dropping delivery target and entering idle state")

                    # Debug print
                    #print("rostopic pub /cmd central_master_controller/command \"cmd: 0x70\"") 
                    os.system("rostopic pub /cmd central_master_controller/command \"cmd: 0x70\"")

                elif cmd_key == "log":
                    if cmd[1] == "state":
                        # Debug print
                        #print("rostopic pub /cmd central_master_controller/command \"cmd: 0x10\"") 
                        os.system("rostopic pub /cmd central_master_controller/command \"cmd: 0x10\"")

                elif cmd_key == "bash":
                    bash_cmd = ""
                    for word in cmd[1:]: 
                        bash_cmd += word + " "

                    os.system(bash_cmd)

                else:
                    print("*Error: Did not find " + cmd_raw + "\n")

        except KeyboardInterrupt:
            print("\n\n- Keyboard Exception")
            print("***Exiting py_term")

            self.kill_bot()

        except ValueError:
            print("\n- ValueError Exception")
            print("***Exiting py_term")

            self.kill_bot()

        except SystemExit:
            print("\n- SystemExit Exception")
            print("***Exiting py_term")

            self.kill_bot()

if __name__ == "__main__":
    term = Terminal()
    term.run()
    