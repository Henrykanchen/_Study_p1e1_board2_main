/*
  Autonomous Vehicles and Pedestrian Interaction with Visual Signals Study - Phase 1

  Experiment 1. Empirical Assessment of light signals for peripheral perception

  This is the main processing code for the experiment. It runs on its own Ardiuno Uno.

  It is responsible for:
  1) Communicate with HC-05 bluetooth module to receive participant input via custom android app;
  2) Transmite command to control LEDs;
  3) Communicate with Arduino ROS node to receive vehicle location;
  4) Logging experiment data

  The circuit:
  1)  
  HC-06 Bluetooth module for 
    - RXD pin to pin 10 (serial Tx) through a voltage 1 Kohms / 2 Kohms voltage divider
    - TXD pin to pin 10 (serial Rx)
    - GND pin to ground
    - VCC pin to 5V
  2)
  LEDs control commands are sent to LEDs control board (#1) via serial TX pin 12 
  The GND pins on both boards are common
  
  The following commands are accepted:
  0 -> Turn off the leds
  1 - 8 -> Select test mode 
  ':' ';" -> Vehicle detection 

  3) 
  Request for ROS data are sent to ROS board (#3) via serial TX pin 7 
  ROS data are received from ROS board (#3) via default serial RX pin 0
  The GND pins on both boards are common

  
  Programmer: 
    Henry Chen (Uwaterloo)
    May 2019

  
  This example code is based on 
  https://howtomechatronics.com/tutorials/arduino/arduino-and-hc-05-bluetooth-module-tutorial/
  https://www.teachmemicro.com/arduino-bluetooth/
  https://arduino.stackexchange.com/questions/49682/sending-float-values-from-python-to-arduino-via-serial-communication
  
  */

/////#include <ros.h>
/////#include <std_msgs/String.h>
#include <SoftwareSerial.h>
SoftwareSerial Serial_Bluetooth(9, 10); // RX, TX
SoftwareSerial Serial_LED(11, 12); // RX, TX
SoftwareSerial Serial_ROS(6, 7); // RX, TX

/*
ros::NodeHandle  nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";
*/

int bluetooth_ctrl_data; // bluetooth control data


void setup()
{
  Serial_LED.begin(9600);         // Serial communication line for led control
  Serial_ROS.begin(9600);         // Serial communication line for ROS node
  Serial_Bluetooth.begin(9600);   // Serial communication line for Bluetooth APP control

  Serial.begin(9600);              // Serial Monitor for debugging
  Serial.println("Experiment Ready...");

  /*
  // Note: Serial communication must be initiated before ROS node initization
  nh.initNode();
  nh.advertise(chatter);
  */
  
}

void loop()
{ 
  // Function 1: Communicate with HC-05 bluetooth module to receive participant input via custom android app;
  // receiving bluetooth control data
  if (Serial_Bluetooth.available()){ 
    
    bluetooth_ctrl_data=Serial_Bluetooth.read();
    Serial.print("Bluetooth Received: ");
    Serial.println(bluetooth_ctrl_data);

    // Receive participant input
    if(bluetooth_ctrl_data == 61){  // ASCII '='
      Serial.println("Signal Detected!");
      //Serial_Bluetooth.print("Signal Detected!");./
    }
    
    if(bluetooth_ctrl_data == 62){  // ASCII '>'
      Serial.println("Signal Understood!!");
      //Serial_Bluetooth.print("Signal Understood!!");
    }   

    // Select test mode
    if(bluetooth_ctrl_data == 48){    //ASCII '0'
      //Serial.println("Baseline (OFF)");
      //Serial_Bluetooth.print("Baseline (OFF)");
    }    
    else if(bluetooth_ctrl_data == 49){  //ASCII '1'
      //Serial.println("Test 2a - Top Turquoise");
      //Serial_Bluetooth.print("Test 2a - Top Turquoise");
    }
    else if(bluetooth_ctrl_data == 50){  //ASCII '2'
      //Serial.println("Test 2b - Top Amber");
      //Serial_Bluetooth.print("Test 2b - Top Amber");
    }
    else if(bluetooth_ctrl_data == 51){  //ASCII '3'
      //Serial.println("Test 3a - Front Turquoise");
      //Serial_Bluetooth.print("Test 3a - Front Turquoise"); 
    }
    else if(bluetooth_ctrl_data == 52){  //ASCII '4'
      //Serial.println("Test 3b - Front Amber");
      //Serial_Bluetooth.print("Test 3b - Front Amber"); 
    }
    else if(bluetooth_ctrl_data == 53){  //ASCII '5'
      //Serial.println("Test 4a - Top Blinking Turquoise");
      //Serial_Bluetooth.print("Test 4a - Top Blinking Turquoise");
    }
    else if(bluetooth_ctrl_data == 54){  //ASCII '6'
      //Serial.println("Test 4b - Top Blinking Amber");
      //Serial_Bluetooth.print("Test 4b - Top Blinking Amber");
    }
    else if(bluetooth_ctrl_data == 55){  //ASCII '7'
      //Serial.println("Test 5a - Front Blinking Turquoise");
      //Serial_Bluetooth.print("Test 5a - Front Blinking Turquoise");
    }
    else if(bluetooth_ctrl_data == 56){  //ASCII '8'
      //Serial.println("Test 5b - Front Blinking Amber");
      //Serial_Bluetooth.print("Test 5b - Front Blinking Amber"); 
    }
    else if(bluetooth_ctrl_data == 57){  //ASCII '9'  
      //Serial.println("Test 6a - Top Chase Turquoise");
      //Serial_Bluetooth.print("Test 6a - Top Chase Turquoise");
    }
    else if(bluetooth_ctrl_data == 58){  // ASCII ':'
      //Serial.println("Test 6b - Top Chase Amber");
      //Serial_Bluetooth.print("Test 6b - Top Chase Amber");
    }
    else if(bluetooth_ctrl_data == 59){  // ASCII ';'
      //Serial.println("Test 7a - Front Chase Turquoise");
      //Serial_Bluetooth.print("Test 7a - Front Chase Turquoise");
    }
    else if(bluetooth_ctrl_data == 60){  // ASCII '<'
      //Serial.println("Test 7b - Front Chase Amber");
      //Serial_Bluetooth.print("Test 7b - Front Chase Amber");
    }          
    else{;}

  }

  // Function 2: Transmite command to control LEDs;
  // transmitting test mode to LED control a
  if (bluetooth_ctrl_data >=48 and bluetooth_ctrl_data <=60){
    Serial_LED.write(bluetooth_ctrl_data);
    //Serial.print("sending: ");
    //Serial.println(bluetooth_ctrl_data);
    
    bluetooth_ctrl_data = 0;
    delay(250);
  }

  // Function 3: Communicate with Arduino ROS node to receive vehicle location;

  if (bluetooth_ctrl_data >=61 and bluetooth_ctrl_data <=62){
    Serial_ROS.write(bluetooth_ctrl_data);
    
    bluetooth_ctrl_data = 0;
    delay(250);
  }

  static char buffer[32];
  static size_t pos;

  // receiving ROS node data
  if (Serial.available()){ 
    //Serial.println("ROS data incoming...");
    char c = Serial.read();
     if (c == '\n') {                         // on end of line, parse the number
            buffer[pos] = '\0';
            float value = atof(buffer);
            Serial.print("Vehicle Location: ");
            Serial.println(value);
            pos = 0;
        } else if (pos < sizeof buffer - 1) {  // otherwise, buffer it
            buffer[pos++] = c;
        }
  }
  

/*
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
*/

}
