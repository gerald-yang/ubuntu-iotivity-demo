import socket
import math
import decimal
import time
from threading import Thread
from pygrovepi import grovepi
from pygrovepi import grovepilib

write_list = []
read_list = []

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

def t_read_temp():
	global temperature
	temperature = grovepilib.sensor_read_temp()
	#print temperature

def t_read_humidity():
	global humidity
	humidity = grovepilib.sensor_read_humidity()
	#print humidity

def t_read_light():
	global light
	light = grovepilib.sensor_read_light()
	#print light

def t_read_sound():
	global sound
	sound = grovepilib.sensor_read_sound()
	#print sound
	
def t_read_button():
	global button
	button = grovepilib.button_read()
	#print button

def t_read_ultrasonic():
	global ultrasonic
	ultrasonic = grovepilib.sensor_read_ultrasonic()
	#print ultrasonic


def t_write_led_red():
	global led_red
	d = grovepilib.led_write_red(led_red)
	#print 'write led_red: ' + str(led_red)
	
def t_write_led_green():
	global led_green
	d = grovepilib.led_write_green(led_green)
	#print 'write led_green: ' + str(led_green)
	
def t_write_led_blue():
	global led_blue
	d = grovepilib.led_write_blue(led_blue)
	#print 'write led_blue: ' + str(led_blue)
	
def t_write_lcd():
	global lcd
	grovepilib.lcd_write_str(lcd)
	#print 'write lcd: ' + lcd

def t_write_buzzer():
	global buzzer
	grovepilib.buzzer_write(buzzer)
	#print 'write buzzer: ' + str(buzzer)


def sensorThread():
	global read_list
	global write_list

	while True:
		read_job = len(read_list)
		if read_job > 0:
			for rjob in range(0, read_job):
				write_job = len(write_list)
				if write_job > 0:
					write_list[0]()
					del write_list[0]
				read_list[rjob]()


def clientThread(conn):
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
					#print 'temperature: ' + str(temperature)
					conn.send(str(temperature))
				elif name == 'sensor_humidity':
					#print 'humidity: ' + str(humidity)
					conn.send(str(humidity))
				elif name == 'sensor_light':
					#print 'light: ' + str(light)
					conn.send(str(light))
				elif name == 'sensor_sound':
					#print 'sound: ' + str(sound)
					conn.send(str(sound))
				elif name == 'led_write_red':
					if not value:
						print 'wrong message format: ' + data
					else:
						#print 'led red: ' + value
						led_red = int(value)
						write_list.append(t_write_led_red)
				elif name == 'led_write_green':
					if not value:
						print 'wrong message format: ' + data
					else:
						#print 'led green: ' + value
						led_green = int(value)
						write_list.append(t_write_led_green)
				elif name == 'led_write_blue':
					if not value:
						print 'wrong message format: ' + data
					else:
						#print 'led blue: ' + value
						led_blue = int(value)
						write_list.append(t_write_led_blue)
				elif name == 'lcd_write':
					if not value:
						print 'wrong message format: ' + data
					else:
						#print 'lcd string: ' + unstrip_value
						lcd = unstrip_value
						write_list.append(t_write_lcd)
				elif name == 'buzzer_write':
					if not value:
						print 'wrong message format: ' + data
					else:
						#print 'buzzer: ' + value
						buzzer = float(value)
						write_list.append(t_write_buzzer)
				elif name == 'button_read':
					#print 'button: ' + str(button)
					conn.send(str(button))
				elif name == 'ultrasonic_read':
					#print 'ultrasonic: ' + str(ultrasonic)
					conn.send(str(ultrasonic))



if __name__ == '__main__':
	grovepilib.pin_config()

	read_list.append(t_read_temp)
	read_list.append(t_read_humidity)
	read_list.append(t_read_light)
	read_list.append(t_read_sound)
	read_list.append(t_read_button)
	read_list.append(t_read_ultrasonic)

	print 'read jobs: ' + str(len(read_list))

	sensor_daemon = Thread(target=sensorThread, args=())
	sensor_daemon.daemon = True
	sensor_daemon.start()

    


	print 'Create socket'
	serversocket = socket.socket(
		socket.AF_INET, socket.SOCK_STREAM)
	print 'Bind to localhost port 5566'
	serversocket.bind(('localhost', 5566))
	serversocket.listen(64)

	print 'Start listening'

	while True:
		conn, addr = serversocket.accept()
		print 'Accepted connection from'
		print addr

		print 'Create a thread to handle this connection'
		client_thread = Thread(target=clientThread, args=(conn,))
		client_thread.daemon = True
		client_thread.start()


	serversocket.close()
	print 'Server shutdown'
