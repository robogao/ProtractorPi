/*
  ###########################################################################
  
  A lot of sensors can tell the distance to an object, but determining the angle to an object is much harder. 
  With a 180 degree field of view, the Protractor can sense open pathways and tell the angle to multiple 
  objects up to 30cm (12 inches) away.  With a Protractor mounted to your mobile robot, you can easily find 
  or avoid objects.

  The Protractor is designed to work well with Mini Sumo robots, and can also be used as a general purpose 
  proximity sensor.

  For a complete tutorial on wiring up and using the Protractor go to:
    http://www.robogao.com/Protractor
	
  ############################################################################
  
  This Library was ported from the Protractor Library for Arduino for use on the Raspberry Pi. The WiringPi 
  Library must be installed on the Raspberry Pi before this library can be used.
  
  This Library has been tested on a Raspberry Pi 2B with Raspbian GNU/Linux 9.1 (stretch).
  
  To communicate between the Pi and the Protractor using Serial GPIO (Pin 8 - TX, Pin 10 - RX), the console
  must first be disabled for ttyAMA0.
  1. Check if the console is enabled. In the terminal, type "dmesg | grep tty" (without quotes)
  2. If the terminal outputs "console [ttyAMA0] enabled", then it must first be disabled.
  3. To disable, type "sudo raspi-config" to open the configuration manager
  4. Under Interfacing Options / Serial, choose <No> for option "WOuld you like a login shell to be accessible over serial?"
  5. Choose <Yes> for option "Would you like the serial port hardware to be enabled?"
  6. Exit the Configuration Manager and reboot, which can be done by typing "sudo reboot" in the terminal.
  7. After reboot, open a terminal and again type "dmesg | grep tty", verify that "console [ttyAMA0] enabled" no longer appears
  
  Install the WiringPi Library
  1. Go to: http://wiringpi.com and follow the Download and Install instructions.
  
  
  
*/

//////////////////////BEGIN PROTRACTOR.H //////////////////////

//#include <wiringPi.h>
//#include <wiringSerial.h>

#ifndef PROTRACTOR_H
#define PROTRACTOR_H

// Constants
#define SERIALCOMM 1
#define I2CCOMM  2
#define MAXOBJECTS 4
#define SHOWOBJ  1
#define SHOWPATH 2
#define LEDOFF   3
#define MINDUR   15

// PROTRACTOR COMMANDS
#define REQUESTDATA 0x15
#define SCANTIME 0x20
#define I2CADDR  0x24
#define BAUDRATE 0x26
#define LEDUSAGE 0x30

class Protractor
{
  public:
    Protractor();
    void beginSerial(int baudRate); // Initialize protractor using Serial
    void beginI2C(short int address); // Initialize protractor using I2C
    bool connected(); // Returns 1 if Protractor is successfully contacted, otherwise 0.
    bool read(); // gets all the data for all objects and paths from the protractor. Up to 4 objects and paths may be sensed at a time.
    bool read(short int obs); // gets only obs number of objects and obs number of paths from protractor. Returns the most visible objects and most open pathways first. Minimizes data transfer for time sensitive applications. If obs > 4 then obs = 4.
    short int objectCount(); // returns the number of objects detected
    short int pathCount(); // returns the number of paths detected
    short int objectAngle(); // returns the angle to the most visible object
    short int objectAngle(short int ob); // returns the angle to the object ob in the object list. Valid values of ob are 0 to 3. Object are ranked by intensity.  Most visible object is ob = 0.  Least visible object is ob = 3. If ob exceeds number of data points returned from sensor, returns -1.
	short int objectVisibility(); // returns the visibility of the most visible object
    short int objectVisibility(short int ob); // returns the visibility of the object ob in the object list. Valid values of ob are 0 to 3. Visibility is a relative measure of the amount of light reflected off an object. Visibility is generally not a good indicator of distance. If ob exceeds number of data points returned from sensor, returns -1.
    short int pathAngle(); // returns the angle to the most open pathway
    short int pathAngle(short int pa); // returns the angle to the path pa in the pathway list. Valid values of pa are 0 to 3. Pathways are ranked by openness.  Most open pathway is pa = 0.  Least open pathway is pa = 3. If pa exceeds number of data points returned from sensor, returns -1.
	short int pathVisibility(); // returns the visibility of the most open pathway
    short int pathVisibility(short int pa); // returns the visibility of a path pa in the path list. Valid values of pa are 0 to 3. Visibility is a relative measure of how little light is reflected from a pathway. Visibility can indicate which of several pathways is more open. If pa exceeds number of data points returned from sensor, returns -1.
    void LEDshowObject(); // Set the feedback LEDs to follow the most visible Objects detected
    void LEDshowPath(); // Set the feedback LEDs to follow the most open pathway detected
    void LEDoff(); // Turn off the feedback LEDs
    bool scanTime(short int milliSeconds); // 0 = scan only when called. 1 to 15 = rescan every 15ms, >15 = rescan every milliSeconds, max 32767.  Default time_ms is set to 15ms.
    bool setNewI2Caddress(short int newAddress); // Change the I2C address. Will be remembered after Protractor shutdown. Protractor must be reset to take effect. See manual for instructions on restoring defaults. Default = 0x45 (69d).
    bool setNewSerialBaudRate(int baudRate); // Change the Serial Bus baud rate. Will be remembered after Protractor shutdown. Protractor must be reset to take effect. See manual for instructions on restoring defaults. Default = 9600 baud.
	bool productType(char prodtype[]); // Read the Product Type from the Protractor. Two char array should equal 'P' 'R'. Returns 1 if 'P' 'R' is returned, 0 otherwise.
    unsigned int serialNumber(); // Read the Serial Number from the Protractor.
    short int voltage(); // Read the voltage Vin sensed by the Protractor, value returned is in millivolts. Accuracy is +/- 10% above 6.5 volts, +/- 15% below 6.5 volts.
    bool reflections(unsigned char data[]); // Read the raw data from the sensor. 8 element array passed into function will be populated with the raw values for the amount of light reflected off nearby objects.
	bool commParams(unsigned char data[]);
  private:
    unsigned char _read();
    void _write(unsigned char arrayBuffer[], unsigned char arrayLength);
    unsigned char _available();
    void _requestData(unsigned char numBytes);
    const unsigned long _commWait = 20; // how many milliSeconds to wait for Protractor to respond to a request for data
	const unsigned long _scanWait = 20; // wait this much longer to scan if continuous scan mode is disabled
	unsigned long _waitTime = _commWait;
    unsigned char _buffer[1+4*MAXOBJECTS]; // store data received from Protractor.
    unsigned char _address; // Stores the I2C bus address when communicating over I2C
    unsigned char _numdata; // Number of data points requested from sensor during most recent read
    unsigned char _comm = 0; // Tracks whether we are using I2C or Serial for communication
	int _serial; // Handle for the Serial object.
	//int _wire; // Handle for the I2C object.
};

#endif