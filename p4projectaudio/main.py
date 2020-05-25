import serial
from playsound import playsound

# Serial takes two parameters: serial device and baudrate
ser = serial.Serial('COM3', 115200)
# Read data from serial communication channel and save to a variable
data = ser.read()
# Save data to a new variable
prev_data = data
# Check if data contains a desired char
def contains(char):
    return char in str(data) and char not in str(prev_data)
# Read data and play sound based on received data
while True:
    prev_data = data
    data = ser.read()
    if contains('1'):
        playsound("data/untitled - Track 42.wav")
    elif contains('2'):
        playsound("data/untitled - Track 12.wav")
    elif contains('3'):
        playsound("data/untitled - Track 21.wav")
    elif contains('4'):
        playsound("data/untitled - Track 46.wav")
    elif contains('5'):
        playsound("data/untitled - Track 5.wav")
    elif contains('6'):
        playsound("data/untitled - Track 24.wav")
