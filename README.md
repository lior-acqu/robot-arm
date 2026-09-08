Welcome to Dedekind's GitHub repo. Here you can find:
- All the 3D files I used to print the robot.
- Three example C++ scripts to control the Arduino. More info on them below.
	- Rock-paper-scissors
	- Pick-and-place (called inversekinematics)
	- Manual servo control (servos)
# Scripts
## Rock-Paper-Scissors
When you run this script, you can play rock-paper-scissors with your robot arm. Opening the gripper means paper, closing it means rock, a quick open-close motion means scissors. Have fun!
## Pick-And-Place (inversekinematics)
In this script, you can make the robot pick up things like pens and erasers. If you use the same servos as I do, the weight of these objects is no problem.
The file contains many comments. If you need to change something in the script, you can find everything with ease. Possible things you may want to change:
- Positions: In the `loop()` function, you can change the numbers in the `inverseKin()` calls. The first number is the distance in cm from the shoulder joint. The second one is the angle of the base motor in degrees.
- Corrections: In case you have not been very precise in the assembly of the robot, and now, e.g. the shoulder joint is off by 10 degrees, you can enter corrections in the script that get taken into account by the inverse kinematics function. Just change `shoulder_corr` to `10`.
## Manual Servo Control (servos)
Here, you first run the Arduino script in the IDE. The robot will go to its home position.
Then, you open the `control.py` script. Here, you will probably need to change the `SERIAL_PORT` variable. You can find the right name for the port in the Arduino IDE in the top-left dropdown menu where you can select your Arduino board. Once the name is correct, you can run the script and then control all 5 servos with your keyboard.
- The base uses Q & A.
- The shoulder uses W & S.
- The elbow uses E & D.
- The wrist uses R & F.
- The gripper uses T & G.
# Known Issues
The robot arm works well. The points below are just small improvements.
1. Sometimes, the gripping motion does not work well. When it grabs a thin pen, the two gear racks sometimes lose contacts with their rails and the gripper gear. The pen then drops on the ground and the two gear racks are now a bit wider apart.
	- Possible fix: The clamps should not be at the very end of the gear rack. Instead, they should have a safe margin of 1-2cm to make sure the gear rack is still fully supported by the guardrails.
2. Some parts are not designed in a way that they could be screwed in very tightly. They tend to be a bit loose.
	- Possible fix: Change the parts so that they are a bit easier to assemble, make things a bit tighter where you can.
# Materials
## 3D Parts
The uploaded parts work well. I usually added -0.7mm of X-Y-contour-compensation to make sure that the parts fit.
Support is not needed, except for `ForearmBase.stl` and `UpperArmBase.stl`.
My personal recommendation: Take my files only as an inspiration and design your own parts!
### Used Filament
I used eSUN's PLA+. I think that any regular filament (PLA, PETG) works just fine.
## Used Electronics
### Servos
- 2x MG996R (by Miuzei)
- 2x MG90S (by Miuzei)
- 1x SG90
### Other Electronics
- 10A 5V power supply (by Leicke, yes, it needs that much current)
- Arduino UNO
- PWM Servo Driver PCA9685
## Screws
- A lot of M3 screws (6mm & 10mm length) with matching nuts.