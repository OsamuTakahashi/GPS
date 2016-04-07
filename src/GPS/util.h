/**
  @file util.h

  Utility classes and functions

  @author Osamu Takahashi
*/
#ifndef __GPS_util_h
#define __GPS_util_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

#define MAX_STRING_INPUT_BUFFER_SIZE  32

namespace GPS {

namespace NMEA {
  /**
    UTCTime structure
  */
  struct UTCTime {
    uint8_t hour; //!< 0 - 23
    uint8_t min;  //!< 0 - 59
    uint8_t sec;  //!< 0 - 59
    uint16_t msec;  //!< optional, 3-digit
  } __attribute__((__packed__));

  struct Date {
    uint8_t day;  //!< 0 - 31
    uint8_t mon;  //!< 1 - 12
    uint8_t year; //!< last 2 digit
  } __attribute__((__packed__));

#ifndef NMEA_USE_FLOAT

  /**
    integer part = signed 16 bit
    fractional part = unsigned 16 bit
  */
  struct decimal1616_t {
    int16_t integerPart;
    uint16_t fractionalPart;
  } __attribute__((__packed__));

  /**
    integer part = signed 16 bit
    fractional part = unsigned 8 bit
  */
  struct decimal168_t {
    int16_t integerPart;
    uint8_t fractionalPart;
  } __attribute__((__packed__));

  /**
    integer part = signed 8 bit
    fractional part = unsigned 16 bit
  */
  struct decimal88_t {
    int8_t integerPart;
    uint8_t fractionalPart;
  } __attribute__((__packed__));
#else
# ifndef NMEA_FLOAT
#   define NMEA_FLOAT  float
# endif

  typedef NMEA_FLOAT decimal1616_t;
  typedef NMEA_FLOAT decimal168_t;
  typedef NMEA_FLOAT decimal88_t;
#endif
}

namespace util {

  /**
    A template function; decode string to integer value
    @param T output type
    @param in_buffer source buffer
    @param in_p position in the buffer
    @param in_length buffer length
    @return decoded value
  */
  template<class T>
  T decodeInteger(const int8_t *in_buffer,int in_p,int in_length) {
    T t = 0;
    const int8_t *p = in_buffer + in_p;
    T sign = 1;
    int i = in_p;

    if (*p == '-') {
      sign = -1;
      p++;
      i++;
    }

    for (;i < in_length && isdigit(*p);i++,p++) {
      t = t * 10 + *p - '0';
    }
    return t * sign;
  }

  /**
    A template function; decode limited length decimal string to unsigned integer value
    @param T output type
    @param N length
    @param in_buffer source buffer
    @param in_p position in the buffer
    @param in_length buffer length
    @return decoded value
  */
  template<class T,int N>
  T decodeIntegerN(const int8_t *in_buffer,int in_p,int in_length) {
    T t = 0;
    const int8_t *p = in_buffer + in_p;
    int l = 0;

    for (int i = in_p;i < in_length && l < N && isdigit(*p);i++,p++,l++) {
      t = t * 10 + *p - '0';
    }
    for (;l < N;l++)
      t *= 10;
    return t;
  }

  /**
    A template function; decoding string to a separated decimal structure
    @param I output integer part type
    @param F output fractional part type
    @param FL fractional part length
    @param in_buffer source buffer
    @param in_p position in the buffer
    @param in_length buffer length
    @param out_i decoded integer part
    @param out_f decoded fractional part
  */
  template<class I,class F,int FL>
  void decodeDecimal(const int8_t *in_buffer,int in_p,int in_length,I *out_i,F *out_f) {
    I t = 0,sign = 1;
    const int8_t *p = in_buffer + in_p;
    int i = in_p;

    if (*p == '-') {
      sign = -1;
      p++;
      i++;
    }

    for (;*p != '.' && i < in_length;i++,p++) {
      if (isdigit(*p)) {
        t = t * 10 + *p - '0';
      }
    }
    *out_i = t * sign;
    if (*p == '.') {
      int fc = 0;
      p++;
      i++;
      t = 0;
      for (;isdigit(*p) && i < in_length && fc < FL;i++,p++,fc++) {
        t = t * 10 + *p - '0';
      }
      while (fc < FL) {
        t *= 10;
        fc++;
      }
      *out_f = t;
    }
  }

  /**
    A template function; decoding decimal string
    @param I output type
    @param FL fractional part length
    @param in_buffer source buffer
    @param in_p position in the buffer
    @param in_length buffer length
    @return decoded value
  */
  template<class I,int FL>
  I decodeDecimalAsInteger(const int8_t *in_buffer,int in_p,int in_length) {
    I t = 0,sign = 1;
    const int8_t *p = in_buffer + in_p;
    int i = in_p;

    if (*p == '-') {
      sign = -1;
      p++;
      i++;
    }

    for (;*p != '.' && i < in_length;i++,p++) {
      if (isdigit(*p)) {
        t = t * 10 + *p - '0';
      }
    }
    if (*p == '.') {
      int fc = 0;
      p++;
      i++;
      for (;isdigit(*p) && i < in_length && fc < FL;i++,p++,fc++) {
        t = t * 10 + *p - '0';
      }
      while (fc < FL) {
        t *= 10;
        fc++;
      }
    }
    return t * sign;
  }

  /**
    A Output port wrapper class
    Automatically calicurating NMEA type checksum
    for internal use
  */
  template<class T>
  class PortWrapper {
  public:
    PortWrapper(T &in_port)
      : m_port(in_port) {
      }
    void begin() {
      m_checksum = 0;
      m_segment = 0;
    }
    void end() {
      _makeChecksumString();
      m_port.write(m_buffer,5);
    }
    PortWrapper &operator<<(const char *str) {
      int l = strlen(str);
      m_port.write(str,l);
      _calcChecksum(str,l);
      return *this;
    }
    PortWrapper &operator<<(char c) {
      m_port.write(c);
      m_checksum ^= c;
      return *this;
    }
    PortWrapper &operator<<(int i) {
      //itoa(i,m_buffer,10);
      sprintf(m_buffer,"%d",i);
      return *this << m_buffer;
    }
  private:
    T &m_port;
    char m_buffer[16];
    uint8_t m_segment;
    int m_checksum;

    void _calcChecksum(const char *in_str,int in_l) {
      const char *p;

      if (m_segment == 0) {
        m_segment++;
        in_l--;
        p = in_str + 1;
      } else {
        p = in_str;
      }

      for (int i = 0;i < in_l;i++) {
        m_checksum ^= *p++;
      }
    }
    char _hex(int v) {
      if (v < 10) {
        return '0' + v;
      }
      return 'A' + v - 10;
    }
    void _makeChecksumString() {
      m_buffer[0] = '*';
      m_buffer[1] = _hex((m_checksum >> 4) & 0x0f);
      m_buffer[2] = _hex(m_checksum & 0x0f);
      m_buffer[3] = '\r';
      m_buffer[4] = '\n';
    }
  };

  /**
    A Input stream buffer class
    for internal use
  */
  template<class T>
  class StringInputBuffer {
  public:
    StringInputBuffer(T &in_stream)
      : m_stream(in_stream),
        m_currentPosition(0),
        m_bufferLength(0) {

      }
    int next() {
      if (m_currentPosition < m_bufferLength) {
        return m_buffer[m_currentPosition++];
      }
      if (m_currentPosition == MAX_STRING_INPUT_BUFFER_SIZE) {
        return -2;
      }
      if (m_stream.available()) {
        m_buffer[m_currentPosition] = m_stream.read();
        m_bufferLength++;
        return m_buffer[m_currentPosition++];
      }
      return -1;
    }
    int postition() const {
      return m_currentPosition - 1;
    }
    void rewind(int pos) {
      m_currentPosition = pos;
    }

    int operator[](uint16_t n) const {
      if (n < m_bufferLength) {
        return m_buffer[n];
      }
      return -1;
    }
    void accept() {
      if (m_currentPosition == m_bufferLength) {
        m_currentPosition = 0;
        m_bufferLength = 0;
      } else {
        memcpy(m_buffer,m_buffer + m_currentPosition,m_bufferLength - m_currentPosition);
        m_bufferLength -= m_currentPosition;
        m_currentPosition = 0;
      }
    }

    void decodeUTCTime(NMEA::UTCTime *io_time) const {
      int p = _decodeUTCTime2(io_time);
      if (m_buffer[p++] == '.') {
        io_time->msec = decodeIntegerN<int16_t,3>(m_buffer,p,m_currentPosition);
      }
    }

    void decodeUTCTime2(NMEA::UTCTime *io_time) const {
      _decodeUTCTime2(io_time);
    }

    void decodeDate(NMEA::Date *io_date) const {
      int p = 0;
      io_date->day = decodeIntegerN<uint8_t,2>(m_buffer,p,m_currentPosition);
      io_date->mon = decodeIntegerN<uint8_t,2>(m_buffer,p + 2,m_currentPosition);
      io_date->year = decodeIntegerN<uint8_t,2>(m_buffer,p + 4,m_currentPosition);
    }

#ifndef NMEA_USE_FLOAT
    void decodeDecimal_4_4(NMEA::decimal1616_t *out_d) const {
      decodeDecimal<int16_t,uint16_t,4>(m_buffer,0,m_currentPosition,&out_d->integerPart,&out_d->fractionalPart);
    }
    void decodeDecimal_4_3(NMEA::decimal1616_t *out_d) const {
      decodeDecimal<int16_t,uint16_t,3>(m_buffer,0,m_currentPosition,&out_d->integerPart,&out_d->fractionalPart);
    }
    void decodeDecimal_4_2(NMEA::decimal168_t *out_d) const {
      decodeDecimal<int16_t,uint8_t,2>(m_buffer,0,m_currentPosition,&out_d->integerPart,&out_d->fractionalPart);
    }
    void decodeDecimal_2_2(NMEA::decimal88_t *out_d) const {
      decodeDecimal<int8_t,uint8_t,2>(m_buffer,0,m_currentPosition,&out_d->integerPart,&out_d->fractionalPart);
    }
#else
    // Work in progress...
    void decodeDecimal_4_4(NMEA::decimal1616_t *out_d) const {
      NMEA_FLOAT i,f;
      decodeDecimal<NMEA_FLOAT,NMEA_FLOAT,4>(m_buffer,0,m_currentPosition,&i,&f);
      *out_d = i + f / 10000.0;
    }
    void decodeDecimal_4_3(NMEA::decimal1616_t *out_d) const {
      NMEA_FLOAT i,f;
      decodeDecimal<NMEA_FLOAT,NMEA_FLOAT,4>(m_buffer,0,m_currentPosition,&i,&f);
      *out_d = i + f / 1000.0;
    }
    void decodeDecimal_4_2(NMEA::decimal1616_t *out_d) const {
      NMEA_FLOAT i,f;
      decodeDecimal<NMEA_FLOAT,NMEA_FLOAT,4>(m_buffer,0,m_currentPosition,&i,&f);
      *out_d = i + f / 100.0;
    }
    void decodeDecimal_2_2(NMEA::decimal88_t *out_d) const {
      NMEA_FLOAT i,f;
      decodeDecimal<NMEA_FLOAT,NMEA_FLOAT,4>(m_buffer,0,m_currentPosition,&i,&f);
      *out_d = i + f / 100.0;
    }
#endif /* NMEA_USE_FLOAT */

    int16_t decodeInt16() const {
      return decodeInteger<int16_t>(m_buffer,0,m_currentPosition);
    }

    int32_t decodeLong() const {
      return decodeInteger<int32_t>(m_buffer,0,m_currentPosition);
    }

    uint32_t decodeHex8() const {
      int p = 2;
      uint8_t t[4];

      t[3] = (_hexValue(m_buffer[2]) << 4) | _hexValue(m_buffer[3]) ;
      t[2] = (_hexValue(m_buffer[4]) << 4) | _hexValue(m_buffer[5]) ;
      t[1] = (_hexValue(m_buffer[6]) << 4) | _hexValue(m_buffer[7]) ;
      t[0] = (_hexValue(m_buffer[8]) << 4) | _hexValue(m_buffer[9]) ;
      return *((uint32_t *)&t[0]);
    }

    // *XX format
    uint8_t decodeChecksum() const {
      return (_hexValue(m_buffer[1]) << 4) | _hexValue(m_buffer[2]);
    }

    int calcChecksum(int in_base) const {
      int c = in_base;
      for (int i = 0;i < m_currentPosition;i++) {
        c ^= m_buffer[i];
      }
      return c;
    }
  private:
    T &m_stream;
    int8_t m_buffer[MAX_STRING_INPUT_BUFFER_SIZE];
    int m_currentPosition;
    int m_bufferLength;

    int _decodeUTCTime2(NMEA::UTCTime *io_time) const {
      int p = 0;
      io_time->hour = decodeIntegerN<uint8_t,2>(m_buffer,p,m_currentPosition);
      io_time->min = decodeIntegerN<uint8_t,2>(m_buffer,p + 2,m_currentPosition);
      io_time->sec = decodeIntegerN<uint8_t,2>(m_buffer,p + 4,m_currentPosition);
      return p + 6;
    }

    int _hexValue(int c) const {
      if (isdigit(c)) {
        return c - '0';
      } else
      if ('a' <= c && c <= 'f') {
        return 10 + c - 'a';
      } else
      if ('A' <= c && c <= 'F') {
        return 10 + c - 'A';
      }
      return 0;
    }
  };

} /* util */

} /* GPS */

#endif /* __GPS_util_h */
