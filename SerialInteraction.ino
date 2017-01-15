/*
SerialInteraction.ino
Terry Myers <TerryJMyers@gmail.com> https://github.com/terryjmyers
v1.0
Template to interact over Serial monitor with Arduino

The following is a simple sketch that you can incorporate into your sketches in order to interact with your project programmatically.  It includes a simple serial menu system that greets the user with prompts.  The ideas presented here can eliminate the need rotary encoders, potentiometers, push buttons, etc, if the end user is willing to simply connect to the project to interact with it over serial.

Some ideas:
1. During prototyping, setup commands to set and reset output pins or directly set variables
2. During running projects, setup commands to save configuration data to the EEPROM
3. Update the RTC time by sending the time in Comma separated values (CSV) over the serial interface.
4. Save on wiring up a potentiometer by simply sending a command over serial to mimic the position

*/

  //Serial Read tags - delete if not doing Serial Reads, Also delete fucntions SerialMonitor and SerialEvent
    #define SERIAL_BUFFER_SIZE 20
    String sSerialBuffer; //Create a global string that is added to character by character to create a final serial read
    String sLastSerialLine; //Create a global string that is the Last full Serial line read in from sSerialBuffer

  bool debug=1;
  float Potentiometer = 0.0;
  
//SETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUP
void setup() {
  
  //Setup Serial 
  //In case the user uses the default setting in Arduino, give them a message to change BAUD rate
    Serial.begin(9600);
    Serial.println(F("Set baud rate to 250 000"));
    Serial.flush ();   // wait for send buffer to empty
    delay (2);    // let last character be sent
    Serial.end ();      // close serial
    Serial.begin(250000);
    Serial.println();


  //Set pin 9 as an output for the example below
  pinMode(9,OUTPUT);

  //Set serial read buffer size to reserve the space to concatnate all bytes recieved over serial
    sSerialBuffer.reserve(SERIAL_BUFFER_SIZE);
    
  //Printout a beggining message to great the user
    Serial.println();
    DrawLine(); //====================================================================
      Serial.println(F("NAME OF YOUR PROGRAM"));
      Serial.println(F("Add a General Description here"));
      Serial.println();  
      Serial.println(F("Send '?' for help (all commands must preceed a LF or CR character"));
    DrawLine(); //====================================================================
}
//SETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUPSETUP






//MAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOP
//MAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOP
void loop() {

  SerialMonitor(); //Respond to serial commands
  
}
//MAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOP
//MAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOPMAINLOOP






//==============================================================================================
void SerialMonitor() {
  //Edit this function down below to add more serial commands to process
  //This function should be called every scan as it will only do something when data with a LF char has been read from the serial port

  //declare some variables
    #define STRINGARRAYSIZE 8
    String aSerialStringParse[STRINGARRAYSIZE]; //Create a small array to store the parsed strings 0-7
    int i;           
 
  //If a full serial data line has been recieved
  if (!sLastSerialLine.equalsIgnoreCase("")) { //A full line has been read from the Serial buffer, do something!
          
    //--Split the incoming serial data to an array of strings, where the [0]th element is the number of CSVs, and elements [1]-[X] is each CSV
    //If no Commas are detected the string placed into the [1]st array
    StringSplit(sLastSerialLine, &aSerialStringParse[0]); //Do a Stringsplit if any commas are detected
        /*
    aSerialStringParse[] will now look like this:
      aSerialStringParse[0] = the number of CSV found
      aSerialStringParse[1] = The first CSV found
      ...
      aSerialStringParse[N] = The Nth CSV found
    e.g., send 9,255 over serial comms with a CR or a LF
      aSerialStringParse:
        aSerialStringParse[0] = 1
        aSerialStringParse[1] = 9
        aSerialStringParse[2] = 255
      You can then trigger off of the 9 to say set pin 9's PWM output to the second elements value of 255
      And indeed this is programmed below as an example
    */

    /*
    MANUAL COMMANDS
     Commands can be any text that you can trigger off of.  
     It can also be paramaterized via a CSV Format with no more than eight(8) CSVs: 'Param1,Param2,Param3,Param4,Param5,Param6,Param7,Param8'
     For Example have the user type in '9,255' for a command to manually set pin 9 PWM to 255.
     */
    if (aSerialStringParse[0].toInt() == 1) {
      //Process single string serial commands without a CSV.  Add more to this section.

        //print help menu
        if (aSerialStringParse[1] == "?") {
          printHelpMenu(); //print a help menu
        } 

        //toggle debug bool
        if (aSerialStringParse[1] == "debug" ) {
          if (debug) {
            debug=0;Serial.println(F("debug off"));
          }else{
            debug=1;Serial.println(F("debug on"));
          }
        } 

    } else if(aSerialStringParse[0].toInt() > 1) {
      //Process multiple serial commands that arrived in a CSV format
      //Do something with the values by adding custom if..then statements

        //Process a command keyed off of the letter m
        if (aSerialStringParse[1]=="m") {
          Serial.print(F("m detected, value:"));
          i = aSerialStringParse[2].toInt();
          Serial.println(i);
        }

        //Process a command keyed off of the letter t
        if (aSerialStringParse[1]=="t") {
          Serial.print(F("t detected, value(s):"));
          i = aSerialStringParse[2].toInt();
          Serial.print(i);
          Serial.print(F(","));
          i = aSerialStringParse[3].toInt();
          Serial.println(i);
        }

        //Set pin 9 PWM
        if (aSerialStringParse[1] == "9") {
          Serial.print(F("pin 9 PWM changed to: "));
          i = constrain(aSerialStringParse[2].toInt(),0,255);
          Serial.println(i);
          analogWrite(9, i);
        }
        
        //Set a floating point to a number mimicing a potentiometer value
        if (aSerialStringParse[1] == "p") {
          Serial.print(F("tha variable 'Potentiometer' has been set to: "));
          Potentiometer = constrain(aSerialStringParse[2].toFloat(),0.0,100.0);
          Serial.println(Potentiometer);
        }
            
    } //end else if(aSerialStringParse[0].toInt() > 1)
     sLastSerialLine = ""; //Clear out buffer, This should ALWAYS be the last line in this if..then
  } //end if (!sLastSerialLine.equalsIgnoreCase(""))
} //END SerialMonitor()
//==============================================================================================
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();// get the new byte:
    if (inChar == '\n' || inChar == '\r' ) {//if the incoming character is a LF or CR finish the string
      sLastSerialLine = sSerialBuffer; //Transfer the entire string into the last serial line
      sSerialBuffer = ""; // clear the string buffer to prepare it for more data:
      break;
    }
    sSerialBuffer += inChar;// add it to the inputString:
  }
}
//===============================================================================================
void DrawLine() {
  Serial.println(F("==============================================================================================="));
}
//===============================================================================================
void StringSplit(String text, String *StringSplit) {


  //Place the size of the string into element 0, and the string into element 1
  StringSplit[0] = 1;
  StringSplit[1] = text;
  uint16_t CommaPosition = text.indexOf(','); //search the text for a comma

  if (CommaPosition > 0) { //Comma separated values detected, do a string split
    uint16_t StringStart=0;
    uint16_t i;

    //Perform a String split to a string Array and place the size in [0]
    for (i = 1; i <= STRINGARRAYSIZE - 1; i++) { //Start at 1 and increment only 5 times
      StringSplit[i] = text.substring(StringStart, CommaPosition); //Get the string from the start to the next comma position
      if (CommaPosition == -1) { //Check if its the last one
        StringSplit[0] = i; //Set the 0th element to the length
        break;
      }
      StringStart = CommaPosition + 1; //Set the start position of the next search based on the end position of this search
      CommaPosition = text.indexOf(',', StringStart); //Find the next comma

      if (i == STRINGARRAYSIZE-1 && CommaPosition != -1) { //if you've reached the max and there is still another comma error. We should just keep going and increasing the size of the array
        StringSplit[0] = i; //Set the 0th element to the length
        Serial.println(F("ERROR: You have too many parameters"));
        break;
      }
    }
    #ifdef DEBUG
    SerialPrintArray(&StringSplit[0]);
    #endif
  } //End (CommaPosition > 0)
}
//===============================================================================================
void SerialPrintArray(String *Array) {
  //Print out the array structure to the serial monitor
  //The array must have the length in its 0th element
  Serial.print(F("Array size ("));
  Serial.print(Array[0]);
  Serial.print(F("), elements:  "));
  for (int i = 1; i <= Array[0].toInt(); i++) {
    Serial.print(Array[i]);
    if (i < Array[0].toInt()) {Serial.print(",");}
  }
  Serial.println();
}
//===============================================================================================
void printHelpMenu() {
  Serial.println();
  DrawLine();
  Serial.println(F("HELP MENU"));
  Serial.println();
  Serial.println(F("FUNCTIONAL DESCRIPTION:"));
  Serial.println(F("     This program does something, say something about it"));
  Serial.println(F("     Here is another sentance decribing what your program does"));
  Serial.println();
  Serial.println(F("COMMANDS:"));
  Serial.println(F("     1. 'debug' - to turn debugging code on/off"));
  Serial.println(F("     2. '9,XXX' - Where XXX is an byte(0-255) to set the PWM of pin 9"));
  Serial.println(F("     3. 'p,XXX' - Where XXX is floating point value between 0.0 and 100.0 to set the variable 'Potentiometer' to."));
  Serial.println(F("     4. 'm,XXX' - Where XXX is an integer for something blah blah"));
  Serial.println(F("     5. 't,XXX,YYY' - Where XXX is an integer for something, and YYY is an integer for something blah blah"));
  Serial.println();
  Serial.println(F("For additional information please contact YOURNAME <YOUR_EMAIL@GMAIL.COM"));
  DrawLine();
  Serial.println();
}
//===============================================================================================

