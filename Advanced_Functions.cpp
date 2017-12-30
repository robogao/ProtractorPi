/* PROTRACTOR - A Proximity Sensor that Measures Angles
This is an example for the Protractor Sensor. This example will demonstrate how to use the Protractor's
advanced functions:
 - Read the deviced Product Type "PR". This can be used to verify the device is a Protractor
 - Read the Protractor's Serial Number
 - Read the Voltage Vin sensed by the Protractor. If Vin is too low the Protractor will disable itself and flash the Blue LEDs.
 - Read the raw data from the Protractor. Returns 8 values which correspond to the amount of light reflected by nearby objects.
 
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

  // Read the Product Type
  char prodType[2]; // 2 element array in which the Product Type can be stored.
  myProtractor.productType(prodType); // Read the Product Type from the Protractor and store in the prodType array
  fprintf(stdout,"Product Type = %c%c\n",prodType[0],prodType[1]);

  // Read the Serial Number
  fprintf(stdout,"Serial Number = %8X\n",myProtractor.serialNumber());
  
  while(1) {
    // Read the Voltage
    fprintf(stdout,"Voltage (mV) = %5i\n",myProtractor.voltage());
    delay(500);

    // Read the Raw Data Reflections
    byte data[8]; // 8 element array into which the raw data can be stored. Each value represents the amount of light reflected from nearby objects.
    myProtractor.reflections(data); // Read the raw data from the Protractor and store in the data array
    printf(stdout,"Reflections = ");
    for(int i = 0; i < 8; i++){
	  printf(stdout,"%3i ",data[i]);
    }
    fprintf(stdout,"\n");

    delay(1000);
  }
}
