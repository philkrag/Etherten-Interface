/*

// /////////////////////////////////////////////////////////////////////// COPYRIGHT NOTICE
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with Pip-Project.  If not, see <http://www.gnu.org/licenses/>.
// /////////////////////////////////////////////////////////////////////// COPYRIGHT NOTICE

// /////////////////////////////////////////////////////////////////////// VERSION CONTROL
// PAGE CREATED BY: Phillip Kraguljac
// PAGE CREATED DATE: 2015-xx-xx
// DATE       || NAME           || MODIFICATION
// 2018-07-07   || Phillip Kraguljac    || Modified.
// /////////////////////////////////////////////////////////////////////// VERSION CONTROL

*/

#include <SPI.h>                            // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>                    // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#define UDP_TX_PACKET_MAX_SIZE 860          //increase UDP size

String returnAck="";
char returnAckIndex[50];

String cmdSentString="";
char cmdSentChar[50];

const int inputPin2 =  2;
int inputPin2State = 0;
int inputPin2Memory = 0;



byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 80);
IPAddress destinationIP(192, 168, 0, 20);
//String destinationIP = "192.168.0.100";

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
char  ReplyBuffer[] = "ACK";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {

pinMode(inputPin2, INPUT);

// start the Ethernet and UDP:
Ethernet.begin(mac, ip);
Udp.begin(localPort);

Serial.begin(9600);
}

void loop() {

// if there's data available, read a packet
int packetSize = Udp.parsePacket();
if (packetSize) {
Serial.print("Received packet of size ");
Serial.println(packetSize);
Serial.print("From ");
IPAddress remote = Udp.remoteIP();
for (int i = 0; i < 4; i++) {
Serial.print(remote[i], DEC);
if (i < 3) {
Serial.print(".");
}
}
Serial.print(", port ");
Serial.println(Udp.remotePort());

// read the packet into packetBufffer
Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
Serial.println("Contents:");
Serial.println(packetBuffer);


// send a reply to the IP address and port that sent us the packet we received
Udp.beginPacket(Udp.remoteIP(), 8889);
returnAck = "MOD=>ACK:"+String(packetBuffer).substring(6, 9);
returnAck.toCharArray(returnAckIndex, 50);

//Serial.println(returnAcklowledgementChar);
Udp.write(returnAckIndex);
Udp.endPacket();
}


inputPin2State=digitalRead(inputPin2);

if(inputPin2State==1&&inputPin2State!=inputPin2Memory){
inputPin2Memory=inputPin2State;

Serial.println("1");

Udp.beginPacket(destinationIP, 8889);
cmdSentString = "MOD=>ENC:PIN=>002:VAL=>0255:VAL=>0000:VAL=>0000";
cmdSentString.toCharArray(cmdSentChar, 50);
Udp.write(cmdSentChar);
Udp.endPacket();
}
if(inputPin2State==0&&inputPin2State!=inputPin2Memory){
inputPin2Memory=inputPin2State;
Serial.println("0");  

Udp.beginPacket(destinationIP, 8889);    
cmdSentString = "MOD=>ENC:PIN=>002:VAL=>0000:VAL=>0000:VAL=>0000";
cmdSentString.toCharArray(cmdSentChar, 50);
Udp.write(cmdSentChar);  
Udp.endPacket();
}



// NEED TO MODIFY FOR mod=>xxx:pin=>xxx:val=>xxxx:val=>xxxx:val=>xxxx:


//  delay(10);
}
