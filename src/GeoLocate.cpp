#include "GeoLocate.h"

/* GeoLocate by GuruSR (https://www.github.com/GuruSR/GeoLocate)
 * Version 1.0, June 26, 2022
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
RTC_DATA_ATTR char City[100];		// That long because of a New Zealand location is 85 characters in length!
RTC_DATA_ATTR char Longitude[10];
RTC_DATA_ATTR char Latitude[10];
RTC_DATA_ATTR bool GInited;
bool Gbegan;
WiFiClient gWiFiC;
HTTPClient gHTTP;

// Asks for the response, not currently Async...
bool GeoLocate::beginGeoFromWeb(){
    if (!GInited) init();
    if (WiFi.status() != WL_CONNECTED) return false;
    gHTTP.begin(gWiFiC, GLURL);  // Call it and leave.
    Gbegan = true;
    return Gbegan;
}

void GeoLocate::endGeoFromWeb(){
    if (Gbegan){
        gHTTP.end();
        Gbegan = false;
    }
}

// Has the response happened?
bool GeoLocate::gotGeoFromWeb(){
    String S, payload;
    if (!GInited) init();
    if (WiFi.status() != WL_CONNECTED) return false;
    if (gHTTP.GET() == HTTP_CODE_OK) {
        payload = gHTTP.getString();
        JSONVar root = JSON.parse(payload);
        S = JSON.stringify(root["city"]);
        S.replace('"',' ');
        S.trim();
		strcpy(City,S.c_str());
		City[99] = 0;	// Long City Name protection.
        S = JSON.stringify(root["lat"]);
        S.replace('"',' ');
        S.trim();
		strcpy(Latitude,S.c_str());
        S = JSON.stringify(root["lon"]);
        S.replace('"',' ');
        S.trim();
		strcpy(Longitude,S.c_str());
        return true;
    }
    return false;
}

void GeoLocate::init(){
    int I;
    for (I = 0; I < 100; I++) { City[I] = 0; if (I < 10) { Longitude[I] = 0; Latitude[I] = 0; } }
    GInited = true;
    Gbegan = false;
}
