import serial
import socket

# Generate read from thread
# import logging
import threading
import time


# Thread Class
class MyThread(threading.Thread):

    # Thread class with a _stop() method.
    # The thread itself has to check
    # regularly for the stopped() condition.

    def __init__(self, *args, **kwargs):
        super(MyThread, self).__init__(*args, **kwargs)
        self._stop = threading.Event()
        self.counter = 0

        # function using _stop function

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()

    def run(self):
        while True:
            self.counter = self.counter + 1
            if self.stopped():
                return
            print("Hello, world!")
            time.sleep(1)


# serial connection
ser = serial.Serial('COM3', 115200)

# TCP connection
TCP_IP = '192.168.10.1'
TCP_PORT = 23
BUFFER_SIZE = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

# send data serial
ser.write(b'hello\r\n')
data = s.recv(BUFFER_SIZE)
print("received data:", data)

# send data TCP
s.send(b'hello\r\n')
data = ser.readline()
print("received data:", data)

# init Thread
t1 = MyThread()
t1.start()
# number of times the thread has run
t1.counter

# stop thread
t1.stop()

# close Connection
ser.close()
s.close()
