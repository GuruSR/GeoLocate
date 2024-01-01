# GeoLocate
Useful for ESP32 or Watchy users, gives 3 values after doing simple calls, captures City, Longitude and Latitude in char arrays for use with other online APIs (Weather, etc).

3 functions exist:

| Function | Description |
| --------------- | ----------- |
| `bool beginGeoFromWeb();` | This starts the process, WiFi has to be available and an active connection, if any of that fails, returns `false`. |
| `bool gotGeoFromWeb();` | Returns `true` if it has the data or `false` if it doesn't, usually wait 2 to 4 seconds for this to happen. |
| `void endGeoFromWeb();` | Tells GeoLocate it is finished, so it can close any active connection (if any). |

| Variable | Description|
| --------------- | ----------- |
| char City[128] | City located at IP address.  The 128 char length is because a New Zealand location has 85 characters in length!
| char Longitude[32] | Longitude where the IP address is located. |
| char Latitude[32] | Latitude where the IP address is located. |

This code is designed as a thread, you start the request and then ask it if it has a result to create a non-blocking method.
