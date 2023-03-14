/* ScreamDeck code for 36-key USB keypad
   by: Mel Richardson
   date: September 1, 2022
   reference: code modified from https://www.sparkfun.com/tutorials/337
   license: MIT license. If you find this code useful, please
   feel free to use this code however you'd like, commercially
   or otherwise. Just keep this license on there whatever you do.

   This code implements a 36-key USB keypad. You can send Function keys,
   with modifiers based on button location to the computer. I ran my own
   wires, so your pinouts may vary. Multi-touch is not supported.

   The intention is to have software running on the computer to
   catch the codes and do something once recieved. It will absolutely
   run shortcuts directly to though.

                    SHIFT      | |       CTRL      | |       ALT
             -----------------------------------------------------------
Row 1, 3, 5  | F13 | F14 | F15 | | F13 | F14 | F15 | | F13 | F14 | F15 |
Row 1, 3, 5  | F16 | F17 | F18 | | F16 | F17 | F18 | | F16 | F17 | F18 |
Row 2, 4, 6  | F19 | F20 | F21 | | F19 | F20 | F21 | | F19 | F20 | F21 |
Row 2, 4, 6  | F22 | F23 | F24 | | F22 | F23 | F24 | | F22 | F23 | F24 |
             -----------------------------------------------------------
         Col    1     2     3
         Col    4     5     6
         Col    1     2     3
         Col    4     5     6
*/

#include "Keyboard.h"

// ---VARIABLES---
int buttonPins[12] = {8, 10, 9, 16, 14, 15, 2, 4, 5, 6, 3, 7}; // States which pins are connected to buttons
int modes[3] = {1, 2, 3};                                      //  States mode numbers available to allow cycling through the modes
/*
    Mode 1 = sendFunctionPress = Entire board sends modified function keys
    Mode 2 = sendButtonPress1 = Right 1/3rd are numbers from numpad, rest is sendFunctionPress
    Mode 3 = sendButtonPress2 = Special  Shortcuts
    Mode 4 = Not Used
    Mode 5 = Not Used
    Mode 6 = Not Used
    Mode 7 = Not Used
    Mode 8 = Not Used
    Mode 9 = sendTestPress = Each button sends a number to verify which col/row it is wired to
*/
String recievedButton = "00"; // Used to monitor which button was pressed
int timeout;                  // Used for timeout variable for multipress
int mode = modes[0];

// ---RUN UPON BOOTUP---

void setup()
{
  for (int i = 0; i < 12; i++)
  {
    pinMode(buttonPins[i], INPUT);     // Set all keypad pins as inputs
    digitalWrite(buttonPins[i], HIGH); // pull all keypad pins high
    pinMode(LED_BUILTIN, OUTPUT);      // Setup change mode LED indicator
  }
}

// ---CONTINUOUS RUN---

void loop()
{
  recievedButton = getRecievedButton(); // read which buttons are pressed
  if (recievedButton == "45")
  { // Change Mode Button has been pressed
    for (int i = 0; i < sizeof(modes); i++)
    {
      if (mode == modes[i])
      {
        if (mode == (sizeof(modes) - 1))
        {
          mode == modes[0];
        }
        else
        {
          mode == modes[i++];
        }
      }
    }
    // Blink to indicate new mode
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    for (int i = 0; i < mode; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
  else if (recievedButton != "00")
  { // If a button is pressed go into here
    if (mode == 1)
    {
      sendFunctionPress(recievedButton); // send the button over USB
    }
    else if (mode == 2)
    {
      sendButtonPress1(recievedButton); // send the button over USB
    }
    else if (mode == 3)
    {
      sendButtonPress2(recievedButton); // send the button over USB
    }
    else if (mode == 9)
    {
      sendTestPress(recievedButton); // send button code over USB for testing purposes
    }
    timeout = 2000;                                                // top of the repeat delay
    while ((getRecievedButton() == recievedButton) && (--timeout)) // Decrement timeout and check if key is being held down
      delayMicroseconds(1);
    while (getRecievedButton() == recievedButton)
    { // while the same button is held down
      // sendButtonPress(recievedButton);  // re-write to re-send the button press over USB
      delay(50); // 50ms repeat rate
    }
  }
}

// ---FUNCTIONS---

String getRecievedButton()
{
  int rowPins[6] = {buttonPins[0], buttonPins[1], buttonPins[2], buttonPins[3], buttonPins[4], buttonPins[5]};
  int columnPins[6] = {buttonPins[6], buttonPins[7], buttonPins[8], buttonPins[9], buttonPins[10], buttonPins[11]};
  String recievedButton = "00"; // returned string. if no button is pressed, returns "00"

  /* initialize all pins, inputs w/ pull-ups */
  for (int i = 0; i < 12; i++)
  {
    pinMode(buttonPins[i], INPUT);
    digitalWrite(buttonPins[i], HIGH);
  }

  for (int row = 0; row < 6; row++)
  {                                  // initial for loop to check all 6 rows
    pinMode(rowPins[row], OUTPUT);   // set the row pin as an output
    digitalWrite(rowPins[row], LOW); // pull the row pins low
    for (int col = 0; col < 6; col++)
    { // embedded for loop to check all 6 columns of each row
      if (!digitalRead(columnPins[col]))
      {
        recievedButton = String(((row + 1) * 10) + (col + 1)); // set the status bit of the keypad return value
      }
    }
    pinMode(rowPins[row], INPUT);     // reset the row pin as an input
    digitalWrite(rowPins[row], HIGH); // pull the row pin high
  }
  return recievedButton;
}

void sendTestPress(String recievedButton)
{
  Keyboard.begin();
  Keyboard.print(recievedButton);
  Keyboard.write(KEY_RETURN);
  Keyboard.end();
}

void sendFunctionPress(String recievedButton)
{
  Keyboard.begin();
  if (recievedButton[0] == '1' || recievedButton[0] == '2')
  {
    Keyboard.press(KEY_LEFT_SHIFT);
  }
  else if (recievedButton[0] == '3' || recievedButton[0] == '4')
  {
    Keyboard.press(KEY_LEFT_CTRL);
  }
  else if (recievedButton[0] == '5' || recievedButton[0] == '6')
  {
    Keyboard.press(KEY_LEFT_ALT);
  }

  if (recievedButton[0] == '1' || recievedButton[0] == '3' || recievedButton[0] == '5')
  {
    if (recievedButton[1] == '1')
    {
      Keyboard.press(KEY_F13);
    }
    else if (recievedButton[1] == '2')
    {
      Keyboard.press(KEY_F14);
    }
    else if (recievedButton[1] == '3')
    {
      Keyboard.press(KEY_F15);
    }
    else if (recievedButton[1] == '4')
    {
      Keyboard.press(KEY_F16);
    }
    else if (recievedButton[1] == '5')
    {
      Keyboard.press(KEY_F17);
    }
    else if (recievedButton[1] == '6')
    {
      Keyboard.press(KEY_F18);
    }
  }
  else if (recievedButton[0] == '2' || recievedButton[0] == '4' || recievedButton[0] == '6')
  {
    if (recievedButton[1] == '1')
    { // My OS does not see F19, so I changed this button
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_F13);
    }
    else if (recievedButton[1] == '2')
    {
      Keyboard.press(KEY_F20);
    }
    else if (recievedButton[1] == '3')
    {
      Keyboard.press(KEY_F21);
    }
    else if (recievedButton[1] == '4')
    {
      Keyboard.press(KEY_F22);
    }
    else if (recievedButton[1] == '5')
    {
      Keyboard.press(KEY_F23);
    }
    else if (recievedButton[1] == '6')
    { // My OS does not see F24, so I changed this button
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_F18);
    }
  }
  Keyboard.releaseAll();
  Keyboard.end();
}

void sendButtonPress1(String recievedButton)
{
  if (recievedButton[0] < '5')
  {
    sendFunctionPress(recievedButton);
  }
  else
  {
    Keyboard.begin();
    if (recievedButton == "51")
    {
      Keyboard.press(KEY_KP_1);
    }
    else if (recievedButton == "52")
    {
      Keyboard.press(KEY_KP_2);
    }
    else if (recievedButton == "53")
    {
      Keyboard.press(KEY_KP_3);
    }
    else if (recievedButton == "54")
    {
      Keyboard.press(KEY_KP_4);
    }
    else if (recievedButton == "55")
    {
      Keyboard.press(KEY_KP_5);
    }
    else if (recievedButton == "56")
    {
      Keyboard.press(KEY_KP_6);
    }
    else if (recievedButton == "61")
    {
      Keyboard.press(KEY_KP_7);
    }
    else if (recievedButton == "62")
    {
      Keyboard.press(KEY_KP_8);
    }
    else if (recievedButton == "63")
    {
      Keyboard.press(KEY_KP_9);
    }
    else if (recievedButton == "64")
    {
      Keyboard.press(KEY_KP_DOT);
    }
    else if (recievedButton == "65")
    {
      Keyboard.press(KEY_KP_0);
    }
    else if (recievedButton == "66")
    {
      Keyboard.press(KEY_KP_ENTER);
    }
    Keyboard.releaseAll();
    Keyboard.end();
  }
}

void sendButtonPress2(String recievedButton)
{
  if (recievedButton[0] < '5')
  {
    sendFunctionPress(recievedButton);
  }
  else
  {
    Keyboard.begin();
    if (recievedButton == "51")
    {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('s');
    }
    else if (recievedButton == "52")
    {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('l');
    }
    else if (recievedButton == "53")
    {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('t');
    }
    else if (recievedButton == "54")
    {
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_ARROW);
    }
    else if (recievedButton == "55")
    {
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_RIGHT_ARROW);
    }
    else if (recievedButton == "56")
    {
      Keyboard.press('6');
    }
    else if (recievedButton == "61")
    {
      Keyboard.press('7');
    }
    else if (recievedButton == "62")
    {
      Keyboard.press('8');
    }
    else if (recievedButton == "63")
    {
      Keyboard.press('9');
    }
    else if (recievedButton == "64")
    {
      Keyboard.press('.');
    }
    else if (recievedButton == "65")
    {
      Keyboard.press('0');
    }
    else if (recievedButton == "66")
    {
      Keyboard.press(KEY_RETURN);
    }
    Keyboard.releaseAll();
    Keyboard.end();
  }
}
