//#include <WiFi.h>
#include "time.h"
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <BluetoothSerial.h>
#include <WiFiManager.h>


#define OUTPUT1 15
#define OUTPUT2 22
#define FLASH 4
#define LED_PRESET1 16
#define LED_PRESET2 17
#define LED_PRESET3 18
#define LED_PRESET4 19
#define INPUT_SENSOR_HUM 36


#define LED_ONBOARD 2 //On Board LED

//uint32_t time = 2000;
#define time 2000

#define EEPROM_SIZE 200

#define EEPROM_tiempoConfiguradoON1 0//String (8 bytes)
#define EEPROM_tiempoConfiguradoOFF1 10//String (8 bytes)
#define EEPROM_tiempoConfiguradoON2 20//String (8 bytes)
#define EEPROM_tiempoConfiguradoOFF2 30//String (8 bytes)
#define EEPROM_tiempoConfiguradoON3 40//String (8 bytes)
#define EEPROM_tiempoConfiguradoOFF3 50//String (8 bytes)
#define EEPROM_tiempoConfiguradoON4 60//String (8 bytes)
#define EEPROM_tiempoConfiguradoOFF4 70//String (8 bytes)

#define EEPROM_ADDRESS_WIFI_SSID 80//String (40 bytes)
#define EEPROM_ADDRESS_WIFI_PASS 120//String (40 bytes)

//#define EEPROM_ADDRESS_NUMSENSOR 10//uint8_t (1 byte)
//#define EEPROM_ADDRESS_MQTT_SERVER 100//String

//enum salidas {OUTPUT1,OUTPUT2,OUTPUT3,OUTPUT4}salida;

//const char* ssid     = "milton";
//const char* password = "paternal";

String ssid = "milton";
String password = "paternal";

WiFiManager wm;

<<<<<<< HEAD
/*

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 184);
IPAddress local_IP2(10, 101, 42, 65);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
*/
=======
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96

WiFiClient espClient1;
PubSubClient client1(espClient1);


hw_timer_t * timer;

//**** NTP CONFIG (hora mundial) ********
const char* ntpServer = "pool.ntp.org";//server del rtc global
const long  gmtOffset_sec = -10800;//3600;
const int   daylightOffset_sec = 0;//3600;

struct tm timeStamp;

char timeYear[10];
char timeMonth[10];
char timeDayName[10];
char timeDayNumber[10];
char timeHour24[10];
char timeHour12[10];
char timeMinute[10];
char timeSecond[10];
char fechaHora[50];

String tiempoConfiguradoON1 = "--:--:--";
String tiempoConfiguradoOFF1 = "--:--:--";
String tiempoConfiguradoON2 = "--:--:--";
String tiempoConfiguradoOFF2 = "--:--:--";
String tiempoConfiguradoON3 = "--:--:--";
String tiempoConfiguradoOFF3 = "--:--:--";
String tiempoConfiguradoON4 = "--:--:--";
String tiempoConfiguradoOFF4 = "--:--:--";

String tiempoActual = {};

String horaON1 = {};
String minutoON1 = {};
String horaOFF1 = {};
String minutoOFF1 = {};
String horaON2 = {};
String minutoON2 = {};
String horaOFF2 = {};
String minutoOFF2 = {}; 
String horaON3 = {};
String minutoON3 = {};
String horaOFF3 = {};
String minutoOFF3 = {}; 
String horaON4 = {};
String minutoON4 = {};
String horaOFF4 = {};
String minutoOFF4 = {};  
String preset1 = {};
String preset2 = {};
String preset3 = {};
String preset4 = {};

//**** NTP CONFIG (hora mundial) ********




char flagTimer = 0;
int contadorTimer = 0;
char flagProceso1 = 0;
char flagProceso2 = 0;
int contadorProceso1 = 0;
int contadorProceso2 = 0;
int tiempo1 = 1;
<<<<<<< HEAD
int tiempo2 = 10;
int milisegundos = 1000;//1 segundo
String macAdd = {};
=======
int tiempo2 = 1;
int milisegundos = 1000;//1 segundo
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96


//Servidor en la nube
const char *mqtt_server = "broker.hivemq.com";//"10.24.37.150";//"broker.hivemq.com";//"test.mosquitto.org";//"10.24.37.150";//"broker.hivemq.com";//"10.24.37.150";//"broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_pass = "";
char root_topic_subscribe[100] = "riegoARG/equipo_1";//"undefi";
char root_topic_publish[100] = "riegoARG/equipo_1";//"undefi";
<<<<<<< HEAD
//String root_topic_subscribe = "riegoARG/equipo_1";//"undefi";
//String root_topic_publish = "riegoARG/equipo_1";//"undefi";
=======
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96

//**************************************
//*********** BLUETOOTH  *****************
//**************************************

BluetoothSerial SerialBT;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

String clientId = "";
int flagConexionOK = 0;//para cuando wifi y mqtt conectan correctamente
int flagModoRed = 0;
int cantFallos = 0;


String startHtml = "<!DOCTYPE HTML><html>\
                      <head>\
                        <title>ESP Web Server</title>\
                        <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>\
	                      <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/css/bootstrap.min.css' integrity='sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm' crossorigin='anonymous'>\
                         <style>\
                            body {background-color: powderblue;}\
                            h1   {color: blue;}\
                            p    {color: red;}\
                            div.autoMargin {margin: 50px;}\    
                          </style>\
                      </head>\
                      <body>";

String endHtml = "</body></html>";                      

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

WebServer server(80);

const int led = 4;


//************ FUNCIONES *****************


void accionarSalidas(void);
void writeString(char add,String data);
String read_String(char add);
void handleNotFound (void);
void handleMenu(void);
void handleRoot(void);



//timer de usos generales 1ms
void IRAM_ATTR onTimer() {

    contadorProceso1++;
    contadorProceso2++;

    if(contadorProceso1 == tiempo1){//proceso 1
        contadorProceso1 = 0;//resetea el contador
        flagProceso1 = 1;        
        //Serial.println("flagProceso1");
    }
    if(contadorProceso2 == tiempo2){//proceso 2
        contadorProceso1 = 0;//resetea el contador
        //contadorProceso2 = 0;//resetea el contador
        flagProceso2 = 1; 
        //Serial.println("flagProceso2");
    }
}

void setup(){


    Serial.begin(115200);
    Serial.println("Sistema de riego Encendido");

    SerialBT.begin("riegoARG"); //Bluetooth device name
<<<<<<< HEAD

	macAdd = WiFi.macAddress();
    Serial.println( "MAC address: " + macAdd );
	cambiarConfigMQTT(1);
=======
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
    
    
    pinMode(OUTPUT1, OUTPUT);
    pinMode(OUTPUT2, OUTPUT);
    pinMode(LED_ONBOARD, OUTPUT);
    pinMode(LED_PRESET1, OUTPUT);
    pinMode(LED_PRESET2, OUTPUT);
    pinMode(LED_PRESET3, OUTPUT);
    pinMode(LED_PRESET4, OUTPUT);
    pinMode(INPUT_SENSOR_HUM, INPUT);
    //pinMode(FLASH, OUTPUT);

    digitalWrite(OUTPUT1, HIGH);
    
    
    //configuración de timer para usos generales // 1ms
    timer = timerBegin(2, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    //timerAlarmWrite(timer, 1000, true);//valor en microsegundos
    timerAlarmWrite(timer, milisegundos * 1000, true);//valor en microsegundos
    timerAlarmEnable(timer);

    setupModoRed();

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
/*
    tiempoConfiguradoON1 = "";
    tiempoConfiguradoOFF1 = "";
    tiempoConfiguradoON2 = "";
    tiempoConfiguradoOFF2 = "";
    tiempoConfiguradoON3 = "";
    tiempoConfiguradoOFF3 = "";
    tiempoConfiguradoON4 = "";
    tiempoConfiguradoOFF4 = "";
  */  

    cargarDesdeEEPROM();

   
    if (MDNS.begin("esp32")) {
      Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/menu", handleMenu);
    server.on("/reset", handleReset);

     server.on("/inline", []() {
      server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");

    //String localIP = (String)WiFi.localIP();
    String localIP = WiFi.localIP().toString();
    
    client1.publish(root_topic_publish, localIP.c_str() );


    

}

void loop(){

 
    server.handleClient();
    client1.loop();

    cambioDeParametros();

    if(flagProceso1){//si el timer alcanzó el tiempo para encender la electroválvula
        Serial.println("flagProceso1");
        flagProceso1 = 0;
        
        
        //digitalWrite(FLASH, HIGH);
        //accionarSalidas();
        obtenerFechaHora();
        Serial.println(fechaHora);
        Serial.println(tiempoActual);
        
        if(tiempoActual == tiempoConfiguradoON1 && preset1 == "ON"){

          digitalWrite(OUTPUT1, LOW);//logica inversa
          digitalWrite(LED_PRESET1, HIGH);//LED para mostrar encendido
          Serial.println("ON1");
          client1.publish(root_topic_publish, "ON1");

        }else if(tiempoActual == tiempoConfiguradoOFF1 && preset1 == "ON"){

          digitalWrite(OUTPUT1, HIGH);//logica inversa
          digitalWrite(LED_PRESET1, LOW);//LED para mostrar encendido
          Serial.println("OFF1");
          client1.publish(root_topic_publish, "OFF1");

        }else if(tiempoActual == tiempoConfiguradoON2 && preset2 == "ON"){

          digitalWrite(OUTPUT1, LOW);//logica inversa
          digitalWrite(LED_PRESET2, HIGH);//LED para mostrar encendido
          Serial.println("ON2");
          client1.publish(root_topic_publish, "ON2");

        }else if(tiempoActual == tiempoConfiguradoOFF2 && preset2 == "ON"){

          digitalWrite(OUTPUT1, HIGH);//logica inversa
          digitalWrite(LED_PRESET2, LOW);//LED para mostrar encendido
          Serial.println("OFF2");
          client1.publish(root_topic_publish, "OFF2");

        }else if(tiempoActual == tiempoConfiguradoON3 && preset3 == "ON"){

          digitalWrite(OUTPUT1, LOW);//logica inversa
          digitalWrite(LED_PRESET3, HIGH);//LED para mostrar encendido
          Serial.println("ON3");
          client1.publish(root_topic_publish, "ON3");

        }else if(tiempoActual == tiempoConfiguradoOFF3 && preset3 == "ON"){

          digitalWrite(OUTPUT1, HIGH);//logica inversa
          digitalWrite(LED_PRESET3, LOW);//LED para mostrar encendido
          Serial.println("OFF3");
          client1.publish(root_topic_publish, "OFF3");

        }else if(tiempoActual == tiempoConfiguradoON4 && preset4 == "ON"){

          digitalWrite(OUTPUT1, LOW);//logica inversa
          digitalWrite(LED_PRESET4, HIGH);//LED para mostrar encendido
          Serial.println("ON4");
          client1.publish(root_topic_publish, "ON4");

        }else if(tiempoActual == tiempoConfiguradoOFF4 && preset4 == "ON"){

          digitalWrite(OUTPUT1, HIGH);//logica inversa
          digitalWrite(LED_PRESET4, LOW);//LED para mostrar encendido
          Serial.println("OFF4");
          client1.publish(root_topic_publish, "OFF4");

        }

    }


	if(flagProceso2 && flagConexionOK == 0){

		flagProceso2 = 0;

		Serial.println("Intentando recuperar la conexión");
		comprobarConexion();//si alguna conexión se perdió, la reestablece
		if(flagConexionOK){//si la recuperó
			Serial.print("Se ha recuperado la conexión. flagConexionOK = ");
			Serial.println(flagConexionOK);
		}else{//si no la recuperó
		
			Serial.print("[PROBLEMAS] No se ha recuperado la conexión. flagConexionOK = ");
			Serial.println(flagConexionOK);
		}
	}



    /*
    if(flagProceso2){//si el timer alcanzó el tiempo para apagar la electroválvula
        Serial.println("flagProceso2");
        flagProceso2 = 0;

        digitalWrite(OUTPUT1, LOW);
        //digitalWrite(FLASH, LOW);
        //accionarSalidas();
    }
    */
/*
    Serial.print(timeHour24);
    Serial.print(":");
    Serial.print(timeMinute);
    Serial.print(":");
    Serial.println(timeSecond); 
*/
    

    

    
}

void accionarSalidas(void){


        digitalWrite(OUTPUT1, HIGH);
        delay(2000);
        digitalWrite(OUTPUT1, LOW);
        

}

//realiza la lectura del timestamp desde el server ntp
void obtenerFechaHora(void){

  if(!getLocalTime(&timeStamp)){
      Serial.println("Failed to obtain time");
  }else{

      //guarda cada componente del timeStamp en variables
      strftime(timeYear,10, "%Y", &timeStamp);
      strftime(timeMonth,10, "%B", &timeStamp);
      strftime(timeDayName,10, "%A", &timeStamp);
      strftime(timeDayNumber,10, "%d", &timeStamp);
      strftime(timeHour24,10, "%H", &timeStamp);   
      strftime(timeHour12,10, "%I", &timeStamp);
      strftime(timeMinute,10, "%M", &timeStamp);
      strftime(timeSecond,10, "%S", &timeStamp);
      
      strcpy(fechaHora, "");//borra el contenido del string

      //arma un string con los componentes que interesan
      strcat(fechaHora, timeYear);
      strcat(fechaHora, " ");
      strcat(fechaHora, timeMonth);
      strcat(fechaHora, " ");
      strcat(fechaHora, timeDayNumber);
      strcat(fechaHora, " ");
      strcat(fechaHora, timeHour24);
      strcat(fechaHora, ":");
      strcat(fechaHora, timeMinute);
      strcat(fechaHora, ":");
      strcat(fechaHora, timeSecond);

      tiempoActual = String(timeHour24) + ":" + String(timeMinute) + ":" + String(timeSecond);

  }
}

void cargarDesdeEEPROM(void){

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  
  tiempoConfiguradoON1 = EEPROM.readString(EEPROM_tiempoConfiguradoON1);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoON1 EEPROM: ");
  Serial.println(tiempoConfiguradoON1);

  tiempoConfiguradoOFF1 = EEPROM.readString(EEPROM_tiempoConfiguradoOFF1);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoOFF1 EEPROM: ");
  Serial.println(tiempoConfiguradoOFF1);

  tiempoConfiguradoON2 = EEPROM.readString(EEPROM_tiempoConfiguradoON2);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoON2 EEPROM: ");
  Serial.println(tiempoConfiguradoON2);

  tiempoConfiguradoOFF2 = EEPROM.readString(EEPROM_tiempoConfiguradoOFF2);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoOFF2 EEPROM: ");
  Serial.println(tiempoConfiguradoOFF2);

  tiempoConfiguradoON3 = EEPROM.readString(EEPROM_tiempoConfiguradoON3);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoON3 EEPROM: ");
  Serial.println(tiempoConfiguradoON3);

  tiempoConfiguradoOFF3 = EEPROM.readString(EEPROM_tiempoConfiguradoOFF3);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoOFF3 EEPROM: ");
  Serial.println(tiempoConfiguradoOFF3);

  tiempoConfiguradoON4 = EEPROM.readString(EEPROM_tiempoConfiguradoON4);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoON4 EEPROM: ");
  Serial.println(tiempoConfiguradoON4);

  tiempoConfiguradoOFF4 = EEPROM.readString(EEPROM_tiempoConfiguradoOFF4);//lee tiempoConfigurado de la eeprom
  Serial.print("tiempoConfiguradoOFF4 EEPROM: ");
  Serial.println(tiempoConfiguradoOFF4);

  ssid = EEPROM.readString(EEPROM_ADDRESS_WIFI_SSID);//lee SSID de la eeprom
  Serial.print("SSID EEPROM: ");
  Serial.println(ssid);

  password = EEPROM.readString(EEPROM_ADDRESS_WIFI_PASS);//lee SSID de la eeprom
  Serial.print("PASS EEPROM: ");
  Serial.println(password);
 
  //cambiarConfigMQTT(numeroSensor);

}

<<<<<<< HEAD


void handleRoot() {

/*
=======
void writeString(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
  EEPROM.commit();
}


String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

void handleRoot2() {
  //digitalWrite(led, 1);
  /*
  String htmlToSend = startHtml + "<form action = '/menu' method = 'POST'>\
                        <label for='horaON1'>HoraON1:</label><br>\
                        <input type='text' id='horaON1' name='horaON1' value=''><br>\
                        <label for='minutoON1'>MinutoON1:</label><br>\
                        <input type='text' id='minutoON1' name='minutoON1' value=''><br><br>\
                        <label for='horaOFF1'>HoraOFF1:</label><br>\
                        <input type='text' id='horaOFF1' name='horaOFF1' value=''><br>\
                        <label for='minutoOFF1'>MinutoOFF1:</label><br>\
                        <input type='text' id='minutoOFF1' name='minutoOFF1' value=''><br><br>\
                        <input type='submit' value='Aceptar'>\
                      </form>" + endHtml;
*/
/*
String htmlToSend = startHtml + "<form action = '/menu' method = 'POST'>\
                        <label for='horaON1'>HoraON1:</label>\
                          <select name='horaON1' id='horaON1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON1'>MinutoON1:</label>\
                        <select name='minutoON1' id='minutoON1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                        <label for='horaOFF1'>HoraOFF1:</label>\
                          <select name='horaOFF1' id='horaOFF1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF1'>MinutoOFF1:</label>\
                        <select name='minutoOFF1' id='minutoOFF1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>\
                          <label for='horaON2'>HoraON2:</label>\
                          <select name='horaON2' id='horaON2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON2'>MinutoON2:</label>\
                        <select name='minutoON2' id='minutoON2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <label for='horaOFF2'>HoraOFF2:</label>\
                          <select name='horaOFF2' id='horaOFF2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF2'>MinutoOFF2:</label>\
                        <select name='minutoOFF2' id='minutoOFF2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>\
                        <label for='horaON3'>HoraON3:</label>\
                          <select name='horaON3' id='horaON3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>"
                          +
                          /*
                        <label for='minutoON3'>MinutoON3:</label>\
                        <select name='minutoON3' id='minutoON3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>"
                          
                          "<label for='horaOFF3'>HoraOFF3:</label>\
                          <select name='horaOFF3' id='horaOFF3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF3'>MinutoOFF3:</label>\
                        <select name='minutoOFF3' id='minutoOFF3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>\
                          
                         +
                          "<label for='horaON4'>HoraON4:</label>\
                          <select name='horaON4' id='horaON4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON4'>MinutoON4:</label>\
                        <select name='minutoON4' id='minutoON4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                        <label for='horaOFF4'>HoraOFF4:</label>\
                          <select name='horaOFF4' id='horaOFF4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF4'>MinutoOFF4:</label>\
                        <select name='minutoOFF4' id='minutoOFF4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <input type='checkbox' name='preset1' value='ON'>\
                          <label for='preset1'> Activar preset 1</label><br>\
                          <input type='checkbox' name='preset2' value='ON'>\
                          <label for='preset2'> Activar preset 2</label><br>\
                          <input type='checkbox' name='preset3' value='ON'>\
                          <label for='preset3'> Activar preset 3</label><br>\
                          <input type='checkbox' name='preset4' value='ON'>\
                          <label for='preset4'> Activar preset 4</label><br>\
                          <br>\
                        <input type='submit' value='Aceptar'>\
                      </form>" + endHtml;
*/
/*
  String preset1html = "<form action = '/menu' method = 'POST'>\
                        <label for='horaON1'>HoraON1:</label>\
                          <select name='horaON1' id='horaON1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON1'>MinutoON1:</label>\
                        <select name='minutoON1' id='minutoON1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                        <label for='horaOFF1'>HoraOFF1:</label>\
                          <select name='horaOFF1' id='horaOFF1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF1'>MinutoOFF1:</label>\
                        <select name='minutoOFF1' id='minutoOFF1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>";
  
  String preset2html = "<label for='horaON2'>HoraON2:</label>\
                          <select name='horaON2' id='horaON2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON2'>MinutoON2:</label>\
                        <select name='minutoON2' id='minutoON2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <label for='horaOFF2'>HoraOFF2:</label>\
                          <select name='horaOFF2' id='horaOFF2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF2'>MinutoOFF2:</label>\
                        <select name='minutoOFF2' id='minutoOFF2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>";

String preset3html =     "<label for='horaON3'>HoraON3:</label>\
                          <select name='horaON3' id='horaON3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON3'>MinutoON3:</label>\
                        <select name='minutoON3' id='minutoON3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <label for='horaOFF3'>HoraOFF3:</label>\
                          <select name='horaOFF3' id='horaOFF3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF3'>MinutoOFF3:</label>\
                        <select name='minutoOFF3' id='minutoOFF3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>";

String preset4html =     "<label for='horaON4'>HoraON4:</label>\
                          <select name='horaON4' id='horaON4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON4'>MinutoON4:</label>\
                        <select name='minutoON4' id='minutoON4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                        <label for='horaOFF4'>HoraOFF4:</label>\
                          <select name='horaOFF4' id='horaOFF4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF4'>MinutoOFF4:</label>\
                        <select name='minutoOFF4' id='minutoOFF4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <input type='checkbox' name='preset1' value='ON'>\
                          <label for='preset1'> Activar preset 1</label><br>\
                          <input type='checkbox' name='preset2' value='ON'>\
                          <label for='preset2'> Activar preset 2</label><br>\
                          <input type='checkbox' name='preset3' value='ON'>\
                          <label for='preset3'> Activar preset 3</label><br>\
                          <input type='checkbox' name='preset4' value='ON'>\
                          <label for='preset4'> Activar preset 4</label><br>\
                          <br>\
                        <input type='submit' value='Aceptar'>\
                      </form>";
*/

  // String prueba = "<p>hola</p><h1>La página rara</h1>";
  // String htmlToSend = startHtml /* preset2html + preset3html + preset4html */ + endHtml;

  // //server.send(200, "text/plain", "hello from esp32!");
  // server.send(200, "text/html", htmlToSend);
  // //delay(500);
  // //digitalWrite(led, 0);

}

void handleRoot() {
  //digitalWrite(led, 1);
 /*
  String htmlToSend = startHtml + "<form action = '/menu' method = 'POST'>\
                        <label for='horaON1'>HoraON1:</label><br>\
                        <input type='text' id='horaON1' name='horaON1' value=''><br>\
                        <label for='minutoON1'>MinutoON1:</label><br>\
                        <input type='text' id='minutoON1' name='minutoON1' value=''><br><br>\
                        <label for='horaOFF1'>HoraOFF1:</label><br>\
                        <input type='text' id='horaOFF1' name='horaOFF1' value=''><br>\
                        <label for='minutoOFF1'>MinutoOFF1:</label><br>\
                        <input type='text' id='minutoOFF1' name='minutoOFF1' value=''><br><br>\
                        <input type='submit' value='Aceptar'>\
                      </form>" + endHtml;
*/
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
String htmlToSend = startHtml + "<div class='autoMargin'>" +"<form action = '/menu' method = 'POST'>\
                        <fieldset>\
                        <legend>Preset 1</legend>\
                          <label for='horaON1'>HoraON1:</label>\
                          <input type='number' id='horaON1' name='horaON1' min='00' max='23'><br>\
                          <label for='minutoON1'>MinutoON1:</label>\
                          <input type='number' id='minutoON1' name='minutoON1' min='00' max='59'><br><br>\
                          <label for='horaOFF1'>HoraOFF1:</label>\
                          <input type='number' id='horaOFF1' name='horaOFF1' min='00' max='23'><br>\
                          <label for='minutoOFF1'>MinutoOFF1:</label>\
                          <input type='number' id='minutoOFF1' name='minutoOFF1' min='00' max='59'>\
                        </fieldset>\
                        \
                        <fieldset>\
                        <legend>Preset 2</legend>\
                        <label for='horaON2'>HoraON2:</label>\
                        <input type='number' id='horaON2' name='horaON2' min='00' max='23'><br>\
                        <label for='minutoON2'>MinutoON2:</label>\
                        <input type='number' id='minutoON2' name='minutoON2' min='00' max='59'><br><br>\
                        <label for='horaOFF2'>HoraOFF2:</label>\
                        <input type='number' id='horaOFF2' name='horaOFF2' min='00' max='23'><br>\
                        <label for='minutoOFF2'>MinutoOFF2:</label>\
                        <input type='number' id='minutoOFF2' name='minutoOFF2' min='00' max='59'>\
                        </fieldset>\
                        \
                        <fieldset>\
                        <legend>Preset 3</legend>\
                        <label for='horaON3'>HoraON3:</label>\
                        <input type='number' id='horaON3' name='horaON3' min='00' max='23'><br>\
                        <label for='minutoON3'>MinutoON3:</label>\
                        <input type='number' id='minutoON3' name='minutoON3' min='00' max='59'><br><br>\
                        <label for='horaOFF3'>HoraOFF3:</label>\
                        <input type='number' id='horaOFF3' name='horaOFF3' min='00' max='23'><br>\
                        <label for='minutoOFF3'>MinutoOFF3:</label>\
                        <input type='number' id='minutoOFF3' name='minutoOFF3' min='00' max='59'>\
                        </fieldset>\
                        \
                        <fieldset>\
                        <legend>Preset 4</legend>\
                        <label for='horaON4'>HoraON4:</label>\
                        <input type='number' id='horaON4' name='horaON4' min='00' max='23'><br>\
                        <label for='minutoON4'>MinutoON4:</label>\
                        <input type='number' id='minutoON4' name='minutoON4' min='00' max='59'><br><br>\
                        <label for='horaOFF4'>HoraOFF4:</label>\
                        <input type='number' id='horaOFF4' name='horaOFF4' min='00' max='23'><br>\
                        <label for='minutoOFF4'>MinutoOFF4:</label>\
                        <input type='number' id='minutoOFF4' name='minutoOFF4' min='00' max='59'>\
                        </fieldset>\
                        \
                        <input type='checkbox' name='preset1' value='ON'>\
                        <label for='preset1'> Activar preset 1</label><br>\
                        <input type='checkbox' name='preset2' value='ON'>\
                        <label for='preset2'> Activar preset 2</label><br>\
                        <input type='checkbox' name='preset3' value='ON'>\
                        <label for='preset3'> Activar preset 3</label><br>\
                        <input type='checkbox' name='preset4' value='ON'>\
                        <label for='preset4'> Activar preset 4</label><br>\
                        <br>\
						<p>Nota: solo se modificar&aacuten los presets chequeados</p>\
						<p>Aquellos presets no chequeados (en blanco) mantendr&aacuten la configuraci&oacuten establecida previamente</p>\
						<br>\
                        <input type='submit' value='Aceptar'>\
                      </form>" + "</div>" + endHtml;
                       

<<<<<<< HEAD
  server.send(200, "text/html", htmlToSend);
*/

=======
/*
String htmlToSend = startHtml + "<form action = '/menu' method = 'POST'>\
                        <label for='horaON1'>HoraON1:</label>\
                          <select name='horaON1' id='horaON1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON1'>MinutoON1:</label>\
                        <select name='minutoON1' id='minutoON1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                        <label for='horaOFF1'>HoraOFF1:</label>\
                          <select name='horaOFF1' id='horaOFF1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF1'>MinutoOFF1:</label>\
                        <select name='minutoOFF1' id='minutoOFF1'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>\
                          <label for='horaON2'>HoraON2:</label>\
                          <select name='horaON2' id='horaON2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON2'>MinutoON2:</label>\
                        <select name='minutoON2' id='minutoON2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <label for='horaOFF2'>HoraOFF2:</label>\
                          <select name='horaOFF2' id='horaOFF2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF2'>MinutoOFF2:</label>\
                        <select name='minutoOFF2' id='minutoOFF2'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>\
                        <label for='horaON3'>HoraON3:</label>\
                          <select name='horaON3' id='horaON3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON3'>MinutoON3:</label>\
                        <select name='minutoON3' id='minutoON3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <label for='horaOFF3'>HoraOFF3:</label>\
                          <select name='horaOFF3' id='horaOFF3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF3'>MinutoOFF3:</label>\
                        <select name='minutoOFF3' id='minutoOFF3'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                          <br>\
                          <label for='horaON4'>HoraON4:</label>\
                          <select name='horaON4' id='horaON4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoON4'>MinutoON4:</label>\
                        <select name='minutoON4' id='minutoON4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                        <label for='horaOFF4'>HoraOFF4:</label>\
                          <select name='horaOFF4' id='horaOFF4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                          </select>\
                        <label for='minutoOFF4'>MinutoOFF4:</label>\
                        <select name='minutoOFF4' id='minutoOFF4'>\
                            <option value='00'>00</option>\
                            <option value='01'>01</option>\
                            <option value='02'>02</option>\
                            <option value='03'>03</option>\
                            <option value='04'>04</option>\
                            <option value='05'>05</option>\
                            <option value='06'>06</option>\
                            <option value='07'>07</option>\
                            <option value='08'>08</option>\
                            <option value='09'>09</option>\
                            <option value='10'>10</option>\
                            <option value='11'>11</option>\
                            <option value='12'>12</option>\
                            <option value='13'>13</option>\
                            <option value='14'>14</option>\
                            <option value='15'>15</option>\
                            <option value='16'>16</option>\
                            <option value='17'>17</option>\
                            <option value='18'>18</option>\
                            <option value='19'>19</option>\
                            <option value='20'>20</option>\
                            <option value='21'>21</option>\
                            <option value='22'>22</option>\
                            <option value='23'>23</option>\
                            <option value='24'>24</option>\
                            <option value='25'>25</option>\
                            <option value='26'>26</option>\
                            <option value='27'>27</option>\
                            <option value='28'>28</option>\
                            <option value='29'>29</option>\
                            <option value='30'>30</option>\
                            <option value='31'>31</option>\
                            <option value='32'>32</option>\
                            <option value='33'>33</option>\
                            <option value='34'>34</option>\
                            <option value='35'>35</option>\
                            <option value='36'>36</option>\
                            <option value='37'>37</option>\
                            <option value='38'>38</option>\
                            <option value='39'>39</option>\
                            <option value='40'>40</option>\
                            <option value='41'>41</option>\
                            <option value='42'>42</option>\
                            <option value='43'>43</option>\
                            <option value='44'>44</option>\
                            <option value='45'>45</option>\
                            <option value='46'>46</option>\
                            <option value='47'>47</option>\
                            <option value='48'>48</option>\
                            <option value='49'>49</option>\
                            <option value='50'>50</option>\
                            <option value='51'>51</option>\
                            <option value='52'>52</option>\
                            <option value='53'>53</option>\
                            <option value='54'>54</option>\
                            <option value='55'>55</option>\
                            <option value='56'>56</option>\
                            <option value='57'>57</option>\
                            <option value='58'>58</option>\
                            <option value='59'>59</option>\
                          </select>\
                          <br>\
                        <input type='submit' value='Aceptar'>\
                      </form>" + endHtml;
*/
  //String prueba = "<p>hola</p><h1>La página rara</h1>";
  //String htmlToSend = startHtml + prueba + endHtml;

  //server.send(200, "text/plain", "hello from esp32!");
  server.send(200, "text/html", htmlToSend);
  //delay(500);
  //digitalWrite(led, 0);
}


void handleMenu() {
  //digitalWrite(led, 1);
  
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
  horaON1 = server.arg(0);
  minutoON1 = server.arg(1);
  horaOFF1 = server.arg(2);
  minutoOFF1 = server.arg(3);
  horaON2 = server.arg(4);
  minutoON2 = server.arg(5);
  horaOFF2 = server.arg(6);
  minutoOFF2 = server.arg(7);
  horaON3 = server.arg(8);
  minutoON3 = server.arg(9);
  horaOFF3 = server.arg(10);
  minutoOFF3 = server.arg(11);
  horaON4 = server.arg(12);
  minutoON4 = server.arg(13);
  horaOFF4 = server.arg(14);
  minutoOFF4 = server.arg(15);

	if(server.hasArg("preset1")){
		preset1 = server.arg("preset1");
		Serial.println("tiene argumento preset1");
	}else{
		preset1 = "OFF";
	}

	if(server.hasArg("preset2")){
		Serial.println("tiene argumento preset2");
		preset2 = server.arg("preset2");
	}else{
		preset2 = "OFF";
	}

	if(server.hasArg("preset3")){
		Serial.println("tiene argumento preset3");
		preset3 = server.arg("preset3");
	}else{
		preset3 = "OFF";
	}

	if(server.hasArg("preset4")){
		Serial.println("tiene argumento preset4");
		preset4 = server.arg("preset4");
	}else{
		preset4 = "OFF";
	}

<<<<<<< HEAD
=======
/*
  preset1 = server.arg(16);
  
  preset2 = server.arg(17);
  preset3 = server.arg(18);
  preset4 = server.arg(19);
  */
//String prueba = "hola";
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96

if(horaON1.length() == 1){
  horaON1 = "0" + horaON1;
}
if(horaON2.length() == 1){
  horaON2 = "0" + horaON2;
}
if(horaON3.length() == 1){
  horaON3 = "0" + horaON3;
}
if(horaON4.length() == 1){
  horaON4 = "0" + horaON4;
}
if(horaOFF1.length() == 1){
  horaOFF1 = "0" + horaOFF1;
}
if(horaOFF2.length() == 1){
  horaOFF2 = "0" + horaOFF2;
}
if(horaOFF3.length() == 1){
  horaOFF3 = "0" + horaOFF3;
}
if(horaOFF4.length() == 1){
  horaOFF4 = "0" + horaOFF4;
}

<<<<<<< HEAD

=======
/*
    if(horaON1 == "0"){
      horaON1 = "00";
    }
    if(minutoON1 == "0"){
      minutoON1 = "00";
    }
    if(horaON2 == "0"){
      horaON2 = "00";
    }
    if(minutoON2 == "0"){
      minutoON2 = "00";
    }
    if(horaON3 == "0"){
      horaON3 = "00";
    }
    if(minutoON3 == "0"){
      minutoON3 = "00";
    }
    if(horaON4 == "0"){
      horaON4 = "00";
    }
    if(minutoON4 == "0"){
      minutoON4 = "00";
    }
    if(horaOFF1 == "0"){
      horaOFF1 = "00";
    }
    if(minutoOFF1 == "0"){
      minutoOFF1 = "00";
    }
    if(horaOFF2 == "0"){
      horaOFF2 = "00";
    }
    if(minutoOFF2 == "0"){
      minutoOFF2 = "00";
    }
    if(horaOFF3 == "0"){
      horaOFF3 = "00";
    }
    if(minutoOFF3 == "0"){
      minutoOFF3 = "00";
    }
    if(horaOFF4 == "0"){
      horaOFF4 = "00";
    }
    if(minutoOFF4 == "0"){
      minutoOFF4 = "00";
    }
*/
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
  Serial.print("HoraON1 = ");
  Serial.println(horaON1);
  Serial.print("MinutoON1 = ");
  Serial.println(minutoON1);
  Serial.print("HoraOFF1 = ");
  Serial.println(horaOFF1);
  Serial.print("MinutoOFF1 = ");
  Serial.println(minutoOFF1);

  Serial.print("HoraON2 = ");
  Serial.println(horaON2);
  Serial.print("MinutoON2 = ");
  Serial.println(minutoON2);
  Serial.print("HoraOFF2 = ");
  Serial.println(horaOFF2);
  Serial.print("MinutoOFF2 = ");
  Serial.println(minutoOFF2);

  Serial.print("HoraON3 = ");
  Serial.println(horaON3);
  Serial.print("MinutoON3 = ");
  Serial.println(minutoON3);
  Serial.print("HoraOFF3 = ");
  Serial.println(horaOFF3);
  Serial.print("MinutoOFF3 = ");
  Serial.println(minutoOFF3);

  Serial.print("HoraON4 = ");
  Serial.println(horaON4);
  Serial.print("MinutoON4 = ");
  Serial.println(minutoON4);
  Serial.print("HoraOFF4 = ");
  Serial.println(horaOFF4);
  Serial.print("MinutoOFF4 = ");
  Serial.println(minutoOFF4);

  Serial.print("preset1 = ");
  Serial.println(preset1);
  Serial.print("preset2 = ");
  Serial.println(preset2);
  Serial.print("preset3 = ");
  Serial.println(preset3);
  Serial.print("preset4 = ");
  Serial.println(preset4);

/*
  tiempoConfiguradoON1 = "--:--:--";
  tiempoConfiguradoOFF1 = "--:--:--";
  tiempoConfiguradoON2 = "--:--:--";
  tiempoConfiguradoOFF2 = "--:--:--";
  tiempoConfiguradoON3 = "--:--:--";
  tiempoConfiguradoOFF3 = "--:--:--";
  tiempoConfiguradoON4 = "--:--:--";
  tiempoConfiguradoOFF4 = "--:--:--";
 
*/


  if(preset1 == "ON"){


    tiempoConfiguradoON1 = horaON1 + ":" + minutoON1 + ":" + "00";
    tiempoConfiguradoOFF1 = horaOFF1 + ":" + minutoOFF1 + ":" + "00";

    EEPROM.writeString(EEPROM_tiempoConfiguradoON1, tiempoConfiguradoON1);
    EEPROM.commit();
    EEPROM.writeString(EEPROM_tiempoConfiguradoOFF1, tiempoConfiguradoOFF1);
    EEPROM.commit();

    //enciende un LED para indicar que el preset está configurado.
    digitalWrite(LED_PRESET1, HIGH);

  }else{
    //apaga un LED para indicar que el preset no está configurado.
    digitalWrite(LED_PRESET1, LOW);
  }
  if(preset2 == "ON"){

    tiempoConfiguradoON2 = horaON2 + ":" + minutoON2 + ":" + "00";
    tiempoConfiguradoOFF2 = horaOFF2 + ":" + minutoOFF2 + ":" + "00";


    EEPROM.writeString(EEPROM_tiempoConfiguradoON2, tiempoConfiguradoON2);
    EEPROM.commit();
    EEPROM.writeString(EEPROM_tiempoConfiguradoOFF2, tiempoConfiguradoOFF2);
    EEPROM.commit();
    //enciende un LED para indicar que el preset está configurado.
    digitalWrite(LED_PRESET2, HIGH);

  }else{
    //apaga un LED para indicar que el preset no está configurado.
    digitalWrite(LED_PRESET2, LOW);
  }
  if(preset3 == "ON"){
 
    tiempoConfiguradoON3 = horaON3 + ":" + minutoON3 + ":" + "00";
    tiempoConfiguradoOFF3 = horaOFF3 + ":" + minutoOFF3 + ":" + "00";

    EEPROM.writeString(EEPROM_tiempoConfiguradoON3, tiempoConfiguradoON3);
    EEPROM.commit();
    EEPROM.writeString(EEPROM_tiempoConfiguradoOFF3, tiempoConfiguradoOFF3);
    EEPROM.commit();
    //enciende un LED para indicar que el preset está configurado.
    digitalWrite(LED_PRESET3, HIGH);

  }else{
    //apaga un LED para indicar que el preset no está configurado.
    digitalWrite(LED_PRESET3, LOW);

  }
  if(preset4 == "ON"){

    tiempoConfiguradoON4 = horaON4 + ":" + minutoON4 + ":" + "00";
    tiempoConfiguradoOFF4 = horaOFF4 + ":" + minutoOFF4 + ":" + "00";
    
    EEPROM.writeString(EEPROM_tiempoConfiguradoON4, tiempoConfiguradoON4);
    EEPROM.commit();
    EEPROM.writeString(EEPROM_tiempoConfiguradoOFF4, tiempoConfiguradoOFF4);
    EEPROM.commit();
    //enciende un LED para indicar que el preset está configurado.
    digitalWrite(LED_PRESET4, HIGH);

  }else{
    //apaga un LED para indicar que el preset no está configurado.
    digitalWrite(LED_PRESET4, LOW);
  
  }


<<<<<<< HEAD
	String botonConfigurar = "<form action = '/menu' method = 'POST'>\
                        <input type='submit' value='Configurar'>\	
                        </form><br>";
  	String botonResetear = "<form action = '/reset' method = 'POST'>\
=======
  String botonVolver = "<form action = '/' method = 'POST'>\
                        <input type='submit' value='Volver'>\
                        </form><br>";
  String botonResetear = "<form action = '/reset' method = 'POST'>\
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
                        <input type='submit' value='Resetear'>\
                        </form>";

  //String htmlToSend = startHtml + "<h1>Menu</h1><br><h2>tiempoConfigurado</h2><br><p>ON1: " + tiempoConfiguradoON1 + "</p><br>" + "<p>OFF1: " + tiempoConfiguradoOFF1 + "</p>" + "<h2>tiempoConfigurado</h2><br><p>ON2: " + tiempoConfiguradoON2 + "</p><br>" + "<p>OFF2: " + tiempoConfiguradoOFF2 + "</p>" + "<h2>tiempoConfigurado</h2><br><p>ON3: " + tiempoConfiguradoON3 + "</p><br>" + "<p>OFF3: " + tiempoConfiguradoOFF3 + "</p>" + "<h2>tiempoConfigurado</h2><br><p>ON4: " + tiempoConfiguradoON4 + "</p><br>" + "<p>OFF4: " + tiempoConfiguradoOFF4 + "</p>" + "<form action = '/' method = 'POST'><input type='submit' value='Volver'></form>" + endHtml;
  //String htmlToSend = startHtml + "<form action = '/' method = 'POST'><input type='submit' value='Volver'></form>" + endHtml;
  
  String htmlToSend = startHtml + 
<<<<<<< HEAD
                      "<div class = 'autoMargin' ><h1>Info</h1><br><h2>Preset 1</h2><p>ON1: " +
=======
                      "<div class = 'autoMargin' ><h1>Menu</h1><br><h2>Preset 1</h2><p>ON1: " +
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
                      tiempoConfiguradoON1 + " ---- " + "OFF1: " + tiempoConfiguradoOFF1 + 
                      "</p>" + "<h2>Preset 2</h2><p>ON2: " + tiempoConfiguradoON2 + 
                      " ---- " + "OFF2: " + tiempoConfiguradoOFF2 + "</p>" + 
                      "<h2>Preset 3</h2><p>ON3: " + tiempoConfiguradoON3 + 
                      " ---- " + "OFF3: " + tiempoConfiguradoOFF3 + "</p>" + 
                      "<h2>Preset 4</h2><p>ON4: " + tiempoConfiguradoON4 + 
                      " ---- " + "OFF4: " + tiempoConfiguradoOFF4 + "</p><br>" +
<<<<<<< HEAD
                      botonConfigurar + "<br>" + botonResetear + "</div>" + endHtml;

                      
  server.send(200, "text/html", htmlToSend);



}


void handleMenu() {
  
  

	String htmlToSend = startHtml + "<div class='autoMargin'>" + "<h1>Men&uacute</h1>" + "<form action = '/' method = 'POST'>\
<fieldset>\
	<legend>Preset 1</legend>\
		<table>\
			<tr>\
				<td>\
					<label for='horaON1'>HoraON1:</label>\
				</td>\
				<td>\
					<input type='number' id='horaON1' name='horaON1' min='00' 	max='23'>\
				</td>\
				<td>\
					<label for='minutoON1'>MinutoON1:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoON1' name='minutoON1' min='00' max='59'>\
				</td>\
			</tr>\
			<tr>\
				<td>\
					<label for='horaOFF1'>HoraOFF1:</label>\
				</td>\
				<td>\
					<input type='number' id='horaOFF1' name='horaOFF1' min='00' max='23'>\
				</td>\
				<td>\
					<label for='minutoOFF1'>MinutoOFF1:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoOFF1' name='minutoOFF1' min='00' max='59'>\
				</td>\
			</tr>\
		</table>\
</fieldset>\
\
<fieldset>\
	<legend>Preset 2</legend>\
		<table>\
			<tr>\
				<td>\
					<label for='horaON2'>HoraON2:</label>\
				</td>\
				<td>\
					<input type='number' id='horaON2' name='horaON2' min='00' 	max='23'>\
				</td>\
				<td>\
					<label for='minutoON2'>MinutoON2:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoON2' name='minutoON2' min='00' max='59'>\
				</td>\
			</tr>\
			<tr>\
				<td>\
					<label for='horaOFF2'>HoraOFF2:</label>\
				</td>\
				<td>\
					<input type='number' id='horaOFF2' name='horaOFF2' min='00' max='23'>\
				</td>\
				<td>\
					<label for='minutoOFF2'>MinutoOFF2:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoOFF2' name='minutoOFF2' min='00' max='59'>\
				</td>\
			</tr>\
		</table>\
</fieldset>\
\
<fieldset>\
	<legend>Preset 3</legend>\
		<table>\
			<tr>\
				<td>\
					<label for='horaON3'>HoraON3:</label>\
				</td>\
				<td>\
					<input type='number' id='horaON3' name='horaON3' min='00' 	max='23'>\
				</td>\
				<td>\
					<label for='minutoON3'>MinutoON3:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoON3' name='minutoON3' min='00' max='59'>\
				</td>\
			</tr>\
			<tr>\
				<td>\
					<label for='horaOFF3'>HoraOFF3:</label>\
				</td>\
				<td>\
					<input type='number' id='horaOFF3' name='horaOFF3' min='00' max='23'>\
				</td>\
				<td>\
					<label for='minutoOFF3'>MinutoOFF3:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoOFF3' name='minutoOFF3' min='00' max='59'>\
				</td>\
			</tr>\
		</table>\
</fieldset>\
\
<fieldset>\
	<legend>Preset 4</legend>\
		<table>\
			<tr>\
				<td>\
					<label for='horaON4'>HoraON4:</label>\
				</td>\
				<td>\
					<input type='number' id='horaON4' name='horaON4' min='00' 	max='23'>\
				</td>\
				<td>\
					<label for='minutoON4'>MinutoON4:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoON4' name='minutoON4' min='00' max='59'>\
				</td>\
			</tr>\
			<tr>\
				<td>\
					<label for='horaOFF4'>HoraOFF4:</label>\
				</td>\
				<td>\
					<input type='number' id='horaOFF4' name='horaOFF4' min='00' max='23'>\
				</td>\
				<td>\
					<label for='minutoOFF4'>MinutoOFF4:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoOFF4' name='minutoOFF4' min='00' max='59'>\
				</td>\
			</tr>\
		</table>\
</fieldset>\
\
                        <input type='checkbox' name='preset1' value='ON'>\
                        <label for='preset1'> Activar preset 1</label><br>\
                        <input type='checkbox' name='preset2' value='ON'>\
                        <label for='preset2'> Activar preset 2</label><br>\
                        <input type='checkbox' name='preset3' value='ON'>\
                        <label for='preset3'> Activar preset 3</label><br>\
                        <input type='checkbox' name='preset4' value='ON'>\
                        <label for='preset4'> Activar preset 4</label><br>\
                        <br>\
						<p>Nota: solo se modificar&aacuten los presets chequeados</p>\
						<p>Aquellos presets no chequeados (en blanco) mantendr&aacuten la configuraci&oacuten establecida previamente</p>\
						<br>\
                        <input type='submit' value='Aceptar'>\
                      </form>" + "</div>" + endHtml;


  server.send(200, "text/html", htmlToSend);
=======
                      botonVolver + "<br>" + botonResetear + "</div>" + endHtml;

                      
  server.send(200, "text/html", htmlToSend);
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
  //delay(500);
  //digitalWrite(led, 0);
}

<<<<<<< HEAD

void handleMenu_old() {
  
  

	String htmlToSend = startHtml + "<div class='autoMargin'>" + "<h1>Men&uacute</h1>" + "<form action = '/' method = 'POST'>\
                        <fieldset>\
                        <legend>Preset 1</legend>\
                          <label for='horaON1'>HoraON1:</label>\
                          <input type='number' id='horaON1' name='horaON1' min='00' max='23'><br>\
                          <label for='minutoON1'>MinutoON1:</label>\
                          <input type='number' id='minutoON1' name='minutoON1' min='00' max='59'><br><br>\
                          <label for='horaOFF1'>HoraOFF1:</label>\
                          <input type='number' id='horaOFF1' name='horaOFF1' min='00' max='23'><br>\
                          <label for='minutoOFF1'>MinutoOFF1:</label>\
                          <input type='number' id='minutoOFF1' name='minutoOFF1' min='00' max='59'>\
                        </fieldset>\
<fieldset>\
	<legend>Preset 1</legend>\
		<table>\
			<tr>\
				<td>\
					<label for='horaON1'>HoraON1:</label>\
				</td>\
				<td>\
					<input type='number' id='horaON1' name='horaON1' min='00' 	max='23'>\
				</td>\
				<td>\
					<label for='minutoON1'>MinutoON1:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoON1' name='minutoON1' min='00' max='59'>\
				</td>\
			</tr>\
			<tr>\
				<td>\
					<label for='horaOFF1'>HoraOFF1:</label>\
				</td>\
				<td>\
					<input type='number' id='horaOFF1' name='horaOFF1' min='00' max='23'>\
				</td>\
				<td>\
					<label for='minutoOFF1'>MinutoOFF1:</label>\
				</td>\
				<td>\
					<input type='number' id='minutoOFF1' name='minutoOFF1' min='00' max='59'>\
				</td>\
			</tr>\
		</table>\
</fieldset>\
                        \
                        <fieldset>\
                        <legend>Preset 2</legend>\
                        <label for='horaON2'>HoraON2:</label>\
                        <input type='number' id='horaON2' name='horaON2' min='00' max='23'><br>\
                        <label for='minutoON2'>MinutoON2:</label>\
                        <input type='number' id='minutoON2' name='minutoON2' min='00' max='59'><br><br>\
                        <label for='horaOFF2'>HoraOFF2:</label>\
                        <input type='number' id='horaOFF2' name='horaOFF2' min='00' max='23'><br>\
                        <label for='minutoOFF2'>MinutoOFF2:</label>\
                        <input type='number' id='minutoOFF2' name='minutoOFF2' min='00' max='59'>\
                        </fieldset>\
                        \
                        <fieldset>\
                        <legend>Preset 3</legend>\
                        <label for='horaON3'>HoraON3:</label>\
                        <input type='number' id='horaON3' name='horaON3' min='00' max='23'><br>\
                        <label for='minutoON3'>MinutoON3:</label>\
                        <input type='number' id='minutoON3' name='minutoON3' min='00' max='59'><br><br>\
                        <label for='horaOFF3'>HoraOFF3:</label>\
                        <input type='number' id='horaOFF3' name='horaOFF3' min='00' max='23'><br>\
                        <label for='minutoOFF3'>MinutoOFF3:</label>\
                        <input type='number' id='minutoOFF3' name='minutoOFF3' min='00' max='59'>\
                        </fieldset>\
                        \
                        <fieldset>\
                        <legend>Preset 4</legend>\
                        <label for='horaON4'>HoraON4:</label>\
                        <input type='number' id='horaON4' name='horaON4' min='00' max='23'><br>\
                        <label for='minutoON4'>MinutoON4:</label>\
                        <input type='number' id='minutoON4' name='minutoON4' min='00' max='59'><br><br>\
                        <label for='horaOFF4'>HoraOFF4:</label>\
                        <input type='number' id='horaOFF4' name='horaOFF4' min='00' max='23'><br>\
                        <label for='minutoOFF4'>MinutoOFF4:</label>\
                        <input type='number' id='minutoOFF4' name='minutoOFF4' min='00' max='59'>\
                        </fieldset>\
                        \
                        <input type='checkbox' name='preset1' value='ON'>\
                        <label for='preset1'> Activar preset 1</label><br>\
                        <input type='checkbox' name='preset2' value='ON'>\
                        <label for='preset2'> Activar preset 2</label><br>\
                        <input type='checkbox' name='preset3' value='ON'>\
                        <label for='preset3'> Activar preset 3</label><br>\
                        <input type='checkbox' name='preset4' value='ON'>\
                        <label for='preset4'> Activar preset 4</label><br>\
                        <br>\
						<p>Nota: solo se modificar&aacuten los presets chequeados</p>\
						<p>Aquellos presets no chequeados (en blanco) mantendr&aacuten la configuraci&oacuten establecida previamente</p>\
						<br>\
                        <input type='submit' value='Aceptar'>\
                      </form>" + "</div>" + endHtml;


  server.send(200, "text/html", htmlToSend);
  //delay(500);
  //digitalWrite(led, 0);
}
=======
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
void handleReset(){

  tiempoConfiguradoON1 = "--:--:--";
  tiempoConfiguradoOFF1 = "--:--:--";
  tiempoConfiguradoON2 = "--:--:--";
  tiempoConfiguradoOFF2 = "--:--:--";
  tiempoConfiguradoON3 = "--:--:--";
  tiempoConfiguradoOFF3 = "--:--:--";
  tiempoConfiguradoON4 = "--:--:--";
  tiempoConfiguradoOFF4 = "--:--:--";

  EEPROM.writeString(EEPROM_tiempoConfiguradoON1, tiempoConfiguradoON1);
  EEPROM.commit();
  EEPROM.writeString(EEPROM_tiempoConfiguradoOFF1, tiempoConfiguradoOFF1);
  EEPROM.commit();

  EEPROM.writeString(EEPROM_tiempoConfiguradoON2, tiempoConfiguradoON2);
  EEPROM.commit();
  EEPROM.writeString(EEPROM_tiempoConfiguradoOFF2, tiempoConfiguradoOFF2);
  EEPROM.commit();

  EEPROM.writeString(EEPROM_tiempoConfiguradoON3, tiempoConfiguradoON3);
  EEPROM.commit();
  EEPROM.writeString(EEPROM_tiempoConfiguradoOFF3, tiempoConfiguradoOFF3);
  EEPROM.commit();

  EEPROM.writeString(EEPROM_tiempoConfiguradoON4, tiempoConfiguradoON4);
  EEPROM.commit();
  EEPROM.writeString(EEPROM_tiempoConfiguradoOFF4, tiempoConfiguradoOFF4);
  EEPROM.commit();




  String botonVolver = "<form action = '/menu' method = 'POST'>\
                        <input type='submit' value='Volver'>\
                        </form>";

<<<<<<< HEAD
  String htmlToSend = startHtml + "<div class='autoMargin'>" + "<h1>Reset</h1>" +"<p>Se han reseteado las configuraciones</p><br>" +  botonVolver + "</div>"  +  endHtml;
=======
  String htmlToSend = startHtml + "<p>Resetear configuraciones</p>" + botonVolver +  endHtml;
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
  server.send(200, "text/html", htmlToSend);

}

<<<<<<< HEAD

=======
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
void handleNotFound() {
  digitalWrite(led, 1);

	if(server.uri().endsWith(".ico")){
		Serial.println("se trata de favicon.ico");
    server.send(200, "text/plain", "se trata de favicon.ico");
	}else{
		Serial.println("URL no encontrada: no es favicon.ico");
		Serial.println("URL no encontrada, resetea pathDirectory y redirect a /");
    server.send(404, "text/plain", "URL no encontrada, resetea pathDirectory y redirect a /");
		//server. redirect("/");                     //Toda página solicitada no definida se redirecciona a la raíz ("/")
	}
/*
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
*/
}
/*
void handleNotFound(AsyncWebServerRequest *request) { //rutina de atención para páginas solicitadas no definidas 
    //pathDirectory = ""; //resetea el directorio para una nueva navegación
	
	Serial.print("URL = ");
	Serial.println(request->url());
	if(request->url().endsWith(".ico")){
		Serial.println("se trata de favicon.ico");
	}else{
		Serial.println("URL no encontrada: no es favicon.ico");
		Serial.println("URL no encontrada, resetea pathDirectory y redirect a /");
		pathDirectory = ""; //resetea el directorio para una nueva navegación
		request->redirect("/");                     //Toda página solicitada no definida se redirecciona a la raíz ("/")
	}
	//Serial.println("URL no encontrada");
	//request->redirect("/");                     //Toda página solicitada no definida se redirecciona a la raíz ("/")
}

*/
void setupModoRed(void){

  setup_wifi();


  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  client1.setServer(mqtt_server, mqtt_port);//inicializa server en broker local
  client1.setCallback(callback);

  //intenta conectar con MQTT
  if(!client1.connected()){
    reconnect();//si no está conectado, lo reconecta
  }
  if(client1.connected()){//si logró reconectarse o ya estaba conectado
    Serial.println("Conexión OK: Wifi y MQTT conectados");
    Serial.println("MODO RED");
    //flagCambioModoLocal = 0;//no hace falta cambiar a modo local
    flagConexionOK = 1;
    
  }else{
    Serial.println("Conexión Errónea: Wifi y MQTT no conectados");
    Serial.println("MODO LOCAL...(temporal)");
    //flagCambioModoLocal = 1;
    //flagModoRed = 0;
    flagConexionOK = 0;
  }

}

//*****************************
//***    CONEXION MQTT      ***
//*****************************

void reconnect() {

  //alimentar watchDog
  //esp_task_wdt_reset();
  //flagCambioModoLocal = 0;
  int resultado = 0;

  while (!client1.connected() == 1 && flagConexionOK == 0) {
    Serial.print("Intentando conexión Mqtt1...");
    // Creamos un cliente ID
    clientId = "IOTICOS_H_W_";
    clientId += String(random(0xffff), HEX);
    //String clientId = "UNDEFI_handtemp";
    // Intentamos conectar
    resultado = client1.connect(clientId.c_str(),mqtt_user,mqtt_pass);
    Serial.print("resultado: ");
    Serial.print(resultado);
    if(resultado == 1){
      Serial.println("Conectado!");
      flagConexionOK = 1;
    
    }else{
      
      cantFallos++;
      Serial.print("cantFallos: ");
      Serial.print(cantFallos);
      Serial.print("falló :( con error -> ");
      Serial.print(client1.state());
      Serial.println(" Intentamos de nuevo en 1 segundo");
      delay(1000);

      if(cantFallos == 3){
        cantFallos = 0;
        flagConexionOK = 0;
        break;
        
      }
    }
  }
}


//*****************************
//***       CALLBACK        ***
//*****************************

//puede cambiar parámetros a través del puerto serie o por bluetooth
//Se debe enviar un caracter de identificación del parámetro a cambiar y
//luego el valor.
//Por ejemplo: cambiar el tiempo entre lecturas de temperatura
//enviar T100  siendo T: tiempoEntreLecturas; 100: 100 ms
//los parámetros que se pueden modificar son:
//  distanciaConfigurada--> D;
//  distanciaTolerancia--> t;
//  tempFiebre--> F;
//  tempMin--> m;
//  tempMax--> M;
//  tempOffset--> O;
//  tiempoEntreLecturas--> T;
//  cantLecturas--> C;
//  emisividad--> E;
//  Wifi--> W;  [Ejemplo: Wmyssid mypassword](El espacio se usa como delimitador)
//  debug--> d  [1 para activarlo; 0 para desactivarlo]
//  cantSensoresIR-->S
//  consultarLecturas-->P
//  escannearDispositivosI2C-->s  [1 para activarlo; 0 para desactivarlo]
//  cambiarDireccionI2C-->A       [A90 91]
//  analizarLecturasCantidad-->U
//  intercambiarSensores-->I;
//  cambiarNumeroSensor-->Q;


void callback(char* topic, byte* payload, unsigned int length){
  
  String incoming = "";
  char charParamID = ' ';
  String valorParam = "";
  int inChar = 0;
  

  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");

  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }

  incoming.trim();
  Serial.println("Mensaje -> " + incoming);

  //obtiene el identificador
  charParamID = incoming.charAt(0);
  
  Serial.println(charParamID);
  
  //obtiene el valor
  for(int i = 1; i < incoming.length(); i++){
    valorParam += incoming.charAt(i);
  }

  Serial.println(valorParam);

  //evalua el identificador y los parámetros enviados
  switchCaseParametros(charParamID, valorParam);

  //borra el contenido y lo prepara para recibir uno nuevo
  incoming = "";

}


void switchCaseParametros(char charParamID, String valorParam){

  int inChar = 0;
  int index = 0;
  int valorParamLength = 0;
  int endIndex = 0;
  int modoDebug = 0;
  int consultarLecturas = 0;
  int correccionActivada = 0;
  uint8_t numSensor = 0;
  uint16_t direccion = 0;
  int scanActivado = 0;
  byte oldAddress = 0;
  byte newAddress = 0;
  int analizarLecturasCantidad = 0;
  int intercambioSensores = 0;
  int color = 0;
  String nombreSensor = "";


  switch(charParamID){
   
    case 'W':

      Serial.println("Wifi: ");
      valorParamLength = strlen(valorParam.c_str());
      endIndex = valorParamLength;

      index = valorParam.indexOf(' ');

      ssid = valorParam.substring(0, index);
      Serial.println(ssid);
      password = valorParam.substring(index + 1, endIndex - 1);
      Serial.println(password);

      //guarda config wifi en EEPROM
      EEPROM.writeString(EEPROM_ADDRESS_WIFI_SSID, ssid);
      EEPROM.commit();
      EEPROM.writeString(EEPROM_ADDRESS_WIFI_PASS, password);
      EEPROM.commit();

      setup_wifi();

    break;
    case 'R':

      Serial.println("Reset: ");
      delay(3000);
      ESP.restart();

    break;
    case 'T':

      Serial.println("Reset credenciales wifi: ");
      wm.resetSettings();
      delay(3000);
      ESP.restart();

    break;
    case 'I'://inicia riego
      digitalWrite(OUTPUT1, LOW);//lógica inversa
      Serial.println("Inicia riego por MQTT");
    break;
    case 'F'://finaliza riego
      digitalWrite(OUTPUT1, HIGH);//lógica inversa
      Serial.println("Finaliza riego por MQTT");
    break;
    default:
      Serial.println("Parámetro incorrecto");
    break;

  }  
}


void setup_wifi(void){

  /*
    // Connect to Wi-Fi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");

    Serial.println(WiFi.localIP());
*/
 //  WiFiManager wm;
<<<<<<< HEAD
/*
   // Configures static IP address
  if (!WiFi.config(local_IP2, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
	//WiFi.config(local_IP2, gateway, subnet, primaryDNS, secondaryDNS);
  }
*/
=======
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
 
   bool res;
 
   //esta función es bloqueante, ojo!!


   wm.setConfigPortalTimeout(120);//timeout para modo AP (150 segundos)
   res = wm.autoConnect("MedTemp","12345678"); // password protected ap
   if(res){


     ssid = wm.getWiFiSSID(true);
     password = wm.getWiFiPass(true);


     Serial.println(ssid);
     Serial.println(password);


    
     Serial.println("Conectado a red WiFi!");
     Serial.println(WiFi.localIP());


   }else{
     Serial.println("No es posible conectar a WiFi");


   }



}


//puede cambiar parámetros a través del puerto serie o por bluetooth
//Se debe enviar un caracter de identificación del parámetro a cambiar y
//luego el valor.
//Por ejemplo: cambiar el tiempo entre lecturas de temperatura
//enviar T100  siendo T: tiempoEntreLecturas; 100: 100 ms
//los parámetros que se pueden modificar son:
//  distanciaConfigurada--> D;
//  distanciaTolerancia--> t;
//  tempFiebre--> F;
//  tempMin--> m;
//  tempMax--> M;
//  tempOffset--> O;
//  tiempoEntreLecturas--> T;
//  cantLecturas--> C;
//  emisividad--> E;
//  Wifi--> W;  [Ejemplo: Wmyssid mypassword](El espacio se usa como delimitador)
//  debug--> d  [1 para activarlo; 0 para desactivarlo]
//  cantSensoresIR-->S
//  consultarLecturas-->P
//  escannearDispositivosI2C-->s  [1 para activarlo; 0 para desactivarlo]
//  cambiarDireccionI2C-->A       [A90 91]
//  analizarLecturasCantidad-->U
//  intercambiarSensores-->I;
void cambioDeParametros(void){

  char charParamID = ' ';
  String valorParam = "";
  int inChar = 0;
  String inString = "";
    
  
  //**** Chequeo por Serie o Bluetooth ***************
  while (Serial.available() > 0 || SerialBT.available() > 0) {

    if(Serial.available() > 0){
      inChar = Serial.read();
    }else if(SerialBT.available() > 0){
      inChar = SerialBT.read();
    }
    

    if(inChar != '\n'){
      Serial.print((char)inChar);

      inString += (char)inChar;//encola los caracteres recibidos

    }else{//si llegó el caracter de terminación
      
      Serial.print("Input string: ");
      Serial.println(inString);
      Serial.print("string length: ");
      Serial.println(inString.length());


      //obtiene el identificador
      charParamID = inString.charAt(0);
      
      Serial.println(charParamID);
      
      //obtiene el valor
      for(int i = 1; i < inString.length(); i++){
        valorParam += inString.charAt(i);
      }

      Serial.println(valorParam);

      //evalua el identificador y los parámetros enviados
      switchCaseParametros(charParamID, valorParam);
      
      //borra el contenido y lo prepara para recibir uno nuevo
      inString = "";
    
    }
  }

<<<<<<< HEAD
}

void cambiarConfigMQTT(uint8_t numSensor){


/*
	String root_topic_publishStr = (String)root_topic_publish + macAdd;
	Serial.print("root_topic_publishStr: ");
	Serial.println(root_topic_publishStr);

	String root_topic_subscribeStr = (String)root_topic_subscribe + macAdd;
	Serial.print("root_topic_subscribeStr: ");
	Serial.println(root_topic_subscribeStr);

	String root_topic = "riegoARG/";

	root_topic_subscribe = root_topic + macAdd;
	root_topic_publish = root_topic + macAdd;

*/

	char strTopico[80] = "riegoARG/";

	strcpy(root_topic_publish, strTopico);
	strcat(root_topic_publish, macAdd.c_str());
	strcpy(root_topic_subscribe, strTopico);
	strcat(root_topic_subscribe, macAdd.c_str());

	Serial.println();
	Serial.println("******************************************");
	Serial.print("root_topic_publish: ");
  	Serial.println(root_topic_publish);
  	Serial.println("******************************************");
	Serial.println();

	Serial.println();
	Serial.println("******************************************");
	Serial.print("root_topic_subscribe: ");
  	Serial.println(root_topic_subscribe);
  	Serial.println("******************************************");
	Serial.println();


}

void comprobarConexion(void){

  setupModoRed();//configura MQTT, revisa conectividad
 
=======
>>>>>>> a0f134e985b5d61f5d4cc72d27e5551a1065dd96
}