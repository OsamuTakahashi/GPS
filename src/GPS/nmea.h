/**
  @file nmea.h

  A NMEA Message parsing library

  @author Osamu Takahashi
*/
#ifndef __GPS_nmea_h
#define __GPS_nmea_h

#include <string.h>
#include <inttypes.h>
#include <GPS/util.h>

#define YY_SC_TO_UI(c) ((unsigned int) (unsigned char) c)
#define YY_MAX_STATE  64
#define YY_STOP       148

#define YY_RULE_SETUP
#define YY_BREAK break;
#define ECHO

/**
  NMEA message ID
*/

#define NMEA_GPGGA     1
#define NMEA_GPGLL     2
#define NMEA_GPGSA     3
#define NMEA_GPGSV     4
#define NMEA_GPMSS     5
#define NMEA_GPRMC     6
#define NMEA_GPVTG     7
#define NMEA_GPZDA     8
#define NMEA_PSRF140   9
#define NMEA_PSRF150   10
#define NMEA_PSRF151   11
#define NMEA_PSRF152   12
#define NMEA_PSRF154   13
#define NMEA_PSRF155   14

/**
  NMEA Token definitions, internal use
*/

#define NMEA_TOKEN(n)   (1024 + n)

#define NMEA_TOKEN_GPGGA     NMEA_TOKEN(NMEA_GPGGA)
#define NMEA_TOKEN_GPGLL     NMEA_TOKEN(NMEA_GPGLL)
#define NMEA_TOKEN_GPGSA     NMEA_TOKEN(NMEA_GPGSA)
#define NMEA_TOKEN_GPGSV     NMEA_TOKEN(NMEA_GPGSV)
#define NMEA_TOKEN_GPMSS     NMEA_TOKEN(NMEA_GPMSS)
#define NMEA_TOKEN_GPRMC     NMEA_TOKEN(NMEA_GPRMC)
#define NMEA_TOKEN_GPVTG     NMEA_TOKEN(NMEA_GPVTG)
#define NMEA_TOKEN_GPZDA     NMEA_TOKEN(NMEA_GPZDA)
#define NMEA_TOKEN_PSRF140   NMEA_TOKEN(NMEA_PSRF140)
#define NMEA_TOKEN_PSRF150   NMEA_TOKEN(NMEA_PSRF150)
#define NMEA_TOKEN_PSRF151   NMEA_TOKEN(NMEA_PSRF151)
#define NMEA_TOKEN_PSRF152   NMEA_TOKEN(NMEA_PSRF152)
#define NMEA_TOKEN_PSRF154   NMEA_TOKEN(NMEA_PSRF154)
#define NMEA_TOKEN_PSRF155   NMEA_TOKEN(NMEA_PSRF155)

#define NMEA_CHECKSUM  NMEA_TOKEN(0)
#define NMEA_NL        NMEA_TOKEN(-1)
#define NMEA_NUM       NMEA_TOKEN(-2)
#define NMEA_FLT_NUM   NMEA_TOKEN(-3)
#define NMEA_HEX8      NMEA_TOKEN(-4)

#define NMEA_STATE_MASK 0x07ff

#ifndef NMEA_USE_PARTLY
#define NMEA_USE_GGA
#define NMEA_USE_GLL
#define NMEA_USE_GSA
#define NMEA_USE_GSV
#define NMEA_USE_MSS
#define NMEA_USE_RMC
#define NMEA_USE_VTG
#define NMEA_USE_ZDA
#define NMEA_USE_150
#define NMEA_USE_151
#define NMEA_USE_152
#define NMEA_USE_154
#endif

namespace GPS {

namespace NMEA {

  /**
    for SetSerialPort
  */
  enum ProtocolType {
    PROTOCOL_SiRF_binary   = 0,
    PROTOCOL_NMEA          = 1
  };

  /**
    QueryRateControl target message type
  */
  enum MessageType {
    MSG_GGA  = 0,
    MSG_GLL  = 1,
    MSG_GSA  = 2,
    MSG_GSV  = 3,
    MSG_RMC  = 4,
    MSG_VTG  = 5,
    MSG_MSS  = 6,
    MSG_ZDA  = 8
  };

  /**
    QueryRateControl control mode
  */
  enum QueryRateControlMode {
    SET_RATE = 0,
    QUERY    = 1
  };

  /**
    Parsed GGA message structure
    Filled with 0xff if unused
    @see UTCTime
    @see decimal1616_t
    @see decimal88_t
  */
  struct GGA {
    UTCTime       utcTime;
    decimal1616_t latitude;         //!< .4 format
    int8_t        nsIndicator;
    decimal1616_t longitude;        //!< .4 format
    int8_t        ewIndicator;
    int8_t        positionFixIndicator;
    int8_t        satelitesUsed;
    decimal88_t   hdop;             //!< .2 format
    decimal88_t   mslAltitude;      //!< .2 format
    int8_t        units;
    decimal88_t   geoidSeparation;  //!< .2 format
    int8_t        units2;
    uint16_t      ageOfDiffCorr;
    uint16_t      diffRefStationID;
  } __attribute__((__packed__));

  /**
    Parsed GLL message structure
    Filled with 0xff if unused
    @see UTCTime
    @see decimal1616_t
  */
  struct GLL {
    decimal1616_t latitude;
    int8_t        nsIndicator;
    decimal1616_t longitude;
    int8_t        ewIndicator;
    UTCTime       utcTime;
    int8_t        status;
    int8_t        mode;
  } __attribute__((__packed__));

  /**
    Parsed GSA message structure
    Filled with 0xff if unused
    @see UTCTime
    @see decimal88_t
  */
  struct GSA {
    int8_t        mode1;
    int8_t        mode2;
    int8_t        satelliteUsed[12];
    decimal88_t   pdop;
    decimal88_t   hdop;
    decimal88_t   vdop;
  } __attribute__((__packed__));

  /**
    Parsed GSV message structure
    Filled with 0xff if unused
  */
  struct GSV {
    int8_t        numberOfMessages;
    int8_t        messageNumber;
    int8_t        satellitesInView;
    struct {
      int8_t      satelliteID;
      int8_t      elevation;
      int16_t     azimuth;
      int8_t      snr;
    } satellites[4];
  } __attribute__((__packed__));

  /**
    Parsed MSS message structure
    Filled with 0xff if unused
    @see decimal88_t
  */
  struct MSS {
    int8_t        signalStrength;
    int8_t        signalToNoiseRatio;
    decimal168_t  beaconFrequency;
    int8_t        beaconBitRate;
    int8_t        channelNumber;
  } __attribute__((__packed__));

  /**
    Parsed RMC message structure
    Filled with 0xff if unused
    @see UTCTime
    @see Date
    @see decimal1616_t
    @see decimal168_t
  */
  struct RMC {
    UTCTime       utcTime;
    int8_t        status;
    decimal1616_t latitude;
    int8_t        nsIndicator;
    decimal1616_t longitude;
    int8_t        ewIndicator;
    decimal168_t  speedOverGround;
    decimal168_t  courseOverGround;
    Date          date;
    decimal168_t  magneticVariation;
    int8_t        ewIndicator2;
    int8_t        mode;
  } __attribute__((__packed__));

  /**
    Parsed VTG message structure
    Filled with 0xff if unused
    @see decimal168_t
    @see decimal88_t
  */
  struct VTG {
    decimal168_t  course;
    int8_t        reference;
    decimal168_t  course2;
    int8_t        reference2;
    decimal88_t   speed;
    int8_t        units;
    decimal88_t   speed2;
    int8_t        units2;
    int8_t        mode;
  } __attribute__((__packed__));

  /**
    Parsed ZDA message structure
    Filled with 0xff if unused
    @see UTCTime
  */
  struct ZDA {
    UTCTime       utcTime;
    int8_t        day;
    int8_t        month;
    int16_t       year;
    int8_t        localZoneHour;
    int8_t        localZoneMinutes;
  } __attribute__((__packed__));

  /**
    Parsed 150 message structure
    Filled with 0xff if unused
  */
  struct _150 {
    int8_t        okToSend;
  } __attribute__((__packed__));

  /**
    Parsed 151 message structure
    Filled with 0xff if unused
  */
  struct _151 {
    uint8_t       gpsTimeValidFlag;
    uint16_t      gpsWeek;
    uint16_t      gpsTOW;
    uint32_t      ephReqMask;
  } __attribute__((__packed__));

  /**
    Parsed 152 message structure
    Filled with 0xff if unused
  */
  struct _152 {
    uint32_t      satPosValidityFlag;
    uint32_t      satClkValidityFlag;
    uint32_t      satHealthFlag;
  } __attribute__((__packed__));

  /**
    Parsed 154 message structure
    Filled with 0xff if unused
  */
  struct _154 {
    int8_t        ackID;
  } __attribute__((__packed__));

  /**

  */
  struct Message {
    /// NMEA message ID
    uint8_t       messageID;
    union {
#ifdef NMEA_USE_GGA
      GGA         gga;
#endif
#ifdef NMEA_USE_GLL
      GLL         gll;
#endif
#ifdef NMEA_USE_GSA
      GSA         gsa;
#endif
#ifdef NMEA_USE_GSV
      GSV         gsv;
#endif
#ifdef NMEA_USE_MSS
      MSS         mss;
#endif
#ifdef NMEA_USE_RMC
      RMC         rmc;
#endif
#ifdef NMEA_USE_VTG
      VTG         vtg;
#endif
#ifdef NMEA_USE_ZDA
      ZDA         zda;
#endif
#ifdef NMEA_USE_150
      _150        okToSend;
#endif
#ifdef NMEA_USE_151
      _151        gpsDataAndEEM;
#endif
#ifdef NMEA_USE_152
      _152        eei;
#endif
#ifdef NMEA_USE_154
      _154        eeAck;
#endif
    };
  } __attribute__((__packed__));

  //! ParserHandler function type
  typedef void (*ParserHandler)(const Message &);

  /**
    NMEA command output class
    @param T output stream class
  */
  template<class Port>
  class CommandBuilder {
  public:
    /**
      A constructor
      @param in_port a output stream
    */
    CommandBuilder(Port &in_port)
      : m_port(util::PortWrapper<Port>(in_port)) {
    }

    /**
      Output SetSerialPort message ($PSRF100) to a device
      @param protocolType either PROTOCOL_SiRF_binary or PROTOCOL_NMEA
      @param baud baud rate
      @param dataBits data bits, default = 8
      @param stopBits stop bits, default = 1
      @param parity parity, default = 0
      @see ProtocolType
    */
    void SetSerialPort(ProtocolType protocolType,int baud,int dataBits = 8,int stopBits = 1,int parity = 0) {
      m_port.begin();
      m_port
        << "$PSRF100,"
        << protocolType
        << ","
        << baud
        << ","
        << dataBits
        << ","
        << stopBits
        << ","
        << parity;
      m_port.end();
    }

    /**
      Output QueryRateControl message ($PSRF103) to a device
      @param message one of MessageType
      @param mode one of QueryRateControlMode
      @param rate
      @param checksumEnable
      @see MessageType
      @see QueryRateControlMode
    */
    void QueryRateControl(MessageType message,QueryRateControlMode mode,int rate,int checksumEnable) {
      m_port.begin();
      m_port
        << "$PSRF103,"
        << message
        << ","
        << mode
        << ","
        << rate
        << ","
        << checksumEnable;
      m_port.end();

    }
  private:
    util::PortWrapper<Port> m_port;
  };

  /**
    Lexer related variables
  */

  extern const uint8_t yy_accept[];
  extern const uint8_t yy_ec[];
  extern const uint8_t yy_meta[];
  extern const uint8_t yy_base[];
  extern const uint8_t yy_nxt[];
  extern const uint8_t yy_def[];
  extern const uint8_t yy_chk[];

  /**
    NMEA Lexer class, internal use
  */
  template<class T>
  class Lexer {
  public:
    Lexer(T &in_buffer)
      : m_buffer(in_buffer),
        m_checksum(0),
        m_last_checksum(0) {
        yy_current_state = 0;

      }

    int yylex();
    uint8_t checksum() const {
      return m_checksum & 0xff;
    }
    void clearChecksum() {
      m_checksum = 0;
      m_last_checksum = 0;
    }
    const GPS::util::StringInputBuffer<T> &buffer() const {
      return m_buffer;
    }
  private:
    GPS::util::StringInputBuffer<T> m_buffer;
    int yy_current_state;
    int yy_last_accepting_state;
    int yy_last_accepting_cpos;
    int m_checksum;
    int m_last_checksum;
  };

#define TK2ST(n)  ((n - 1024) << 11)
#define TK2ID(n)  (n - 1024)

  /**
    NMEA Parser class
    @param T input stream class
  */
  template<class T>
  class Parser {
  public:
    /**
      constructor
      @param in_stream a input stream
    */
    Parser(T &in_stream)
      : m_lexer(in_stream),
        m_current_state(0),
        m_handler(NULL) {

        }

    /**
      Set ParserHandler
      @param in_handler a parser handler pointer
    */
    void setHandler(ParserHandler in_handler) {
      m_handler = in_handler;
    }

    /**
      parse from stream
    */
    void yyparse(void) {
    retry:
      int t = m_lexer.yylex();
      if (t >= 0) {
        switch(m_current_state & 0xf800) {
          case STATE_0:
            _state0(t);
            break;
#ifdef NMEA_USE_GGA
          case STATE_GGA:
            _state_GPGGA(t);
            break;
#endif
#ifdef NMEA_USE_GLL
          case STATE_GLL:
            _state_GPGLL(t);
            break;
#endif
#ifdef NMEA_USE_GSA
          case STATE_GSA:
            _state_GPGSA(t);
            break;
#endif
#ifdef NMEA_USE_GSV
          case STATE_GSV:
            _state_GPGSV(t);
            break;
#endif
#ifdef NMEA_USE_MSS
          case STATE_MSS:
            _state_GPMSS(t);
            break;
#endif
#ifdef NMEA_USE_RMC
          case STATE_RMC:
            _state_GPRMC(t);
            break;
#endif
#ifdef NMEA_USE_VTG
          case STATE_VTG:
            _state_GPVTG(t);
            break;
#endif
#ifdef NMEA_USE_ZDA
          case STATE_ZDA:
            _state_GPZDA(t);
            break;
#endif
#ifdef NMEA_USE_150
          case STATE_150:
            _state_150(t);
            break;
#endif
#ifdef NMEA_USE_151
          case STATE_151:
            _state_151(t);
            break;
#endif
#ifdef NMEA_USE_152
          case STATE_152:
            _state_152(t);
            break;
#endif
#ifdef NMEA_USE_154
          case STATE_154:
            _state_154(t);
            break;
#endif
          case STATE_WAIT_NL:
            _wait_NL(t);
            break;
          default:
            _error(t);
            break;
        }
        goto retry;
      }
    }

    const GPS::util::StringInputBuffer<T> &buffer() const {
      return m_lexer.buffer();
    }
  private:
    Lexer<T> m_lexer;
    ParserHandler m_handler;
    Message m_message;

    uint16_t m_current_state;

    enum {
      STATE_0 = 0,
      STATE_GGA = TK2ST(NMEA_TOKEN_GPGGA),
      STATE_GLL = TK2ST(NMEA_TOKEN_GPGLL),
      STATE_GSA = TK2ST(NMEA_TOKEN_GPGSA),
      STATE_GSV = TK2ST(NMEA_TOKEN_GPGSV),
      STATE_MSS = TK2ST(NMEA_TOKEN_GPMSS),
      STATE_RMC = TK2ST(NMEA_TOKEN_GPRMC),
      STATE_VTG = TK2ST(NMEA_TOKEN_GPVTG),
      STATE_ZDA = TK2ST(NMEA_TOKEN_GPZDA),
      STATE_150 = TK2ST(NMEA_TOKEN_PSRF150),
      STATE_151 = TK2ST(NMEA_TOKEN_PSRF151),
      STATE_152 = TK2ST(NMEA_TOKEN_PSRF152),
      STATE_154 = TK2ST(NMEA_TOKEN_PSRF154),
      STATE_WAIT_NL = 0xe800,
      STATE_ERROR = 0xf800
    };

    void _clearData() {
      memset(&m_message,0xff,sizeof(Message));
    }

    void _state0(int in_token) {
      switch(m_current_state & NMEA_STATE_MASK) {
        case 0:
          switch (in_token) {
            case '$':
              m_current_state++;
              m_lexer.clearChecksum();
              break;
            default:
              m_current_state = STATE_ERROR;
              _error(in_token);
          }
          break;
        case 1:
          switch (in_token) {
#ifdef NMEA_USE_GGA
            case NMEA_TOKEN_GPGGA:
#endif
#ifdef NMEA_USE_GLL
            case NMEA_TOKEN_GPGLL:
#endif
#ifdef NMEA_USE_GSA
            case NMEA_TOKEN_GPGSA:
#endif
#ifdef NMEA_USE_GSV
            case NMEA_TOKEN_GPGSV:
#endif
#ifdef NMEA_USE_MSS
            case NMEA_TOKEN_GPMSS:
#endif
#ifdef NMEA_USE_RMC
            case NMEA_TOKEN_GPRMC:
#endif
#ifdef NMEA_USE_VTG
            case NMEA_TOKEN_GPVTG:
#endif
#ifdef NMEA_USE_ZDA
            case NMEA_TOKEN_GPZDA:
#endif
//        case NMEA_TOKEN_PSRF140:
#ifdef NMEA_USE_150
            case NMEA_TOKEN_PSRF150:
#endif
#ifdef NMEA_USE_151
            case NMEA_TOKEN_PSRF151:
#endif
#ifdef NMEA_USE_152
            case NMEA_TOKEN_PSRF152:
#endif
#ifdef NMEA_USE_154
            case NMEA_TOKEN_PSRF154:
#endif
//        case NMEA_TOKEN_PSRF155:
              m_current_state = TK2ST(in_token);
              _clearData();
              m_message.messageID = TK2ID(in_token);
              break;
            default:
              m_current_state = STATE_ERROR;
              _error(in_token);
          }
      }
    }



#ifdef NMEA_USE_GGA
    void _state_GPGGA(int in_token);
#endif
#ifdef NMEA_USE_GLL
    void _state_GPGLL(int in_token);
#endif
#ifdef NMEA_USE_GSA
    void _state_GPGSA(int in_token);
#endif
#ifdef NMEA_USE_GSV
    void _state_GPGSV(int in_token);
#endif
#ifdef NMEA_USE_MSS
    void _state_GPMSS(int in_token);
#endif
#ifdef NMEA_USE_RMC
    void _state_GPRMC(int in_token);
#endif
#ifdef NMEA_USE_VTG
    void _state_GPVTG(int in_token);
#endif
#ifdef NMEA_USE_ZDA
    void _state_GPZDA(int in_token);
#endif
#ifdef NMEA_USE_150
    void _state_150(int in_token);
#endif
#ifdef NMEA_USE_151
    void _state_151(int in_token);
#endif
#ifdef NMEA_USE_152
    void _state_152(int in_token);
#endif
#ifdef NMEA_USE_154
    void _state_154(int in_token);
#endif
    void _wait_NL(int in_token) {
      if (in_token == NMEA_NL) {
        if (m_handler)
          (*m_handler)(m_message);
        m_current_state = 0;
      } else {
        m_current_state = STATE_ERROR;
        _error(in_token);
      }
    }
    void _error(int in_token) {
      if (in_token == NMEA_NL) {
        m_current_state = 0;
      }
    }
  };

  // implementations ...

  template<class T>
  int Lexer<T>::yylex() {
    next:
    if (yy_current_state == 0) {
      m_buffer.accept();
      yy_current_state = 1;
      m_checksum = m_last_checksum;
    }
    do {
      int next = m_buffer.next();
      if (next < 0)
        return -1;

      char yy_c = yy_ec[next];
      if ( yy_accept[yy_current_state] ) {
        yy_last_accepting_state = yy_current_state;
        yy_last_accepting_cpos = m_buffer.postition();
      }
      while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state ) {
        yy_current_state = (int) yy_def[yy_current_state];
        if ( yy_current_state >= YY_MAX_STATE )
          yy_c = yy_meta[(unsigned int) yy_c];
      }
      yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
    } while ( yy_base[yy_current_state] != YY_STOP );

    int yy_act = yy_accept[yy_current_state];
    if ( yy_act == 0 ) {
      m_buffer.rewind(yy_last_accepting_cpos);
      yy_current_state = yy_last_accepting_state;
      yy_act = yy_accept[yy_current_state];
    }
    yy_current_state = 0;
    m_last_checksum = m_buffer.calcChecksum(m_checksum);

    switch ( yy_act ) {
      case 1:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPGGA;
      	YY_BREAK
      case 2:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPGLL;
      	YY_BREAK
      case 3:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPGSA;
      	YY_BREAK
      case 4:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPGSV;
      	YY_BREAK
      case 5:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPMSS;
      	YY_BREAK
      case 6:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPRMC;
      	YY_BREAK
      case 7:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPVTG;
      	YY_BREAK
      case 8:
        YY_RULE_SETUP
        return NMEA_TOKEN_GPZDA;
      	YY_BREAK
      case 9:
        YY_RULE_SETUP
        return NMEA_TOKEN_PSRF140;
      	YY_BREAK
      case 10:
        YY_RULE_SETUP
        return NMEA_TOKEN_PSRF150;
      	YY_BREAK
      case 11:
        YY_RULE_SETUP
        return NMEA_TOKEN_PSRF151;
      	YY_BREAK
      case 12:
        YY_RULE_SETUP
        return NMEA_TOKEN_PSRF152;
      	YY_BREAK
      case 13:
        YY_RULE_SETUP
        return NMEA_TOKEN_PSRF154;
      	YY_BREAK
      case 14:
        YY_RULE_SETUP
        return NMEA_TOKEN_PSRF155;
      	YY_BREAK
      case 15:
        YY_RULE_SETUP
        return NMEA_NUM;
      	YY_BREAK
      case 16:
        YY_RULE_SETUP
        return NMEA_FLT_NUM;
      	YY_BREAK
      case 17:
        YY_RULE_SETUP
        return NMEA_HEX8;
        YY_BREAK
      case 18:
        YY_RULE_SETUP
        return NMEA_CHECKSUM;
      	YY_BREAK
      case 19:
        YY_RULE_SETUP
        ;
      	YY_BREAK
      case 20:
      /* rule 20 can match eol */
        YY_RULE_SETUP
        m_checksum = 0;
        m_last_checksum = 0;
        return NMEA_NL;
      	YY_BREAK
      case 21:
        YY_RULE_SETUP
        return m_buffer[0];
      	YY_BREAK
      case 22:
        YY_RULE_SETUP
      	YY_BREAK
    }
    goto next;
  }

#define NMEA_SKIP        else \
  if (in_token == ',') { \
    m_current_state += 2; \
  }

#define NMEA_SKIP_LAST(n)  if (in_token == NMEA_CHECKSUM) { \
          m_current_state += n; \
          goto retry; \
        } else


#define NMEA_ON_ERROR    else { \
        m_current_state = STATE_ERROR;  \
        _error(in_token); \
      } \
      break

#define NMEA_ON_CHECKSUM  if (in_token == NMEA_CHECKSUM) { \
          if (buffer().decodeChecksum() == m_lexer.checksum())  \
            m_current_state = STATE_WAIT_NL; \
          else \
            m_current_state = STATE_ERROR;  \
        } \
        NMEA_ON_ERROR;


#ifdef NMEA_USE_GGA
  template<class T>
  void Parser<T>::_state_GPGGA(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
        // UTC Time
      case 1:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeUTCTime(&m_message.gga.utcTime);
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Latitude
      case 3:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_4(&m_message.gga.latitude);
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // N/S Indicator
      case 5:
        if (in_token == 'N' || in_token == 'S') {
          m_message.gga.nsIndicator = in_token;
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Longitude
      case 7:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_4(&m_message.gga.longitude);
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // E/W Indicator
      case 9:
        if (in_token == 'E' || in_token == 'W') {
          m_message.gga.ewIndicator = in_token;
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Position Fix Indicator
      case 11:
        if (in_token == NMEA_NUM) {
          m_message.gga.positionFixIndicator = buffer().decodeInt16();
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Stelites Used
      case 13:
        if (in_token == NMEA_NUM) {
          m_message.gga.satelitesUsed = buffer().decodeInt16();
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // HDOP
      case 15:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_2_2(&m_message.gga.hdop);
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // MSL Altitude
      case 17:
        if (in_token == NMEA_FLT_NUM) {
           buffer().decodeDecimal_2_2(&m_message.gga.mslAltitude);
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Units
      case 19:
        if (isalpha(in_token)) {
          m_message.gga.units = in_token;
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Geoid Separation
      case 21:
        if (in_token == NMEA_FLT_NUM) {
           buffer().decodeDecimal_2_2(&m_message.gga.geoidSeparation);
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Units
      case 23:
        if (isalpha(in_token)) {
          m_message.gga.units2 = in_token;
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Age if Duff, Corr.
      case 25:
        if (in_token == NMEA_FLT_NUM) {
          m_message.gga.ageOfDiffCorr = buffer().decodeInt16();
          m_current_state++;
        } //else
        // if (in_token == ',') {
        //   m_current_state += 2;
        // }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Diff. Ref. Station ID
      case 27:
        NMEA_SKIP_LAST(1)
        if (in_token == NMEA_NUM) {
          m_message.gga.diffRefStationID = buffer().decodeInt16();
          m_current_state++;
        } //else
        // if (in_token == NMEA_CHECKSUM) {
        //   m_current_state++;
        //   _state_GPGGA(in_token);
        // }
        NMEA_ON_ERROR;
      case 28:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  } /* _state_GPGGA */
#endif /* NMEA_USE_GGA */

#ifdef NMEA_USE_GLL
  template<class T>
  void Parser<T>::_state_GPGLL(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
        // Latitude
      case 1:
        if (in_token == NMEA_FLT_NUM) {
           buffer().decodeDecimal_4_4(&m_message.gll.latitude);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // N/S Indicator
      case 3:
        if (in_token == 'N' || in_token == 'S') {
          m_message.gll.nsIndicator = in_token;
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Longitude
      case 5:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_4(&m_message.gll.longitude);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // E/W Indicator
      case 7:
        if (in_token == 'E' || in_token == 'W') {
          m_message.gll.ewIndicator = in_token;
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // UTC Time
      case 9:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeUTCTime(&m_message.gll.utcTime);
          m_current_state++;
        }
        NMEA_ON_ERROR;
        // Status
      case 11:
        if (in_token == 'A' || in_token == 'V') {
          m_message.gll.status = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;

        // Mode
      case 13:
        NMEA_SKIP_LAST(1)
        if (isalpha(in_token)) {
          m_message.gll.mode = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 14:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_GLL */

#ifdef NMEA_USE_GSA
  template<class T>
  void Parser<T>::_state_GPGSA(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == 'A' || in_token == 'M') {
          m_message.gsa.mode1 = in_token;
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 3:
        if (in_token == NMEA_NUM) {
          m_message.gsa.mode2 = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 5:
      case 7:
      case 9:
      case 11:
      case 13:
      case 15:
      case 17:
      case 19:
      case 21:
      case 23:
      case 25:
      case 27:
        if (in_token == NMEA_NUM) {
          m_message.gsa.satelliteUsed[((m_current_state & NMEA_STATE_MASK) - 5) / 2] = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // PDOP
      case 29:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_2_2(&m_message.gsa.pdop);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // HDOP
      case 31:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_2_2(&m_message.gsa.hdop);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // VDOP
      case 33:
        NMEA_SKIP_LAST(1)
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_2_2(&m_message.gsa.vdop);
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 34:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_GSA */

#ifdef NMEA_USE_GSV
  template<class T>
  void Parser<T>::_state_GPGSV(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_NUM) {
          m_message.gsv.numberOfMessages = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 3:
        if (in_token == NMEA_NUM) {
          m_message.gsv.messageNumber = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 5:
        if (in_token == NMEA_NUM) {
          m_message.gsv.satellitesInView = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;

      case 7:
      case 15:
      case 23:
      case 31:
        if (in_token == NMEA_NUM) {
          m_message.gsv.satellites[((m_current_state & NMEA_STATE_MASK) - 7) / 8].satelliteID = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 9:
      case 17:
      case 25:
      case 33:
        if (in_token == NMEA_NUM) {
          m_message.gsv.satellites[((m_current_state & NMEA_STATE_MASK) - 9) / 8].elevation = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 11:
      case 19:
      case 27:
      case 35:
        if (in_token == NMEA_NUM) {
          m_message.gsv.satellites[((m_current_state & NMEA_STATE_MASK) - 11) / 8].azimuth = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 13:
      case 21:
      case 29:
      case 37:
        NMEA_SKIP_LAST(1)
        if (in_token == NMEA_NUM) {
          int n = ((m_current_state & NMEA_STATE_MASK) - 13) / 8;
          m_message.gsv.satellites[n].snr = buffer().decodeInt16();
          if (4 * (m_message.gsv.messageNumber - 1) + n + 1 >= m_message.gsv.satellitesInView) {
            m_current_state = (m_current_state & 0xf700) | 38;
          } else {
            m_current_state++;
          }
        }
        NMEA_SKIP
        NMEA_ON_ERROR;

      case 38:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_GSV */

#ifdef NMEA_USE_MSS
  template<class T>
  void Parser<T>::_state_GPMSS(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_NUM) {
          m_message.mss.signalStrength = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 3:
        if (in_token == NMEA_NUM) {
          m_message.mss.signalToNoiseRatio = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 5:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_2(&m_message.mss.beaconFrequency);
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 7:
        if (in_token == NMEA_NUM) {
          m_message.mss.beaconBitRate = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 8:
        NMEA_SKIP_LAST(2)
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 9:
        NMEA_SKIP_LAST(1)
        if (in_token == NMEA_NUM) {
          m_message.mss.channelNumber = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 10:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_MSS */

#ifdef NMEA_USE_RMC
  template<class T>
  void Parser<T>::_state_GPRMC(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeUTCTime(&m_message.rmc.utcTime);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 3:
        if (in_token == 'A' || in_token == 'V') {
          m_message.rmc.status = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 5:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_4(&m_message.gll.latitude);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // N/S Indicator
      case 7:
        if (in_token == 'N' || in_token == 'S') {
          m_message.rmc.nsIndicator = in_token;
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // Longitude
      case 9:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_4(&m_message.rmc.longitude);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // E/W Indicator
      case 11:
        if (in_token == 'E' || in_token == 'W') {
          m_message.rmc.ewIndicator = in_token;
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 13:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_2(&m_message.rmc.speedOverGround);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 15:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_2(&m_message.rmc.courseOverGround);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 17:
        if (in_token == NMEA_NUM) {
          buffer().decodeDate(&m_message.rmc.date);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 19:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_2(&m_message.rmc.magneticVariation);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
        // E/W Indicator
      case 21:
        NMEA_SKIP_LAST(3)
        if (in_token == 'E' || in_token == 'W') {
          m_message.rmc.ewIndicator2 = in_token;
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 22:
        NMEA_SKIP_LAST(2)
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 23:
        NMEA_SKIP_LAST(1)
        if (isascii(in_token)) {
          m_message.rmc.mode = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 24:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_RMC */

#ifdef NMEA_USE_VTG
  template<class T>
  void Parser<T>::_state_GPVTG(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_2(&m_message.vtg.course);
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 3:
        if (isalpha(in_token)) {
          m_message.vtg.reference = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 5:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_4_2(&m_message.vtg.course2);
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 7:
        if (isalpha(in_token)) {
          m_message.vtg.reference2 = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 9:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_2_2(&m_message.vtg.speed);
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 11:
        if (isalpha(in_token)) {
          m_message.vtg.units = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 13:
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeDecimal_2_2(&m_message.vtg.speed2);
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 15:
        if (isalpha(in_token)) {
          m_message.vtg.units2 = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 16:
        NMEA_SKIP_LAST(2)
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 17:
        NMEA_SKIP_LAST(1)
        if (isascii(in_token)) {
          m_message.vtg.mode = in_token;
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 18:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_VTG */

#ifdef NMEA_USE_ZDA
  template<class T>
  void Parser<T>::_state_GPZDA(int in_token) {
    retry:
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_NUM) {
          buffer().decodeUTCTime2(&m_message.zda.utcTime);
          m_message.zda.utcTime.msec = 0;
          m_current_state++;
        } else
        if (in_token == NMEA_FLT_NUM) {
          buffer().decodeUTCTime(&m_message.zda.utcTime);
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 3:
        if (in_token == NMEA_NUM) {
          m_message.zda.day = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 5:
        if (in_token == NMEA_NUM) {
          m_message.zda.month = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 7:
        if (in_token == NMEA_NUM) {
          m_message.zda.year = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 9:
        if (in_token == NMEA_NUM) {
          m_message.zda.localZoneHour = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 11:
        NMEA_SKIP_LAST(1)
        if (in_token == NMEA_NUM) {
          m_message.zda.localZoneMinutes = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 12:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_ZDA */

#ifdef NMEA_USE_150
  template<class T>
  void Parser<T>::_state_150(int in_token) {
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_NUM) {
          m_message.okToSend.okToSend = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 2:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_150 */

#ifdef NMEA_USE_151
  template<class T>
  void Parser<T>::_state_151(int in_token) {
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_NUM) {
          m_message.gpsDataAndEEM.gpsTimeValidFlag = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 3:
        if (in_token == NMEA_NUM) {
          m_message.gpsDataAndEEM.gpsWeek = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 5:
        if (in_token == NMEA_NUM) {
          m_message.gpsDataAndEEM.gpsTOW = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_SKIP
        NMEA_ON_ERROR;
      case 7:
        if (in_token == NMEA_HEX8) {
          m_message.gpsDataAndEEM.ephReqMask = buffer().decodeHex8();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 8:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_151 */

#ifdef NMEA_USE_152
  template<class T>
  void Parser<T>::_state_152(int in_token) {
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_HEX8) {
          m_message.eei.satPosValidityFlag = buffer().decodeHex8();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 3:
        if (in_token == NMEA_HEX8) {
          m_message.eei.satClkValidityFlag = buffer().decodeHex8();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 5:
        if (in_token == NMEA_HEX8) {
          m_message.eei.satHealthFlag = buffer().decodeHex8();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 6:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_152 */

#ifdef NMEA_USE_154
  template<class T>
  void Parser<T>::_state_154(int in_token) {
    switch(m_current_state & NMEA_STATE_MASK) {
      case 1:
        if (in_token == NMEA_NUM) {
          m_message.eeAck.ackID = buffer().decodeInt16();
          m_current_state++;
        }
        NMEA_ON_ERROR;
      case 2:
        NMEA_ON_CHECKSUM
      default:
        if (in_token == ',') {
          m_current_state++;
        }
        NMEA_ON_ERROR;
    }
  }
#endif /* NMEA_USE_154 */


} /* NMEA */

} /* GPS */

#endif /* __GPS_nmea_h */
