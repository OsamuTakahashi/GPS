#include <CUnit/CUnit.h>
#include <GPS.h>
#include "TestInputStream.h"

void test_lex_NUM(void) {
  TestInputStream stream("0,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_NUM);
  CU_ASSERT(lex.buffer().decodeInt16() == 0);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_NUM2(void) {
  TestInputStream stream("-1,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_NUM);
  //CU_ASSERT(lex.buffer().decodeInt16() == -1);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_NUM3(void) {
  TestInputStream stream("    \t1,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_NUM);
  CU_ASSERT(lex.buffer().decodeInt16() == 1);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_NUM4(void) {
  TestInputStream stream("256,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_NUM);
  CU_ASSERT(lex.buffer().decodeInt16() == 256);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_FLT_NUM(void) {
  TestInputStream stream("0.0,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);
  GPS::NMEA::decimal1616_t v;

  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  lex.buffer().decodeDecimal_4_4(&v);
  CU_ASSERT(v.integerPart == 0);
  CU_ASSERT(v.fractionalPart == 0);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_FLT_NUM2(void) {
  TestInputStream stream("-1.0,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);
  GPS::NMEA::decimal1616_t v;

  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  lex.buffer().decodeDecimal_4_4(&v);
  CU_ASSERT(v.integerPart == -1);
  CU_ASSERT(v.fractionalPart == 0);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_FLT_NUM3(void) {
  TestInputStream stream("123.456,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);
  GPS::NMEA::decimal1616_t v;

  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  lex.buffer().decodeDecimal_4_4(&v);
  CU_ASSERT(v.integerPart == 123);
  CU_ASSERT(v.fractionalPart == 4560);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_NL(void) {
  TestInputStream stream("\r\n");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_NL);
  CU_ASSERT(lex.yylex() == -1);
}

void test_lex_NL2(void) {
  TestInputStream stream("  \t \r\n");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_NL);
  CU_ASSERT(lex.yylex() == -1);
}

void test_lex_HEX8(void) {
  TestInputStream stream("0x1234ABCD,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_HEX8);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_HEX8_2(void) {
  TestInputStream stream("0x1234ABCD0,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_HEX8);
  CU_ASSERT(lex.yylex() == NMEA_NUM);
}

void test_lex_HEX8_3(void) {
  TestInputStream stream("0x1234ABC,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_NUM);
  CU_ASSERT(lex.yylex() == 'x');
}

void test_lex_CHECKSUM(void) {
  TestInputStream stream("*0F,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_CHECKSUM);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_CHECKSUM2(void) {
  TestInputStream stream("*00,");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == NMEA_CHECKSUM);
  CU_ASSERT(lex.yylex() == ',');
}

void test_lex_ggaSentence(void) {
  TestInputStream stream("$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n");
  GPS::NMEA::Lexer<TestInputStream> lex(stream);

  CU_ASSERT(lex.yylex() == '$');
  CU_ASSERT(lex.yylex() == NMEA_TOKEN_GPGGA);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == 'N');
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == 'W');
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == 'M');
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_FLT_NUM);
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == 'M');
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == ',');
  CU_ASSERT(lex.yylex() == NMEA_NUM);
  CU_ASSERT(lex.yylex() == NMEA_CHECKSUM);
  CU_ASSERT(lex.yylex() == NMEA_NL);
  CU_ASSERT(lex.yylex() == -1);
}

void init_lexertest(void) {
  CU_pSuite suite;

  suite = CU_add_suite("Lexer", NULL, NULL);
  CU_add_test(suite, "test_lex_NUM", test_lex_NUM);
  CU_add_test(suite, "test_lex_NUM2", test_lex_NUM2);
  CU_add_test(suite, "test_lex_NUM3", test_lex_NUM3);
  CU_add_test(suite, "test_lex_NUM4", test_lex_NUM4);
  CU_add_test(suite, "test_lex_FLT_NUM", test_lex_FLT_NUM);
  CU_add_test(suite, "test_lex_FLT_NUM2", test_lex_FLT_NUM2);
  CU_add_test(suite, "test_lex_FLT_NUM3", test_lex_FLT_NUM3);
  CU_add_test(suite, "test_lex_NL", test_lex_NL);
  CU_add_test(suite, "test_lex_NL2", test_lex_NL2);
  CU_add_test(suite, "test_lex_HEX8", test_lex_HEX8);
  CU_add_test(suite, "test_lex_HEX8_2", test_lex_HEX8_2);
  CU_add_test(suite, "test_lex_HEX8_3", test_lex_HEX8_3);
  CU_add_test(suite, "test_lex_CHECKSUM", test_lex_CHECKSUM);
  CU_add_test(suite, "test_lex_CHECKSUM2", test_lex_CHECKSUM2);
  CU_add_test(suite, "test_lex_ggaSentence", test_lex_ggaSentence);
}
