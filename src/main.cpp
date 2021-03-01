
//#define test

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SimpleTimer.h>

SoftwareSerial GpsSerial(2, 3);
SimpleTimer timer;
TinyGPSPlus gps;

float current_lat;
float current_lng;
long gps_last_updated = 0;

void Debugger(){
    #ifdef test
        Serial.print("!");
        Serial.print("37.123456");
        Serial.print("*");
        Serial.print("127.654321");
        Serial.print("#");
    #else
        Serial.print("!");
        Serial.print(current_lat, 6);
        Serial.print("*");
        Serial.print(current_lng, 6);
        Serial.print("#");
    #endif
}

void setup() {
    // UART setup
    Serial.begin(9600);
    GpsSerial.begin(9600);

    // pinmode setup
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // timer scheduleing setup
    timer.setInterval(1000, Debugger);
}

void loop() {
    // timer schedule check
    timer.run();

    // NMEA 0183 setup
    if(GpsSerial.available()){
        gps.encode(GpsSerial.read());
    }
    if(gps.location.isValid() || (millis() - gps_last_updated > 300)){
        current_lat = gps.location.lat();
        current_lng = gps.location.lng();
        gps_last_updated = millis();
    }

    // GPS state show
    if(current_lat != 0){
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else{
        digitalWrite(LED_BUILTIN, LOW);
    }
}