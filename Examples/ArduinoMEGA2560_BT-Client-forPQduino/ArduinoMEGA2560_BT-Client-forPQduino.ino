//This is an example of how to get data form PQduino by bluetooth
//and show this data into an LCD screen, you need to upload this code in the ARDUINO BOARD.
//
//
//Im using this library for screen, is the best I found:
//
//
//
// UTFT_Demo_480x320 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 480x320 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
extern uint8_t BigFont[];// Declare which fonts we will be using

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ILI9481,38,39,40,41);

unsigned long last_refresh,last_screen;
int screen=1,refresh=1;
String voltage,current,energy,P_power,S_power,Q_power,thdv,thdi,temp,fp,harm3,harm5,harm7,harm9,harm11,harm13,freq,wave_sel,reg,waveform;
int trend_voltage[478],trend_current[478],trend_p[478];
int v_waveform[128],i_waveform[128];

void setup()
{
  Serial3.begin(115200);
  myGLCD.InitLCD();
  myGLCD.setFont(BigFont);
  myGLCD.clrScr();
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 479, 18);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(0, 317, 479, 319);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("PQduino interfacing", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);
}

void loop()
{
  if(Serial3.available())             //Cuando haya datos disponibles
  {
    String data1 = Serial3.readStringUntil('#');    //voltage
    String data2 = Serial3.readStringUntil('#');    //current
    String data3 = Serial3.readStringUntil('#');    //energy
    String data4 = Serial3.readStringUntil('#');    //P_power
    String data5 = Serial3.readStringUntil('#');    //S_power
    String data6 = Serial3.readStringUntil('#');    //Q_power
    String data7 = Serial3.readStringUntil('#');    //thdv
    String data8 = Serial3.readStringUntil('#');    //thdi
    String data9 = Serial3.readStringUntil('#');    //temp
    String data10 = Serial3.readStringUntil('#');   //fp
    String data11 = Serial3.readStringUntil('#');   //harm3
    String data12 = Serial3.readStringUntil('#');   //harm5
    String data13 = Serial3.readStringUntil('#');   //harm7
    String data14 = Serial3.readStringUntil('#');   //harm9
    String data15 = Serial3.readStringUntil('#');   //harm11
    String data16 = Serial3.readStringUntil('#');   //harm13
    String data17 = Serial3.readStringUntil('#');   //freq
    String data18 = Serial3.readStringUntil('#');   //wave_sel
    data18=data18.substring(0,data18.length());
    String data19 = Serial3.readStringUntil('#');   //reg
    for (int i=0; i<127; i++){
      String sample=Serial3.readStringUntil('$');
      if (data18=="0"){i_waveform[i]=sample.toFloat();}
      if (data18=="1"){v_waveform[i]=sample.toFloat();}
    }
    String data20 = Serial3.readStringUntil('\n');  //waveform, last element
  
    voltage=data1.substring(0,data1.length())+" V";
    current=data2.substring(0,data2.length())+" A";
    P_power=data4.substring(0,data4.length())+" W";
    Q_power=data6.substring(0,data6.length())+" var";
    fp=data10.substring(0,data10.length())+" % ind";
    thdv=data7.substring(0,data7.length())+" %";
    thdi=data8.substring(0,data8.length())+" %";
    temp=data9.substring(0,data9.length())+" C";
    
    trend_voltage[477]=data1.toFloat();
    trend_current[477] = 10*data2.toFloat();
    trend_p[477] = data4.toFloat();

    for (int i=1; i<478; i++){
      trend_voltage[i-1]=trend_voltage[i];
      trend_current[i-1]=trend_current[i];
      trend_p[i-1]=trend_p[i];
    }
    if (refresh==1){
    switch (screen){
    case 1:
      myGLCD.setColor(0, 0, 255);
      myGLCD.drawRect(0, 18, 479, 318);
      myGLCD.setColor(0,255,255);
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 19, 478, 317); 
      myGLCD.setColor(0,255,255);
      myGLCD.print("Sumary", 5, 20);
      myGLCD.print(voltage, 5, 50);
      myGLCD.print(current, 5, 70);
      myGLCD.print(P_power, 5, 90);
      myGLCD.print(Q_power, 5, 110);
      myGLCD.print(fp, 5, 130);
      myGLCD.print(thdv, 5, 150);
      myGLCD.print(thdi, 5, 170);
      myGLCD.print(temp, 5, 190);
    break;
    case 2:
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 19, 478, 317); 
      myGLCD.setColor(0,255,255);
      myGLCD.print("Voltage Trend", 5, 20);
      myGLCD.print(voltage, 5, 40);
      for (int i=1; i<478; i++){
        myGLCD.drawLine(i-1, 320-(trend_voltage[i-1]+10), i, 320-(trend_voltage[i]+10));
      }
    break;
    case 3:
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 19, 478, 317); 
      myGLCD.setColor(0,255,255);
      myGLCD.print("Current Trend", 5, 20);
      myGLCD.print(current, 5, 40);
      for (int i=1; i<478; i++){
        myGLCD.drawLine(i-1, 320-(3*trend_current[i-1]+10), i, 320-(3*trend_current[i]+10));
      }   
    break;
    case 4:
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 19, 478, 317); 
      myGLCD.setColor(0,255,255);
      myGLCD.print("Active Power Trend", 5, 20);
      myGLCD.print(P_power, 5, 40);
      for (int i=1; i<478; i++){
        myGLCD.drawLine(i-1, 320-(trend_p[i-1]+10), i, 320-(trend_p[i]+10));
      }
    break;
    case 5:
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 19, 478, 317); 
      myGLCD.setColor(0,255,255);
      myGLCD.print("Voltage and Current Waveforms", 5, 20);
      for (int i=1; i<127; i++){
        myGLCD.drawLine(i-1+50, 320-(v_waveform[i-1]+10+128), i+50, 320-(v_waveform[i]+10+128));
      }
      for (int i=1; i<128; i++){
        myGLCD.drawLine(i-1+50+240, 320-(i_waveform[i-1]+10+128), i+50+240, 320-(i_waveform[i]+10+128));
      }
    break;
    }
    refresh=0;
    }
  }
  if ((millis()-last_refresh) > 4000) {
    last_refresh = millis(); 
    refresh=1;
  }
  if ((millis()-last_screen) > 4000) {
    last_screen = millis(); 
    screen++;
    if (screen>5){screen=1;}
  }
  delay(10);
}
  
