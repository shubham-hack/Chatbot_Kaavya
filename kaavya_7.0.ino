/*Kaavya is a friendly chatbot she can tell you the current location of devices she can talk with you she can sing for you and respond on your different language
She can make you happy by telling jokes and riddles and funny chutkulas of hindi. There is a display and a GSM and GPRS tracker*/
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "BluetoothSerial.h"
#include <TinyGPS++.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino resetpin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


SoftwareSerial mySoftwareSerial(26, 25); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
void displayInfo();

//GSM Module RX pin to ESP32 2
//GSM Module TX pin to ESP32 4
#define rxPin 4
#define txPin 2
HardwareSerial sim800(1);

int RXPin = 16;
int TXPin = 17;

int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);


/* Check if Bluetooth configurations are enabled in the SDK
If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

String voice; // to read the voice commands
int RandomNumber; // to play random audio file

String smsStatus,senderNumber,receivedDate,msg;
boolean isReply = false;
const String PHONE = "+91XXXXXXXXXX";


void setup()
{
    // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  // display.clearDisplay();

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  // DF/SD card code begin here
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  // Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30);  // Set volume (0 to 30)
  // DF/SD card code ends here

  SerialBT.begin("Kaavya chatbot"); // Bluetooth name, if no name is given, default is 'ESP32'
  SerialBT.begin();
  RandomNumber=random(1,3);  
  myDFPlayer.play(RandomNumber);  // Play the first mp3
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE);        
  display.setCursor(1,2);             
  display.println("Not Connected!");
  display.display();

  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";

  sim800.println("AT+CMGF=1"); //SMS text mode
  delay(1000);
  sim800.println("AT+CMGD=1,4"); //delete all saved SMS
  delay(1000);  
  
}

void loop()
{
  voice=""; 

  if (Serial.available())
  {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available())
  {
    display.clearDisplay();
    display.setCursor(1,2);
    display.println("This is kavya");
    display.display();
    voice = SerialBT.readString();
    voice.toLowerCase();
    Serial.println(voice);
  }
  
    while (gpsSerial.available() > 0){
    if (gps.encode(gpsSerial.read())){
      // if (voice.indexOf("my location") > 0){
      //   displayInfo();
      // }
        }
    }


  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
  // GSM begin
  while(sim800.available()){
    parseData(sim800.readString());
  }

  while(Serial.available())  {
    sim800.println(Serial.readString());
  }
// GSM ends

/*.................................CHAT bot english version begins.........................*/  
  if (voice.indexOf("your name") > 0)
  {    
    RandomNumber = random(3, 6);                   
    Serial.println("Inside");  
    voice = "";
    Serial.println(RandomNumber);
    myDFPlayer.play(RandomNumber);
  }

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
    
    // if (Serial.available()){
    // SerialBT.write("Location Found");
    //  }    
  else if (voice.indexOf("my location") > 0){
        displayInfo();
    }
  else if ( (voice.indexOf("address") > 0) || (voice.indexOf("stay") > 0) ) 
  { 
    voice = "";
    RandomNumber = random(6, 8);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("phone number") > 0) 
  {
    RandomNumber = random(9, 11);
    voice = "";
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("who asked you") > 0) 
  {
    RandomNumber = random(12, 14);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("gender") > 0) || (voice.indexOf("male") > 0) || (voice.indexOf("female") > 0) ) 
  { 
    RandomNumber = random(15, 17);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("read") > 0) 
  {
    RandomNumber = random(18, 20);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("yes") > 0) || (voice.indexOf("of course") > 0) || (voice.indexOf("surely") > 0) ) 
  {
    RandomNumber = random(21, 23);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("beautiful") > 0) || (voice.indexOf("cute") > 0) ) 
  {
    Serial.println(RandomNumber);
    RandomNumber = random(24, 26);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("married") > 0) || (voice.indexOf("maritial status") > 0) ) 
  {  
    RandomNumber = random(27, 29);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("friend") > 0) 
  {  
    RandomNumber = random(30, 32);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("google now") > 0) 
  {
    myDFPlayer.play(33);
  }
  else if (voice.indexOf("siri") > 0) 
  {
    myDFPlayer.play(34);
  }
  else if ( (voice.indexOf("game") > 0) || (voice.indexOf("sport") > 0) ) 
  {  
    RandomNumber = random(35, 37);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("depressed") > 0) || (voice.indexOf("sad") > 0) || ((voice.indexOf("not") > 0) && (voice.indexOf("good") > 0)) > 0 ) 
  {
    RandomNumber = random(38, 42);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("sing") > 0) && (voice.indexOf("song") > 0) ) 
  {
    RandomNumber = random(46, 47);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("you") > 0) && (voice.indexOf("human") > 0) ) 
  {  
    RandomNumber = random(48, 60);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("shut up") > 0) 
  {  
    RandomNumber = random(51, 53);
    Serial.println("inside");
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("bore") > 0) 
  {
    RandomNumber = random(55, 56);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("fact") > 0) || (voice.indexOf("interesting") > 0) || (voice.indexOf("some") > 0) ) 
  {
    RandomNumber = random(59, 68);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("joke") > 0) 
  {
    RandomNumber = random(69, 73);
    myDFPlayer.play(RandomNumber);
  }
  else if (( voice.indexOf("age") > 0) || (voice.indexOf("old") > 0) ) 
  {
    RandomNumber = random(74, 76);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("story") > 0) 
  {
    RandomNumber = random(77, 80);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("how are you") > 0) || (voice.indexOf("how do you do") > 0) ) 
  {  
    RandomNumber = random(80, 82);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("thank") > 0) 
  {
    myDFPlayer.play(84);
  }
  else if ( (voice.indexOf("sibling") > 0) || (voice.indexOf("brother") > 0) || (voice.indexOf("sister") > 0) ) 
  {  
    RandomNumber = random(85, 87);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("introduc") > 0) 
  { 
    RandomNumber = random(88, 89);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("love") > 0) 
  {
    RandomNumber = random(90, 92);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("mean") > 0) && (voice.indexOf("Eve") > 0) ) 
  {  
    RandomNumber = random(93, 95);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("boring") > 0) || (voice.indexOf("stupid") > 0) || (voice.indexOf("annoying") > 0) ) 
  {  
    myDFPlayer.play(96);
  }
  else if ( (voice.indexOf("owner") > 0) || (voice.indexOf("inventor") > 0) || (voice.indexOf("creat") > 0) ) 
  {
    myDFPlayer.play(97);
  }
  else if (voice.indexOf("hate") > 0) 
  {  
    myDFPlayer.play(98);
  }
  else if ( ((voice.indexOf("my") > 0) || (voice.indexOf("we") > 0)) && (voice.indexOf("friend") > 0) ) 
  {  
    myDFPlayer.play(99);
  }
  else if (voice.indexOf("can you") > 0) 
  {  
    RandomNumber = random(101, 103);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("can i") > 0) 
  {  
    RandomNumber = random(104, 106);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("you are") > 0) || (voice.indexOf("you're") > 0) ) 
  {  
    RandomNumber = random(107, 111);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("i dont") > 0) 
  {  
    RandomNumber = random(111, 114);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("i feel") > 0) 
  {  
    RandomNumber = random(116, 118);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("why don't you") > 0) || (voice.indexOf("why do not you") > 0) || (voice.indexOf("why dont you") > 0) ) 
  {  
    RandomNumber = random(119, 121);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("why can't i") > 0) 
  {
    RandomNumber = random(122, 126);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("are you") > 0) 
  {
    RandomNumber = random(127, 129);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("i can't") > 0) 
  {
    RandomNumber = random(130, 132);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("i am") > 0) || (voice.indexOf("i'm") > 0) ) 
  {
    RandomNumber = (133, 136);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("you ") > 0) 
  {
    RandomNumber = random(137, 139);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("what") > 0) || (voice.indexOf("how") > 0) || (voice.indexOf("where") > 0) ) 
  {
    RandomNumber = random(140, 149);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("i want") > 0)
  {
    RandomNumber = random(150, 154);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("name") > 0) 
  {
    RandomNumber = random(155, 157);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("cause") > 0) 
  {
    RandomNumber = random(158, 161);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("sorry") > 0) 
  {
    RandomNumber = random(162, 164);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("dream") > 0) 
  {
    RandomNumber = random(165, 168);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("always") > 0) 
  {
    RandomNumber = random(169, 172);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("think") > 0) 
  {
    RandomNumber = random(173, 176);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("alike") > 0) 
  {
    RandomNumber = random(177, 182);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("friend") > 0) 
  {
    RandomNumber = random(183, 187);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("computer") > 0) 
  {
    RandomNumber = random(188, 193);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("car") > 0) 
  {
    RandomNumber = random(193, 199);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("yes") > 0) || (voice.indexOf("yeah") > 0) ) 
  {
    RandomNumber = random(200, 203);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("maybe") > 0) 
  {
    RandomNumber = random(202, 208);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("your") > 0) 
  {
    RandomNumber = random(209, 211);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("no") > 0) 
  {
    RandomNumber = random(212, 215);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("hello") > 0) || (voice.indexOf("hi ") > 0) ) 
  {
    RandomNumber = random(220, 226);
    myDFPlayer.play(RandomNumber);
  }
  /*.................................Chat bot english ends............................*/

  /*.................................Chat bot hindi begins............................*/
  else if ( (voice.indexOf("aapka naam") > 0) || (voice.indexOf("tumhara naam") > 0) || (voice.indexOf("tera naam") > 0) )
  {
    RandomNumber = random(226, 229);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("umar") > 0) || (voice.indexOf("umer") > 0) || (voice.indexOf("umra") > 0))
  {
    myDFPlayer.play(229);
  }
  else if ( (voice.indexOf("kaise") > 0) || (voice.indexOf("kaisi") > 0) )
  {
    RandomNumber =  random(230, 233);
    myDFPlayer.play(RandomNumber);
  }
  else if (voice.indexOf("chutkula") > 0)
  {
    RandomNumber = random(233, 239);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("mahila") > 0) || (voice.indexOf("purush") > 0) ) 
  {  
    myDFPlayer.play(239);
  }
  else if (voice.indexOf("mahadweep") > 0)
  {
    myDFPlayer.play(240);
  }
  else if (voice.indexOf("aapki shaadi") > 0)
  {
    myDFPlayer.play(241);
  }
  else if (voice.indexOf("namaste") > 0)
  {
    myDFPlayer.play(242);
  }
  else if (voice.indexOf("newton ke niyam") > 0)
  {
    RandomNumber = random(243, 246);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("newton") > 0) && (voice.indexOf("pahla") > 0) && (voice.indexOf("niyam") > 0))
  {
    myDFPlayer.play(243);
  }
  else if ( (voice.indexOf("newton") > 0) && (voice.indexOf("dusra") > 0) && (voice.indexOf("niyam") > 0))
  {  
    myDFPlayer.play(244);
  }
  else if ( (voice.indexOf("newton") > 0) && (voice.indexOf("teesra") > 0) && (voice.indexOf("niyam") > 0))
  {
    myDFPlayer.play(245);
  }
  else if ( (voice.indexOf("grah") > 0) || (voice.indexOf("greh") > 0) )
  {
    myDFPlayer.play(246);
  }
  else if ( ((voice.indexOf("pradhanmantri") > 0) && (voice.indexOf("india") > 0)) || ((voice.indexOf("pradhanmantri") > 0) && (voice.indexOf("bharat") > 0)) )
  {
    myDFPlayer.play(247);
  }
  else if ( (voice.indexOf("kavita") > 0))
  {
    myDFPlayer.play(248);
  }
  else if ( ((voice.indexOf("rashtrapati") > 0) && (voice.indexOf("india") > 0)) || ((voice.indexOf("rashtrapati") > 0) && (voice.indexOf("bharat") > 0)))
  {
    myDFPlayer.play(249);
  }
  else if (voice.indexOf("indradhanush") > 0)
  {  
    myDFPlayer.play(250);
  }
  else if ( (voice.indexOf("kya chahiye") > 0) )
  {
    myDFPlayer.play(251);
  }
  else if ( ((voice.indexOf("kaha ho") > 0)) || ((voice.indexOf("kaha hai") > 0)) )
  {
    myDFPlayer.play(252);
  }
  else if ( (voice.indexOf("vishwa") > 0) && (voice.indexOf("vijeta") > 0) && (voice.indexOf("football") > 0) )
  {
    myDFPlayer.play(253);
  }
  else if ( (voice.indexOf("rajya") > 0) && (voice.indexOf("india") > 0) || (voice.indexOf("rajya") > 0) && (voice.indexOf("bharat") > 0) ) 
  {
    myDFPlayer.play(254);
  }
  else if ( (voice.indexOf("paheliya") > 0) || (voice.indexOf("paheliyan") > 0) )
  {
    RandomNumber = random(255, 258);
    myDFPlayer.play(RandomNumber);
  }
  else if ( (voice.indexOf("tongue") > 0) && (voice.indexOf("twister") > 0) && (voice.indexOf("sunao") > 0) )
  {
    RandomNumber = random(260, 263);
    myDFPlayer.play(RandomNumber);
  }
  else if ( ((voice.indexOf("rajdhani") > 0) && (voice.indexOf("india") > 0)) || ((voice.indexOf("rajdhani") > 0) && (voice.indexOf("bharat") > 0)) )
  {
    myDFPlayer.play(263);
  }
  else if ( ((voice.indexOf("tumhare") > 0) && (voice.indexOf("sapne") > 0)) || ((voice.indexOf("aapke") > 0) && (voice.indexOf("sapne") > 0)) )
  {
    myDFPlayer.play(264);
  }
  else if ( ((voice.indexOf("tumhara") > 0) && (voice.indexOf("sapna") > 0)) || ((voice.indexOf("aapka") > 0) && (voice.indexOf("sapna") > 0)) )
  {
    myDFPlayer.play(264);
  }
  else if ( (voice.indexOf("kisne") > 0) && (voice.indexOf("banaya") > 0) )
  {
    myDFPlayer.play(265);
  }
  else if ( (voice.indexOf("dost") > 0))
  {  
    myDFPlayer.play(266);
  }
  else if(voice != "")
  {
    RandomNumber = random(258, 260);
    myDFPlayer.play(RandomNumber);
  }
/*.................................Chat bot hindi ends............................*/

/*.................................Detecting error in memory card...................*/
  if (myDFPlayer.available()) 
  {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value)
{
  delay(1000);
  switch (type) 
  {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) 
      {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
/*.................................Detecting error in memory card ends...................*/


void displayInfo()  //Chat bot displaying location
{
  if (gps.location.isValid())
  {
    myDFPlayer.play(267);
    display.clearDisplay();
    display.setCursor(1,2);
    Serial.print("Latitude: ");
    display.println("lt:"+String(gps.location.lat(), 6));
    display.println("ln:"+String(gps.location.lng(), 6));
    display.println("at:"+String(gps.altitude.meters()));
    display.display();
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
    myDFPlayer.play(268);
  }
}  

/*..........................Function for responding to gsm querry.......................................*/
void parseData(String buff){
  Serial.println(buff);

  unsigned int len, index;
  //////////////////////////////////////////////////
  //Remove sent "AT Command" from the response string.
  index = buff.indexOf("\r");
  buff.remove(0, index+2);
  buff.trim();
  //////////////////////////////////////////////////
  
  //////////////////////////////////////////////////
  if(buff != "OK"){
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
    
    buff.remove(0, index+2);
    //Serial.println(buff);
    
    if(cmd == "+CMTI"){
      //get newly arrived memory location and store it in temp
      //temp = 4
      index = buff.indexOf(",");
      String temp = buff.substring(index+1, buff.length()); 
      temp = "AT+CMGR=" + temp + "\r"; 
      //AT+CMGR=4 i.e. get message stored at memory location 4
      sim800.println(temp); 
    }
    else if(cmd == "+CMGR"){
      extractSms(buff);
      Serial.println("Sender Number: "+senderNumber);
     Serial.println("PHONE: "+PHONE);
      if(senderNumber == PHONE){
        if(msg == "get location"){
          sendLocation();
        }
//        else if(msg == "get speed"){
//          sendSpeed();
//        }
      }
      sim800.println("AT+CMGD=1,4"); //delete all saved SMS
      delay(1000);
      smsStatus = "";
      senderNumber="";
      receivedDate="";
      msg="";
    }
  //////////////////////////////////////////////////
  }
  else{
  //The result of AT Command is "OK"
  }
}

void extractSms(String buff){
   unsigned int index;
   Serial.println(buff);

    index =buff.indexOf(",");
    smsStatus = buff.substring(1, index-1); 
    buff.remove(0, index+2);
    
    senderNumber = buff.substring(0, 13);
    buff.remove(0,19);
   
    receivedDate = buff.substring(0, 20);
    buff.remove(0,buff.indexOf("\r"));
    buff.trim();
    
    index =buff.indexOf("\n\r");
    buff = buff.substring(0, index);
    buff.trim();
    msg = buff;
    buff = "";
    msg.toLowerCase();

    //Serial.println("----------------------------------");
    //Serial.println(smsStatus);
    //Serial.println(senderNumber);
    //Serial.println(receivedDate);
    //Serial.println(msg);
    //Serial.println("----------------------------------");
}
void sendLocation()
{
  // Can take up to 60 seconds
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 2000;)
  {
    while (gpsSerial.available())
    {
      if (gps.encode(gpsSerial.read()))
      {
        newData = true;
      }
    }
  }
  if (newData)      //If newData is true
  {
    Serial.print("Latitude= "); 
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Longitude= "); 
    Serial.println(gps.location.lng(), 6);
    newData = false;
    delay(300);
    ///*
    sim800.print("AT+CMGF=1\r");
    delay(1000);
    sim800.print("AT+CMGS=\""+PHONE+"\"\r");
    delay(1000);
    sim800.print("http://maps.google.com/maps?q=loc:");
    sim800.print(gps.location.lat(), 6);
    sim800.print(",");
    sim800.print(gps.location.lng(), 6);
    delay(100);
    sim800.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
    delay(1000);
    Serial.println("GPS Location SMS Sent Successfully.");
    //*/
  }
}
/*..........................Function for responding to gsm querry end.......................................*/
