#include <LiquidCrystal.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN A15
#define GASPIN A8
#define DHTTYPE DHT11
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11



LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
// LiquidCrystal lcd(22, 24, 26, 28, 30, 32);
SoftwareSerial sim(23, 25);


int _timeout;
String _buffer;
// String number = "+971585334599";  Dhanesh's Number
String number = "+971528324543";
int i;
int gas_val;
int gas_initial;
String gas;
String humid;
String temperature;
int humidity_i;
int temperature_i;
char gas1[10];
char humid1[10];
char temperature1[10];
bool emergency = 0;

char readhr[17];
char readi[17];
char reade[17];
char readc[17];

byte eye1_a[8] = {
  0b00000,
  0b00001,
  0b00010,
  0b00100,
  0b01000,
  0b01000,
  0b10000,
  0b10000
};

byte eye1_b[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b11111
};

byte eye1_c[8] = {
  0b00000,
  0b10000,
  0b01000,
  0b00100,
  0b00010,
  0b00010,
  0b00001,
  0b00001
};


byte eye1_d[8] = {
  0b00001,
  0b00001,
  0b00010,
  0b00010,
  0b00100,
  0b01000,
  0b10000,
  0b00000
};

byte eye1_e[8] = {
  0b11111,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
byte eye1_f[8] = {
  0b10000,
  0b10000,
  0b01000,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00000
};


byte eye2_a[8] = {
  0b00000,
  0b00001,
  0b00011,
  0b00111,
  0b01111,
  0b01111,
  0b11111,
  0b11111
};

byte eye2_b[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b11111
};

byte eye2_c[8] = {
  0b00000,
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11111,
  0b11111
};


byte eye2_d[8] = {
  0b00001,
  0b00001,
  0b00010,
  0b00010,
  0b00100,
  0b01000,
  0b10000,
  0b00000
};

byte eye2_e[8] = {
  0b11111,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
byte eye2_f[8] = {
  0b10000,
  0b10000,
  0b01000,
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b00000
};


char send[5];
String str= "N";
int light;

int i_count = 0;

void SendMessage() {
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r");  //Mobile phone number to send message
  delay(200);
  String SMS = "Hello, how are you? greetings from miliohm.com admin";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);  // ASCII code of CTRL+Z
  delay(200);
  _buffer = _readSerial();
}
void RecieveMessage() {
  Serial.println("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay(200);
  sim.println("AT+CNMI=1,2,0,0,0");  // AT Command to receive a live SMS
  delay(200);
  Serial.write("Unread Message done");
}
String _readSerial() {
  _timeout = 0;
  while (!sim.available() && _timeout < 12000) {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
void callNumber() {
  sim.print(F("ATD"));
  sim.print(number);
  sim.print(F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(DHTPIN, INPUT);
  pinMode(GASPIN, INPUT);
  pinMode(IN1, OUTPUT);  //before useing io pin, pin mode must be set first
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(A9, INPUT);
  pinMode(24, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(30, OUTPUT);

  lcd.createChar(0, eye1_a);
  lcd.createChar(1, eye1_b);
  lcd.createChar(2, eye1_c);
  lcd.createChar(3, eye1_d);
  lcd.createChar(4, eye1_e);
  lcd.createChar(5, eye1_f);

  // lcd.createChar(6, eye2_a);
  // lcd.createChar(7, eye2_b);
  // lcd.createChar(8, eye2_c);
  // lcd.createChar(9, eye2_d);
  // lcd.createChar(10, eye2_e);
  // lcd.createChar(11, eye2_f);
  
  _buffer.reserve(50);
  // Serial.println("System Started...");
  sim.begin(9600);
  delay(1000);
  // Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");

  gas_val = analogRead(A8);

  for(i=0; i<=10; i++){
    gas_initial = analogRead(A8);
    humidity_i = dht.readHumidity();
    temperature_i = dht.readTemperature();
    delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  gas_val = analogRead(A8);
  int humidity = dht.readHumidity();
  int temp = dht.readTemperature();
  
  light = analogRead(A9);

  if(light > 200){
    analogWrite(24, 0);
    analogWrite(26, 0);
    analogWrite(28, 0);
    analogWrite(30, 0);
  }
  if(light < 200 && light > 100){
    analogWrite(24, 80);
    analogWrite(26, 80);
    analogWrite(28, 80);
    analogWrite(30, 80);
  }
  if(light < 100 && light > 50){
    analogWrite(24, 100);
    analogWrite(26, 100);
    analogWrite(28, 100);
    analogWrite(30, 100);

  }
  if(light<50 && light >  0){
    analogWrite(24, 200);
    analogWrite(26, 200);
    analogWrite(28, 200);
    analogWrite(30, 200);
  }
  if(light == 0){
    analogWrite(24, 255);
    analogWrite(26, 255);
    analogWrite(28, 255);
    analogWrite(30, 255);
  }




  Serial.readBytesUntil('*', readi, 10);
  Serial.readBytesUntil('#', reade, 10);
  // Serial.readBytesUntil('@', readhr, 10);
  Serial.readBytesUntil('!', readc, 10);


  String hr_s = String(readhr);
  int heartrate = hr_s.toInt();

  // Serial.println(readi);
  // Serial.println(reade);
  // Serial.println(heartrate);
  // Serial.println(readc);


  analogWrite(29, 255);
  analogWrite(31, 255);

  // lcd.setCursor(0, 0);
  // lcd.write("h: ");
  // lcd.setCursor(3, 0);
  // lcd.print(humidity);

  // lcd.setCursor(0, 1);
  // lcd.write("t: ");
  // lcd.setCursor(3, 1);
  // lcd.print(temp);
  // lcd.setCursor(11, 0);
  // lcd.print("gas:");
  // lcd.setCursor(12, 1);
  // lcd.print(gas_val);

  i_count = i_count + 1;
  Serial.println(i_count);
  if(i_count == 3){
    i_count = 0;
    lcd.createChar(0, eye2_a);
    lcd.createChar(1, eye2_b);
    lcd.createChar(2, eye2_c);
    lcd.createChar(3, eye2_d);
    lcd.createChar(4, eye2_e);
    lcd.createChar(5, eye2_f);

    lcd.clear();
    lcd.setCursor(2,0);
    lcd.write(byte(0));
    lcd.setCursor(3,0); // Display eye 1
    lcd.write(byte(1));
    lcd.setCursor(4,0);
    lcd.write(byte(2));
    lcd.setCursor(4,1); // Display eye 1
    lcd.write(byte(3));
    lcd.setCursor(3,1);
    lcd.write(byte(4));
    lcd.setCursor(2,1); // Display eye 1
    lcd.write(byte(5));


    lcd.setCursor(11,0);
    lcd.write(byte(0));
    lcd.setCursor(12,0); // Display eye 1
    lcd.write(byte(1));
    lcd.setCursor(13,0);
    lcd.write(byte(2));
    lcd.setCursor(13,1); // Display eye 1
    lcd.write(byte(3));
    lcd.setCursor(12,1);
    lcd.write(byte(4));
    lcd.setCursor(11,1); // Display eye 1
    lcd.write(byte(5));
    delay(750);
    lcd.createChar(0, eye1_a);
    lcd.createChar(1, eye1_b);
    lcd.createChar(2, eye1_c);
    lcd.createChar(3, eye1_d);
    lcd.createChar(4, eye1_e);
    lcd.createChar(5, eye1_f);
    
  }
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.write(byte(0));
  lcd.setCursor(3,0); // Display eye 1
  lcd.write(byte(1));
  lcd.setCursor(4,0);
  lcd.write(byte(2));
  lcd.setCursor(4,1); // Display eye 1
  lcd.write(byte(3));
  lcd.setCursor(3,1);
  lcd.write(byte(4));
  lcd.setCursor(2,1); // Display eye 1
  lcd.write(byte(5));

  lcd.setCursor(11,0);
  lcd.write(byte(0));
  lcd.setCursor(12,0); // Display eye 1
  lcd.write(byte(1));
  lcd.setCursor(13,0);
  lcd.write(byte(2));
  lcd.setCursor(13,1); // Display eye 1
  lcd.write(byte(3));
  lcd.setCursor(12,1);
  lcd.write(byte(4));
  lcd.setCursor(11,1); // Display eye 1
  lcd.write(byte(5));
  
  delay(1000);
  
  // Serial.print("humidity: ");
  // Serial.println(humidity);
  // Serial.print("Temp: ");
  // Serial.println(temp);
  // Serial.print("gas: ");
  // Serial.println(gas_val);
  gas = gas_val;
  humid = humidity;
  temperature = temp;
  gas.toCharArray(gas1, 10);
  humid.toCharArray(humid1, 10);
  temperature.toCharArray(temperature1, 10);
  // Serial.print(gas_val);
  // Serial.print("_");
  // Serial.print(humid1);
  // Serial.print(".");
  // Serial.print(temperature1);
  // Serial.print("#");
  // Serial.print(emergency);
  // Serial.print("!");
  // Serial.println("");

  // emergency = 0;

  // if (gas_val > 80){
  //   switch (gas_val)
  //   {
  //     case 300:
  //       SendMessage();
  //       break;
  //     case 20:
  //       RecieveMessage();
  //       break;
  //     case 100:
  //       // callNumber();
  //       Serial.println("called");
  //       break;

  //   }
  // }


  // if (sim.available() > 0){
  //   Serial.write(sim.read());
  // }

  // warn("CO2 rising.. please make sure to be safe");

  if(gas_val - gas_initial > 40){
    emergency = 1;
    warn("High rise in CO2");
    str = "C";
    str.toCharArray(send, 2);
    
  }
  if(humidity - humidity_i > 30){
    emergency = 1;
    warn("Humidity High");
    str = "H";
    str.toCharArray(send, 2);
  }
  if(temp - temperature_i > 3){
    emergency = 1;
    warn("Temperature High");
    str = "T";
    str.toCharArray(send, 2);
  }
  Serial.println(gas_val);
  Serial.println(temp);
  Serial.print(humidity);
  Serial.print("ht");

  if(gas_val - gas_initial > 40 && humidity - humidity_i > 30){
    str = "Q";
    str.toCharArray(send, 2);
  }
  if(gas_val - gas_initial > 40 && temp - temperature_i > 3){
    str = "W";
    str.toCharArray(send, 2);
  }
  if(humidity - humidity_i > 30 && temp - temperature_i > 3){
    str = "R";
    str.toCharArray(send, 2);
    if(gas_val - gas_initial > 40){
      str = "E";
      str.toCharArray(send, 2);
    }
  }
  
  // if(temp - temperature_i > 10){
  //   emergency = 1;
  // }
  // if(heartrate > 140){
  //   emergency = 1;
  //   warn("Heartrate High");
  // }
  // if(heartrate > 0 && heartrate < 50){
  //   emergency = 1;
  //   warn("Heartrate Low");
  // }
  if(String(reade) == "HELP Detec"){
    emergency = 1;
    warn("EMERGENCY!");
    str = "E";
    str.toCharArray(send, 2);
  }
  if(String(readi) == "Detected"){
    emergency = 1;
    warn("Intrusion!");
    str = "I";
    str.toCharArray(send, 2);
  }
  Serial.print(send);
  Serial.println(".");
  

  if(String(readc) == "F"){
    forward();
    delay(800);
    stop();

  }
  if(String(readc) == "B"){
    back();
    delay(800);
    stop();

  }if(String(readc) == "R"){
    right();
    delay(2000);
    stop();

  }if(String(readc) == "L"){
    left();
    delay(2000);
    stop();

  }


  readi[10] = {0};
  readhr[10]= {0};
  reade[10]= {0};
  readc[10]= {0};
  send[2] = {0};
  memset(&(readi[0]), 0, 14);
  memset(&(readhr[0]), 0, 14);
  memset(&(reade[0]), 0, 14);
  memset(&(readc[0]), 0, 14);
  memset(&(send[0]), 0, 5);
  str = "N";
  str.toCharArray(send, 2);
}

void warn(String warning){
  lcd.clear();
  lcd.setCursor(0, 0);
  int size = strlen(warning.c_str()) - 16;
  lcd.print(warning);
  // for(i = 0; i <= size; i++){
  //   lcd.scrollDisplayLeft();
  //   delay(250);
  // }
  // digitalWrite(51, HIGH);
  // delay(100);
  // digitalWrite(51, LOW);
  // delay(200);
  // digitalWrite(51, HIGH);
  // delay(100);
  // digitalWrite(51, LOW);
  // delay(200);
  // lcd.clear();
  // digitalWrite(51, HIGH);
  // delay(100);
  // digitalWrite(51, LOW);
  // delay(200);
  // digitalWrite(51, HIGH);
  // delay(100);
  // digitalWrite(51, LOW);
  // delay(200);
  // lcd.clear();
  // digitalWrite(51, HIGH);
  // delay(100);
  // digitalWrite(51, LOW);
  // delay(200);
  // digitalWrite(51, HIGH);
  // delay(100);
  // digitalWrite(51, LOW);
  // delay(200);
  lcd.clear();
}


void stop() {
  digitalWrite(ENA, LOW);    //enable L298n A channel
  digitalWrite(ENB, LOW);    //enable L298n B channel
  digitalWrite(IN1, LOW);    //set IN1 hight level
  digitalWrite(IN2, LOW);     //set IN2 low level
  digitalWrite(IN3, LOW);     //set IN3 low level
  digitalWrite(IN4, LOW);    //set IN4 hight level
  Serial.println("Forward");  //send message to serial monitor
}

void forward() {
  digitalWrite(ENA, HIGH);    //enable L298n A channel
  digitalWrite(ENB, HIGH);    //enable L298n B channel
  digitalWrite(IN1, HIGH);    //set IN1 hight level
  digitalWrite(IN2, LOW);     //set IN2 low level
  digitalWrite(IN3, LOW);     //set IN3 low level
  digitalWrite(IN4, HIGH);    //set IN4 hight level
  Serial.println("Forward");  //send message to serial monitor
}

void back() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Left");
}

void right() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

float analogToPPM(int sensorValue) {
  // Replace these values with your calibration data
  float RS_R0 = 2.5;             // Ratio of RS (Sensor Resistance) to R0 (Ro in clean air)
  float CO2Curve = 116.6020682;  // CO2 sensor curve
  float ppm = CO2Curve * pow(((sensorValue / RS_R0) - 1), -2.769034857);

  // You may need to fine-tune the curve and parameters for your specific sensor

  return ppm;
}
