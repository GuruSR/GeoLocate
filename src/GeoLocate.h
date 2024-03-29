#ifndef WATCHY_GEOLOCATE_H
#define WATCHY_GEOLOCATE_H

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

#include <Arduino.h>
#include <Arduino_JSON.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <string.h>

class GeoLocate{
    public:
        bool beginGeoFromWeb();
        void endGeoFromWeb();
        bool gotGeoFromWeb();
        char City[128];
        char Longitude[31];
        char Latitude[31];
    private:
        void init();
        static void GeoGet(void * parameter);
        void StoreGot();
};
#endif
