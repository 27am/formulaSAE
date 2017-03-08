/*  
 *  LoRa 868 SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
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
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.0
 *  Implementation:    Andrea Merlina, Valerio Bruschi
 */


/*
 * LIBRARIES
 */
#include <SPI.h>
#include <Wire.h>
#include <arduinoUtils.h>       // Cooking API libraries
#include <arduinoLoRa.h>         // Include the SX1272 and SPI library:


/*
 * VARIABLES
 */
int e;
char my_packet[100];


/*
 * just open the serial communication and start LoRa module
 */
void setup()
{
  Serial.begin(9600);

  startLORA();
}


/*
 * in the loop is simply implemented the receiving process.
 * when a packet arrives it is printed to serial output.
 * this is the reason why all printing have been commented: just numerical values must be outputted,
 * otherwise the visualization program won't be able to handle them
 */
void loop(void)
{
  //Receive message
  e = sx1272.receivePacketTimeout(10000);
  
  if (e == 0)
  {
    //Serial.print(F("Receive packet, state "));
    //Serial.println(e, DEC);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    //Serial.print(F("Message: "));
     Serial.println(my_packet);
    

  }
  else {
//    Serial.print(F("Receive packet, state "));
//    Serial.println(e, DEC);
  }

}



void startLORA()
{
  //Serial.println(F(" # starting SX1272 module # "));

  e = sx1272.ON();
  e |= sx1272.setMode(4);
  e |= sx1272.setHeaderON();
  e |= sx1272.setChannel(CH_10_868); //set channel and band
  e |= sx1272.setCRC_ON();
  e |= sx1272.setPower('M');         //MAX
  e |= sx1272.setNodeAddress(8);  

  // Print a success message
  //if (e == 0)
  //    Serial.println(F(" # SX1272 successfully configured # "));
  //else
  //    Serial.println(F(" # SX1272 initialization failed # "));

}
