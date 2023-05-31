#include <Wire.h>
#define RTCADDR 0x32
#define battype 0x10  //10 primary battery only CR type

int Sec, Min, Hor, Wek, Day, Mon, Yer = 0;

int slushreg = 0;

void setup() {
  pinMode(0, OUTPUT); 
  Serial.begin(115200);
  //digitalWrite(0, LOW); 
  delay(10); 
  //digitalWrite(1, HIGH); 
  //slushreg = ByteRead(RTCADDR);
  //Serial.println(compileTime());
  // put your setup code here, to run once:
  Serial.println("Startup begin");
  //Wire.setSDA(7);
  //Wire.setSCL(6);
  Wire.setClock(400000);
  Wire.begin();
  Serial.println((ByteRead(0x1D), HEX)); 
  //SWRESET(); 
  Serial.println((ByteRead(0x1C), HEX)); 
  REGBANK();
  //InitializeHardware(); 
  //ByteWrite(0x1D, 0x02); 
  Serial.println("registers banked");
  //Manhandle(); 
  Serial.println("Manhandled");
  REGBANK(); 
  dumpreg(); 
}

void loop() {
  /*
  //ByteWrite(0x1E, 0x40); //stop clock
  Day = ByteRead(0x02);
  Sec = ByteRead(0x11);
  Min = ByteRead(0x12);
  Wek = ByteRead(0x13);
  Day = ByteRead(0x14);
  Mon = ByteRead(0x15);
  Yer = ByteRead(0x16);

  Sec = BCD2Dec(Sec);
  Min = BCD2Dec(Min);
  Hor = BCD2Dec(Hor);
  Wek = Wek;
  Day = BCD2Dec(Day);
  Mon = BCD2Dec(Mon);
  Yer = BCD2Dec(Yer);


  // put your main code here, to run repeatedly:
  Serial.print("RX1830 time output string\n");
  Serial.print(Hor);
  Serial.print(":");
  Serial.print(Min);
  Serial.print(":");
  Serial.print(Sec, BIN);
  Serial.print("     Date:  ");
  Serial.print(Wek, BIN);
  Serial.print(" ");
  Serial.print(Mon);
  Serial.print("/");
  Serial.print(Day, BIN);
  Serial.print("/");
  Serial.print(Yer);
  Serial.println("   Hope it works");


  //ByteWrite(0x1E, 0x00); //start clock
  REGBANK();
  Serial.println("\n");
  delay(1000);
  */
}

void ByteWrite(uint8_t reg, uint8_t dat) {
  Wire.beginTransmission(RTCADDR);
  Wire.write(reg);
  Wire.write(dat);
  Wire.endTransmission();
}

byte ByteRead(uint8_t reg) {
  uint8_t data = 0;
  Wire.beginTransmission(RTCADDR);
  Wire.write(reg);
  Wire.requestFrom(RTCADDR, 8);
  data = Wire.read();  //RECEIVE 1BYTE
  Wire.endTransmission();
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

void InitializeHardware(){
  ByteWrite(0x30, 0x00);     //Disable digital offset
  ByteWrite(0x1C, 0x04);     //no idea ngl
  ByteWrite(0x1D, 0x00);     //??? yea your guess as good as mine
  ByteWrite(0x1E, 0x40);     //STOP BIT LOCATED HERE (It's time to stop)
  ByteWrite(0x1F, battype);  //Primary battery type, 10 for CR2032
  Serial.println("init regs set");
  //Calendar time set below
  Sec = Dec2BCD(10);
  Min = Dec2BCD(10);
  Hor = Dec2BCD(10);   //note 24H
  Wek = DayOfWeek(6);  //0-6, indexed at
  Day = Dec2BCD(12);
  Mon = Dec2BCD(12);
  Yer = Dec2BCD(23);
  Serial.println("Bytes converted");
  //Write thos bytes
  /*
  ByteWrite(0x10, Sec);  //seconds
  ByteWrite(0x11, Min);  //Minutes
  ByteWrite(0x12, Hor);  //Hours
  ByteWrite(0x13, Wek);  //Week (day of)
  ByteWrite(0x14, Day);  //Day (of month)
  ByteWrite(0x15, Mon);  //Month
  ByteWrite(0x16, Yer);  //Year
  */
  //Time is set
  Serial.println("bytes written, start clock");
  ByteWrite(0x1E, 0x00);  //Start your clocks and GOOOOO
  VLFBIT();
}

int VLFBIT(){
  int bit = 0x02; 
  int test = ByteRead(0x1D); //Read entire flag register
  Serial.println(test, BIN);
  if(test & (1<<bit)){
  Serial.println("VLF BIT SET");
  return(1); 
  }
  else
  Serial.println("VLF BIT NOT SET");
  return(0);
}

void REGBANK(){
  Serial.print("Seconds: ");
  //Serial.print((ByteRead(0x10)), BIN);
  Serial.println((BCD2Dec(ByteRead(0x10))));
  Serial.print("Minutes: ");
   Serial.println((BCD2Dec(ByteRead(0x11))));
  Serial.print("Hours: ");
   Serial.println((BCD2Dec(ByteRead(0x12))));
  Serial.print("Week: ");
  Serial.println((ByteRead(0x13)), BIN);
  Serial.print("Day: ");
  Serial.println((ByteRead(0x14)), BIN);
  Serial.print("Month: ");
  Serial.println((ByteRead(0x15)), BIN);
  Serial.print("Year: ");
  Serial.println((ByteRead(0x16)), BIN);
  Serial.println((ByteRead(0x17)), BIN);
  Serial.println((ByteRead(0x18)), BIN);
  Serial.println((ByteRead(0x19)), BIN);
  Serial.println((ByteRead(0x1A)), BIN);
  Serial.println((ByteRead(0x1B)), BIN);
  Serial.println((ByteRead(0x1C)), BIN);
  Serial.println((ByteRead(0x1D)), BIN);
  Serial.println((ByteRead(0x1E)), BIN);
  Serial.println((ByteRead(0x1F)), BIN);
}


void SWRESET(){
  ByteRead(0x00);//dummy read
  ByteWrite(0x1E, 0x00);
  ByteWrite(0x1E, 0x80);
  ByteWrite(0x50, 0x6C);
  ByteWrite(0x53, 0x01);
  ByteWrite(0x66, 0x03);
  ByteWrite(0x6B, 0x02);
  ByteWrite(0x6B, 0x01);
  delay(125); 
  Serial.println("SW RESET DONE"); 
}

void Manhandle(){
  ByteWrite(0x10, 0x00);
  ByteWrite(0x11, 0x0F);
  ByteWrite(0x12, 0x0E);
  ByteWrite(0x13, 00001000);
  ByteWrite(0x14, 0x1F);
  ByteWrite(0x15, 0x05);
  ByteWrite(0x16, 0x17);
  ByteWrite(0x17, 0x00);
  ByteWrite(0x18, 0x00);
  ByteWrite(0x19, 0x00);
  ByteWrite(0x1A, 0x00);
  ByteWrite(0x1B, 0x00);
  ByteWrite(0x1C, 0x04); //one set 
  ByteWrite(0x1D, 0x06); //two set 
  ByteWrite(0x1E, 0x00);
  ByteWrite(0x1F, 0x00); //red set 
  ByteWrite(0x20, 0x00); //periwinkle set 
  ByteWrite(0x21, 0x00);
  ByteWrite(0x22, 0x00);
  ByteWrite(0x23, 0x00);
  ByteWrite(0x30, 0x00);
  ByteWrite(0x31, 0x00);
  return(0); 

}


void dumpreg(){
  Serial.println("Commence dump of all registers to 255"); 
  for(int i; i < 0xff; i++){
     int J = ByteRead(i); 
     Serial.print(i, HEX);
     Serial.print(":   "); 
     Serial.println(J, BIN); 
  }
  Serial.println("DUMPED LIKE THAT FIRE OUT BACK"); 
}