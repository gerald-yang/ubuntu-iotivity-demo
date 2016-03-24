//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// OCClient.cpp : Defines the entry point for the console application.
//
#include <string>
#include <map>
#include <cstdlib>
#include <mutex>
#include <list>
#include <condition_variable>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>

#include "OCPlatform.h"
#include "OCApi.h"

#include "influxDB.h"
#include "rpiSensorNode.h"
#include "ledNode.h"
#include "lcdNode.h"

using namespace std;
using namespace OC;

int debug_mode;

static void print_menu()
{
	std::cout << "Demo gateway debug menu" << std::endl;
	std::cout << "0  : Print this menu" << std::endl;
	std::cout << "1  : Read sensors" << std::endl;
	std::cout << "2  : Control LEDs" << std::endl;
	std::cout << "3  : Write string to LCD" << std::endl;
	std::cout << "4  : Write buzzer" << std::endl;
	std::cout << "5  : Button control" << std::endl;
	std::cout << "6  : Read ultrasonic" << std::endl;
	std::cout << "7  : Read sensors (Arduino)" << std::endl;
	std::cout << "8  : Write LED (Arduino)" << std::endl;
	std::cout << "9  : Write string to LCD (Arduino)" << std::endl;
	std::cout << "10  : Write buzzer (Arduino)" << std::endl;
	std::cout << "111 : Button control (Arduino)" << std::endl;
}

static void print_menu_led_p()
{
	std::cout << "1 : Turn on red LED" << std::endl;
	std::cout << "2 : Turn on green LED" << std::endl;
	std::cout << "3 : Turn on blue LED" << std::endl;
	std::cout << "4 : Turn off red LED" << std::endl;
	std::cout << "5 : Turn off green LED" << std::endl;
	std::cout << "6 : Turn off blue LED" << std::endl;
	std::cout << "7 : Read LED status" << std::endl;
}

static void print_menu_led_a()
{
	std::cout << "Enter LED value" << std::endl;
}

static void print_menu_lcd()
{
	std::cout << "Enter string" << std::endl;
}

static void print_menu_buzzer_p()
{
	std::cout << "Enter how long to beep" << std::endl;
}

static void print_menu_buzzer_a()
{
	std::cout << "Enter tone to beep" << std::endl;
}

static void print_menu_button_p()
{
	std::cout << "1 : Register button status" << std::endl;
	std::cout << "2 : De-register button status" << std::endl;
	std::cout << "3 : Read button status" << std::endl;
}

static void print_menu_button_a()
{
	std::cout << "1 : Register button status" << std::endl;
	std::cout << "2 : De-register button status" << std::endl;
	std::cout << "3 : Read button status" << std::endl;
}

void *find_all_resource(void *)
{
	// Raspberry Pi 2 server
	std::string sensor_p_rt = "?rt=grovepi.sensor";
	std::string led_p_rt = "?rt=grovepi.led";
	std::string lcd_p_rt = "?rt=grovepi.lcd";
	std::string buzzer_p_rt = "?rt=grovepi.buzzer";
	std::string button_p_rt = "?rt=grovepi.button";
	std::string ultrasonic_p_rt = "?rt=grovepi.ultrasonic";

	// Arduino server
	std::string sensor_a_rt = "?rt=grove.sensor";
	std::string led_a_rt = "?rt=grove.led";
	std::string lcd_a_rt = "?rt=grove.lcd";
	std::string buzzer_a_rt = "?rt=grove.buzzer";
	std::string button_a_rt = "?rt=grove.button";

	return NULL;
}

#if 0
std::string trim(std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');

	if(first == std::string::npos)
		return std::string();
	else
		return str.substr(first, (last-first+1));
}

void *socket_server_for_restful_api(void *)
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serveraddr; 
	char buffer[65];
	std::string data, name, value;
	int data_len;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serveraddr, '0', sizeof(serveraddr));   

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(6655); 

	bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)); 

	while(true) {
		listen(listenfd, 10); 

		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		
		while(true) {		
			memset(buffer, 0, sizeof(buffer)); 

			data_len = read(connfd, buffer, sizeof(buffer));

			if(data_len < 0) {
				std::cout << "error: read failed" << std::endl;
				break;
			} else if(data_len == 0) {
				std::cout << "connection closed" << std::endl;
				break;
			}

			data.assign(buffer);
			std::cout << "Read: " << data << ":" << std::endl;

			name = data.substr(0, 32);
			name = trim(name);
			std::cout << "Name: " << name << ":" << std::endl;

			value = data.substr(32, 32);
			value = trim(value);
			if(value.empty()) {
				std::cout << "read status" << std::endl;
			} else {
				std::cout << "write status: " << value << ":" << std::endl;
			}

			std::ostringstream value_str;
			memset(buffer, 0, sizeof(buffer));
			if(name == "sensor_p_temp") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_temp;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write temperature value failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to temperature sensor" << std::endl;
				}
			} else if(name == "sensor_p_humidity") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_humidity;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write humidity value failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to humidity sensor" << std::endl;
				}
			} else if(name == "sensor_p_light") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_light;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write light value failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to light sensor" << std::endl;
				}
			} else if(name == "sensor_p_sound") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_sound;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write sound value failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to sound sensor" << std::endl;
				}
			} else if(name == "led_p_red") {
				if(value.empty()) {
					value_str << mydemo.led_p_red;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write red LED failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					mydemo.led_p_red = std::stoi(value);
					putLedRepresentationP(ledResourceP);
				}
			} else if(name == "led_p_green") {
				if(value.empty()) {
					value_str << mydemo.led_p_green;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write green LED failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					mydemo.led_p_green = std::stoi(value);
					putLedRepresentationP(ledResourceP);
				}
			} else if(name == "led_p_blue") {
				if(value.empty()) {
					value_str << mydemo.led_p_blue;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write blue LED failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					mydemo.led_p_blue = std::stoi(value);
					putLedRepresentationP(ledResourceP);
				}
			} else if(name == "lcd_p") {
				if(value.empty()) {
					strncpy(buffer, mydemo.lcd_p_str.c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write LCD failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					mydemo.lcd_p_str = value;
					putLcdRepresentationP(lcdResourceP);
				}
			} else if(name == "buzzer_p") {
				if(value.empty()) {
					std::cout << "error: do not read buzzer" << std::endl;
				} else {
					mydemo.buzzer_p = std::stoi(value);
					putBuzzerRepresentationP(buzzerResourceP);
				}
			} else if(name == "button_p") {
				if(value.empty()) {
					value_str << mydemo.button_p;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write button status failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to button" << std::endl;
				}
			} else if(name == "sensor_p_ultrasonic") {
				if(value.empty()) {
					value_str << mydemo.ultrasonic_p;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write ultrasonic value failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to ultrasonic" << std::endl;
				}
			} else if(name == "led_a") {
				if(value.empty()) {
					value_str << mydemo.led_a_status;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write LED failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					mydemo.led_a_status = std::stoi(value);
					putLedRepresentationA(ledResourceA);
				}
			} else if(name == "lcd_a") {
				if(value.empty()) {
					strncpy(buffer, mydemo.lcd_a_str.c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write LCD failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					mydemo.lcd_a_str = value;
					putLcdRepresentationA(lcdResourceA);
				}
			} else if(name == "buzzer_a") {
				if(value.empty()) {
					std::cout << "error: do not read buzzer" << std::endl;
				} else {
					mydemo.buzzer_a = std::stoi(value);
					putBuzzerRepresentationA(buzzerResourceA);
				}
			} else if(name == "button_a") {
				if(value.empty()) {
					value_str << mydemo.button_a;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write button status failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to button" << std::endl;
				}
			} else if(name == "touch_a") {
				if(value.empty()) {
					value_str << mydemo.touch_a;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						std::cout << "error: write touch status failed" << std::endl;
						break;
					} else if (data_len == 0) {
						std::cout << "connection closed" << std::endl;
					}
				} else {
					std::cout << "error: do not write value to touch" << std::endl;
				}
			} else {
				std::cout << "Unknown name: " << name << std::endl;
			}

		}
		close(connfd);
	}

	return NULL;
}
#endif
	
string wait_for_network(char *ifname)
{
	struct ifaddrs *ifaddr, *ifa;
	int s;
	char host[NI_MAXHOST];
	string host_ip = "IP error";

	while(1) {
		if (getifaddrs(&ifaddr) == -1) {
			cout << "getifaddrs" << endl;
			return host_ip;
		}

		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr == NULL)
				continue;  

			s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

			if((strcmp(ifa->ifa_name, ifname) == 0) && (ifa->ifa_addr->sa_family == AF_INET)) {
				if (s != 0) {
					cout << "getnameinfo() failed: " << gai_strerror(s) << endl;
				} else {
					host_ip.clear();
					host_ip = host;
					freeifaddrs(ifaddr);
					return host_ip;
				}
			}
		}

		freeifaddrs(ifaddr);
		sleep(1);
	}

	return host_ip;
}

void printUsage()
{
	cout << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "Usage :" << endl;
	cout << "demoGateway <network interface> <influxdb IP address>:<port> <debug mode>" << endl;
	cout << "-------------------------------------------------------------------------" << endl << endl;
}

static FILE* client_open(const char* /*path*/, const char *mode)
{
	return fopen("./oic_svr_db_client.json", mode);
}

void rule1()
{
	cout << "rule 1" << endl;
}

void rule2()
{
	cout << "rule 2" << endl;
}

int main(int argc, char* argv[])
{
	OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };
	string host_ip;
	string display_ip;

	cout << to_string(argc) << endl;

	if(argc != 5) {
		printUsage();
		return -(1);
	}

	// Wait for specific network interface up
	cout << "Waiting for network interface: " << argv[1] << endl;
	host_ip = wait_for_network(argv[1]);
	if(host_ip == "IP error") {
		cout << "Network interface " << argv[1] << " is not available" << endl;
		return 1;
	}
	cout << argv[1] << " is up" << endl;

	// Configure InfluxDB class
	cout << "InfluxDB IP: " << argv[2] << endl;
	InfluxDB db(argv[2]);

	// Display host IPv4 address on LCD panel
	display_ip = argv[3];

	// Enable deubg menu
	if(argv[4][0] == '1')
		debug_mode = 1;
	else
		debug_mode = 0;

	// Create PlatformConfig object
	cout << "Configuring ... ";
	PlatformConfig cfg {
		OC::ServiceType::InProc,
		OC::ModeType::Both,
		"0.0.0.0",
		0,
		OC::QualityOfService::LowQos,
		&ps
	};

	OCPlatform::Configure(cfg);
	cout << "done" << endl;


	std::cout << "Start client" << std::endl;
	RpiSensorNode rpiSensor("RPI2 sensors", "grovepi.sensor");
	LedNode led("RPI2 LEDs", "grovepi.led");
	LcdNode lcd("RPI2 LCD", "grovepi.lcd");


	rpiSensor.findResource();
	led.findResource();
	lcd.findResource();

	lcd.lcd = display_ip;
	lcd.put(true);

	led.red = 0;
	led.green = 0;
	led.blue = 0;
	led.put(true);


	list<void (*)()> ruleList;
	list<void (*)()>::iterator ruleListIter;

	ruleList.push_back(&rule1);
	ruleList.push_back(&rule2);

	while(true) {
		if(debug_mode) {
			cout << "read sensors" << endl;
			if(rpiSensor.found()) {
				rpiSensor.get(true);
				sleep(1);
			}
			//print_menu();
		} else {
			for(ruleListIter = ruleList.begin(); ruleListIter != ruleList.end(); ruleListIter++) {
				(*ruleListIter)();
			}
		}
	}

	return 0;
}
