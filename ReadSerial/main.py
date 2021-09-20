import serial
from datetime import datetime

ser = serial.Serial('COM5', 115200)
ser.flushInput()

while True:

    with open("bno055_100hz_bw64.txt", 'a') as file:
        date = datetime.now().strftime("%H:%M:%S.%f")
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes.decode("utf-8")
        file.write(date + "\t" + decoded_bytes.replace("\n", ""))