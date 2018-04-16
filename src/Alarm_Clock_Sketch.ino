/*  Digital & Binary Clock With Twin MAX72XX LED Matrix & RTC
                     V2.0 - 29.Dec.2017
                      Author: LAGSILVA
*/

#include <LedControl.h>                 // Library for LED Matrix - MAX72XX
#include <Wire.h>                       // Library for DS1307RTC - Pins of Arduino UNO: A4 (SDA), A5 (SCL)
#include <DS1307RTC.h>                  // Library for Real Time Clock


#define DS1307_I2C_ADDRESS 0x68         // This is the I2C address (RTC)

// Global Variables
byte hours, minutes, seconds, days, months, daysWeek, k;
byte timeMode, font, pos, pmStatus, clockType;
byte unitshours, unitsminutes, unitsseconds, tenhours, tenminutes, tenseconds, tempDez, tempUni;
byte unitdays, unitmonths, tendays, tenmonths;
unsigned long tempo;
int buttonState = 0;

/*
  Pin numbers do Arduino para conexao com o MAX72XX (matriz de LED com controlador MAX72XX)
  pin 2 is connected to the DataIn (DIN)
  pin 3 is connected to LOAD (CS)
  pin 4 is connected to the CLK (CLK)
*/

LedControl lc = LedControl(2, 4, 3, 2);  // LedControl(int dataPin, int clkPin, int csPin, int numDevices)

const int Speaker = 9;
const int stopAlarm = 12;
const int snooze = 13;

int alarmHour = 5;
int alarmMinutes = 25;
int tempAlarmHour = alarmHour;
int tempAlarmMinutes = alarmMinutes;
// Array of Digits - 0 a 9 - Size 5x3 (02 sets of fonts)

byte num[2][10][3] = {

  // Stylish Font type
  {
    {0x70, 0x88, 0x70}, // 0
    {0x00, 0xF8, 0x40}, // 1
    {0x48, 0xA8, 0x98}, // 2
    {0xF8, 0xA8, 0x88}, // 3
    {0x78, 0x20, 0xE0}, // 4
    {0xB8, 0xA8, 0xE8}, // 5
    {0x38, 0x28, 0xF8}, // 6
    {0xC0, 0xB8, 0x80}, // 7
    {0xF8, 0xA8, 0xF8}, // 8
    {0xF8, 0xA0, 0xE0}, // 9
  },

  // Conventional Font type
  {
    {0xF8, 0x88, 0xF8}, // 0
    {0x00, 0xF8, 0x40}, // 1
    {0xE8, 0xA8, 0xB8}, // 2
    {0xF8, 0xA8, 0xA8}, // 3
    {0xF8, 0x20, 0xE0}, // 4
    {0xB8, 0xA8, 0xE8}, // 5
    {0xB8, 0xA8, 0xF8}, // 6
    {0xF8, 0x80, 0x80}, // 7
    {0xF8, 0xA8, 0xF8}, // 8
    {0xF8, 0xA0, 0xE0}, // 9
  },

};


void setup() {

  Wire.begin();

  pinMode(Speaker, OUTPUT);
  pinMode(stopAlarm, OUTPUT);
  pinMode(snooze, OUTPUT);
  
  // Read datum of Time Mode (24hs or AM-PM)stored in RTC
  Wire.beginTransmission(DS1307_I2C_ADDRESS);               // Open I2C line in write mode
  Wire.write((byte)0x08);                                   // Set the register pointer to (0x08)
  Wire.endTransmission();                                   // End Write Transmission
  Wire.requestFrom(DS1307_I2C_ADDRESS, 1);                  // In this case read only 1 byte

  pos = Wire.read();                                        // Read the Time Mode and Font Type stored at RTC memory
  timeMode = pos & 1;                                       // Time Mode (24hs = 0 / AM-PM = 1)
  font = pos >> 1;                                          // Font Type (Stylish = 0 / Conventional = 1)
  clockType = pos >> 2;                                     // Clock Type (Digital = 0 / Binary =1)

  pos = (pos + 1) % 6;                                      // Change the Time Mode and Font Type when Arduino is restarted

  // Write data of Clock Mode (Decimal or Binary) in RTC
  Wire.beginTransmission(DS1307_I2C_ADDRESS);               // Open I2C line in write mode
  Wire.write((byte)0x08);                                   // Set the register pointer to (0x08)
  Wire.write(pos);                                          // Record at RTC memory the Mode of Time
  Wire.endTransmission();                                   // End Write Transmission

  // Setup of Display "0"
  lc.shutdown(0, false);                // Wakeup Display "0"
  lc.setIntensity(0, 4);                // Set the Brightness of Display ( 0 to 15)
  lc.clearDisplay(0);                   // Clear Display "0"

  // Setup of Display "1"
  //lc.shutdown(1, false);                // Wakeup Display "1"
 // lc.setIntensity(1, 4);                // Set the Brightness of Display ( 0 to 15)
 // lc.clearDisplay(1);                   // Clear Display "1"

  setSyncProvider(RTC.get);             // Function to read RTC (Real Time Clock)
  setSyncInterval(18000);             // Set the number of secondss between re-sync
  setTime(5, 23, 0, 04, 10, 2018);   // Set the Time and Date
 
}


void loop() {
  
      days = day();
      months = month();
      daysWeek = weekday();

      tempo = millis();

      while (millis() - tempo <= 3000) {             // 06 secondss to plot Time

        hours = hour();
        minutes = minute();

        if (hours > 11)
        {
          lc.setLed(0, 0, 0, true);                // Set PM status
        }

          hours = hours % 12;
          if (hours == 0) {
            hours = 12;
          }
        

        unitshours = hours % 10;
        tenhours = hours / 10;
        unitsminutes = minutes % 10;
        tenminutes = minutes / 10;
       
        // Plot Time
        for (k = 0; k < 3; k++) {

          tempDez = num[font][tenhours][k] >> 2;
          tempUni = num[font][unitshours][k] >> 2;
          lc.setRow(0, k + 5 , tempDez);
          lc.setRow(0, k + 1 , tempUni);
        }
        

        // Blink secondss
        lc.setLed(1, 7, 7, true);
        delay(500);
        lc.setLed(1, 7, 7, false);
        delay(500);

      }

      lc.clearDisplay(0);
      lc.clearDisplay(1);

       for (k = 0; k < 3; k++) {
       tempDez = num[font][tenminutes][k] >> 2;
       tempUni = num[font][unitsminutes][k] >> 2;
       lc.setRow(0, k + 5 , tempDez);
       lc.setRow(0, k + 1 , tempUni);

       }

      delay(3000);
      
      lc.clearDisplay(0);
      lc.clearDisplay(1);

      
      unitdays = days % 10;
      tendays = days / 10;
      unitmonths = months % 10;
      tenmonths = months / 10;

      // Plot Date
      for (k = 0; k < 3; k++) {

        tempDez = num[font][tendays][k] >> 2;
        tempUni = num[font][unitdays][k] >> 2;
        lc.setRow(0, k + 5 - 1, tempDez);
        lc.setRow(0, k + 1 - 1, tempUni);

      }

      delay(2000);      // 04 secondss to plot Date

      lc.clearDisplay(0);
      lc.clearDisplay(1);

      for (k = 0; k < 3; k++) {

        tempDez = num[font][tenmonths][k] >> 2;
        tempUni = num[font][unitmonths][k] >> 2;
        lc.setRow(0, k + 5 - 1, tempDez);
        lc.setRow(0, k + 1 - 1, tempUni);
      }

      delay(2000);
      
      lc.clearDisplay(0);
      lc.clearDisplay(1);

   if (hours == alarmHour && minutes == alarmMinutes)
       {
        
         while(digitalRead(stopAlarm) == LOW)
         {
          buttonState = digitalRead(stopAlarm);
          tetrisSong();
          noTone(Speaker);
          if(digitalRead(stopAlarm) == HIGH)
          {
            break;
          }
          if(digitalRead(snooze)== HIGH)
          {
            noTone(Speaker);
            alarmMinutes += 5;
            if (alarmMinutes >= 60)
            {
              alarmHour += 1;
              alarmMinutes -= 60;
            }
            if (alarmHour >= 24)
            {
              alarmHour = 0; 
            }
            break;
          }
          alarmMinutes = tempAlarmMinutes;
          alarmHour = tempAlarmHour;
         }
       }

}

// By Luke Cyca
// https://lukecyca.com
// https://github.com/lukecyca/TetrisThemeArduino


// A rest
#define _R     (0)

// Note frequencies based on http://www.phy.mtu.edu/~suits/notefreqs.html
#define _C0    (16.35)
#define _CS0   (17.32)
#define _D0    (18.35)
#define _DS0   (19.45)
#define _E0    (20.60)
#define _F0    (21.83)
#define _FS0   (23.12)
#define _G0    (24.50)
#define _GS0   (25.96)
#define _A0    (27.50)
#define _AS0   (29.14)
#define _B0    (30.87)
#define _C1    (32.70)
#define _CS1   (34.65)
#define _D1    (36.71)
#define _DS1   (38.89)
#define _E1    (41.20)
#define _F1    (43.65)
#define _FS1   (46.25)
#define _G1    (49.00)
#define _GS1   (51.91)
#define _A1    (55.00)
#define _AS1   (58.27)
#define _B1    (61.74)
#define _C2    (65.41)
#define _CS2   (69.30)
#define _D2    (73.42)
#define _DS2   (77.78)
#define _E2    (82.41)
#define _F2    (87.31)
#define _FS2   (92.50)
#define _G2    (98.00)
#define _GS2   (103.83)
#define _A2    (110.00)
#define _AS2   (116.54)
#define _B2    (123.47)
#define _C3    (130.81)
#define _CS3   (138.59)
#define _D3    (146.83)
#define _DS3   (155.56)
#define _E3    (164.81)
#define _F3    (174.61)
#define _FS3   (185.00)
#define _G3    (196.00)
#define _GS3   (207.65)
#define _A3    (220.00)
#define _AS3   (233.08)
#define _B3    (246.94)
#define _C4    (261.63)
#define _CS4   (277.18)
#define _D4    (293.66)
#define _DS4   (311.13)
#define _E4    (329.63)
#define _F4    (349.23)
#define _FS4   (369.99)
#define _G4    (392.00)
#define _GS4   (415.30)
#define _A4    (440.00)
#define _AS4   (466.16)
#define _B4    (493.88)
#define _C5    (523.25)
#define _CS5   (554.37)
#define _D5    (587.33)
#define _DS5   (622.25)
#define _E5    (659.25)
#define _F5    (698.46)
#define _FS5   (739.99)
#define _G5    (783.99)
#define _GS5   (830.61)
#define _A5    (880.00)
#define _AS5   (932.33)
#define _B5    (987.77)
#define _C6    (1046.50)
#define _CS6   (1108.73)
#define _D6    (1174.66)
#define _DS6   (1244.51)
#define _E6    (1318.51)
#define _F6    (1396.91)
#define _FS6   (1479.98)
#define _G6    (1567.98)
#define _GS6   (1661.22)
#define _A6    (1760.00)
#define _AS6   (1864.66)
#define _B6    (1975.53)
#define _C7    (2093.00)
#define _CS7   (2217.46)
#define _D7    (2349.32)
#define _DS7   (2489.02)
#define _E7    (2637.02)
#define _F7    (2793.83)
#define _FS7   (2959.96)
#define _G7    (3135.96)
#define _GS7   (3322.44)
#define _A7    (3520.00)
#define _AS7   (3729.31)
#define _B7    (3951.07)
#define _C8    (4186.01)
#define _CS8   (4434.92)
#define _D8    (4698.63)
#define _DS8   (4978.03)
#define _E8    (5274.04)
#define _F8    (5587.65)
#define _FS8   (5919.91)
#define _G8    (6271.93)
#define _GS8   (6644.88)
#define _A8    (7040.00)
#define _AS8   (7458.62)
#define _B8    (7902.13)

// beats per minute
#define BPM   (180.0)


float lead_notes[] = {
  // part 1
  _E5, _B4, _C5, _D5, _C5, _B4, _A4, _A4, _C5, _E5, _D5, _C5, _B4, _B4, _C5, _D5, _E5, _C5, _A4, _A4, _R,
  _D5, _F5, _A5, _G5, _F5, _E5, _C5, _E5, _D5, _C5, _B4, _B4, _C5, _D5, _E5, _C5, _A4, _A4, _R,

  // part 2
  _E4, _C4, _D4, _B3, _C4, _A3, _GS3, _B3,
  _E4, _C4, _D4, _B3, _C4, _E4, _A4, _A4, _GS4, _R

};
float lead_times[] = {
  // part 1
  1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
  1.5, 0.5, 1.0, 0.5, 0.5, 1.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,

  // part 2
  2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0,
  2.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0

};

float bass_notes[] = {
  // part 1
  _E2, _E3, _E2, _E3, _E2, _E3, _E2, _E3, _A1, _A2, _A1, _A2, _A1, _A2, _A1, _A2, _GS1, _GS2, _GS1, _GS2, _GS1, _GS2, _GS1, _GS2, _A1, _A2, _A1, _A2, _A1, _B2, _C3, _E3,
  _D2, _D3, _D2, _D3, _D2, _D3, _D2, _D3, _C2, _C3, _C2, _C3, _C2, _C3, _C2, _C3, _B1, _B2, _B1, _B2, _B1, _B2, _B1, _B2, _A1, _A2, _A1, _A2, _A1, _A2, _A1, _A2,

  // part 2
  _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2,
  _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2


};
float bass_times[] = {
  // part 1
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,

  // part 2
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5

};


// duration is in microseconds
void play_one_note(float frequency, unsigned long duration) {
  unsigned long period = 1000000.0/frequency;
  for (unsigned int cycles=duration/period; cycles>0; cycles--) {
    // half the time on
    digitalWrite(Speaker, HIGH);
    delayMicroseconds( period/2 );

    // half the time off
    digitalWrite(Speaker, LOW);
    delayMicroseconds( period/2 );
  }

  // If the duration wasn't a multiple of the period, delay the remainder
  delayMicroseconds(duration % period);
}

// Time (in microseconds) to spend on each note while simulating polyphony
// If this is too small, low frequency notes will be inaudible.
#define POLY_DELTA (14400)

// duration is in microseconds
void play_two_notes(float freq1, float freq2, unsigned long duration) {
    for (unsigned long t=0; t<duration; t+=2*POLY_DELTA) {
      play_one_note(freq1, POLY_DELTA);
      play_one_note(freq2, POLY_DELTA);
    }
}

int lead_note_count = sizeof(lead_notes) / sizeof(float);
int bass_note_count = sizeof(bass_notes) / sizeof(float);



void tetrisSong() {
  int curr_lead_note = 0;
  int curr_bass_note = 0;
  float curr_lead_note_time_remaining = lead_times[curr_lead_note];
  float curr_bass_note_time_remaining = bass_times[curr_bass_note];
  float lead_freq, bass_freq, note_value;
  unsigned long duration;

  while (curr_lead_note < lead_note_count && curr_bass_note < bass_note_count && buttonState == LOW) {
    buttonState = digitalRead(stopAlarm);
    buttonState = digitalRead(snooze);
    lead_freq = lead_notes[curr_lead_note];
    bass_freq = bass_notes[curr_bass_note];
    note_value = min(curr_lead_note_time_remaining, curr_bass_note_time_remaining);
    duration = note_value * 1000000 * (60.0/BPM);

    if (lead_freq > 0 && bass_freq > 0) {
       buttonState = digitalRead(stopAlarm);
       buttonState = digitalRead(snooze);
      play_two_notes(lead_freq, bass_freq, duration);
    } else if (lead_freq > 0) {
       buttonState = digitalRead(stopAlarm);
       buttonState = digitalRead(snooze);
      play_one_note(lead_freq, duration);
    } else if (bass_freq > 0) {
       buttonState = digitalRead(stopAlarm);
       buttonState = digitalRead(snooze);
      play_one_note(bass_freq, duration);
    } else {
       buttonState = digitalRead(stopAlarm);
       buttonState = digitalRead(snooze);
      delay( duration/1000 );
    }

    // Advance lead note
    curr_lead_note_time_remaining -= note_value;
    if (curr_lead_note_time_remaining < 0.001) {
      curr_lead_note++;
      curr_lead_note_time_remaining = lead_times[curr_lead_note];
    }

    // Advance bass note
     buttonState = digitalRead(stopAlarm);
     buttonState = digitalRead(snooze);
    curr_bass_note_time_remaining -= note_value;
    if (curr_bass_note_time_remaining < 0.001) {
      curr_bass_note++;
      curr_bass_note_time_remaining = bass_times[curr_bass_note];
    }
        buttonState = digitalRead(stopAlarm);
        buttonState = digitalRead(snooze);
  }

}


