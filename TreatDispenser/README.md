# Treat Dispenser

This circuit is designed to train my pet rats to push a button to receive a treat.

The button can only be pressed when a LED is lit.
Once the button has been pressed, a servo rotates to deliver a treat, then resets.

The LED then turns off for a set amount of time, before turning back on and re-activating the button.

Circuit:

* Servo attached to pin 9, +5v and ground
* LED attached to pin 3 and ground via resistor
* Pushbutton attached to pin 2, +5v via resistor and ground

![Treat Dispenser Circuit](https://github.com/jackbrewer/ArduinoProjects/raw/master/TreatDispenser/TreatDispenser_bb.png)

*A breadboard example of the circuit*

I plan to extend this circuit to include a more animal friendly trigger – probably a knock sensor. The pushbutton will still be kept as a manual override.

A suitable delivery & refil case / mechanism still needs to be thought about – lots of ideas, but a lack of materials to test with.

Once the rats have learnt to use this circuit, it could be extended with magnetic sensors to detect and dispense when they run in their exercise wheel.