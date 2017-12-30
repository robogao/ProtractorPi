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
*/
#include <stdio.h>
#include <string.h>
#include <WiringPi.h>
#include <WiringSerial.h>
#include <ProtractorPi.h>

Protractor::Protractor()
{
}

short int Protractor::beginSerial(int baudRate) // Initialize the Protractor with Serial communication
{
  //if (baudRate >= 1200 && baudRate <= 250000 && (_serial = serialOpen ("/dev/ttyAMA0", baudRate)) < 0){
  if (baudRate >= 1200 && baudRate <= 250000 && (_serial = serialOpen ("/dev/serial0", baudRate)) < 0){ // Use this for Raspbian
    fprintf (stdout, "Unable to open serial port") ;
    return -1;
  } else {
    _comm = SERIALCOMM;
	return 0;
  }
}
// int Protractor::beginI2C(short int address) // Initialize the Protractor with I2C communication
// {
  // _address = address;
  // if ((_wire = ...openI2Cport... (address)) < 0){
    // fprintf (stdout, "Unable to open i2c port") ;
    // return -1;
  // } else {
    // _comm = I2CCOMM;
	// return 0;
  // }
// }

/////// BASIC FUNCTIONS ///////

bool Protractor::read() { // get all of the data from the protractor.
  return read(MAXOBJECTS);
}
bool Protractor::read(short int obs) { // gets obs number of objects and obs number of paths from protractor. Returns the most visible objects and most open pathways. Minimizes data transfer for time sensitive applications.
  if(obs > MAXOBJECTS) obs = MAXOBJECTS;
  _numdata = obs;
  unsigned char numBytes = 1 + obs*4;
  _requestData(numBytes); // Request data from the obstacle sensor
  
  int i = 0;
  unsigned long startTime = millis();
  unsigned long duration = 0;
  for(int i = 0; i < numBytes; i++) _buffer[i] = 0;
  while(i < numBytes && duration < _waitTime){
	if(_available()) {
		_buffer[i] = _read();
		startTime = millis();
		i++;
	}
	duration =  millis() - startTime;
  }
  if(i == 0){
	  return 0;
  } else {
	  return 1;
  }
}

short int Protractor::objectCount() { // returns the number of objects detected
  return (short int)(_buffer[0] >> 4); // number of objects detected is the high nibble of _buffer[0]
}

short int Protractor::pathCount() { // returns the number of paths detected
  return (short int)(_buffer[0] & 0b00001111); // number of paths detected is the low nibble of _buffer[0]
}

short int Protractor::objectAngle() { // returns the angle to the most visible object
  return objectAngle(0);
}
short int Protractor::objectAngle(short int ob) { // returns the angle to the object ob in the object list, indexed from 1. Left most object is 1. If ob exceeds number of objects detected, return zero.
  if(ob >= objectCount() || ob < 0){
    return -1;
  }else{
    short int angle = _buffer[1+4*ob]*180/255;  // ob0->_buffer[1], ob1->_buffer[5], etc.
    return angle;
  }
}
short int Protractor::objectVisibility() { // returns the visibility of the most visible object
  return objectVisibility(0);
}
short int Protractor::objectVisibility(short int ob) { // returns the visibility of the object ob in the object list, indexed from 1. Left most object is 1. If ob exceeds number of objects detected, return zero.
  if(ob >= objectCount() || ob < 0){
    return -1;
  }else{
    short int vis = _buffer[2+4*ob];  // ob0->_buffer[2], ob1->_buffer[6], etc.
    return vis;
  }
}

short int Protractor::pathAngle() { // returns the angle to the most open pathway
  return pathAngle(0);
}
short int Protractor::pathAngle(short int pa) { // returns the angle to the path pa in the pathway list, indexed from 1. Left most path is 1. If pa exceeds number of pathways detected, return zero.
  if(pa >= pathCount() || pa < 0){
    return -1;
  }else{
	short int angle = _buffer[3+4*pa]*180/255;
    return angle;
  }
}
short int Protractor::pathVisibility() {
  return pathVisibility(0);
}
short int Protractor::pathVisibility(short int pa) { // returns the angle to the path pa in the pathway list, indexed from 1. Left most path is 1. If pa exceeds number of pathways detected, return zero.
  if(pa >= pathCount() || pa < 0){
    return -1;
  }else{
    short int vis = _buffer[4+4*pa];  // pa0->_buffer[4], pa1->_buffer[8], etc.
    return vis;
  }
}

/////// ADVANCED FUNCTIONS ///////

bool Protractor::connected() { // Determine if the Protractor is connected
  char pt[2];
  return productType(pt);
}

bool Protractor::productType(char prodtype[]) { // Read out the Product Type from the Protractor. Should return 'P','R'.
  unsigned char sendData[2] = {0x00,'\n'};
  _write(sendData,2); // Tell Protractor that the next time data is requested it should give back the product type
  delay(2); // Allow time for Protractor to process request
  read(1);
  prodtype[0] = _buffer[0];
  prodtype[1] = _buffer[1];
  if(prodtype[0] == 'P' && prodtype[1] == 'R'){
    return 1;
  }else{
    return 0;
  }
}

unsigned int Protractor::serialNumber() { // Read out the Serial Number from the Protractor.
  unsigned char sendData[2] = {0xC0,'\n'};
  _write(sendData,2);
  delay(2); // Allow time for Protractor to process request
  read(1);
  unsigned int serialNumber = _buffer[0] + ((unsigned int)_buffer[1] << 8) + ((unsigned int)_buffer[2] << 16) + ((unsigned int)_buffer[3] << 24);
  return serialNumber;
}

short int Protractor::voltage() { // Read out the voltage Vin sensed by the Protractor in milliVolts
  unsigned char sendData[2] = {0xB2,'\n'};
  _write(sendData,2);
  delay(2); // Allow time for Protractor to process request
  bool dataReceived = read(1);
  short int voltage = _buffer[0] + ((short int)_buffer[1] << 8);
  if(dataReceived){
    return voltage;
  }else{
	return -1;
  }
}

bool Protractor::reflections(unsigned char data[]) { // Read out the reflections from the Protractor.
  unsigned char sendData[2] = {0xE0,'\n'};
  _write(sendData,2);
  delay(2); // Allow time for Protractor to process request
  bool dataReceived = read(2);
  for(int i = 0; i < 8; i++) {
    data[i] = _buffer[i];
  }
  return dataReceived;
}

bool Protractor::commParams(unsigned char data[]) { // Read out the reflections from the Protractor.
  unsigned char sendData[2] = {0xE1,'\n'};
  _write(sendData,2);
  delay(2); // Allow time for Protractor to process request
  bool dataReceived = read(3);
  for(int i = 0; i < 12; i++) {
    data[i] = _buffer[i];
  }
  return dataReceived;
}

/////// SETTINGS ///////

bool Protractor::scanTime(short int milliSeconds) {//0 = scan only when called. 1 to 15 = rescan every 15ms, >15 = rescan every time_ms milliseconds.  Default time_ms is set to 30ms.
  if(milliSeconds >= 1 && milliSeconds <= MINDUR-1) milliSeconds = MINDUR;  // Values within 1 and 14 milliSeconds aren't allowed, the sensor requires a minimum 15 seconds to complete a scan.
  _waitTime = (milliSeconds == 0) ? _commWait : _commWait + _scanWait; // When continuous scan is disabled, future requests for data will need to wait longer for Protractor to scan.
  if(milliSeconds >= 0 && milliSeconds <= 32767) {  // Values less than 0 or greater than 32767 aren't allowed.
    unsigned char sendData[4] = {SCANTIME,(unsigned char)(milliSeconds & 0x00FF),(unsigned char)(milliSeconds >> 8),'\n'};
    _write(sendData,4); // Send a signal (SCANTIME) to tell Protractor that it needs to change its time between scans to milliSeconds. 
    delay(1); // Allow time for Protractor to process request
	return 1;
  }else{
	return 0;
  }
}

bool Protractor::setNewI2Caddress(short int newAddress) { // change the I2C address. Will be stored after shutdown. See manual for instructions on restoring defaults. Default = 0x45 (69d).
  if(newAddress >= 2 && newAddress <= 127) {
	unsigned char sendData[3] = {I2CADDR,(unsigned char)newAddress,'\n'};
    _write(sendData,3); // Send a signal (I2CADDR) to tell Protractor that it needs to change its I2C address to the newAddress
    delay(1); // Allow time for Protractor to process request
	return 1;
  }else{
	return 0;
  }
}

bool Protractor::setNewSerialBaudRate(int newBaudRate) { // change the Serial Bus baud rate. Will be stored after shutdown. See manual for instructions on restoring defaults. Default = 9600 baud. 0 = 1200, 1 = 2400, 2 = 4800, 3 = 9600, 4 = 19200, 5 = 28800, 6 = 38400, 7 = 57600, 8 = 115200, 9 = 230400
  if(newBaudRate >= 1200 && newBaudRate <= 250000) {
	unsigned char sendData[6] = {BAUDRATE,(unsigned char)(newBaudRate & 0x00FF),(unsigned char)(newBaudRate >> 8),(unsigned char)(newBaudRate >> 16),(unsigned char)(newBaudRate >> 24),'\n'};
    _write(sendData,6); // Send a signal (BAUDRATE) to tell Protractor that it needs to change its baudrate to the newBaudRate
    delay(1); // Allow time for Protractor to process request
	return 1;
  }else{
	return 0;
  }
}

void Protractor::LEDshowObject() { // Set the feedback LEDs to follow the most visible Objects detected
  unsigned char sendData[3] = {LEDUSAGE,SHOWOBJ,'\n'};
  _write(sendData,3); // Send a signal (LEDUSAGE) to tell Protractor that it needs to SHOWOBJ
  delayMicroseconds(200); // Allow time for Protractor to process request
}

void Protractor::LEDshowPath() { // Set the feedback LEDs to follow the most open pathway detected
  unsigned char sendData[3] = {LEDUSAGE,SHOWPATH,'\n'};
  _write(sendData,3); // Send a signal (LEDUSAGE) to tell Protractor that it needs to SHOWPATH
  delayMicroseconds(200); // Allow time for Protractor to process request
}

void Protractor::LEDoff() { // Turn off the feedback LEDs
  unsigned char sendData[3] = {LEDUSAGE,LEDOFF,'\n'};
  _write(sendData,3); // Send a signal (LEDUSAGE) to tell Protractor that it needs to turn the feedback LEDOFF
  delayMicroseconds(200); // Allow time for Protractor to process request
}

/////// PRIVATE FUNCTIONS ///////

unsigned char Protractor::_available() {
  if(_comm == I2CCOMM){
    //return _wire->available();
	return 0;
  }
  else if(_comm == SERIALCOMM){
    //return _serial->available();
	return serialDataAvail(_serial);
  }
  else{
	return 0;
  }
}

unsigned char Protractor::_read() {
  // if(_comm == I2CCOMM){
    // //return _wire->read();
	// return 0;
  // }
  // else
  if(_comm == SERIALCOMM){
    //return _serial->read();
	return serialGetchar(_serial);
  }
  else{
	return 0;
  }
}

void Protractor::_write(unsigned char arrayBuffer[], unsigned char arrayLength) {
  // if(_comm == I2CCOMM){
    // _wire->beginTransmission(_address);
    // _wire->write(arrayBuffer,arrayLength);
    // _wire->endTransmission();
  // }
  // else 
  if(_comm == SERIALCOMM){
	serialPuts(_serial,arrayBuffer);
	serialFlush(_serial);
  }
}

void Protractor::_requestData(unsigned char numBytes) {
  // if(_comm == I2CCOMM){
    // _wire->requestFrom(_address, numBytes);
  // }
  // else
  if(_comm == SERIALCOMM) {
    unsigned char sendData[3] = {REQUESTDATA,numBytes,'\n'};
    _write(sendData,3); // Send a signal (char SENDDATA) to tell Protractor that it needs to send data, tell it the number of data points to send.	
  }
}