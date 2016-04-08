#include <SoftwareSerial.h>
#include <GPS.h>

SoftwareSerial Serial2(2, 3);

class DebugPort {
public:
	int available() {
		return Serial.available();
	}
	int read() {
		int c = Serial.read();
		Serial2.print((char )c);
		return c;
	}
};

DebugPort debugPort;

GPS::NMEA::CommandBuilder<HardwareSerial> nmea(Serial);
GPS::NMEA::Parser<DebugPort> parser(debugPort);

GPS::NMEA::UTCTime g_UTCTime;
GPS::NMEA::Date g_date;
GPS::NMEA::decimal1616_t
	g_latitude,
	g_longitude;

void gpsHandler(const GPS::NMEA::Message &in_msg) {
	switch(in_msg.messageID) {
		case NMEA_GPGGA:
			g_UTCTime = in_msg.gga.utcTime;
			if (in_msg.gga.latitude.integerPart >= 0) {
				g_latitude = in_msg.gga.latitude;
				g_longitude = in_msg.gga.longitude;
			}
			break;
		case NMEA_GPRMC:
			g_UTCTime = in_msg.rmc.utcTime;
			if (in_msg.rmc.latitude.integerPart >= 0) {
				g_latitude = in_msg.rmc.latitude;
				g_longitude = in_msg.rmc.longitude;
			}
			g_date = in_msg.rmc.date;
			break;
	}
}

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);

  nmea.SetSerialPort(GPS::NMEA::PROTOCOL_NMEA,9600);
  parser.setHandler(gpsHandler);

  memset(&g_latitude, 0xff, sizeof(GPS::NMEA::decimal1616_t));
	memset(&g_longitude, 0xff, sizeof(GPS::NMEA::decimal1616_t));
	memset(&g_UTCTime, 0xff, sizeof(GPS::NMEA::UTCTime));
}

void loop() {
	parser.yyparse();
}
