#include <CUnit/CUnit.h>
#include <GPS.h>
#include <string.h>

class TestPort {
public:
  TestPort()
    :m_position(0) {
    }

  void write(char c) {
    m_buffer[m_position++] = c;
  }
  void write(const char *in_str,int in_l) {
    memcpy(m_buffer + m_position,in_str,in_l);
    m_position += in_l;
  }

  const char *string() {
    m_buffer[m_position] = 0;
    return m_buffer;
  }
private:
  char m_buffer[128];
  int m_position;
};

class TestStream {
public:
  TestStream(const char *in_msg)
    : m_msg(in_msg) {}
  bool available() const {
    return *m_msg;
  }
  char read() {
    return *m_msg++;
  }
private:
  const char *m_msg;
};

void test_decodeIntegerN(void) {
  const char *txt = "123";
  int16_t i;
  i = GPS::util::decodeIntegerN<int16_t,3>((const int8_t *)txt,0,3);
  CU_ASSERT(i == 123);
}

void test_decodeIntegerN_2(void) {
  const char *txt = "123";
  int16_t i;
  i = GPS::util::decodeIntegerN<int16_t,4>((const int8_t *)txt,0,3);
  CU_ASSERT(i == 1230);
}

void test_decodeIntegerN_3(void) {
  const char *txt = "123";
  int16_t i;
  i = GPS::util::decodeIntegerN<int16_t,2>((const int8_t *)txt,0,3);
  CU_ASSERT(i == 12);
}

void test_decodeDecimal_4_4(void) {
  const char *txt = "12345.6789";
  int16_t i;
  uint16_t f;
  GPS::util::decodeDecimal<int16_t,uint16_t,4>((const int8_t *)txt,0,10,&i,&f);
  CU_ASSERT(f == 6789);
  CU_ASSERT(i == 12345);
}

void test_decodeDecimal_4_4_2(void) {
  const char *txt = "12345.678";
  int16_t i;
  uint16_t f;
  GPS::util::decodeDecimal<int16_t,uint16_t,4>((const int8_t *)txt,0,10,&i,&f);
  CU_ASSERT(f == 6780);
  CU_ASSERT(i == 12345);
}

void test_decodeDecimal_4_4_3(void) {
  const char *txt = "-12345.6789";
  int16_t i;
  uint16_t f;
  GPS::util::decodeDecimal<int16_t,uint16_t,4>((const int8_t *)txt,0,11,&i,&f);
  CU_ASSERT(f == 6789);
  CU_ASSERT(i == -12345);
}

void test_decodeDecimal_4_4_4(void) {
  const char *txt = "12345.67891";
  int16_t i;
  uint16_t f;
  GPS::util::decodeDecimal<int16_t,uint16_t,4>((const int8_t *)txt,0,10,&i,&f);
  CU_ASSERT(f == 6789);
  CU_ASSERT(i == 12345);
}

void test_decodeDecimal_4_4_float(void) {
  const char *txt = "12345.6789";
  float i;
  float f;
  GPS::util::decodeDecimal<float,float,4>((const int8_t *)txt,0,10,&i,&f);
  CU_ASSERT(f == 6789);
  CU_ASSERT(i == 12345);
}

void test_decodeUTCTime(void) {
  GPS::NMEA::UTCTime t;
  TestStream st("104549.04");
  GPS::util::StringInputBuffer<TestStream> inst(st);
  for (int i = 0;i < 9;i++) inst.next();
  inst.decodeUTCTime(&t);

  CU_ASSERT(t.hour == 10);
  CU_ASSERT(t.min == 45);
  CU_ASSERT(t.sec == 49);
  CU_ASSERT(t.msec == 40);
}

void test_portWrapper(void) {
  TestPort port;
  GPS::util::PortWrapper<TestPort> pw(port);
  pw.begin();
  pw
    << "$GPGGA"
    << ','
    << "002153.000,3342.6618,N,11751.3858,W,1,"
    << 10
    << ",1.2,27.0,M,"
    << -34
    << ".2,M,,0000";
  pw.end();
  CU_ASSERT(strcmp(port.string(),"$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E\r\n") == 0);
}

void test_decimal1616_t(void) {
  GPS::NMEA::decimal1616_t a,b;
  memset(&a,0xff,sizeof(GPS::NMEA::decimal1616_t));
  b.integerPart = 123;
  b.fractionalPart = 456;
  a = b;
  CU_ASSERT(a.integerPart == b.integerPart);
  CU_ASSERT(b.fractionalPart == b.fractionalPart);
}

void init_utiltest(void) {
  CU_pSuite suite;

  suite = CU_add_suite("Util", NULL, NULL);
  CU_add_test(suite, "test_decodeIntegerN", test_decodeIntegerN);
  CU_add_test(suite, "test_decodeIntegerN_2", test_decodeIntegerN_2);
  CU_add_test(suite, "test_decodeIntegerN_3", test_decodeIntegerN_3);
  CU_add_test(suite, "test_decodeDecimal_4_4", test_decodeDecimal_4_4);
  CU_add_test(suite, "test_decodeDecimal_4_4_2", test_decodeDecimal_4_4_2);
  CU_add_test(suite, "test_decodeDecimal_4_4_3", test_decodeDecimal_4_4_3);
  CU_add_test(suite, "test_decodeDecimal_4_4_4", test_decodeDecimal_4_4_4);
  CU_add_test(suite, "test_decodeDecimal_4_4_float", test_decodeDecimal_4_4_float);
  CU_add_test(suite, "test_decodeUTCTime", test_decodeUTCTime);
  CU_add_test(suite, "test_portWrapper", test_portWrapper);
  CU_add_test(suite, "test_decimal1616_t", test_decimal1616_t);
}
