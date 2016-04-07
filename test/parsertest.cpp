#include <CUnit/CUnit.h>
#include <GPS.h>
#include "TestInputStream.h"

#include <stdio.h>

const GPS::NMEA::Message *g_msg = NULL;

static
void handler(const GPS::NMEA::Message &in_msg) {
  g_msg = &in_msg;
}

void test_parse_GGA(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGGA);
  CU_ASSERT(g_msg->gga.nsIndicator == 'N');
  CU_ASSERT(g_msg->gga.diffRefStationID == 0);
}

void test_parse_GGA_2(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGGA,,,,,,0,00,,,M,0.0,M,,0000*48\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGGA);
}

void test_parse_GGA_3(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGGA,074429.310,,,,,0,00,,,M,0.0,M,,0000*58\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGGA);
}

void test_parse_GGA_4(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGGA,104549.04,2447.2038,N,12100.4990,E,1,06,01.7,00078.8,M,0016.3,M,,*5C\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGGA);
  CU_ASSERT(g_msg->gga.utcTime.hour == 10);
  CU_ASSERT(g_msg->gga.utcTime.min == 45);
  CU_ASSERT(g_msg->gga.utcTime.sec == 49);
  CU_ASSERT(g_msg->gga.latitude.integerPart == 2447);
  CU_ASSERT(g_msg->gga.latitude.fractionalPart == 2038);
  CU_ASSERT(g_msg->gga.nsIndicator == 'N');
  CU_ASSERT(g_msg->gga.longitude.integerPart == 12100);
  CU_ASSERT(g_msg->gga.longitude.fractionalPart == 4990);
  CU_ASSERT(g_msg->gga.ewIndicator == 'E');
}

void test_parse_GGA_5(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGGA,075321.000,3520.4535,N,13929.3265,E,1,06,1.2,70.0,M,39.6,M,,0000*61\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGGA);
}

void test_parse_GGA_6(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGGA,075318.181,,,,,0,00,,,M,0.0,M,,0000*56\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGGA);
  CU_ASSERT(g_msg->gga.utcTime.hour == 7);
  CU_ASSERT(g_msg->gga.utcTime.min == 53);
  CU_ASSERT(g_msg->gga.utcTime.sec == 18);
  CU_ASSERT(g_msg->gga.utcTime.msec == 181);
}

void test_parse_GLL(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGLL,3723.2475,N,12158.3416,W,161229.487,A,A*41\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGLL);
  CU_ASSERT(g_msg->gll.nsIndicator == 'N');
  CU_ASSERT(g_msg->gll.ewIndicator == 'W');
}

void test_parse_GLL_2(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGLL,2447.2073,N,12100.5022,E,104548.04,A,A*65\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGLL);
}

void test_parse_GSA(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSA,A,3,07,02,26,27,09,04,15,,,,,,1.8,1.0,1.5*33\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSA);
  CU_ASSERT(g_msg->gsa.mode1 == 'A');
  CU_ASSERT(g_msg->gsa.mode2 == 3);
}

void test_parse_GSA_2(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSA,M,1,,,,,,,,,,,,,,,*12\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSA);
}

void test_parse_GSA_3(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSA,A,3,26,21,,,09,17,,,,,,,10.8,02.1,10.6*07\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSA);
}

void test_parse_GSV(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSV,2,1,07,07,79,048,42,02,51,062,43,26,36,256,42,27,27,138,42*71\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
}

void test_parse_GSV_2(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSV,2,2,07,09,23,313,42,04,19,159,41,15,12,041,42*41\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
}

void test_parse_GSV_3(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSV,3,1,12,01,00,000,,02,00,000,,03,00,000,,04,00,000,*7C\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
}

void test_parse_GSV_4(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSV,3,2,12,05,00,000,,06,00,000,,07,00,000,,08,00,000,*77\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
}

void test_parse_GSV_5(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSV,3,3,12,09,00,000,,10,00,000,,11,00,000,,12,00,000,*71\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
}

void test_parse_MSS(void) {
  g_msg = NULL;
  TestInputStream stream("$GPMSS,55,27,318.0,100,1*57\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPMSS);
}

void test_parse_RMC(void) {
  g_msg = NULL;
  TestInputStream stream("$GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598,,*10\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPRMC);
  CU_ASSERT(g_msg->rmc.date.day == 12);
  CU_ASSERT(g_msg->rmc.date.mon == 5);
  CU_ASSERT(g_msg->rmc.date.year == 98);
}

void test_parse_RMC_2(void) {
  g_msg = NULL;
  TestInputStream stream("$GPRMC,,V,,,,,,,,,,N*53\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPRMC);
}

void test_parse_RMC_3(void) {
  g_msg = NULL;
  TestInputStream stream("$GPRMC,074428.310,V,,,,,,,151111,,,N*46\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPRMC);
}

void test_parse_RMC_4(void) {
  g_msg = NULL;
  TestInputStream stream("$GPRMC,104549.04,A,2447.2038,N,12100.4990,E,016.0,221.0,250304,003.3,W,A*22\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPRMC);
}

void test_parse_VTG(void) {
  g_msg = NULL;
  TestInputStream stream("$GPVTG,309.62,T, ,M,0.13,N,0.2,K,A*23\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPVTG);
}

void test_parse_VTG_2(void) {
  g_msg = NULL;
  TestInputStream stream("$GPVTG,221.0,T,224.3,M,016.0,N,0029.6,K,A*1F\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPVTG);
}

void test_parse_ZDA(void) {
  g_msg = NULL;
  TestInputStream stream("$GPZDA,181813,14,10,2003,00,00*4F\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPZDA);
}

void test_parse_ZDA_2(void) {
  g_msg = NULL;
  TestInputStream stream("$GPZDA,104548.04,25,03,2004,,*6C\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPZDA);
}

void test_parse_150(void) {
  g_msg = NULL;
  TestInputStream stream("$PSRF150,1*3E\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_PSRF150);
  CU_ASSERT(g_msg->okToSend.okToSend == 1);
}

void test_parse_150_2(void) {
  g_msg = NULL;
  TestInputStream stream("$PSRF150,0*3F\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_PSRF150);
  CU_ASSERT(g_msg->okToSend.okToSend == 0);
}

void test_parse_151(void) {
  g_msg = NULL;
  TestInputStream stream("$PSRF151,1,1324,,0x40000001*5A\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_PSRF151);
  CU_ASSERT(g_msg->gpsDataAndEEM.ephReqMask == 0x40000001);
}

void test_parse_152(void) {
  g_msg = NULL;
  TestInputStream stream("$PSRF152,0x10000041,0x10000041,0x10000041*41\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_PSRF152);
}

void test_parse_154(void) {
  g_msg = NULL;
  TestInputStream stream("$PSRF154,107*3D\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_PSRF154);
  CU_ASSERT(g_msg->eeAck.ackID == 107);
}

void test_parse_stream(void) {
  g_msg = NULL;
  TestInputStream stream("$GPGSV,3,1,12,04,00,000,22,14,24,171,31,21,18,122,33,23,12,319,25*7F\r\n");
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
  g_msg = NULL;
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg == NULL);

  stream.set("$GPGSV,3,2,12,25,07,064,20,27,13,207,20,29,36,048,24,31,71,060,24*7B\r\n");
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
  g_msg = NULL;
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg == NULL);

  stream.set("$GPGSV,3,3,12,26,61,302,,16,37,269,,32,26,177,,57,06,185,*7A\r\n");
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSV);
  g_msg = NULL;
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg == NULL);

  stream.set("$GPRMC,074020.359,V,,,,,,,150316,,,N*43\r\n");
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPRMC);
  g_msg = NULL;
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg == NULL);

  stream.set("$GPGGA,074021.359,,,,,0,00,,,M,0.0,M,,0000*59\r\n");
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGGA);
  g_msg = NULL;
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg == NULL);

  stream.set("$GPGSA,M,1,,,,,,,,,,,,,,,*12\r\n");
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSA);
}

void test_parse_stream2(void) {
  g_msg = NULL;
  TestInputStream stream(
    "$GPGSV,3,1,12,04,00,000,22,14,24,171,31,21,18,122,33,23,12,319,25*7F\r\n"
    "$GPGSV,3,2,12,25,07,064,20,27,13,207,20,29,36,048,24,31,71,060,24*7B\r\n"
    "$GPGSV,3,3,12,26,61,302,,16,37,269,,32,26,177,,57,06,185,*7A\r\n"
    "$GPRMC,074020.359,V,,,,,,,150316,,,N*43\r\n"
    "$GPGGA,074021.359,,,,,0,00,,,M,0.0,M,,0000*59\r\n"
    "$GPGSA,M,1,,,,,,,,,,,,,,,*12\r\n"
  );
  GPS::NMEA::Parser<TestInputStream> parser(stream);
  parser.setHandler(handler);
  parser.yyparse();
  CU_ASSERT_FATAL(g_msg != NULL);
  CU_ASSERT(g_msg->messageID == NMEA_GPGSA);
}


void init_parsertest(void) {
  CU_pSuite suite;

  suite = CU_add_suite("Parser", NULL, NULL);
  CU_add_test(suite, "test_parse_GGA", test_parse_GGA);
  CU_add_test(suite, "test_parse_GGA_2", test_parse_GGA_2);
  CU_add_test(suite, "test_parse_GGA_3", test_parse_GGA_3);
  CU_add_test(suite, "test_parse_GGA_4", test_parse_GGA_4);
  CU_add_test(suite, "test_parse_GGA_5", test_parse_GGA_5);
  CU_add_test(suite, "test_parse_GGA_6", test_parse_GGA_6);
  CU_add_test(suite, "test_parse_GLL", test_parse_GLL);
  CU_add_test(suite, "test_parse_GLL_2", test_parse_GLL_2);
  CU_add_test(suite, "test_parse_GSA", test_parse_GSA);
  CU_add_test(suite, "test_parse_GSA_2", test_parse_GSA_2);
  CU_add_test(suite, "test_parse_GSA_3", test_parse_GSA_3);
  CU_add_test(suite, "test_parse_GSV", test_parse_GSV);
  CU_add_test(suite, "test_parse_GSV_2", test_parse_GSV_2);
  CU_add_test(suite, "test_parse_GSV_3", test_parse_GSV_3);
  CU_add_test(suite, "test_parse_GSV_4", test_parse_GSV_4);
  CU_add_test(suite, "test_parse_GSV_5", test_parse_GSV_5);
  CU_add_test(suite, "test_parse_MSS", test_parse_MSS);
  CU_add_test(suite, "test_parse_RMC", test_parse_RMC);
  CU_add_test(suite, "test_parse_RMC_2", test_parse_RMC_2);
  CU_add_test(suite, "test_parse_RMC_3", test_parse_RMC_3);
  CU_add_test(suite, "test_parse_RMC_4", test_parse_RMC_4);
  CU_add_test(suite, "test_parse_VTG", test_parse_VTG);
  CU_add_test(suite, "test_parse_VTG_2", test_parse_VTG_2);
  CU_add_test(suite, "test_parse_ZDA", test_parse_ZDA);
  CU_add_test(suite, "test_parse_ZDA_2", test_parse_ZDA_2);
  CU_add_test(suite, "test_parse_150", test_parse_150);
  CU_add_test(suite, "test_parse_150_2", test_parse_150_2);
  CU_add_test(suite, "test_parse_151", test_parse_151);
  CU_add_test(suite, "test_parse_152", test_parse_152);
  CU_add_test(suite, "test_parse_154", test_parse_154);
  CU_add_test(suite, "test_parse_stream", test_parse_stream);
  CU_add_test(suite, "test_parse_stream2", test_parse_stream2);
}
