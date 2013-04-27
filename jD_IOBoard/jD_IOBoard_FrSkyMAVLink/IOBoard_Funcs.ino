/*
  jD-IOBoard general funcion calls
*/

// Roll all outputs one time one by one and have delay XX ms between status changes
void SlowRoll(int FlashDly) {
  for(int pos = 0; pos <= 5; pos ++ ) {
    digitalWrite(Out[pos], EN);
    delay(FlashDly);
    digitalWrite(Out[pos], DI);
    delay(FlashDly);   
  }
}

// Switch all outputs ON
void AllOn() {
   for(int looper = 0; looper <= 5; looper++) {
    digitalWrite(Out[looper], EN);
  }  
}

// Switch all outputs OFF
void AllOff() {
   for(int looper = 0; looper <= 5; looper++) {
    digitalWrite(Out[looper], DI);
  }  
}

#ifdef HEARTBEAT
void HeartBeat() {
  c_hbMillis = millis();
  if(c_hbMillis - p_hbMillis > d_hbMillis) {
    // save the last time you blinked the LED 
    p_hbMillis = c_hbMillis;   
    // if the LED is off turn it on and vice-versa:
      if (ledState == LOW)
        ledState = HIGH;
      else
        ledState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState); 
    messageCounter++;   
  }
}
#endif 

 
// Our generic flight modes for ArduCopter & ArduPlane
void CheckFlightMode() {
  byte loopPos;
  
  if(apm_mav_type == 2) { // ArduCopter MultiRotor or ArduCopter Heli
    if(iob_mode == 0) flMode = STAB;   // Stabilize
    if(iob_mode == 1) flMode = ACRO;   // Acrobatic
    if(iob_mode == 2) flMode = ALTH;   // Alt Hold
    if(iob_mode == 3) flMode = AUTO;   // Auto
    if(iob_mode == 4) flMode = GUID;   // Guided
    if(iob_mode == 5) flMode = LOIT;   // Loiter
    if(iob_mode == 6) flMode = RETL;   // Return to Launch
    if(iob_mode == 7) flMode = CIRC;   // Circle
    if(iob_mode == 8) flMode = POSI;   // Position
    if(iob_mode == 9) flMode = LAND;   // Land
    if(iob_mode == 10) flMode = OFLO;  // OF_Loiter
  } else if(apm_mav_type == 1) { // ArduPlane
    if(iob_mode == 2 ) flMode = STAB;  // Stabilize
    if(iob_mode == 0) flMode = MANU;   // Manual
    if(iob_mode == 12) flMode = LOIT;  // Loiter
    if(iob_mode == 11 ) flMode = RETL; // Return to Launch
    if(iob_mode == 5 ) flMode = FBWA;  // FLY_BY_WIRE_A
    if(iob_mode == 6 ) flMode = FBWB;  // FLY_BY_WIRE_B
    if(iob_mode == 15) flMode = GUID;  // GUIDED
    if(iob_mode == 10 ) flMode = AUTO; // AUTO
    if(iob_mode == 1) flMode = CIRC;   // CIRCLE
  }
  
#ifndef NEWPAT  
  patt = flMode + 1;
#else 
  while(flMode != readEEPROM(mbind01_ADDR + (loopPos))) {
   loopPos = loopPos + 2;
  }    
  pattByteA = readEEPROM(pat01_ADDR + loopPos);
  pattByteB = readEEPROM(pat01_ADDR + (loopPos + 1));
#endif  
}

// Update main pattern
void RunPattern() {
#ifndef NEWPAT  
   digitalWrite(REAR, flight_patt[patt][patt_pos]);
#else
      if(patt_pos >= 0 && patt_pos <= 7) {
        if(getLBit(pattByteA, patt_pos)) 
          digitalWrite(REAR, EN);
        else
          digitalWrite(REAR, DI);
      } else {
        if(getLBit(pattByteB, patt_pos - 8)) 
          digitalWrite(REAR, EN);
        else
          digitalWrite(REAR, DI);
      }
#endif
}

// Clear main pattern
void ClearPattern() {
   digitalWrite(REAR, 0);
}

// Updating base leds state
void updateBase() {
   digitalWrite(LEFT, le_patt[LeRiPatt][patt_pos]);
   digitalWrite(RIGHT, ri_patt[LeRiPatt][patt_pos]);
}

// Reads current state of high power output and save them to parameter
void GetIO() {
  for(int pos = 0; pos <= 5; pos++) {
   IOState[pos] = digitalRead(Out[pos]);
  }
}

// Recalls save value for highpower IO states and outputs them back
void PutIO() {
  for(int pos = 0; pos <= 5; pos++) {
   digitalWrite(Out[pos], IOState[pos]);
  }
}

// Checking if BIT is active in PARAM, return true if it is, false if not
byte isBit(byte param, byte bitfield) {
 if((param & bitfield) == bitfield) return 1;
  else return 0;  
}

void updatePWM() {
    curPwm = millis();
    if(curPwm - prePwm > 5) {
      // save the last time you blinked the LED 
      prePwm = curPwm;
    if (pwm1dir) {
      pwm1++;
    } else pwm1--;
    if(pwm1 >= 255 && pwm1dir == 1) pwm1dir = 0;
    if(pwm1 <= 20 && pwm1dir == 0) pwm1dir = 1;
    analogWrite(FRONT, pwm1);
    }
}

boolean getLBit(byte Reg, byte whichBit) {
  boolean State;
  switch(whichBit) {
    case 0:
     State = Reg & (1 << 7);
     return State;
     break;
    case 1:
     State = Reg & (1 << 6);
     return State;
     break;
    case 2:
     State = Reg & (1 << 5);
     return State;
     break;
    case 3:
     State = Reg & (1 << 4);
     return State;
     break;
    case 4:
     State = Reg & (1 << 3);
     return State;
     break;
    case 5:
     State = Reg & (1 << 2);
     return State;
     break;
    case 6:
     State = Reg & (1 << 1);
     return State;
     break;
    case 7:
     State = Reg & (1 << 0);
     return State;
     break;
  }
}

