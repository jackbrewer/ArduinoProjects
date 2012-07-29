# Web Alert

This circuit allows for an RGB LED to provide colourful notifications based on web events. 

Currently it monitors an XML file, which supplies a page view count for my blog. The circuit polls this XML file at regular intervals, then reports the visitor count during that interval by changing the LED colour.

Eventually, support will be added for monitoring Twitter, Facebook, Google Analytics, Email etc.

Circuit:
 
 * Ethernet shield attached to pins 10, 11, 12, 13
 * RGB LED attached to pins 3, 5, 6 and ground
 
This is my first attempt at a web based Arduino project and was based on these [Twitter Client](http://arduino.cc/en/Tutorial/TwitterClient) and [RGB LED](http://oomlout.com/RGBL/CODE/_RGBL_AnalogTest.txt) tutorials.

The current code is pretty messy and repetitive but I'm planning a massive re-write with added functionality and refactoring.