GPS
====

GPS message parsing library for embedded environments.

## Usage

### Arduino

```
#include <GPS.h>

GPS::NMEA::Parser<HardwareSerial> parser(Serial);

void gpsHandler(const GPS::NMEA::Message &in_msg) {
  switch(in_msg.messageID) {
    case NMEA_GPGGA:
      ...
  }
}

void setup(void) {
  parser.setHandler(gpsHandler);
}

void loop() {
  parser.yyparse();
}
```

You can see more under examples/ directory.

## Install

#### platform.io

Clone GPS repository to your project lib/ directory.

## GPS Modules

| model  |GGA|GLL|GSA|GSV|MSS|RMC|VTG|ZDA|
|:------:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|GMS6-CR6|o  |o  |o  |o  |-  |o  |o  |-  |

## Licence

[MIT](https://github.com/tcnksm/tool/blob/master/LICENCE)
