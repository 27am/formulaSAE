/*  
 *  ------ CAN Bus Get Engine RPM -------- 
 *  
 *  Explanation: This is the basic Code for Waspmote Pro.
 *  
 *  Copyright (C) 2013 Libelium Comunicaciones Distribuidas S.L. 
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
 
// Include always these libraries before using the CAN BUS functions
#include <arduinoCAN.h>
#include <SPI.h>

// Create an instance
CAN myCAN = CAN();

void setup()
{ 	
  // Inits the UART
  Serial.begin(115200);
  delay(100);
  
  // Print init message 
  Serial.println("Initializing CANBUS...");	
  
  // Configuring the BUS at 500 Kbit/s
  // Only allowed in SOCKET 0
  myCAN.begin(500);   
    
  Serial.println("CANBUS initialized at 500 KBits/s");  
}

void loop()
{
  // Read the value of RPM if the engine	
  long engineRpm = myCAN.getEngineRPM();      

  // Print received data in the serial monitor
  Serial.print("RPM of engine : "); 
  Serial.println(engineRpm);
  delay(1000); 
}



