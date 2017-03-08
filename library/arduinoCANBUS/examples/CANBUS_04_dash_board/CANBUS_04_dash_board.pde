/*  
 *  ------ CAN Bus Dash Board -------- 
 *  
 *  This sketch shows how to get the most important parameters from 
 *  a vehicle using the standard OBD-II PIDs codes. This codes are 
 *  used to request data from a vehicle, used as a diagnostic tool. 
 *  
 *  Copyright (C) 2014 Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify  
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation, either version 3 of the License, or  
 *  (at your option) any later version.  
 *   
 *  This program is distributed in the hope that it will be useful,  
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of  
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  
 *  GNU General Public License for more details.  
 *   
 *  You should have received a copy of the GNU General Public License  
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 *
 *  Version:          0.3
 *  Design:           David Gascon
 *  Implementation:   Ahmad Saad, Rubén Martín
 */

#include <Wire.h>

// Cooking API libraries
#include <arduinoUtils.h>

// Include always these libraries before using the CAN Bus functions
#include <arduinoCAN.h>
#include <SPI.h>

// Create an instance of the object
CAN myCAN = CAN();


void setup() {
  // Turn on the Serial 
  Serial.begin(115200);
  delay(100);

  // Print init message 
  Serial.println("Initializing CANBUS...");	

  // Configuring the BUS at 500 Kbit/s
  // Only allowed in SOCKET 0
  myCAN.begin(500);   

  Serial.println("CANBUS initialized at 500 KBits/s");  
  Serial.println();
}


void loop() {

  // Read the value of the Vehicle Speed 
  int vehicleSpeed = myCAN.getVehicleSpeed(); 

  // Read the value of RPM of the engine
  int engineRPM = myCAN.getEngineRPM(); 

  // Read the engine fuel rate
  int engineFuelRate = myCAN.getEngineFuelRate();

  // Get the fuel level
  int fuelLevel = myCAN.getFuelLevel();

  // Get the throttle position
  int throttlePosition = myCAN.getThrottlePosition();

  //Get the fuel pressure value 
  int fuelPressure = myCAN.getFuelPressure(); 


  Serial.println(F("<============================================>"));
  Serial.print(F("\tVehicle Speed =>  ")); 
  Serial.print(vehicleSpeed);
  Serial.println("  Km / h");

  Serial.print(F("\tEngine RPM =>  ")); 
  Serial.print(engineRPM);
  Serial.println("  RPM");

  Serial.print(F("\tEngine Fuel Rate =>  ")); 
  Serial.print(engineFuelRate);
  Serial.println("  L/h");

  Serial.print(F("\tFuel Level =>  ")); 
  Serial.print(fuelLevel);
  Serial.println("  %");

  Serial.print(F("\tThrottle Position =>  ")); 
  Serial.print(throttlePosition);
  Serial.println(" % ");

  Serial.print(F("\tFuel Pressure =>  ")); 
  Serial.print(fuelPressure);
  Serial.println("  KPa");

  Serial.println(F("<============================================>"));
  Serial.println();

  delay(1000); 
}


