#include <Arduino.h>
#include "password.h"

#define INPUT_SIZE 64
char input[INPUT_SIZE];
uint8_t input_idx = 0;
bool char_limit = 0;
bool puzzle_started = 0;
/* CURRENT PASSWORD */
const char password[] = {"patricia"};

bool password_update(){
  if(!puzzle_started){
    puzzle_started++;
    Serial.println("Password puzzle started. Please enter password:");        
  }
  bool password_correct = 0;
  while (Serial.available() > 0) { /* Reads when character is available */
    char c = Serial.read(); /* Read next input character */
    
    // End of input password
    if (c == '\n') {
      input[input_idx] = '\0';

      // Compare entered password with correct password
      if(strcmp(input, password))
        Serial.println("Incorrect");
      else{
        Serial.println("Login successful, welcome Henning. The magic code is serial_puzzle_key");
        password_correct++;
        puzzle_started=0;
      }
      // Reset input for next attempt
      input_idx = 0;
    }
    // 
    else if (c != '\r' && input_idx < INPUT_SIZE - 1) {
      input[input_idx++] = c;
    }
    if(input_idx == INPUT_SIZE - 1 && !char_limit){
      Serial.println("\nYou have passed the characterlimit of ");
      Serial.println(INPUT_SIZE);
      char_limit++;
    }
  }
  return password_correct;
}