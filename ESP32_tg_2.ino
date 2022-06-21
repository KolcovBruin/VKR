#include <ESP32Servo.h>


#include <DHT.h>
#include <DHT_U.h>

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
//#include "DHT.h"

// Replace with your network credentials
//const char* ssid = "iPhone 6s (Григорий)";
//const char* password = "12345678";
DHT dht(15, DHT11);
DHT dht_2(15, DHT11);
String keyboardJson;
Servo myservo;
const char* ssid = "Huawei v1";
const char* password = "18021978";
//short int state=0;
// Initialize Telegram BOT
#define BOTtoken "5234426537:AAHVjC8-E90LjrBacUGLKIgSV0O_SimO8O8"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "247504167"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;


//////
struct Lvl_user
{
    String login;
    String pass;
};
Lvl_user List_lvl[3];
/////////
struct user
{
  String usr_id;
  //String access;
  String login;
  user *next=NULL;
  short int state=0;
};
///////////////
user *el;
user *cr_el;
user *lst_el;
short int cnt_usr;

void new_user(String user_id)
{
  Serial.println("newUser");
  //delay(10000);
  if(cnt_usr==0)
  {
    //el=new user;
    el->usr_id=user_id;
    el->state=1;
    cr_el=el;
    lst_el=el;
  }
  if(cnt_usr!=0)
  {
    
    lst_el=new user;
    cr_el->next=lst_el;
    lst_el->usr_id=user_id;
    lst_el->state=1;
     cnt_usr++;
  }
  bot.sendMessage(user_id, "Вы еще не авторизованы. Введите Логин", "");
 
}
void message (String chat_id,String text)
{
  if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "выкл") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
      Serial.println("ttttt");

  String Temperature = String(dht.readTemperature(),2); //Измеряем температуру
  //float t = 40;
  if (text == "/temp1")
  {
    String Temperature = String(dht.readTemperature(),2); //Измеряем температуру
    bot.sendMessage(chat_id, Temperature, "");
    myservo.write(90); 
    
  }
  if (text == "/hum1")
  {
      String Humidity = String(dht.readHumidity(),2); //Измеряем влажность
    bot.sendMessage(chat_id, Humidity, "");
  }
  if (text == "/temp2")
  {
    String Temperature = String(dht_2.readTemperature(),2); //Измеряем температуру
    bot.sendMessage(chat_id, Temperature, "");
    myservo.write(90); 
    
  }
  if (text == "/hum2")
  {
    String Humidity = String(dht_2.readHumidity(),2); //Измеряем влажность
    bot.sendMessage(chat_id, Humidity, "");
  }
  if (text=="Объект 1")
  {
    if (digitalRead(1)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Объект №1 близко", "");
    }
    if (!digitalRead(1)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Объект №1 далеко", "");
    }
  }
  if (text=="Объект 2")
  {
    if (digitalRead(3)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Объект №2 близко", "");
    }
    if (!digitalRead(3)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Объект №2 далеко", "");
    }
  }
  if (text == "Уровень СО")
  {
   // String Humidity = String(dht_2.readHumidity(),2); //Измеряем влажность
   String LV_CO="Уровень СО: ";
  // LV_CO+=analogRead(13);
  LV_CO+=20;
 
    bot.sendMessage(chat_id, LV_CO, "");
    if (digitalRead(5)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Высокий уровень СО", "");
    }
    if (!digitalRead(5)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Нормальный уровень СО", "");
    }
  }

  if (text == "Вибрации")
  {
   // String Humidity = String(dht_2.readHumidity(),2); //Измеряем влажность
  
    if (digitalRead(16)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Высокий уровень СО", "");
    }
    if (!digitalRead(16)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Нормальный уровень СО", "");
    }
  }
  if (text == "Уровень шума")
  {
   // String Humidity = String(dht_2.readHumidity(),2); //Измеряем влажность
   String LV_S="Уровень звука: ";
  // LV_CO+=analogRead(13);
  LV_S+=40;
    bot.sendMessage(chat_id, LV_S, "");
    if (digitalRead(5)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Высокий уровень шума", "");
    }
    if (!digitalRead(5)){
    //Serial.println(h);
    bot.sendMessage(chat_id, "Нормальный уровень шума", "");
    }
  }
  //////
  
}

void auth(String user_id,String text)
{
  if (text==List_lvl[0].login||text==List_lvl[1].login||text==List_lvl[2].login)
  {
     bot.sendMessage(user_id, "Введиде Пароль", "");
     cr_el->login=text;
  }
  
  if ((cr_el->login==List_lvl[0].login && text==List_lvl[0].pass)||(cr_el->login==List_lvl[1].login && text==List_lvl[1].pass)||(cr_el->login==List_lvl[2].login && text==List_lvl[2].pass))
  {
    String message;
    message="Вы авторизовались как: ";
    message+=cr_el->login;
    bot.sendMessage(user_id, message, "");
    cr_el->state=2;
  }
  
}


// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    bool find=0;
short int cnt=0;
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
   // if (chat_id != CHAT_ID){
      //bot.sendMessage(chat_id, "Unauthorized user", "");
     // continue;
    //}
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    //////////
    cr_el=el;
    while (find!=1&&cnt!=cnt_usr)
    {

      Serial.println(cr_el->usr_id);
      //Serial.println(cnt);
      cnt++;
      
      //cr_el=el;
      if (chat_id==cr_el->usr_id)
      {
        find=1;
        Serial.println("find");
        Serial.println(cr_el->usr_id);
        break;
      }
      if (cnt!=cnt_usr)
      {
      cr_el=cr_el->next;
      }
     // Serial.println("find");
     // Serial.println(find);
    }
   // cnt=0;
   Serial.println("find");
   Serial.println(find);
   if (find ==0)
   {
    new_user(chat_id);
   }
    else
    {
      switch(cr_el->state)
    {
      case 0:
      //new_user(chat_id);
      break;
      case 1:
      
      auth(chat_id,text);
      break;
      case 2:
      message(chat_id,text);
      break;
    }
    }
   
    
    cnt=0;
    

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Добро пожаловать, " + from_name + ".\n";
      welcome += "Для дальнейшего использования необходимо пройти аутентификацию.\n\n";
      welcome += "Дополнительную инфоомацию можно получить в разделе меню: Инфо \n";
      
     // bot.sendMessage(chat_id, welcome, "");

     // String keyboardJson = "[[\"/led_on\", \"выкл\"],[\"/state\"]]";
     // bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);

      keyboardJson = "[[{ \"text\" : \"Авторизация\", \"callback_data\" : \"/Авторизация\" }],[{ \"text\" : \"Информация\", \"callback_data\" : \"Информация\" }]]";
        bot.sendMessageWithInlineKeyboard(chat_id, welcome, "", keyboardJson);
    }

    
    
  }
}

void setup() {
  Serial.begin(115200);
dht.begin();
dht_2.begin();
myservo.attach(13);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
     //WiFi.begin(ssid, password);
  }
   //Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  
  List_lvl[0].login="Admin";
  List_lvl[0].pass="2244";
  List_lvl[1].login="Engineer";
  List_lvl[1].pass="1134";
  List_lvl[2].login="Operator";
  List_lvl[2].pass="1111";

 cnt_usr++;
  el=new user;
  cr_el=el;

  
}

void loop() {
    
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi");
   // WiFi.begin(ssid, password);
  }  
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }




  
}
