/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the Serial 
baud rate of the Protractor.

The Protractor supports serial baud rates between 1200 and 250000 baud. Requests to operate at baud rates 
outside this range will be ignored by the Protractor.

Requests to change the Protractor's serial baud rate do not take effect until after the Protractor is
rebooted. To reboot, the Protractor's power supply and the Raspberry Pi must both be unplugged or powered 
down at the same time.

How to use this program:
0. Before running this program, use the Basics_Serial example to verify your setup is working properly
1. Set variable currentBaudRate to whatever the Protractor's current Baud Rate is (default = 9600)
2. Set variable newBaudRate to whatever you want the Protractor's Baud Rate to be (example = 115200)
3. Run this program
4. Reboot the Protractor by unplugging it from the power supply and raspberry Pi. Plug it back in after 10 seconds.
5. Use the Basics_Serial example to verify the baud rate has changed successfully - avoid running this program a second time.
    - In Basics_Serial, replace the "9600" on line "myProtractor.beginSerial(9600)" with the newBaudRate
    - For example: "myProtractor.beginSerial(115200)"
Note: Running this program multiple times can lead to confusion. Be sure to update the currentBaudRate 
and newBaudRate each time the program is run between reboots. See the tutorial for info on resetting the Protractor
to the default Baud Rate.

For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
*/
#include <stdio.h>
#include <string.h>
#include <ProtractorPi.h>

Protractor myProtractor;

int main() {
  fprintf(stdout,"Protractor Sensor Demo!");

  int currentBaudRate = 9600; // Whatever the Protractor's current baud rate is, default is 9600
  int newBaudRate = 115200; // Pick a new baud rate for the Protractor.
  
  //Initialize Protractor on Serial
  myProtractor.beginSerial(currentBaudRate); // Protractor's default baud rate is 9600
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(!connected) {
    fprintf(stdout,"Communication Error, Check Wiring and currentBaudRate are correct\n");
	return 0;
  }else{
    fprintf(stdout,"Connected to Protractor\n");
	
    // Once initialized, a new baud rate can be set at any time.
    // The new baud rate does not take effect until the Protractor is rebooted.
    bool baudrateSet = myProtractor.setNewSerialBaudRate(newBaudRate); // Next time the Protractor is rebooted, serial communication will be at the new baud rate
	delay(10);
	if(baudrateSet){
	  fprintf(stdout,"New Baud Rate Set: %i\n\n"
	                 "-----------------------------------------------------------------\n"
					 "Protractor must be Power Cycled for new baud rate to take effect.\n"
					 "-----------------------------------------------------------------\n\n");
      return 1;
	}else{
	  fprintf(stdout,"Unable to set new baud rate, baud rate is not valid\n\n");
	  return 0;
	}
  }
}