/*

Web Alert

This circuit allows for an RGB LED to provide colourful notifications based on web events.

Circuit:
* Ethernet shield attached to pins 10, 11, 12, 13
* RGB LED attached to pins 3, 5, 6

*/
#include <SPI.h>
#include <Ethernet.h>

// Define Ethernet variables
byte mac[] = { 0x90, 0xA2, 0xDA, 0x90, 0xA2, 0xDA };
IPAddress ip(192,168,1,20);
EthernetClient client;

// Define request variables
const unsigned long requestInterval = 10000;  // delay between requests
char serverName[] = "website.co.uk";  // Request URL where XML is hosted
boolean requested;                  // whether you've made a request since connecting
unsigned long lastAttemptTime = 0;  // last time you connected to the server, in milliseconds

// Define content variables
String currentLine = "";            // string to hold the text from server
String countString = "";            // string to hold the counter value recieved from the XML
boolean reading = false;            // if you're currently reading the file
boolean firstReading = true;        // is this the first reading since powered on?

// Define counting logic variables
int delta = 0;                      // the number of hits since last check
int lastCount = 0;                  // last times reading - stored to calculate delta
int currentCount = 0;               // this times reading

// Define colour variables
byte BLACK[] = {0, 0, 0};
byte RED[] = {255, 0, 0};
byte GREEN[] = {0, 255, 0};

int newColor = 0;
int currColor = 0;

#define numColors 7
const byte colors[numColors][3] = {
  {0,0,0},         // Black
  {0, 0, 255},     // Blue
  {158, 4, 79},    // Pink
  {200,60,0},      // Orange
  {255, 255, 0},   // Yellow
  {255, 0, 0},     // Red
  {0, 255, 0}      // Green
};

// Define RGB LED pins
int ledAnalogOne[] = {3, 5, 6};


void setup() {

  // Set up LED and run a color check function
  for(int i = 0; i < 3; i++){
   pinMode(ledAnalogOne[i], OUTPUT);   // Set the three LED pins as outputs
  }
  setColor(ledAnalogOne, BLACK);       // Turn off led
  colorSweep();                        // Sweep through all array colours

  // reserve space for the strings
  currentLine.reserve(256);
  countString.reserve(10);

  // Open serial communications
  Serial.begin(9600);


  // attempt a DHCP connection
  Serial.println("Attempting to get an IP address using DHCP:");
  if (!Ethernet.begin(mac)) {
    // if DHCP fails, show a visual cue and start with a hard-coded address
    statusColor(RED, 4); // Flash red 4 times
    Serial.println("Failed to get an IP address using DHCP, trying manually");
    Ethernet.begin(mac, ip);
  } else {
     // Connected - show a visual cue
    statusColor(GREEN, 2); // Flash green twice
  }

  Serial.print("My address:");
  Serial.println(Ethernet.localIP());

  // connect to Server:
  connectToServer();
}



void loop()
{
  if (client.connected()) {
    if (client.available()) {

      // read incoming bytes
      char inChar = client.read();
      // add incoming byte to end of line
      currentLine += inChar;
      // if you get a newline, clear the line
      if (inChar == '\n') {
        currentLine = "";
      }

      // if you're currently reading the bytes of a node,
      // add them to the delta String
      if (reading) {
        if (inChar != '<') {
          countString += inChar;
        } else {
          // if you got a "<" character,
          // you've reached the end of the node
          reading = false;

          // Archive old count number
          lastCount = currentCount;

          currentCount = getInt(countString);

          if (firstReading){
            delta = 0;
            firstReading = false;
          } else {
            delta = currentCount - lastCount;
          }

          Serial.print("Delta: ");
          Serial.println(delta);

          if (delta > 0) {
            if (delta > numColors){
              delta = numColors;
              Serial.print("Adjusted Delta: ");
              Serial.println(delta);
            }


            for(int i = 0; i < numColors; i++){
              if (delta == i+1) {
                Serial.print("Colour: ");
                // Serial.println(i);
                fadeToColor(ledAnalogOne, colors[currColor], colors[i+1], 1);// use colors[i+1] to offset away from the first value (black)
                currColor = i+1;

              }
            }
            delta = 0;

          } else {
            fadeToColor(ledAnalogOne, colors[currColor], BLACK, 5);
            currColor = 0;
          }

          // close the connection to the server:
          client.stop();
        }
      }




      // if the current line ends with <text>, it will
      // be followed by the tweet:
      if ( currentLine.endsWith("<blog>")) {
        // blog count is beginning. Clear the count string:
        reading = true;
        countString = "";
      }

    }
  }
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and two minutes have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
}



void connectToServer() {
  // attempt to connect, and wait a millisecond:
  Serial.println("connecting to server...");
  if (client.connect(serverName, 80)) {
    Serial.println("making HTTP request...");
    // make HTTP GET request to twitter:
    client.println("GET /count.xml HTTP/1.1");
    client.println("HOST: website.co.uk");
    client.println();
  } else {
    Serial.println("Can't connect");
    statusColor(RED, 2);
  }
  // note the time of this connect attempt:
  lastAttemptTime = millis();
}

void setColor(int* led, byte* color){
 for(int i = 0; i < 3; i++){             //iterate through each of the three pins (red green blue)
   analogWrite(led[i], 255 - color[i]);  //set the analog output value of each pin to the input value (ie led[0] (red pin) to 255- color[0] (red input color)
                                         //we use 255 - the value because our RGB LED is common anode, this means a color is full on when we output analogWrite(pin, 0)
                                         //and off when we output analogWrite(pin, 255).
 }
}


int getInt(String text)
{
  char temp[20];
  text.toCharArray(temp, 19);
  int x = atoi(temp);
  if (x == 0 && text != "0")
  {
    x = -1;
  }
  return x;
}

void colorSweep(){
  fadeToColor(ledAnalogOne, BLACK, colors[0], 1);
  for(int i=0; i < numColors-1; i++){
    if (i == numColors-1){
      newColor = 0;
    } else {
      newColor = i+1;
    }
    fadeToColor(ledAnalogOne, colors[i], colors[newColor], 1);
  }
  fadeToColor(ledAnalogOne, colors[numColors-1], BLACK, 1);
}

void statusColor(byte* color, int count){
  for (int i = 0; i < count; i++){
    fadeToColor(ledAnalogOne, BLACK, color, 1);
    fadeToColor(ledAnalogOne, color, BLACK, 1);
  }
}

void fadeToColor(int* led, byte* startColor, byte* endColor, int fadeSpeed){
  int changeRed = endColor[0] - startColor[0];                            //the difference in the two colors for the red channel
  int changeGreen = endColor[1] - startColor[1];                          //the difference in the two colors for the green channel
  int changeBlue = endColor[2] - startColor[2];                           //the difference in the two colors for the blue channel
  int steps = max(abs(changeRed),max(abs(changeGreen), abs(changeBlue))); //make the number of change steps the maximum channel change

  for(int i = 0 ; i < steps; i++){                                        //iterate for the channel with the maximum change
   byte newRed = startColor[0] + (i * changeRed / steps);                 //the newRed intensity dependant on the start intensity and the change determined above
   byte newGreen = startColor[1] + (i * changeGreen / steps);             //the newGreen intensity
   byte newBlue = startColor[2] + (i * changeBlue / steps);               //the newBlue intensity
   byte newColor[] = {newRed, newGreen, newBlue};                         //Define an RGB color array for the new color
   setColor(led, newColor);                                               //Set the LED to the calculated value
   delay(fadeSpeed);                                                      //Delay fadeSpeed milliseconds before going on to the next color
  }
  setColor(led, endColor);                                                //The LED should be at the endColor but set to endColor to avoid rounding errors
}

/* A version of fadeToColor that takes predefined colors (neccesary to allow const int pre-defined colors */
void fadeToColor(int* led, const byte* startColor, const byte* endColor, int fadeSpeed){
  byte tempByte1[] = {startColor[0], startColor[1], startColor[2]};
   byte tempByte2[] = {endColor[0], endColor[1], endColor[2]};
   fadeToColor(led, tempByte1, tempByte2, fadeSpeed);
}
