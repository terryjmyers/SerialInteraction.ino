# SerialInteraction.ino
Template to interact over Serial monitor with Arduino

he following is a simple sketch that you can incorporate into your sketches in order to interact with your project programmatically.  It includes a simple serial menu system that greets the user with prompts.  The ideas presented here can eliminate the need rotary encoders, potentiometers, push buttons, etc, if the end user is willing to simply connect to the project to interact with it over serial.

Some ideas:

1. During prototyping, setup commands to set and reset output pins or directly set variables
2. During running projects, setup commands to save configuration data to the EEPROM
3. Update the RTC time by sending the time in Comma separated values (CSV) over the serial interface.
4. Save on wiring up a potentiometer by simply sending a command over serial to mimic the position
