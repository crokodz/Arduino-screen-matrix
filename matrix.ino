//****************************************
//     Arduino Scrolling Text
//     learnelectronics
//     1 FEB 2018
//     www.youtube.com/c/learnelectronics
//     arduino0169@gmail.com
//
//     Based on sketch by Marco Colli
//*****************************************



#include <MD_Parola.h>              //MD Parola Library
#include <MD_MAX72xx.h>             //MD Maxx72XX Library
#include <SPI.h>                    //SPI library for comms


#define USE_UI_CONTROL 0            //Set to 1 to use control for speed, direction and invert

#if USE_UI_CONTROL                  //includes library if we are using the UI Control
#include <MD_KeySwitch.h>
#endif


#define DEBUG 0                     //Set to 1 for Serial debug info

#if DEBUG                           //debug info sent to serial port
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTX(x) Serial.println(x, HEX)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif


#define MAX_DEVICES 4               //number of units in the chain (max 8?)
#define CLK_PIN   13                //SPI Clk on digital 13
#define DATA_PIN  11                //SPI MOSI on digital 11
#define CS_PIN    10                //SPI CS on digital 10



// Scrolling parameters


uint8_t scrollSpeed = 400;                   // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT; //parameters from Parola library, sets scrolling from left
textPosition_t scrollAlign = PA_LEFT;       //parameters from Parola library, sets align left
uint16_t scrollPause = 100;                // delay in milliseconds

  
#define  BUF_SIZE  75                        //buffer size
char curMessage[BUF_SIZE];                  //sets current message buffer size
char newMessage[BUF_SIZE];                  //sets new message buffer size
bool newMessageAvailable = false;           //flag for if there is no new message



void readSerial(void)                       //serial read routine
{
  static char *cp = newMessage;             //declare the messsage variable as character type

  while (Serial.available())                //if data is coming from the serial port
  {
    *cp = (char)Serial.read();              //add the character to the character string
    if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE-2)) // if the message end or buffer is full
    {
      *cp = '\0';                           // if we get the NewLine character then end the string
                                            // restart the index
      cp = newMessage;                      //cp variable now equals the new message
      newMessageAvailable = true;           //flag for if there is a new message   
    }
    else                                    // move char pointer to next position
      cp++;
  }
}


MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);

void setup()
{
  Serial.begin(57600);                      //serial port begins at 57600
  Serial.print("Type message, end with Newline");



  P.begin();                                //MD Parola parameter - begin
  P.displayClear();                         //MD Parola parameter - clear the display
  P.displaySuspend(false);                  //MD Parola parameter - suspend or not?

  //MD Paroloa display msg using our predefined parameters
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

  strcpy(curMessage, "LIANA LEGASPI VALDEZ"); //prompt user to enter mesg
  newMessage[0] = '\0';                            //first spot in array NewLine
}

void loop()
{


  readSerial();                                   //read the serial port
  if (P.displayAnimate())
  {
    if (newMessageAvailable)                      //if there is a new msg
    {
      strcpy(curMessage, newMessage);             //copy current message to new message
      newMessageAvailable = false;                //set flag
    }
    P.displayReset();                             //MD Parola parameter - reset display
  }
}
