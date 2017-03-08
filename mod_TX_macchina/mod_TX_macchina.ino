/*
    LoRa 868 SX1272 LoRa module

    Copyright (C) Libelium Comunicaciones Distribuidas S.L.
    http://www.libelium.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.

    Version:           1.0
    Implementation:    Andrea Merlina, Valerio Bruschi
*/

/*
 *  LIBRARIES
 */

#include <arduinoCAN.h>    //Cooking-Hacks library
#include <SPI.h>           //standard arduino library
#include <Wire.h>          //standard arduino library
#include <arduinoUtils.h>  //Cooking-Hacks library
#include <arduinoLoRa.h>   //Cooking-Hacks library

/*
 *  VARIABLES
 */
 
int e;                   //variable for LoRa diagnosis
int state;               //variable for keeping the CANBUS messages state machine
char telemetry_pkt[80];
bool rcv = false;
unsigned int rpm, tps, lambda, oil_t, oil_p;  // <--- THOSE ARE THE PARAMETERS TO BE TRANSMITTED. ADD HERE



CAN myCAN = CAN();   // Create an instance of the object

/*
 *  Setup actions
 *  1) start serial communication with PC
 *  2) inizialize CANBUS
 *  3) inizialize LoRa module
 *  
 */
void setup()
{
  Serial.begin(9600);

  myCAN.begin(1000);  //CANBUS initialization at 1 Mbps
  delay(100);

  enableRadioPhase();
  delay(100);

  startLORA();        //inizializziamo LORA
  delay(100);

  enableCarPhase();
}


/*
 *  In the loop we wait until a CAN message is available
 *  If so we check its ID
 *  If ID matches we save the wanted value in a previously declared variable
 */
void loop()
{

  //waiting for a CAN message
  if (myCAN.messageAvailable() == 1)
  {
    rcv = true;  //set the receive flag to true

    myCAN.getMessage(&myCAN.messageRx);     // Read the last message received
    
    //   myCAN.printMessage(&myCAN.messageRx);
    //   Serial.print("#####");
    //   Serial.println(myCAN.messageRx.id, HEX);

   /*
    *  depending on CANBUS message ID some actions are taken
    */
    switch (myCAN.messageRx.id)
    {

      /*
       * in packet with 0x600 ID we are interested in "rpm" and "tps" fields.
       * rpm value is also manipulated following the conversion algorithm.
       * state value is set as its previous value OR 0b00011.
       * EXPLANATION: this is a trick to use a logic mask insted of many true & false for each parameter.
       *              the bit position represent the parameter e.g. rpm, "0" says that it hasn't been collected while "1" that it has been saved 
       *              when all the params are present ( state = 0b11111 ) a Lora packet is formed and transmitted
       */
      case 0x600:
        //Serial.println(F(" # received CAN pkt with id 600 # "));
        rpm = ((unsigned int) (myCAN.messageRx.data[0]) * 256 + myCAN.messageRx.data[1]) / 4;
        tps = (unsigned int) myCAN.messageRx.data[2];
        state |= 0b00011;
        break;



      case 0x601:
        break;

      /*
       * in packet with 0x602 ID we are interested in "oil_t" and "oil_p" fields.
       * 
       */
      case 0x602:
        //Serial.println(F(" # received CAN pkt with id 602 # "));
        oil_t = (unsigned int) myCAN.messageRx.data[3];
        oil_p = (unsigned int) myCAN.messageRx.data[4];
        state |= 0b01100;
        break;

      /*
       * in packet with 0x603 ID we are interested in "lambda" field.
       * 
       */
      case 0x603:
        //     Serial.println(F(" # received CAN pkt with id 603 # "));
        lambda = (unsigned int) myCAN.messageRx.data[3];
        state |= 0b10000;
        break;

      case 0x604:
        break;

      default:
        //Serial.println(F(" # CAN pkt received with other than selected IDs # "));
        rcv = false;
        break;
    }

    enableRadioPhase();

    delay(100);
    state &= 0b11111;


   /*
    * if all the parameters from CANBUS have been collected, create and send a LoRa packet.
    * PARAMETERS ORDER IS IMPORTANT
    */
    if (state == 0b11111)
    {
      sprintf(telemetry_pkt, "%u,%u,%u,%u,%u", rpm,
                                               tps,
                                               oil_t,
                                               oil_p,
                                               lambda);

      e = sx1272.sendPacketTimeout(8, telemetry_pkt);   //send the packet to a node with address "8"
      Serial.print(F("sending telemetry_pkt (already formatted), state "));
      Serial.println(e, DEC);
      Serial.print(F("####------> "));
      Serial.print(rpm, DEC);
      Serial.print(F("||"));
      Serial.print(tps, DEC);
      Serial.print(F("||"));
      Serial.print(oil_t, DEC);
      Serial.print(F("||"));
      Serial.print(oil_p, DEC);
      Serial.print(F("||"));
      Serial.println(lambda, DEC);

      state = 0;

    }

    enableCarPhase();
    delay(100);

  }

}

/*
    start SX1272 LoRa module
*/
void startLORA()
{
  Serial.println(F(" # starting SX1272 module # "));

  e = sx1272.ON();
  e |= sx1272.setMode(4);            // mode 4 has been tested to be a suitable solution. 1 more robust -> 10 less robust
  e |= sx1272.setHeaderON();
  e |= sx1272.setChannel(CH_10_868); //select the channel and the operating band
  e |= sx1272.setCRC_ON();
  e |= sx1272.setPower('M');         //MAX
  e |= sx1272.setNodeAddress(3);

  // Print a success message
  if (e == 0)
    Serial.println(F(" # SX1272 successfully configured # "));
  else
    Serial.println(F(" # SX1272 initialization failed # "));
}

/*
 * turn off the chip select for socket where CANBUS module is
 * turn on the chip select for socket where LoRa module is
 */
void enableRadioPhase()
{
  Utils.unsetCSSocket0();
  Utils.setCSSocket1();
}

/*
 * turn off the chip select for socket where LoRa module is
 * turn on the chip select for socket where CANBUS module is
 */
void enableCarPhase()
{
  Utils.unsetCSSocket1();
  Utils.setCSSocket0();
}




