/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the behavior 
of the Protractor's Feedback LEDs. The behaviors available are:
 - Show Object: Visually indicates the angle to the most visible object
 - Show Path: Visually indicates the angle to the most open path
 - Off: Turn off the feedback LEDs
Changes to the LED behavior take effect immediately. Changes are not remembered and must be set each time 
the Protractor is rebooted.

This example assumes I2C communication, but the same methods can be applied whether I2C or Serial 
communication is used.

For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
*/
#include <stdio.h>
#include <string.h>
#include <ProtractorPi.h>

Protractor myProtractor;

int main() {
  fprintf(stdout,"Protractor Sensor Demo!\n");
  
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
  
  // LED SHOW PATH
  // Set the LEDs to show the location of the most open path.
  myProtractor.LEDshowPath(); // LEDs will indicate the location of the most visible object
  fprintf(stdout,"Showing Path\n");
  delay(10000); // Play with the Protractor for 10 seconds.
  
  // LED OFF
  // Set the LEDs to Off. This may be useful to save power or avoid interference with other optical sensors.
  // NOTE: Setting the LEDs to off will also disable the Green status LED which is useful for troubleshooting.
  myProtractor.LEDoff(); // LEDs are off
  fprintf(stdout,"LEDs Off\n");
  delay(2000); // Wait 2 seconds.
  
  // LED SHOW OBJECT
  // Set the LEDs to show the location of the most visible object.
  myProtractor.LEDshowObject(); // LEDs will indicate the location of the most visible object
  fprintf(stdout,"Showing Objects\n");
  
  // Obtain readings from the Protractor
  while(1){
    myProtractor.read(); // Request the Protractor to send its data to the Raspberry Pi
    
	printf(stdout,"\nNumber of Objects: %i",myProtractor.objectCount());

	if(numObjects > 0){
      printf(stdout,", Angle of Most Visible Object = %3i degrees",myProtractor.objectAngle());
	}
	fprintf(stdout,"\n");
	
    printf(stdout,"\nNumber of Paths:   %i",myProtractor.pathCount());
		
    if(numPaths > 0){
      printf(stdout,", Angle of Most Visible Path   = %3i degrees",myProtractor.pathAngle());
    }
    fprintf(stdout,"\n");

    delay(500);
  }
  return 1;
}