import serial
import time
from pynput import keyboard

########
# Code written mainly by AI, thoroughly controlled and tested by a human
########

# CHANGE THIS to match your Arduino's port (e.g., 'COM3' on Windows or '/dev/ttyACM0' on Mac/Linux)
SERIAL_PORT = '/dev/tty.usbmodem1401' 
BAUD_RATE = 9600

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2) # Wait for Arduino to reset
    print("Connected to Arduino! Use W/S (Shoulder), A/D (Base), Q/E (Elbow). Press ESC to quit.")
except Exception as e:
    print(f"Error connecting to serial port: {e}")
    exit()

def on_press(key):
    time.sleep(0.02) # prevent flooding with signals
    try:
        # Check which key was pressed and send a single character to Arduino
        if key.char == 'w': ser.write(b'W')
        elif key.char == 's': ser.write(b'S')
        elif key.char == 'a': ser.write(b'A')
        elif key.char == 'd': ser.write(b'D')
        elif key.char == 'q': ser.write(b'Q')
        elif key.char == 'e': ser.write(b'E')
        elif key.char == 'r': ser.write(b'R')
        elif key.char == 'f': ser.write(b'F')
        elif key.char == 't': ser.write(b'T')
        elif key.char == 'g': ser.write(b'G')
    except AttributeError:
        if key == keyboard.Key.esc:
            return False # Stop listener and quit

# Start listening to the keyboard
with keyboard.Listener(on_press=on_press) as listener:
    listener.join()

ser.close()
