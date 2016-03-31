import socket
import math
import decimal
import time
from threading import Thread
from pygrovepi import grovepi
from pygrovepi import grovepilib

write_list = []
temperature = 0.0
humidity = 0.0
light = 0
sound = 0
led_red = 0
led_green = 0
led_blue = 0
lcd = "unknown"
button = 0
buzzer = 0
ultrasonic = 0

def sensorThread():
    global write_list

    global temperature
    global humidity
    global light
    global sound
    global led_red
    global led_green
    global led_blue
    global lcd
    global button
    global buzzer
    global ultrasonic

    temperature = grovepilib.sensor_read_temp()
    humidity = grovepilib.sensor_read_humidity()
    light = grovepilib.sensor_read_light()
    sound = grovepilib.sensor_read_sound()
    ultrasonic = grovepilib.sensor_read_ultrasonic()
    button = grovepilib.button_read()


if __name__ == '__main__':
    grovepilib.pin_config()

    sensor_daemon = Thread(target=sensorThread, args=())
    sensor_daemon.daemon = True
    sensor_daemon.start()

    


    print 'Create socket'
    serversocket = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)
    print 'Bind to localhost port 5566'
    serversocket.bind(('localhost', 5566))
    serversocket.listen(5)

    print 'Start listening'

    while True:
        conn, addr = serversocket.accept()
        print 'Accepted connection from'
        print addr

        while True:
            data = conn.recv(64)
            if not data:
                print 'Connection closed'
                break;
            else:
                #print 'Recv: ' + data
                name = data[:32].lstrip().rstrip()
                unstrip_value = data[32:].strip('\0')
                value = unstrip_value.lstrip().rstrip()
                #print name
                #print unstrip_value
                #print value

                if not name:
                    print 'No data available'
                    break;
                else:
                    if name == 'sensor_temp':
                        d = grovepilib.sensor_read_temp()
                        print 'temperature: ' + str(d)
                        conn.send(str(d))
                    elif name == 'sensor_humidity':
                        d = grovepilib.sensor_read_humidity()
                        print 'humidity: ' + str(d)
                        conn.send(str(d))
                    elif name == 'sensor_light':
                        d = grovepilib.sensor_read_light()
                        print 'light: ' + str(d)
                        conn.send(str(d))
                    elif name == 'sensor_sound':
                        d = grovepilib.sensor_read_sound()
                        print 'sound: ' + str(d)
                        conn.send(str(d))
                    elif name == 'led_write_red':
                        if not value:
                            print 'wrong message format: ' + data
                        else:
                            print 'led red: ' + value
                            print len(value)
                            d = grovepilib.led_write_red(int(value))
                    elif name == 'led_write_green':
                        if not value:
                            print 'wrong message format: ' + data
                        else:
                            print 'led green: ' + value
                            d = grovepilib.led_write_green(int(value))
                    elif name == 'led_write_blue':
                        if not value:
                            print 'wrong message format: ' + data
                        else:
                            print 'led blue: ' + value
                            grovepilib.led_write_blue(int(value))
                    elif name == 'lcd_write':
                        if not value:
                            print 'wrong message format: ' + data
                        else:
                            print 'lcd string: ' + unstrip_value
                            grovepilib.lcd_write_str(unstrip_value)
                    elif name == 'buzzer_write':
                        if not value:
                            print 'wrong message format: ' + data
                        else:
                            print 'buzzer: ' + value
                            grovepilib.buzzer_write(float(value))
                    elif name == 'button_read':
                        d = grovepilib.button_read()
                        print 'button: ' + str(d)
                        conn.send(str(d))
                    elif name == 'ultrasonic_read':
                        d = grovepilib.ultrasonic_read()
                        print 'ultrasonic: ' + str(d)
                        conn.send(str(d))


    serversocket.close()
    print 'Server down'
