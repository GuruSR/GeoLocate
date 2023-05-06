#include "GeoLocate.h"

/* GeoLocate by GuruSR (https://www.github.com/GuruSR/GeoLocate)
 * Version 1.0, June  26, 2022
 * Version 1.1, March 20, 2023 - Includes threaded model.
 *
 * This library offers functionality to retrieve the current GeoLocation of the selected IP into Longitude and Latitude data.
 *
 * This library is adapted to work with the Arduino ESP32 and any other project that has similar libraries.
 *
 * MIT License
 *
 * Copyright (c) 2022 GuruSR
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

static const char GLURL[] PROGMEM = "http://ip-api.com/json/?fields=city,lat,lon";
RTC_DATA_ATTR char City[128];        // That long because of a New Zealand location is 85 characters in length!
RTC_DATA_ATTR char Longitude[32];
RTC_DATA_ATTR char Latitude[32];
RTC_DATA_ATTR bool GInited;
RTC_DATA_ATTR TaskHandle_t GeoHandle = NULL;
RTC_DATA_ATTR BaseType_t GeoRet;
RTC_DATA_ATTR bool GGot, Gbegan, GDone;
bool GStore;
String sCity, sLong, sLat;
WiFiClient gWiFiC;
HTTPClient gHTTP;

// Asks for the response, not currently Async...
bool GeoLocate::beginGeoFromWeb(){
    if (!GInited) init();
    if (WiFi.status() != WL_CONNECTED) return false;
    if (GeoHandle == NULL) {
      GDone = false;
      GeoRet = xTaskCreate(GeoLocate::GeoGet,"GeoLocate_Get",20480,NULL,(configMAX_PRIORITIES, -1),&GeoHandle);
      Gbegan = (GeoHandle != NULL);
    }
    return Gbegan;
}

void GeoLocate::endGeoFromWeb(){
    if (Gbegan){
        gHTTP.end();
		if (GeoHandle != NULL) { vTaskDelete(GeoHandle); GeoHandle = NULL; }
        if (GStore) StoreGot();
        Gbegan = false;
    }
}

// Has the response happened?
bool GeoLocate::gotGeoFromWeb(){
    if (!GInited) init();
    if (GStore) StoreGot();
    return GDone;
}

void GeoLocate::init(){
    int I;
    for (I = 0; I < 128; I++) { City[I] = 0; if (I < 32) { Longitude[I] = 0; Latitude[I] = 0; } }
    GInited = true;
    GGot = false;
    Gbegan = false;
}

void GeoLocate::GeoGet(void * parameter){
String S, payload;
bool Good = (WiFi.status() == WL_CONNECTED);
bool Sent = false;
unsigned long Stay = millis() + 4000;
    while (Good && millis() < Stay){
        if (!Sent) { Sent = true; gHTTP.setConnectTimeout(3000); gHTTP.begin(gWiFiC, GLURL); } // Call it and leave.
        if (gHTTP.GET() == HTTP_CODE_OK) {
            payload = gHTTP.getString();
Serial.println(payload);
            JSONVar root = JSON.parse(payload);
            S = JSON.stringify(root["city"]);
            S.replace('"',' ');
            S.trim();
            sCity = S;
            S = JSON.stringify(root["lat"]);
            S.replace('"',' ');
            S.trim();
            sLat = S;
            S = JSON.stringify(root["lon"]);
            S.replace('"',' ');
            S.trim();
            sLong = S;
            GGot = true;
            GStore = true;
            Good = false;
        }
        delay(100);    // 100ms pauses.
    }
    GDone = true;
    GeoHandle = NULL;
    vTaskDelete(GeoHandle);
}

void GeoLocate::StoreGot(){
    if (GStore){
        GStore = false;
        strcpy(City,sCity.c_str());
        City[127] = 0;    // Long City Name protection.
        strcpy(Latitude,sLat.c_str());
        Latitude[31] = 0; // Long Latitude protection.
        strcpy(Longitude,sLong.c_str());
        Longitude[31] = 0; // Long Longitude protection.
    }
}
