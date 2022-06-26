# GeoLocate
Useful for ESP32 or Watchy users, gives 3 values after doing simple calls, captures City, Longitude and Latitude in char arrays for use with other online APIs (Weather, etc).

3 functions exist:

| Function | Description |
| --------------- | ----------- |
| `bool beginGeoFromWeb();` | This starts the process, WiFi has to be available and an active connection, if any of that fails, returns `false`. |
| `bool gotGeoFromWeb();` | Returns `true` if it has the data or `false` if it doesn't, usually wait 2 to 4 seconds for this to happen. |
| `void endGeoFromWeb();` | Tells GeoLocate it is finished, so it can close any active connection (if any). |
