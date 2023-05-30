#include <Wire.h>

#define RTCADDR 0x32

#define battype 0x10  //10 primary battery only CR type

byte Sec,Min,Hor,Wek,Day,Mon,Yer = 0; 

byte slushreg = 0;

void setup() {
  Serial.begin(115200);
  //delay(35);  //wait >30ms for instancing
  //slushreg = ByteRead(RTCADDR);
  //Serial.println(compileTime());
  // put your setup code here, to run once:
  Serial.println("Startup begin");

  ByteWrite(0x30, 0x00);     //Disable digital offset
  ByteWrite(0x1C, 0x04);     //no idea ngl
  ByteWrite(0x1D, 0x00);     //??? yea your guess as good as mine
  ByteWrite(0x1E, 0x40);     //STOP BIT LOCATED HERE (It's time to stop)
  ByteWrite(0x1F, battype);  //Primary battery type, 10 for CR2032
  Serial.println("init regs set");
  //Calendar time set below
  Sec = Dec2BCD(10);
  Min = Dec2BCD(10);
  Hor = Dec2BCD(10);  //note 24H
  Wek = DayOfWeek(6);    //0-6, indexed at
  Day = Dec2BCD(12);
  Mon = Dec2BCD(12);
  Yer = Dec2BCD(23);
  Serial.println("Bytes converted");
  //Write thos bytes
  ByteWrite(0x10, Sec);  //seconds
  ByteWrite(0x11, Min);  //Minutes
  ByteWrite(0x12, Hor);  //Hours
  ByteWrite(0x13, Wek);  //Week (day of)
  ByteWrite(0x14, Day);  //Day (of month)
  ByteWrite(0x15, Mon);  //Month
  ByteWrite(0x16, Yer);  //Year
  //Time is set
  Serial.println("bytes written, start clock");
  ByteWrite(0x1E, 0x00);  //Start your clocks and GOOOOO
}

void loop() {
  // put your main code here, to run repeatedly:
  Sec = ByteRead(0x10); 
  Min = ByteRead(0x11); 
  Hor = ByteRead(0x12); 
  Wek = ByteRead(0x13);
  Day = ByteRead(0x14);
  Mon = ByteRead(0x15);
  Yer = ByteRead(0x16); 

  Sec = BCD2Dec(Sec);  
  Min = BCD2Dec(Sec);
  Hor = BCD2Dec(Sec);
  Wek = Wek;
  Day = BCD2Dec(Sec);
  Mon = BCD2Dec(Sec);
  Yer = BCD2Dec(Sec);

  Serial.print("RX1830 time output string\n"); 
  Serial.print(Hor); 
  Serial.print(":"); 
  Serial.print(Min); 
  Serial.print(":");
  Serial.print(Sec);
  Serial.print("     Date:  "); 
  Serial.print(Mon); 
  Serial.print(":"); 
  Serial.print(Day); 
  Serial.print(":");
  Serial.print(Yer);
  Serial.println("   Hope it works");
  delay(1000);
  
}

void ByteWrite(uint8_t reg, uint8_t dat) {
  Wire.beginTransmission(RTCADDR);
  Wire.write(reg);
  Wire.write(dat);
  Wire.endTransmission(false);
}

byte ByteRead(uint8_t reg) {
  uint8_t data = 0;
  Wire.beginTransmission(RTCADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(RTCADDR, 1);
  data = Wire.read();  //RECEIVE 1BYTE
  return data;
}

uint8_t Dec2BCD(uint8_t N) {
  return N + 6 * (N / 10);
}

uint8_t BCD2Dec(uint8_t N) {
  return N - 6 * (N >> 4);
}

uint8_t DayOfWeek(uint8_t day) {
  byte temphold = 0x01;
  if (day > 1 && day < 8) {
    for (int i; i < day; i++) {
      temphold << 1;
    }
    return temphold;
  } else
    ;
  return temphold;
}


