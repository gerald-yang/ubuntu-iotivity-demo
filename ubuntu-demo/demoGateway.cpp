//******************************************************************
// 
// Demo project for Ubuntu Core
//
// Author: Gerald Yang
//
//******************************************************************

#include <string>
#include <thread>
#include <condition_variable>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <ifaddrs.h>

#include "OCPlatform.h"
#include "OCApi.h"

#include "influxDB.h"
#include "rpiSensorNode.h"
#include "rpiLedNode.h"
#include "lcdNode.h"
#include "ultrasonicNode.h"
#include "buttonNode.h"
#include "rpiSensorResource.h"
#include "rpiLedResource.h"
#include "lcdResource.h"
#include "buttonResource.h"

using namespace std;
using namespace OC;

int debug_mode;

// Client
RpiSensorNode rpiSensor("RPI2 sensors", "grovepi.sensor");
RpiLedNode rpiLed("RPI2 LEDs", "grovepi.led");
LcdNode lcd("RPI2 LCD", "grovepi.lcd");
UltrasonicNode ultrasonic("RPI2 ultrasonic sensor", "grovepi.ultrasonic");
ButtonNode button("RPI2 button", "grovepi.button");

// server
RpiSensorResource gwSensor("", "Gateway sensors", "/gateway/sensorp", "gateway.sensorp", false);
RpiLedResource gwLed("", "Gateway LEDs", "/gateway/ledp", "gateway.ledp", false);
LcdResource gwLcd("", "Gateway LCD", "/gateway/lcdp", "gateway.lcdp", false, 2, 16);
ButtonResource gwButton("", "Gateway button", "/gateway/buttonp", "gateway.buttonp", false);

#if 0
static void printMenu()
{
	cout << "Demo gateway debug menu" << endl;
	cout << "0  : Print this menu" << endl;
	cout << "1  : Read sensors (RPi2)" << endl;
	cout << "2  : Control LEDs (RPi2)" << endl;
	cout << "3  : Write string to LCD (RPi2)" << endl;
	cout << "4  : Write buzzer (RPi2)" << endl;
	cout << "5  : Button control (RPi2)" << endl;
	cout << "6  : Read ultrasonic (RPi2)" << endl;
	cout << "7  : Read sensors (Arduino)" << endl;
	cout << "8  : Write LED (Arduino)" << endl;
	cout << "9  : Write string to LCD (Arduino)" << endl;
	cout << "10 : Write buzzer (Arduino)" << endl;
	cout << "11 : Button control (Arduino)" << endl;
}

static void print_menu_led_p()
{
	cout << "1 : Turn on red LED" << endl;
	cout << "2 : Turn on green LED" << endl;
	cout << "3 : Turn on blue LED" << endl;
	cout << "4 : Turn off red LED" << endl;
	cout << "5 : Turn off green LED" << endl;
	cout << "6 : Turn off blue LED" << endl;
	cout << "7 : Read LED status" << endl;
}

static void print_menu_led_a()
{
	cout << "Enter LED value" << endl;
}

static void print_menu_lcd()
{
	cout << "Enter string" << endl;
}

static void print_menu_buzzer_p()
{
	cout << "Enter how long to beep" << endl;
}

static void print_menu_buzzer_a()
{
	cout << "Enter tone to beep" << endl;
}

static void print_menu_button_p()
{
	cout << "1 : Register button status" << endl;
	cout << "2 : De-register button status" << endl;
	cout << "3 : Read button status" << endl;
}

static void print_menu_button_a()
{
	cout << "1 : Register button status" << endl;
	cout << "2 : De-register button status" << endl;
	cout << "3 : Read button status" << endl;
}

void *find_all_resource(void *)
{
	// Raspberry Pi 2 server
	string sensor_p_rt = "?rt=grovepi.sensor";
	string led_p_rt = "?rt=grovepi.led";
	string lcd_p_rt = "?rt=grovepi.lcd";
	string buzzer_p_rt = "?rt=grovepi.buzzer";
	string button_p_rt = "?rt=grovepi.button";
	string ultrasonic_p_rt = "?rt=grovepi.ultrasonic";

	// Arduino server
	string sensor_a_rt = "?rt=grove.sensor";
	string led_a_rt = "?rt=grove.led";
	string lcd_a_rt = "?rt=grove.lcd";
	string buzzer_a_rt = "?rt=grove.buzzer";
	string button_a_rt = "?rt=grove.button";

	return NULL;
}

string trim(string& str)
{
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');

	if(first == string::npos)
		return string();
	else
		return str.substr(first, (last-first+1));
}

void *socket_server_for_restful_api(void *)
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serveraddr; 
	char buffer[65];
	string data, name, value;
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
				cout << "error: read failed" << endl;
				break;
			} else if(data_len == 0) {
				cout << "connection closed" << endl;
				break;
			}

			data.assign(buffer);
			cout << "Read: " << data << ":" << endl;

			name = data.substr(0, 32);
			name = trim(name);
			cout << "Name: " << name << ":" << endl;

			value = data.substr(32, 32);
			value = trim(value);
			if(value.empty()) {
				cout << "read status" << endl;
			} else {
				cout << "write status: " << value << ":" << endl;
			}

			ostringstream value_str;
			memset(buffer, 0, sizeof(buffer));
			if(name == "sensor_p_temp") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_temp;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write temperature value failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to temperature sensor" << endl;
				}
			} else if(name == "sensor_p_humidity") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_humidity;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write humidity value failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to humidity sensor" << endl;
				}
			} else if(name == "sensor_p_light") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_light;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write light value failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to light sensor" << endl;
				}
			} else if(name == "sensor_p_sound") {
				if(value.empty()) {
					value_str << mydemo.sensor_p_sound;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write sound value failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to sound sensor" << endl;
				}
			} else if(name == "led_p_red") {
				if(value.empty()) {
					value_str << mydemo.led_p_red;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write red LED failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					mydemo.led_p_red = stoi(value);
					putLedRepresentationP(ledResourceP);
				}
			} else if(name == "led_p_green") {
				if(value.empty()) {
					value_str << mydemo.led_p_green;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write green LED failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					mydemo.led_p_green = stoi(value);
					putLedRepresentationP(ledResourceP);
				}
			} else if(name == "led_p_blue") {
				if(value.empty()) {
					value_str << mydemo.led_p_blue;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write blue LED failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					mydemo.led_p_blue = stoi(value);
					putLedRepresentationP(ledResourceP);
				}
			} else if(name == "lcd_p") {
				if(value.empty()) {
					strncpy(buffer, mydemo.lcd_p_str.c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write LCD failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					mydemo.lcd_p_str = value;
					putLcdRepresentationP(lcdResourceP);
				}
			} else if(name == "buzzer_p") {
				if(value.empty()) {
					cout << "error: do not read buzzer" << endl;
				} else {
					mydemo.buzzer_p = stoi(value);
					putBuzzerRepresentationP(buzzerResourceP);
				}
			} else if(name == "button_p") {
				if(value.empty()) {
					value_str << mydemo.button_p;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write button status failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to button" << endl;
				}
			} else if(name == "sensor_p_ultrasonic") {
				if(value.empty()) {
					value_str << mydemo.ultrasonic_p;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write ultrasonic value failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to ultrasonic" << endl;
				}
			} else if(name == "led_a") {
				if(value.empty()) {
					value_str << mydemo.led_a_status;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write LED failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					mydemo.led_a_status = stoi(value);
					putLedRepresentationA(ledResourceA);
				}
			} else if(name == "lcd_a") {
				if(value.empty()) {
					strncpy(buffer, mydemo.lcd_a_str.c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write LCD failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					mydemo.lcd_a_str = value;
					putLcdRepresentationA(lcdResourceA);
				}
			} else if(name == "buzzer_a") {
				if(value.empty()) {
					cout << "error: do not read buzzer" << endl;
				} else {
					mydemo.buzzer_a = stoi(value);
					putBuzzerRepresentationA(buzzerResourceA);
				}
			} else if(name == "button_a") {
				if(value.empty()) {
					value_str << mydemo.button_a;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write button status failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to button" << endl;
				}
			} else if(name == "touch_a") {
				if(value.empty()) {
					value_str << mydemo.touch_a;
					strncpy(buffer, value_str.str().c_str(), sizeof(buffer));
					data_len = write(connfd, buffer, strlen(buffer));
					if (data_len < 0) {
						cout << "error: write touch status failed" << endl;
						break;
					} else if (data_len == 0) {
						cout << "connection closed" << endl;
					}
				} else {
					cout << "error: do not write value to touch" << endl;
				}
			} else {
				cout << "Unknown name: " << name << endl;
			}

		}
		close(connfd);
	}

	return NULL;
}
#endif
	
string wait_for_network_ip(string netif)
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

			if(!strcmp(ifa->ifa_name, "lo")) {
				cout << "skip lo" << endl;
				continue;
			}

			// if netif is "*", check the first interface we have except lo
			if(netif == "-" && (ifa->ifa_addr->sa_family == AF_INET)) {
				if (s != 0) {
					cout << "getnameinfo() failed: " << gai_strerror(s) << endl;
				} else {
					host_ip.clear();
					host_ip = host;
					freeifaddrs(ifaddr);
					return host_ip;
				}
			}

			if((strncmp(ifa->ifa_name, netif.c_str(), netif.size()) == 0) && (ifa->ifa_addr->sa_family == AF_INET)) {
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

bool wait_for_network_ping()
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "www.google.com");
		while ((res = curl_easy_perform(curl)) != CURLE_OK) {
			switch(res) {
				case CURLE_COULDNT_CONNECT:
				case CURLE_COULDNT_RESOLVE_HOST:
				case CURLE_COULDNT_RESOLVE_PROXY:
					break;
				default:
					cout << "request failed: " << curl_easy_strerror(res) << endl;
					curl_easy_cleanup(curl);
					return false;
			}
		}

		/* always cleanup */ 
		curl_easy_cleanup(curl);
		return true;
	}

	cout << "can not initialize curl library" << endl;
	return false;
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

void rule0_loop()
{
	// Get sensor data if sensors are found
	if(rpiSensor.found()) {
		rpiSensor.get(false);
		gwSensor.temperature = rpiSensor.temperature;
		gwSensor.humidity = rpiSensor.humidity;
		gwSensor.light = rpiSensor.light;
		gwSensor.sound = rpiSensor.sound;
	}

	if(ultrasonic.found()) {
		ultrasonic.get(false);
	}

	if(rpiLed.found()) {
		rpiLed.get(false);
		gwLed.red = rpiLed.red;
		gwLed.green = rpiLed.green;
		gwLed.blue = rpiLed.blue;
	}
}

bool rule1_condition()
{
	if(rpiSensor.found() && rpiLed.found())
		return true;
	else
		return false;
}

bool rule1_init_done = false;
void rule1_init()
{
	if(rpiSensor.temperature >= 25) {
		rpiLed.red = 1;
		rpiLed.green = 0;
	} else {
		rpiLed.red = 0;
		rpiLed.green = 1;
	}

	rpiLed.put(false);

	rule1_init_done = true;
}

void rule1_loop()
{
	if(rpiSensor.temperature >= 25 && (rpiLed.green == 1 || rpiLed.red == 0)) {
		rpiLed.red = 1;
		rpiLed.green = 0;
		rpiLed.put(false);
	} else if(rpiSensor.temperature < 25 && (rpiLed.green == 0 || rpiLed.red == 1)) {
		rpiLed.red = 0;
		rpiLed.green = 1;
		rpiLed.put(false);
	}
}

bool rule2_condition()
{
	if(rpiSensor.found() && rpiLed.found())
		return true;
	else
		return false;
}

bool rule2_init_done = false;
void rule2_init()
{
	if(rpiSensor.light >= 500)
		rpiLed.blue = 0;
	else
		rpiLed.blue = 1;

	rpiLed.put(false);

	rule2_init_done = true;
}

void rule2_loop()
{
	if(rpiSensor.light >= 500 && rpiLed.blue == 1) {
		rpiLed.blue = 0;
		rpiLed.put(false);
	} else if(rpiSensor.light < 500 && rpiLed.blue == 0) {
		rpiLed.blue = 1;
		rpiLed.put(false);
	}
}

bool rule3_condition()
{
	return rpiSensor.found();
}

void rule3(InfluxDB& db)
{
	db.writeDB("fukuoka", "temperature", rpiSensor.temperature);
	db.writeDB("fukuoka", "humidity", rpiSensor.humidity);
	db.writeDB("fukuoka", "light", rpiSensor.light);
	db.writeDB("fukuoka", "sound", rpiSensor.sound);
}

bool rule4_condition()
{
	if(lcd.found())
		return true;
	else
		return false;
}

bool rule4_init_done = false;
void rule4_init()
{
	lcd.put(false);
	rule4_init_done = true;
}

void rule5_loop()
{
	if(button.found() && !button.isObserved()) {
		button.observe(false, true);
	}
}

int rule6_button = 0;
void rule6_loop()
{
	if(rule4_init_done && button.found()) {
		if(rule6_button != button.button) {
			rule6_button = button.button;
			lcd.lcd.resize(16, ' ');
			lcd.lcd += "button: ";
			lcd.lcd += to_string(button.button);
			lcd.put(false);
		}
	}
}




void startServer()
{
	while(!gwSensor.isCreated() || !gwLed.isCreated() || !gwLcd.isCreated() || !gwButton.isCreated()) {
		if(rpiSensor.found() && !gwSensor.isCreated()) {
			gwSensor.createResource();
		}

		if(rpiLed.found() && !gwLed.isCreated()) {
			gwLed.createResource();
		}

		if(rpiLed.found() && !gwLcd.isCreated()) {
			gwLcd.createResource();
		}

		if(button.found() && !gwButton.isCreated()) {
			//gwButton.createResource();
		}
	}
}

int main(int argc, char* argv[])
{
	OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink};
	string host_ip;
	string netif = argv[1];

	if(argc != 4) {
		printUsage();
		return -1;
	}

	// Wait for specific network interface up
	cout << "Waiting for network interface: " << argv[1] << endl;
	if(wait_for_network_ping()) {
		host_ip = wait_for_network_ip(netif);
		if(host_ip == "IP error") {
			cout << "Network interface " << argv[1] << " is not available" << endl;
			return -1;
		}
	} else {
		cout << "network is unavailable" << endl;
		return -1;
	}
	sleep(2);
	cout << "network is available, IP: " << host_ip << endl;

	// Configure InfluxDB class
	cout << "InfluxDB IP: " << argv[2] << endl;
	InfluxDB db(argv[2]);

	// Display host IPv4 address on LCD panel
	cout << "IP: " << host_ip << endl;

	// Enable deubg menu
	if(argv[3][0] == '1')
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


	cout << "Starting client ... ";

	rpiSensor.startFindResource();
	rpiLed.startFindResource();
	lcd.startFindResource();
	ultrasonic.startFindResource();
	button.startFindResource();

	lcd.lcd = host_ip;
	lcd.lcd.resize(16, ' ');
	lcd.lcd += "button: ";
	lcd.lcd += to_string(rule6_button);

	rule6_button = button.button;

	cout << "done" << endl;

	cout << "Starting server ... ";
	thread serverThread(startServer);
	cout << "done" << endl;

	// Main loop
	if(debug_mode) {
		cout << "Enter debug mode" << endl;

		while(true) {
			if(rpiSensor.found()) {
				rpiSensor.get(true);
				ultrasonic.get(true);
				sleep(1);
			}
		}
	} else {
		cout << "Run all rules" << endl;

		while(true) {
			// All rules
			rule0_loop();

			if(rule1_condition()) {
				if(rule1_init_done) {
					rule1_loop();
				} else {
					rule1_init();
				}
			}

			if(rule2_condition()) {
				if(rule2_init_done) {
					rule2_loop();
				} else {
					rule2_init();
				}
			}

			if(rule3_condition()) {
				rule3(db);
			}

			if(rule4_condition()) {
				if(!rule4_init_done) {
					rule4_init();
				}
			}

			rule5_loop();

			rule6_loop();

			sleep(1);
		}
	}

	return 0;
}
