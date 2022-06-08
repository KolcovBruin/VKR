#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
//const char* ssid = "iPhone 6s (Григорий)";
//const char* password = "12345678";
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
  if(cnt_usr==0)
  {
    el=new user;
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
    
  }
  bot.sendMessage(user_id, "Вы еще не авторизованы. Введите Логин", "");
  cnt_usr++;
}

void auth(String user_id,String text)
{
  if (text==List_lvl[0].login||text==List_lvl[1].login||text==List_lvl[2].login)
  {
     bot.sendMessage(user_id, "Введиде Пароль", "");
     cr_el->login=text;
  }
  
  if ((cr_el->login==List_lvl[0].login && text==List_lvl[0].pass)||(cr_el->login==List_lvl[1].login && text==List_lvl[0].pass)||(cr_el->login==List_lvl[2].login && text==List_lvl[0].pass))
  {
    String message;
    message="Вы авторизовались как: ";
    message+=cr_el->login;
    bot.sendMessage(user_id, message, "");
  }
  
}


// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
bool find=0;
short int cnt=0;
  for (int i=0; i<numNewMessages; i++) {
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
    while (find!=1&&cnt!=cnt_usr)
    {
      cnt++;
      cr_el=el;
      if (chat_id==cr_el->usr_id)
      {
        find=1;
        break;
      }
      if (cnt!=cnt_usr)
      {
      cr_el=cr_el->next;
      }
    }
    
    switch(cr_el->state)
    {
      case 0:
      new_user(chat_id);
      break;
      case 1:
      
      auth(chat_id,text);
      break;
      case 2:
      //IoT();
      break;
    }
    cnt=0;
    //while (find!=1&&cnt!=cnt_usr)
   // {
    ///  Serial.println(cnt);
    //  Serial.println("cnt");
     // cnt++;
     // cr_el=el;
     // if (chat_id==cr_el->usr_id)
     // {
     //   find=1;
        //Serial.println("Yes");
        //Serial.println(find);
       // Serial.println("Yes");
    //  }
      //cr_el=cr_el->next;
   // }
   // if (cnt_usr==0||find==0)
   // {
    //  new_user(chat_id,text);
    //  Serial.println(cnt_usr);
    //  Serial.println(find);
    //  Serial.println("NO");
   // }
    //if (find==1)
    //{
     // find =0;
      //if (cr_el->access=="admin")
      //{
      //  bot.sendMessage(chat_id, "Hi,Admin", "");
     // }
   // }
    //if(chat_id!=el->usr_id)
   // {
    //  el->next=NULL;
     // el->usr_id=chat_id;
     // Serial.println(el->usr_id);
   // }
    


    
    ///////////

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      
      bot.sendMessage(chat_id, welcome, "");

      String keyboardJson = "[[\"/led_on\", \"выкл\"],[\"/state\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);

      keyboardJson = "[[{ \"text\" : \"Go to Google\", \"url\" : \"https://www.google.com\" }],[{ \"text\" : \"Send\", \"callback_data\" : \"выкл\" }]]";
        bot.sendMessageWithInlineKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson);
    }

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
  }
}

void setup() {
  Serial.begin(115200);

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
     WiFi.begin(ssid, password);
  }
   Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  
  List_lvl[0].login="admin";
  List_lvl[0].pass="2244";
  List_lvl[1].login="Engineer";
  List_lvl[1].pass="1134";
  List_lvl[2].login="Operator";
  List_lvl[2].pass="1111";

 //cnt_usr++;
  //el=new user;
  //el->usr_id="247504167";
  //el->access="admin";
  //cr_el=new user;
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
//el->usr_id="1111";



  
}
