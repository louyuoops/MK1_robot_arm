# MK1 ROBOT ARM

The MK1 robotic arm is an open-source 3D printing robotic arm with a simple structure, requiring only 4 SG90 servos, or alternatively MG90S servos. Note that the dimensions of the transmission arm for MG90S servos are different, so you will need to modify the model parameters accordingly.

Refer to the following video for assembly:   
[![Alt text](https://img.youtube.com/vi/cZBtzuyKoHM/0.jpg)](https://www.youtube.com/watch?v=cZBtzuyKoHM)

During the fabrication process, I utilized a PS2 joystick to control the rotation, forward and backward movements, as well as lifting and lowering of the robotic arm. The red button is used to operate the gripper, the black button is used to release the gripper, and the blue button assists in the lifting and lowering actions.
[![Alt text](./imgs/mk1_robot_arm.png)]()    
In this project, I employed an ESP32 development board, a 16-channel servo controller (using multiple servos with ESP32 always led to issues), and a 5V power supply module. I experimented with various power supply and voltage regulation modules and found that only the one I used proved to be stable.   
