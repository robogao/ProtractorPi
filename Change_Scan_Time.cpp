/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to change the scan time 
of the Protractor.
 - By default, the Protractor scans 180 degrees every 15 milliseconds. This example will demonstrate how 
   to increase the time between scans to save power.
 - By default, the Protractor scans continuously. This example will demonstrate how to disable continuous 
   scanning so that scans occur only when data is requested. There will be approximately 15 milliseconds 
   delay between when the data is requested and when the Protractor responds.
Changes to the scan time take effect immediately. Changes are not remembered and must be set each time the 
Protractor is rebooted.

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
  
  // Increase the Scan Time to 300 milliSecond
  // Notice that the Blue LEDs won't update as fast or smoothly now, but the average power consumption will drop significantly
  bool scanTimeSet = myProtractor.scanTime(300); // scan time must be between 1 and 32767 miliseconds
  if(scanTimeSet){
	fprintf(stdout,"New Scan Time Set\n");
  }else{
	fprintf(stdout,"Unable to set new scan time, scan time is not valid\n");
  }
  delay(10000); // Play with the Protractor for 10 seconds. 
  
  // Disable Continuous Scanning
  myProtractor.scanTime(0); // A Scan Time of Zero will disable continuous scanning.
  fprintf(stdout,"Continuous Scan Disabled\n"); // Note that the Blue LEDs will only update after a scan is requested

  while(1){
    myProtractor.read(); // Request the Protractor to perform a scan and send its data to the Raspberry Pi
    
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

    delay(1000); // Wait this many milliSeconds before requesting another scan
  }
  return 1;
}