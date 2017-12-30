/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to communicate between a 
Protractor and Raspberry Pi using serial communication. The angle between the Protractor and a nearby object 
will be printed to the terminal.  The angle between the Protractor and the most open pathway will also 
be printed to the Terminal.

__ELECTRICAL CONNECTIONS__

Power Connections
The Protractor requires input power of 6V to 14V. Use a high quality power supply capable of sourcing up to 1A.

| ----------- | ------------- |
| PROTRACTOR  | POWER SUPPLY  |
| ----------- | ------------- |
|     GND     |   0V / GND    |
|     Vin     |   6v to 14V   |
| ----------- | ------------- |

Serial Connections
To use the Protractor with a Raspberry Pi using Serial GPIO, make the following connections:

| ----------- | ------------- |
| PROTRACTOR  |  RASPBERRY PI |
| ----------- | ------------- |
|   DG/DGND   |     GND       |
|     VCC     |     3.3V      |
|     TX      | RX/RXD/Pin 10 |
|     RX      | TX/TXD/Pin 8  |
| ----------- | ------------- |

Notes:
Protractor has built-in level shifters and should be connected directly to the Raspberry Pi.
Do not connect Protractor VCC pin to the Raspberry Pi 5V pin. This will damage the Raspberry Pi.
Protractor SDA and SCL pins should be left unconnected when using Serial.

For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
*/
#include <stdio.h>
#include <string.h>
#include <ProtractorPi.h>

Protractor myProtractor;

int main() {
  fprintf(stdout,"Protractor Sensor Demo!");
  
  //Initialize Protractor on Serial
  myProtractor.beginSerial(9600); // Protractor's default baud rate is 9600
  
  // Check communication with the Protractor
  bool connected = myProtractor.read();
  if(!connected) {
    fprintf(stdout,"Communication Error, Check Wiring and Baud Rate are correct\n");
	return 0;
  }else{
    fprintf(stdout,"Connected to Protractor\n");
  }
  
  // Obtain readings from the Protractor
  while(1){
	
	  myProtractor.read(); // Request the Protractor to send its data to the Raspberry Pi
	  
	  // How many objects are within view?
	  int numObjects = myProtractor.objectCount();
	  printf(stdout,"\nNumber of Objects: %i",numObjects);

	  if(numObjects > 0){
		int object = myProtractor.objectAngle();
		printf(stdout,", Angle of Most Visible Object = %3i degrees",object);
	  }
	  fprintf(stdout,"\n");
	  
	  // Print the angles to all objects within view. Objects are in rank order from most visible to least visible.
	  printf(stdout,"Angles, Visibility\n");
	  for(int i = 0; i < numObjects; i++){
		fprintf(stdout,"   %3i, %3i\n",myProtractor.objectAngle(i),myProtractor.objectVisibility(i));
	  }
	  
	  // How many pathways are within view?
	  int numPaths = myProtractor.pathCount();
	  printf(stdout,"\nNumber of Paths:   %i",numPaths);
		
	  // If at least one pathway is in view, print the angle of the most visibile pathway to the Serial Port
	  if(numPaths > 0){
		int path = myProtractor.pathAngle();
		printf(stdout,", Angle of Most Visible Path   = %3i degrees",path);
	  }
	  fprintf(stdout,"\n");
	  
	  // Print the angles to all paths within view. Paths are in rank order from most open to least open.
	  printf(stdout,"Angles, Visibility\n");
	  for(int i = 0; i < numPaths; i++){
		fprintf(stdout,"   %3i, %3i\n",myProtractor.pathAngle(i),myProtractor.pathVisibility(i));
	  }
	  delay(1000);
  }
  return 1;
}
