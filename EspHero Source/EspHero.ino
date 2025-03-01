#include <NintendoExtensionCtrl.h>
#include <BleGamepad.h>





GuitarController guitar;
DrumController drums;
BleGamepad bleGamepad("EspHero", "MVLWVR3", 66);
BleGamepadConfiguration bleGamepadConfig;   
bool usingGuitar = false;
bool usingDrums = false;
int batteryLevel = 0;
int screenNumber = 1;

#define DEBUG true
#include "EspHeroDisplay.h"  // Include the external file


void setup() {
 
  
    if (DEBUG) Serial.begin(115200);  
    if (DEBUG) Serial.println("Initialize System...");

    displayInitialize();
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);

    bleGamepadConfig.setVid(0xD2F6);
    bleGamepadConfig.setPid(0xA916);
    bleGamepadConfig.setSerialNumber("11111116");

    bleGamepadConfig.setButtonCount(10);
    bleGamepadConfig.setHatSwitchCount(0);

    if (DEBUG) Serial.println("Initialize Guitar...");
    guitar.begin();
   // drums.begin();



    // // Try to connect either the guitar or drums
    //  while (!guitar.connect() && !drums.connect()) {
    //      if (DEBUG) Serial.println("Guitar or Drums not detected!");
    //      screenNumber = SCREENUMBER_CONNECT_INSTRUMENT;
    //      drawScreen();
    //      delay(1000);
    //  }
 // Try to connect either the guitar or drums
     while (!guitar.connect()) {
         if (DEBUG) Serial.println("Guitar or Drums not detected!");
         screenNumber = SCREENUMBER_CONNECT_INSTRUMENT;
         drawScreen();
         delay(1000);
     }


     if (guitar.connect()) {
         usingGuitar = true;
         if (DEBUG) Serial.println("Guitar connected!");
     } else if (drums.connect()) {
         usingDrums = true;
         if (DEBUG) Serial.println("Drums connected!");
     }



    if (DEBUG) Serial.println("Initialize BLE...");
    bleGamepad.begin(&bleGamepadConfig);


    while (!bleGamepad.isConnected()) {
        if (DEBUG) Serial.println("No Bluetooth Connection!");
        screenNumber = SCREENUMBER_CONNECT_BLUETOOTH;
        drawScreen();
        delay(1000);
    }


}

void loop() {
  if (bleGamepad.isConnected()) {
    boolean success = false;

    // Update based on the active instrument
    if (usingGuitar) {
        success = guitar.update();
    } else if (usingDrums) {
        success = drums.update();
    }

    if (!success) {  // Handle instrument disconnection
        if (DEBUG) Serial.println("Guitar or Drums LOST!");
        screenNumber = SCREENUMBER_LOST_INSTRUMENT;
        drawScreen();
        delay(1000);
    } else {
        screenNumber = SCREENUMBER_LETSROCK;
        drawScreen();
        if (usingGuitar) {
            handleGuitarInputs();
        } else if (usingDrums) {       
            //handleDrumJoystick();
            handleDrumInputs();
        }

        delay(20);
    }
  } else {
    if (DEBUG) Serial.println("No Bluetooth Connection!");
    screenNumber = SCREENUMBER_CONNECT_BLUETOOTH;
    drawScreen();
    delay(1000);
  }
  
}

// Handle guitar inputs
void handleGuitarInputs() {
    // Handle Strum Bar input
    if (guitar.strumUp()) {
        bleGamepad.press(1);  // Map strum up to a button
        if (DEBUG) Serial.println("Strum up");
    } else if (guitar.strumDown()) {
        bleGamepad.press(2);  // Map strum down to another button
        if (DEBUG) Serial.println("Strum down");
    } else {
        bleGamepad.release(1);
        bleGamepad.release(2);
    }

    // Handle fret buttons
    if (guitar.fretGreen()) {
        bleGamepad.press(3);
        if (DEBUG) Serial.println("Green fret pressed");
    } else {
        bleGamepad.release(3);
    }

    if (guitar.fretRed()) {
        bleGamepad.press(4);
        if (DEBUG) Serial.println("Red fret pressed");
    } else {
        bleGamepad.release(4);
    }

    if (guitar.fretYellow()) {
        bleGamepad.press(5);
        if (DEBUG) Serial.println("Yellow fret pressed");
    } else {
        bleGamepad.release(5);
    }

    if (guitar.fretBlue()) {
        bleGamepad.press(6);
        if (DEBUG) Serial.println("Blue fret pressed");
    } else {
        bleGamepad.release(6);
    }

    if (guitar.fretOrange()) {
        bleGamepad.press(7);
        if (DEBUG) Serial.println("Orange fret pressed");
    } else {
        bleGamepad.release(7);
    }

    // Handle Plus button
    if (guitar.buttonPlus()) {
        bleGamepad.press(8);
        if (DEBUG) Serial.println("Plus button pressed");
    } else {
        bleGamepad.release(8);
    }

        // Handle Plus button
    if (guitar.buttonMinus()) {
        bleGamepad.press(8);
        if (DEBUG) Serial.println("Plus button pressed");
    } else {
        bleGamepad.release(8);
    }



    // Handle whammy bar (mapped to RX axis instead of slider)
    // uint8_t whammy = guitar.whammyBar();
    // int16_t whammyMapped = map(whammy, 0, 31, -32767, 32767);  // Map whammy value to axis range
    // bleGamepad.setAxes(0, 0, 0, whammyMapped, 0, 0, 0, 0);  // Use RX axis for whammy bar
    if (DEBUG) Serial.print("Whammy bar (RX axis) position: ");
    if (DEBUG) Serial.println(whammyMapped);
}


void handleDrumInputs() {
    // Check if a drum pad was hit by evaluating its velocity
    // If the velocity is greater than 0, the pad was hit, so we trigger a button press

    // Green drum hit
    if (drums.drumGreen() && drums.velocityGreen() > 0) {
        bleGamepad.press(3);  // Map green drum to button 3 (Green fret)
        if (DEBUG) Serial.println("Green drum hit (mapped to Green fret)");
    } else {
        bleGamepad.release(3);  // Release button after hit is processed
    }

    // Red drum hit
    if (drums.drumRed() && drums.velocityRed() > 0) {
        bleGamepad.press(4);  // Map red drum to button 4 (Red fret)
        if (DEBUG) Serial.println("Red drum hit (mapped to Red fret)");
    } else {
        bleGamepad.release(4);  // Release button after hit is processed
    }

    // Yellow cymbal hit
    if (drums.cymbalYellow() && drums.velocityYellow() > 0) {
        bleGamepad.press(5);  // Map yellow cymbal to button 5 (Yellow fret)
        if (DEBUG) Serial.println("Yellow cymbal hit (mapped to Yellow fret)");
    } else {
        bleGamepad.release(5);  // Release button after hit is processed
    }

    // Blue drum hit
    if (drums.drumBlue() && drums.velocityBlue() > 0) {
        bleGamepad.press(6);  // Map blue drum to button 6 (Blue fret)
        if (DEBUG) Serial.println("Blue drum hit (mapped to Blue fret)");
    } else {
        bleGamepad.release(6);  // Release button after hit is processed
    }

    // Orange cymbal hit
    if (drums.cymbalOrange() && drums.velocityOrange() > 0) {
        bleGamepad.press(7);  // Map orange cymbal to button 7 (Orange fret)
        if (DEBUG) Serial.println("Orange cymbal hit (mapped to Orange fret)");
    } else {
        bleGamepad.release(7);  // Release button after hit is processed
    }

    // Bass pedal hit
    if (drums.bassPedal() && drums.velocityPedal() > 0) {
        bleGamepad.press(8);  // Map bass pedal to button 8
        if (DEBUG) Serial.println("Bass pedal pressed");
    } else {
        bleGamepad.release(8);  // Release button after hit is processed
    }

    // Handle Plus button hit
    if (drums.buttonPlus()) {
        bleGamepad.press(9);  // Map Plus button to button 9
        if (DEBUG) Serial.println("Plus button pressed");
    } else {
        bleGamepad.release(9);  // Release button after hit is processed
    }
}
