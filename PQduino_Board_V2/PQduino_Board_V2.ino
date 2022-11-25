/*
This source code was based on several tutorials and contributions found on web, with special recognition of:
- WebSocket Server by Dani on www.esploradores.com under license CREATIVE COMMONS (CC BY-SA) https://creativecommons.org/
- ESP8266 OTA update with Web Browser https://www.mischianti.org/2021/11/11/esp8266-ota-update-with-web-browser-custom-web-interface-3/
- ESP8266 NodeMCU: Getting Started with Firebase (Realtime Database) https://randomnerdtutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
- Split-string-into-string-array https://stackoverflow.com/questions/9072320/split-string-into-string-array

This software is written under the CREATIVE COMMONS license with Attribution-ShareAlike(CC BY-SA) https://creativecommons.org/

Redistributions of source code must retain the above creative commons and this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above creative commons notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided  with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHTHOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Needed libraries:
//*************************************
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "ESP8266HTTPUpdateServer.h"
#include <Firebase_ESP_Client.h>
#include "ADE7753.h"
#include <SPI.h>
#include <Math.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "Main_Webpage.h"
//*************************************

// Firebase implementation:
//*************************************
#include "addons/TokenHelper.h"       // Provide the token generation process info.
#include "addons/RTDBHelper.h"        // Provide the RTDB payload printing info and other helper functions.

// Change this for use your batabase:
//------------------------------------------------------------------------
#define API_KEY "AIzaSyC4DxVKXS0Ol_QZh0xGkx7scvNv50KWpJo"
#define USER_EMAIL "pqduino_test@gmail.com"
#define USER_PASSWORD "PQtest2022"
#define DATABASE_URL "https://pqduino-sample-default-rtdb.firebaseio.com"
#define DATABASE_SECRET "RRONfqEPzdx6HYB8OicgYTZ5Fuyyx1wWulqcxddb"
//------------------------------------------------------------------------
// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
byte ap_mode = 1,dbchk=0; //control connection to database
String uid;   // Variable to save USER UID
String databasePath;    // Database main path (to be updated in setup with the user UID)
String VoltagePath_min = "Voltage_min";   // Database child nodes
String VoltagePath_max = "Voltage_max";
String CurrentPath_min = "Current_min";
String CurrentPath_max = "Current_max";
String PowerPath_min = "Power_min";
String PowerPath_max = "Power_max";
String EnergyPath = "Energy";
String TempPath = "Temperature";
String timePath = "timestamp";
String millisPath = "millis";
String rebootPath = "reboot";
byte reboot_now=0;
String parentPath;  // Parent Node (to be updated in every loop)
float V_vals [120];
float V_min, V_max;
float I_vals [120];
float I_min, I_max;
float P_vals [120];
float P_min, P_max;
WiFiUDP ntpUDP;// Define NTP Client to get time
NTPClient timeClient(ntpUDP, "pool.ntp.org");
int timestamp;    // Variable to save current epoch time
unsigned long sendDataPrevMillis = 0;// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis_delete = 0;// Timer variables (send new readings every three minutes)
unsigned long timerDelay = 2*60*1000;// Every 2 minutes
unsigned long timerDelay_delete = 10*60*1000;// Every 10 minutes
//*************************************

// ADE7753 implementation:
//*************************************
#define SPI_Speed 1000000 //SPI Speed
#define CS_Pin D8         //GPIO15 SPI_Chip Select Pin
#define IRQPin D4         //IRQ Notify Pin on ESP8266
ADE7753 myADE(CS_Pin);

signed long WAVE_VAL [128];
signed long maxVal,maxVal_s,pkmax,pkmin,pkerror;
byte  force_pos_wave=true;
signed int pos_factor;

long iA,iB,vA,vB,tA,tB,wios,wvos,waveos;
long voltage_reg, voltage_real;
float voltage_dec;
long current_reg,current_real;
float current_dec;
signed long temp_val;
long period_val;
long freq_real;
float freq_dec;
String REG_DATA;
long P_power_reg, S_power_reg;
signed long Q_power_reg;
float P_power_dec, Q_power_dec, S_power_dec, FP_dec; 
float energy, energy_now;
volatile byte wave_flag=false;
volatile int wave_counter=0;

unsigned long last_meas,last_wave,last_power,millis_energy;
int inputVal;
String myString;
String WaveForm;

float sum,sumi,sumr,sumti,sumtr,sumt=0;
int j,k=0;
float interval= 0.1937984496;
byte thd_sel_v,wave_from_v,wave_sel;
float thd_sum;
float thdv,thdi,harm[22];
//*************************************

// Wifi connection:
//*************************************
String host= "pqduino.local";           //default host name
const char* update_path = "/firmware";
const char* update_username = "admin";
const char* update_password = "admin";

int addr = 0;
IPAddress staticip(192,168,1,200);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
const char *ssidConf = "ESP_Settings";
const char *passConf = "123456789";
#define DEFAULT_SSID      "MY_SSID"
#define DEFAULT_PASSWORD  "MY_PASSWORD"
String ssid = DEFAULT_SSID;              //default ssid
String password = DEFAULT_PASSWORD;      //default password

ESP8266WebServer server (80);
ESP8266HTTPUpdateServer httpUpdater;
WebSocketsServer webSocket = WebSocketsServer(81);
//*************************************

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {                                                                        
  switch(type) {                      
    case WStype_CONNECTED: {IPAddress ip = webSocket.remoteIP(num);webSocket.sendTXT(num,myString);}break;              
    case WStype_DISCONNECTED:break;                         
    case WStype_TEXT:{
      String web_cmd = (char*)payload;

      String web_cmd00 = getValue(web_cmd,'#',0);
      String web_cmd01 = getValue(web_cmd,'#',1);
      String web_cmd02 = getValue(web_cmd,'#',2);
      String web_cmd03 = getValue(web_cmd,'#',3);
      String web_cmd04 = getValue(web_cmd,'#',4);
      String web_cmd05 = getValue(web_cmd,'#',5);
      String web_cmd06 = getValue(web_cmd,'#',6);
      String web_cmd07 = getValue(web_cmd,'#',7);
      String web_cmd08 = getValue(web_cmd,'#',8);
      String web_cmd09 = getValue(web_cmd,'#',9);
      String web_cmd10 = getValue(web_cmd,'#',10);
      String web_cmd11 = getValue(web_cmd,'#',11);
      String web_cmd12 = getValue(web_cmd,'#',12);
      String web_cmd13 = getValue(web_cmd,'#',13);
      String web_cmd14 = getValue(web_cmd,'#',14);
      String web_cmd15 = getValue(web_cmd,'#',15);
      String web_cmd16 = getValue(web_cmd,'#',16);
      String web_cmd17 = getValue(web_cmd,'#',17);
      String web_cmd18 = getValue(web_cmd,'#',18);
      String web_cmd19 = getValue(web_cmd,'#',19);
      String web_cmd20 = getValue(web_cmd,'#',20);

      if (web_cmd=="Update"){
        String esp_cmd="cmd#";
        esp_cmd=esp_cmd+read_String(0)+'#'; //ctr
        esp_cmd=esp_cmd+read_String(10)+'#'; //ptr
        esp_cmd=esp_cmd+read_String(20)+'#'; //name
        esp_cmd=esp_cmd+read_String(40)+'#'; //ssid
        esp_cmd=esp_cmd+read_String(60)+'#'; //password
        esp_cmd=esp_cmd+read_String(80)+'#'; //ch1os
        esp_cmd=esp_cmd+read_String(90)+'#'; //ch2os
        esp_cmd=esp_cmd+read_String(100)+'#'; //phcal
        esp_cmd=esp_cmd+read_String(110)+'#'; //apos
        esp_cmd=esp_cmd+read_String(120)+'#'; //vrmsos
        esp_cmd=esp_cmd+read_String(130)+'#';; //irmsos
        esp_cmd=esp_cmd+read_String(140)+'#'; //dbchk
        esp_cmd=esp_cmd+read_String(150)+'#'; //iA
        esp_cmd=esp_cmd+read_String(160)+'#'; //iB
        esp_cmd=esp_cmd+read_String(170)+'#'; //vA
        esp_cmd=esp_cmd+read_String(180)+'#'; //vB
        esp_cmd=esp_cmd+read_String(190)+'#'; //wios
        esp_cmd=esp_cmd+read_String(200)+'#'; //wvos
        esp_cmd=esp_cmd+read_String(210)+'#'; //tA
        esp_cmd=esp_cmd+read_String(220); //tB
        webSocket.broadcastTXT(esp_cmd);
        refresh_myADE();
      }
      if (web_cmd00=="st"){
        writeString(0,web_cmd01); //ctr
        writeString(10,web_cmd02); //ptr
        writeString(20,web_cmd03); //name
        writeString(40,web_cmd04); //ssid
        writeString(60,web_cmd05); //password
        writeString(80,web_cmd06); //ch1os
        writeString(90,web_cmd07); //ch2os
        writeString(100,web_cmd08); //phcal
        writeString(110,web_cmd09); //apos
        writeString(120,web_cmd10); //vrmsos
        writeString(130,web_cmd11); //irmsos
        writeString(140,web_cmd12); //dbchk
        writeString(150,web_cmd13); //iA
        writeString(160,web_cmd14); //iB
        writeString(170,web_cmd15); //vA
        writeString(180,web_cmd16); //vB
        writeString(190,web_cmd17); //wios
        writeString(200,web_cmd18); //wvos
        writeString(210,web_cmd19); //tA
        writeString(220,web_cmd20); //tB
      }
      if (web_cmd00=="Reboot"){ESP.restart();}
    }
    break; 
    case WStype_ERROR:break; 
    }
}

void setup() {
  pinMode(IRQPin, INPUT_PULLUP);
  pinMode(CS_Pin, OUTPUT);
  myADE.Start(SPI_Speed,CS_Pin);
  digitalWrite (CS_Pin,LOW);
  delay(10);
  attachInterrupt(IRQPin,readWaveReg_I,FALLING);
  
  Serial.begin(115200);
  Serial.println();
  
  EEPROM.begin(512);  //Initialize EEPROM
  refresh_myADE();

  if (dbchk==1){ap_mode=1;}else{ap_mode=0;}
  Serial.print("Try to connect to database?: ");
  Serial.println(ap_mode);

  myADE.write16(MODE, 0x8C);       //Cycle energy accumulation Mode 8C-HPF On, 8D-HPF Off
  myADE.write16(LINECYC, 0x78);    //240 half-cycles (2s) to power accumulation
  myADE.write8(GAIN, 0x00);        //0.5V max per chanel
  refresh_myADE();
  
  delay(10);
   
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  byte tries = 0; 
  WiFi.hostname(ssidConf);
  while (WiFi.status() != WL_CONNECTED) 
  { 
  delay(1000); 
  if (tries++ > 20) 
  {
  ssid = DEFAULT_SSID;              //return to default ssid
  password = DEFAULT_PASSWORD;      //return default password
  ap_mode = 0;
  WiFi.mode(WIFI_AP); 
  WiFi.softAPConfig(staticip, gateway, subnet);
  WiFi.softAP(ssidConf, passConf); 
  break; 
  }
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  server.on("/", []() {server.send_P(200, "text/html", main_webpage);});
  server.on("/setup", []() {server.send_P(200, "text/html", setting_webpage );});
  httpUpdater.setup(&server, update_path, update_username, update_password);
  server.begin();
  if (ap_mode==1){
    if(MDNS.begin(host)) {}
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);        
    config.api_key = API_KEY;     // Assign the api key (required)
    auth.user.email = USER_EMAIL;     // Assign the user sign in credentials
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL; // Assign the RTDB URL (required)
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    fbdo.setBSSLBufferSize(8192 /* Rx buffer size in bytes from 512 - 16384 */, 4096 /* Tx buffer size in bytes from 512 - 16384 */);
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h      // Assign the callback function for the long running token generation task */
    config.max_token_generation_retry = 5; // Assign the maximum retry of token generation
    Firebase.begin(&config, &auth); // Initialize the library with the Firebase authen and config
    // Getting the user UID might take a few seconds
    Serial.println("Getting User UID");
    byte tries_database = 0; 
    while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
    if (tries_database++ > 20) 
    {
      ap_mode = 0;
      break; 
    }
    }
    if (ap_mode ==1){
      // Print user UID
      uid = auth.token.uid.c_str();
      Serial.print("User UID: ");
      Serial.println(uid);
      // Update database path
      databasePath = "/UsersData/" + uid + "/readings"; 
    }  
    configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    reboot_now=1;
  }                  
}

void loop() {
  webSocket.loop();
  server.handleClient();
  if (ap_mode==1){
    MDNS.update();  
    if (voltage_dec!=0 && Firebase.ready() && time(nullptr) > 1618971013 /* timestamp should be valid */ && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){ //set data
      sendDataPrevMillis = millis();

        V_max=0;
        V_min=1000;
        I_max=0;
        I_min=1000;
        P_max=0;
        P_min=1000;
      for (int i=0; i<119; i++){
        if (V_vals[i]>V_max){V_max=V_vals[i];}
        if (V_vals[i]<V_min){V_min=V_vals[i];}
 
        if (I_vals[i]>I_max){I_max=I_vals[i];}
        if (I_vals[i]<I_min){I_min=I_vals[i];}

        if (P_vals[i]>P_max){P_max=P_vals[i];}
        if (P_vals[i]<P_min){P_min=P_vals[i];}
      }
      
        FirebaseJson json;
        json.add(timePath, (uint32_t)time(nullptr));
        json.add(VoltagePath_max, String(V_max));
        json.add(VoltagePath_min, String(V_min));
        json.add(CurrentPath_max, String(I_max));
        json.add(CurrentPath_min, String(I_min));
        json.add(PowerPath_max,String(P_max));
        json.add(PowerPath_min,String(P_min));
        json.add(TempPath,String(temp_val));
        json.add(millisPath,String(millis())); 
        json.add(rebootPath,String(reboot_now));
        json.add(EnergyPath,String(energy-energy_now));
        energy_now=energy;
        reboot_now=0;
        
        if (Firebase.RTDB.push(&fbdo, databasePath.c_str(), &json)){
          Serial.println("JSON set ok");
        }
        else {
          Serial.println("JSON set problem..rebooting");
          //ESP.restart();
        }
    }
// This lines will delete database to limit of 45 days, but apparently it takes much time and causes rebooting in ESP8266
//        if (time(nullptr) > 1618971013 /* timestamp should be valid */ && (millis() - sendDataPrevMillis_delete > timerDelay_delete)){
//            sendDataPrevMillis_delete = millis();
//            Serial.print("Delete old data... ");
//            yield();
//            
//            if (Firebase.RTDB.deleteNodesByTimestamp(&fbdo, databasePath.c_str(), timePath, 5 /* delete 10 nodes at once */, 45*24*60*60 /* retain data within 45 days */)){
//                yield();
//                Serial.println("ok");
//            }
//            else{
//                Serial.println(fbdo.errorReason());
//            }
//        }
  }
  if ((millis()-last_meas) > 500) {
    last_meas = millis(); 
    set_ScreenHead();                   
    get_Voltage();
    get_Current();
    get_Temperature();
    get_Frequency();
    get_Power();
    if (wave_flag==false){
      WaveForm="";
      maxVal=1; // avoid div by 0
      for (int i=0; i<64; i++) {
        if (maxVal < abs(WAVE_VAL[i])) {
          maxVal= abs(WAVE_VAL[i]);
          maxVal_s=WAVE_VAL[i];
        }
      }
      if (force_pos_wave && maxVal_s<0){pos_factor=-1;}else{pos_factor=1;}
      for(int a=0;a<128;a++){WaveForm=WaveForm+"$"+String(WAVE_VAL[a]*pos_factor*128/maxVal);}//{WaveForm=WaveForm+"$"+String(WAVE_VAL[a]*128/1950);}
      if(wave_from_v==true){wave_sel=true;}else{wave_sel=false;}
      get_THD();  //This will destroy WAVE_VAL data;
      get_WAVE();
    }
    for (int i=0; i<119; i++){
      V_vals[i]=V_vals[i+1];
      I_vals[i]=I_vals[i+1];
      P_vals[i]=P_vals[i+1];
      }
    V_vals[119]=voltage_dec; 
    I_vals[119]=current_dec;
    P_vals[119]=P_power_dec;
    myString = String(voltage_dec)+"#"+String(current_dec)
               +"#"+String(energy)+"#"+String(P_power_dec)
               +"#"+String(S_power_dec)+"#"+String(Q_power_dec)
               +"#"+String(thdv/100)+"#"+String(thdi/100)
               +"#"+String(temp_val)+"#"+String(FP_dec)
               +"#"+String(harm[3]/100)+"#"+String(harm[5]/100)
               +"#"+String(harm[7]/100)+"#"+String(harm[9]/100)
               +"#"+String(harm[11]/100)+"#"+String(harm[13]/100)
               +"#"+String(freq_dec)+"#"+String(wave_sel)+"#"+String(REG_DATA)+"#"+WaveForm;
    webSocket.broadcastTXT(myString);                             
  }
  if ((millis()-last_wave) > 4000) {
    last_wave = millis(); 
    thd_sel_v=!(thd_sel_v);
    
    Serial.println("------------------------------");
    Serial.print("IP address: ");Serial.println(WiFi.localIP());
    Serial.println("Voltage: "+String(voltage_dec)+" V"); 
    Serial.println("Current: "+String(current_dec)+" A");  
    Serial.println("Active Power: "+String(P_power_dec)+" W");  
    Serial.println("Reactive Power: "+String(Q_power_dec)+" var");  
    Serial.println("Apparent Power: "+String(S_power_dec)+" VA");  
    Serial.println("Power Factor: "+String(FP_dec)+" %");  
    Serial.println("Frequency: "+String(freq_dec)+" Hz");  
    Serial.println("THD V: "+String(thdv/100)+" %");  
    Serial.println("THD I: "+String(thdi/100)+" %");  
    Serial.println("Temperature: "+String(temp_val)+" °C");  
    
  }
}

void set_ScreenHead(){
  REG_DATA="";
  REG_DATA=REG_DATA+String(myADE.read16bits(MODE))+'-';
  REG_DATA=REG_DATA+String(myADE.read16bits(LINECYC))+'-';
  REG_DATA=REG_DATA+String(myADE.read8bits(GAIN))+'-';
  REG_DATA=REG_DATA+String(myADE.read16bits(IRQEN))+'-';
  REG_DATA=REG_DATA+String(myADE.read8bits(CH1OS))+'-';
  REG_DATA=REG_DATA+String(myADE.read8bits(CH2OS))+'-';
  REG_DATA=REG_DATA+String(myADE.read16bits_s(VRMSOS))+'-';
  REG_DATA=REG_DATA+String(myADE.read16bits_s(IRMSOS));
}
void get_Voltage(){
  myADE.write16(MODE, 0x8C);                            //Cycle energy accumulation Mode 8C-HPF On, 8D-HPF Off
  voltage_reg = myADE.getVRMS(10);
  voltage_real=(voltage_reg*vA+vB)/10000;
  voltage_dec=voltage_real/1000.0;
  delay(0);
}

void get_Current(){
  myADE.write16(MODE, 0x8C);                            //Cycle energy accumulation Mode  8C-HPF On, 8D-HPF Off
  current_reg = myADE.getIRMS(10);
  current_real=(current_reg*iA+iB)/10000;
  current_dec=current_real/1000.0;
  delay(0);
}

void get_Temperature(){
  temp_val = myADE.getTEMP(5);
  temp_val=(temp_val*tA+tB)/1000;
  delay(0);
}

void get_Frequency(){
  period_val = myADE.getPERIOD(5);
  freq_real=447323215/period_val;
  freq_dec=freq_real/1000.0;
  delay(0);
}

void get_Power(){
  P_power_reg=myADE.read24bits(LAENERGY);
  Q_power_reg=myADE.read24bits_s(LVARENERGY);
  S_power_reg=myADE.read24bits(LVAENERGY);
  myADE.getResetInterruptStatus(); 
  FP_dec=0.827*P_power_reg/(S_power_reg); //0.827 factor from ADE7753 datasheet
  if (FP_dec>100.0){FP_dec=100.00;}
  S_power_dec=voltage_dec*current_dec;
  P_power_dec=voltage_dec*current_dec*FP_dec;
  Q_power_dec=sqrt(S_power_dec*S_power_dec-P_power_dec*P_power_dec);
  FP_dec=FP_dec*100.00;
  energy=energy+(P_power_dec*(millis()-millis_energy)/3600000.0);  //Energy is calculated in Wh
  millis_energy=millis();
  delay(0);
}

void get_WAVE(){
  for (int i = 0; i<128; i++){WAVE_VAL [i]=0;}        //Clear WAVE_VAL
  wave_counter=0;
  wave_flag=true;
  delay(0);
  if (thd_sel_v==true) {myADE.write16(MODE, 0x688C);waveos=wvos;}    //Select 14ksps and V as waveform source  688C-HPF On, 688D-HPF Off
  else {myADE.write16(MODE, 0x488C);waveos=wios;}                    //Select 14ksps and I as waveform source 488C-HPF On, 488D-HPF Off
  long lastupdate = 0;
  myADE.getResetInterruptStatus(); 
  lastupdate = millis();
  while(!(myADE.getInterruptStatus() & (ZX))){if((millis()-lastupdate)>100){return;}}
  myADE.getResetInterruptStatus(); 
  myADE.write16(IRQEN, 0x48);                            //Set WSMP in interrupt enable register
}

ICACHE_RAM_ATTR void readWaveReg_I() {
  if (wave_flag==true){
    WAVE_VAL[wave_counter]=waveos+myADE.read24bits_i(WAVEFORM);
    myADE.getResetInterruptStatus();
    wave_counter++;
  }
  if (wave_counter>=128){
    wave_flag=false;
    wave_counter=0;
    myADE.write16(IRQEN, 0x40);                           //Clear interrupt enable register
    myADE.getResetInterruptStatus();
    if (myADE.read16bits(MODE)== 0x688C){wave_from_v=true;} //688C-HPF On, 688D-HPF Off
    else {wave_from_v=false;}
  }
}

void get_THD(){
  for(int i=0;i<1+128;i++){
   float b=2*3.1415*i/(128.0-1.0);
   WAVE_VAL[i] = WAVE_VAL[i]*(0.54-0.46*cos(b));
  }
  for(int g=1; g<22; g++){     
    for(int c=0;c<1+(128);c++){
      j=c-k;
      if(j>=128){k=k+128;}
      sumti=WAVE_VAL[j]*(sin(6.28*g*60.0*c*interval*0.001));
      sumtr=WAVE_VAL[j]*(cos(6.28*g*60.0*c*interval*0.001));              
      sumi=sumi+sumti;
      sumr=sumr+sumtr; 
    }
  sum=sqrt(sumi*sumi+sumr*sumr)/(128);
  harm[g]=sum;
  sumi=0;sumr=0;
  j=0;   k=0; 
  }
  float harm_fun=harm[1];
  for(int f=1;f<22;f++){harm[f]=harm[f]*10000/harm_fun;}
  thd_sum=0;
  for(int f=2;f<22;f++){thd_sum=thd_sum+(harm[f]*harm[f]);}
  if (wave_from_v==true) {thdv=sqrt(thd_sum);}
  else {thdi=sqrt(thd_sum);}
  delay(0);
}

void refresh_myADE(){
  String ctr_reg;
  String ptr_reg;
  String ch1os_reg;
  String ch2os_reg;
  String phcal_reg;
  String apos_reg;
  String vrmsos_reg;
  String irmsos_reg;
  String dbchk_reg;
  String iA_reg;
  String iB_reg;
  String vA_reg;
  String vB_reg;
  String wios_reg;
  String wvos_reg;
  String tA_reg;
  String tB_reg;

  ctr_reg=read_String(0);
  ptr_reg=read_String(10);
  //------------------------------------------
  host=read_String(20);
  ssid=read_String(40);
  password=read_String(60);
  phcal_reg=read_String(100);
  apos_reg=read_String(110);
  //------------------------------------------
  ch1os_reg=read_String(80);
    signed int temp_reg=ch1os_reg.toInt();
    temp_reg=temp_reg*1;
    if (temp_reg>=0){myADE.write8(CH1OS, temp_reg);}
    if (temp_reg<0){temp_reg=-1*temp_reg+32; myADE.write8(CH1OS, temp_reg);}
  //------------------------------------------
  ch2os_reg=read_String(90);
    temp_reg=ch2os_reg.toInt();
    temp_reg=temp_reg*1;
    if (temp_reg>=0){myADE.write8(CH2OS, temp_reg);}
    if (temp_reg<0){temp_reg=-1*temp_reg+32; myADE.write8(CH2OS, temp_reg);}
  //------------------------------------------
  vrmsos_reg=read_String(120);
    temp_reg=vrmsos_reg.toInt();
    temp_reg=temp_reg*1;
    myADE.write16_s(VRMSOS, temp_reg);
  //------------------------------------------
  irmsos_reg=read_String(130);
    temp_reg=irmsos_reg.toInt();
    temp_reg=temp_reg*1;
    myADE.write16_s(IRMSOS, temp_reg);
  //------------------------------------------
  dbchk_reg=read_String(140);
    if (dbchk_reg=="1"){dbchk=1;}else{dbchk=0;}
  //------------------------------------------
  iA_reg=read_String(150);    long int long_iA=iA_reg.toInt();      iA=long_iA*1;
  iB_reg=read_String(160);    long int long_iB=iB_reg.toInt();      iB=long_iB*1;
  vA_reg=read_String(170);    long int long_vA=vA_reg.toInt();      vA=long_vA*1;
  vB_reg=read_String(180);    long int long_vB=vB_reg.toInt();      vB=long_vB*1;
  wios_reg=read_String(190);  long int long_wios=wios_reg.toInt();  wios=long_wios*1;
  wvos_reg=read_String(200);  long int long_wvos=wvos_reg.toInt();  wvos=long_wvos*1;
  tA_reg=read_String(210);    long int long_tA=tA_reg.toInt();      tA=long_tA*1;
  tB_reg=read_String(220);    long int long_tB=tB_reg.toInt();      tB=long_tB*1;
  //------------------------------------------
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

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
  if (k==0xFF){return "0000";}
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}
