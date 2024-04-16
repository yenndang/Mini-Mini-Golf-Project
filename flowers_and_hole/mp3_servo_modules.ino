#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// Tell Arduino which pins are wired to MP3 player (remember RX on Arduino is TX on MP3 player)
SoftwareSerial DF1201SSerial(2, 3);  //Arduino RX, Arduino TX
int shakeState = 0; // Variable to store the shake sensor state
int beamState = 0; // Variable to store the breakbeam sensor state
int sensorState = 0;
#define SERVO_PIN 9  // Define the pin connected to the servo
#define DELAY_TIME 500  // Define delay time between movements
#define NUM_OF_MOVEMENTS 5  // Define the number of back and forth movements
Servo myServo;  // Create a servo object

unsigned long previousMillis = 0;  // Stores the last time the servo was updated

// object that will handle all comms to MP3 player
DFRobot_DF1201S DF1201S;

void setup(void){
  pinMode(13, OUTPUT);
  pinMode(8, INPUT_PULLUP);  // Set up the shake sensor pin as input
  pinMode(7, INPUT_PULLUP);  // Set up the breakbeam sensor pin as input
  // Initialize MP3 player communications
  DF1201SSerial.begin(115200);
  myServo.attach(SERVO_PIN);  // Attach the servo to the specified pin
  while(!DF1201S.begin(DF1201SSerial)){
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }
  DF1201S.setVol(25);  // set a low-ish volume (valid values from 0 to 30)
  delay(500);
  DF1201S.switchFunction(DF1201S.MUSIC); // be a music player, not a USB drive
  delay(500);
  DF1201S.setPlayMode(DF1201S.SINGLE); // tell it to play just one file at a time (do this after switchFunction)
  delay(500);
}

// Original version where servo starts after the breakbeam
void loop(){
  digitalWrite(13, LOW);
  shakeState = digitalRead(8);  // Read the state of the shake sensor
  beamState = digitalRead(7);  // Read the state of the breakbeam sensor
  // sensorState = digitalRead(9);  // Read the state of the servo 
  if (shakeState == LOW) {
      int result = DF1201S.playFileNum(2); // Start playing the file
      if (result == 1) {
          digitalWrite(13, HIGH); // Indicate that playback started
          delay(2000); // Wait for 2 seconds while the file is playing
          DF1201S.pause(); // Pause the playback
          digitalWrite(13, LOW); // Optionally, turn off the LED to indicate playback has paused
      }
      //delay(4000); // Wait for some time before the next loop iteration
  }

    // Handle break beam sensor's state
  if (beamState == LOW) {  // Check if the break beam sensor is interrupted
    int result = DF1201S.playFileNum(1);
    // if (result == 1) { // omiting the delay effect for instantaneous interaction
    //   digitalWrite(13, HIGH);
    //   delay(5000);  // Play for 5 seconds
    //   DF1201S.pause();
    //   digitalWrite(13, LOW);
    // }

    // Move servo back and forth
    for (int i = 0; i < NUM_OF_MOVEMENTS; i++) {
      myServo.write(0);  // Move servo to one extreme position
      delay(DELAY_TIME);
      myServo.write(180);  // Move servo to the other extreme position
      delay(DELAY_TIME);
    }
  }
}
// void loop() { // This is buggy. Attempted to minimize the delay time so both sound and servo may work the same time
// //                Behavior: The servo runs when the beam breaks, the sound is played when the beam is not broken.
//   unsigned long currentMillis = millis();  // Current time in milliseconds

//   // Reading the shake sensor state is independent of the beam state
//   shakeState = digitalRead(8);
//   if (shakeState == LOW) {
//     int result = DF1201S.playFileNum(2);
//     if (result == 1) {
//       digitalWrite(13, HIGH); // Indicate that playback started
//       // No delay here; sound is playing asynchronously
//     } else {
//       digitalWrite(13, LOW);
//     }
//   }

//   // Reading the break beam sensor state
//   beamState = digitalRead(7);
//   if (beamState == LOW) {
//     // Trigger sound effect
//     int result = DF1201S.playFileNum(1);
//     if (result == 1) {
//       digitalWrite(13, HIGH); // Indicate that playback started
//     } else {
//       digitalWrite(13, LOW);
//     }

//     // Move the servo if the break beam is interrupted
//     if (currentMillis - previousMillis >= DELAY_TIME) {
//       previousMillis = currentMillis; // Save the last time you moved the servo

//       static int position = 0; // Keeps track of the servo's position
//       if (position == 0) {
//         myServo.write(0);
//         position = 180;
//       } else {
//         myServo.write(180);
//         position = 0;
//       }
//     }
//   } else {
//     digitalWrite(13, LOW); // Turn off LED if the beam is not broken
//   }
// }


//   // // check if the sensor beam is broken
//   // // if it is, the sensorState is LOW:
//   // if (sensorState == LOW) {     
//   //   // move servo back and forth
//   //   for (int i = 0; i < NUM_OF_MOVEMENTS; i++) {
//   //     //Move servo to one extreme position
//   //     myServo.write(0);
//   //     delay(DELAY_TIME);
//   //     // Move servo to the other extreme position
//   //     myServo.write(180);
//   //     delay(DELAY_TIME);
//   //   }
//   // }
// }

// // void loop(){
// //   digitalWrite(13, LOW);
// //   buttonState = digitalRead(8);
// //   if (buttonState == LOW) {
// //       int result = DF1201S.playFileNum(2); // turn on LED if it play says it worked
// //       if (result == 1) { // turn on Arduino LED if board says it worked
// //         digitalWrite(13, HIGH);
// //       }
// //       delay(4000);
// //   }

// // }