/**
  @file sirf.h

  A SiRF message parsing library
  Work in progress...

  @author Osamu Takahashi
*/
#ifndef __GPS_sirf_h
#define __GPS_sirf_h

#include <stddef.h>
#include <inttypes.h>
#include <GPS/util.h>

namespace GPS {

namespace SiRF {

  enum {
    // Output Message
    MeasureNavigationDataOutID = 2,
    MeasuredTrackerDataOutID = 4,
    ClockStatusDataID = 7,
    CPUThroughputID = 9,
    CommandAcknowledgmentID = 11,
    VisibleListID = 13,
    GeodeticNavigationDataID = 41,

    // Input Message
    SetBinarySerialPortID = 134,
    SetProtocolID = 135,
    SetMessageRateID = 166
  };

  //! Measure Navigation Data Out – Message ID 2
  struct MeasureNavigationDataOut {
    int32_t   xPosition;
    int32_t   yPosition;
    int32_t   zPosition;
    int16_t   xVelocity;
    int16_t   yVelocity;
    int16_t   zVelocity;
    uint8_t   mode1;
    uint8_t   HDOP;
    uint8_t   mode2;
    uint16_t  GPSWeek;
    uint32_t  GPSTOW;
    uint8_t   SVsInFix;
    uint8_t   ch1PRN;
    uint8_t   ch2PRN;
    uint8_t   ch3PRN;
    uint8_t   ch4PRN;
    uint8_t   ch5PRN;
    uint8_t   ch6PRN;
    uint8_t   ch7PRN;
    uint8_t   ch8PRN;
    uint8_t   ch9PRN;
    uint8_t   ch10PRN;
    uint8_t   ch11PRN;
    uint8_t   ch12PRN;
  } __attribute__((__packed__));

  //! Measured Tracker Data Out – Message ID 4
  struct MeasuredTrackerDataOut {
    uint16_t  GPSWeek;
    uint32_t  GPSTOW;
    uint8_t   chans;
    uint8_t   _1stSVid;
    uint8_t   azimuth1;
    uint8_t   elev1;
    uint16_t  state1;
    uint8_t   CN0_1;
    uint8_t   CN0_2;
    uint8_t   CN0_3;
    uint8_t   CN0_4;
    uint8_t   CN0_5;
    uint8_t   CN0_6;
    uint8_t   CN0_7;
    uint8_t   CN0_8;
    uint8_t   CN0_9;
    uint8_t   CN0_10;
    uint16_t  _2ndSVid;
    uint8_t   azimuth2;
    uint8_t   elev2;
    uint16_t  state2;
  } __attribute__((__packed__));

  //! Response: Clock Status Data – Message ID 7
  struct ClockStatusData {
    uint8_t   messageID;
    uint16_t  extendedGPSWeek;
    uint32_t  GPSTOW;
    uint8_t   SVs;
    uint32_t  clockDrift;
    uint32_t  clockBias;
    uint32_t  estimatedGPSTime;
  } __attribute__((__packed__));

  //! CPU Throughput – Message ID 9
  struct CPUThroughput {
    uint16_t  segStatMax;
    uint16_t  segStatLat;
    uint16_t  aveTrkTime;
    uint16_t  lastMillisecond;
  } __attribute__((__packed__));

  //! Command Acknowledgment – Message ID 11
  struct CommandAcknowledgment {
    uint8_t   ackID;
  } __attribute__((__packed__));

  //! Geodetic Navigation Data – Message ID 41
  struct GeodeticNavigationData {
    struct {
      uint16_t solutionNotYetOverdetermined : 1;
      uint16_t reserved1 : 2;
      uint16_t invalidDRSensorData : 1;
      uint16_t invalidDRCalibration : 1;
      uint16_t unavailableDRGpsBasedCalibration : 1;
      uint16_t invalidDRPositionFix : 1;
      uint16_t invalidHeading : 1;

      uint16_t reserved2 : 7;
      uint16_t noTrackingDataAvailable : 1;
    } navValid;
    struct {
      uint16_t sensorDRSolitionType : 1;
      uint16_t navigationSolutionOverdetermined : 1;
      uint16_t velocityDRTimeoutExceeded : 1;
      uint16_t fixHasBeenEditedByMIFunctions : 1;
      uint16_t invalidVelocity : 1;
      uint16_t altitudeHoldDisabled : 1;
      uint16_t sensorDRErrorStatus : 2;

      uint16_t GPSPositionFixType : 3;
      uint16_t tricklePowerInUser : 1;
      uint16_t altitudeHoldStatus : 2;
      uint16_t DOPLimitExceeded : 1;
      uint16_t DGPSCorrectionsApplied : 1;
    } navType;
    uint16_t extendedWeekNumber;
    uint32_t TOW;
    uint16_t UTCYear;
    uint8_t UTCMonth;
    uint8_t UTCDay;
    uint8_t UTCHour;
    uint8_t UTCMinute;
    uint16_t UTCSecond;
    uint32_t satelliteIDList;
    int32_t latitude;
    int32_t longitude;
    int32_t altitudeFromEllipsoid;
    int32_t altitudeFromMSL;
    int8_t mapDatum;
    uint16_t speedOverGround;
    uint16_t courseOverGround;
    int16_t magneticVariation;
    int16_t climbRate;
    int16_t headingRate;
    uint32_t estimatedHorizontalPositionError;
    uint32_t estimatedVerticalPositionError;
    uint32_t estimatedTimeError;
    uint16_t estimatedHorizontalVelocityError;
    int32_t clockBias;
    uint32_t clockBiasError;
    int32_t clockDrift;
    uint32_t clockDriftError;
    uint32_t distance;
    uint16_t distanceError;
    uint16_t headingError;
    uint8_t numberOfSVsInFix;
    uint8_t HDOP;
    uint8_t additionalModeInfo;
  } __attribute__((__packed__));

  // Set Binary Serial Port – Message ID 134
  struct SetBinarySerialPort {
    uint32_t  bitRate;
    uint8_t   dataBits;
    uint8_t   stopBit;
    uint8_t   parity;
    uint8_t   padding;
  } __attribute__((__packed__));

  // Set Protocol – Message ID 135
  struct SetProtocol {
    uint8_t   protocol;
  } __attribute__((__packed__));

  enum Protocol {
    Protocol_Null,
    Protocol_SiRFBinary,
    Protocol_NMEA,
    Protocol_ASCII,
    Protocol_RTCM,
    Protocol_USER,
    Protocol_SiRFLoc,
    Protocol_Statistic
  };

  // Set Message Rate – Message ID 166
  struct SetMessageRate {
    uint8_t   mode;
    uint8_t   messageIDToBeSet;
    uint8_t   updateRate;
    uint32_t  reserved; // Not used, set to zero
  } __attribute__((__packed__));

  struct OutputMessage {
    uint8_t   messageID;
    union {
      ClockStatusData clockStatusData;
      GeodeticNavigationData geodeticNavigationData;
    } messageBody;
  };

  struct InputMessage {
    uint8_t   messageID;
    union {
      SetBinarySerialPort setBinaryMessagePort;
      SetProtocol setProtocol;
      SetMessageRate setMessageRate;
    } messageBody;
  };

  typedef void (*OutputMessageHandler)(const OutputMessage &);

  inline void write32(uint32_t *dst,uint32_t src) {
    uint8_t *p = (uint8_t *)dst;
    *p++ = (src >> 24) & 0xff;
    *p++ = (src >> 16) & 0xff;
    *p++ = (src >> 8) & 0xff;
    *p++ = src & 0xff;
  }

  template<class T>
  class CommandBuilder {
  public:
    CommandBuilder(T &serial)
      : m_serial(serial) {

    }

    void SetBinarySerialPort(int32_t bitRate,int dataBits = 8,int stopBit = 1,int parity = 0) {
      m_msg.messageID = SetBinarySerialPortID;
      write32(&m_msg.messageBody.setBinaryMessagePort.bitRate,bitRate);
      m_msg.messageBody.setBinaryMessagePort.dataBits = dataBits;
      m_msg.messageBody.setBinaryMessagePort.stopBit = stopBit;
      m_msg.messageBody.setBinaryMessagePort.parity = parity;
      m_msg.messageBody.setBinaryMessagePort.padding = 0;
      _write((uint8_t *)&m_msg,sizeof(struct SetBinarySerialPort) + 1);
    }

    void SetProtocol(Protocol protocol) {
      m_msg.messageID = SetProtocolID;
      m_msg.messageBody.setProtocol.protocol = protocol;
      _write((uint8_t *)&m_msg,sizeof(struct SetProtocol) + 1);
    }

    void SetMessageRate(int mode,int messageIDToBeSet,int updateRate) {
      m_msg.messageID = SetMessageRateID;
      m_msg.messageBody.setMessageRate.mode = mode;
      m_msg.messageBody.setMessageRate.messageIDToBeSet = messageIDToBeSet;
      m_msg.messageBody.setMessageRate.updateRate = updateRate;
      m_msg.messageBody.setMessageRate.reserved = 0;
      _write((uint8_t *)&m_msg,sizeof(struct SetMessageRate) + 1);
    }
  private:
    InputMessage
      m_msg;
    T &m_serial;

    int _checksum(const uint8_t *dat,int l) {
      int t = 0;
      for (int i = 0;i < l;i++) {
        t += dat[i];
      }
      return t & 0x7ff;
    }

    void _write(uint8_t *dat,int l) {
      m_serial.write(0xa0);
      m_serial.write(0xa2);
      m_serial.write((l >> 8) & 0x7f);
      m_serial.write(l & 0xff);
      m_serial.write(dat,l);
      int cs = _checksum(dat,l);
      m_serial.write((cs >> 8) & 0xff);
      m_serial.write(cs & 0xff);
      m_serial.write(0xb0);
      m_serial.write(0xb3);
    }
  };

  class MessageParser {
  public:
    MessageParser()
      : m_enabled(1) {
      m_msg.messageID = 0;
    }

    bool parse(int c) {
      bool res = false;
      if (m_enabled) {
        if (m_msg.messageID == 0) {
          switch(c) {
            case GeodeticNavigationDataID:
              m_msg.messageID = c;
              m_msgPos = 1;
              res = true;
              break;
          }
        } else {
          switch(m_msg.messageID) {
            case GeodeticNavigationDataID:
              res = _processGeodeticNavigationData(c);
              break;
          }
        }
        if (!res)
          m_enabled = 0;
      }
      return res;
    }

    const OutputMessage &lastMessage() const {
      return m_msg;
    }

    void reset() {
      m_msg.messageID = 0;
      m_msgPos = 0;
      m_enabled = 1;
      m_done = 0;
    }

    bool done() const {
      return m_done;
    }
  private:
    OutputMessage
      m_msg;
    uint8_t
      m_msgPos,
      m_enabled,
      m_done;

    bool _processGeodeticNavigationData(int c);
  };

  template<class T>
  class PacketParser {
  public:
    PacketParser(T &serial,MessageParser &parser)
      : m_serial(serial),
        m_parser(parser),
        m_state(START_SEQUENCE),
        m_tmpBufferPos(0),
        m_handler(NULL) {

    }
    void polling() {
      for (int l = m_serial.available();l > 0;l--) {
    		int c = m_serial.read();
        switch(m_state) {
          case START_SEQUENCE:
            _processStartSequence(c);
            break;
          case PAYLOAD_LENGTH:
            _processPayloadLength(c);
            break;
          case PAYLOAD:
            _processPayload(c);
            break;
          case CHECKSUM:
            _processChecksum(c);
            break;
          case END_SEQUENCE:
            _processEndSequence(c);
            break;
        }
    	}
    }
    void setHandler(OutputMessageHandler handler) {
      m_handler = handler;
    }
  private:
    enum {
      START_SEQUENCE,
      PAYLOAD_LENGTH,
      PAYLOAD,
      CHECKSUM,
      END_SEQUENCE,

      MAX_SIRF_PAYLOAD_LENGTH = 1024
    };

    T &m_serial;

    uint8_t m_state;
    uint8_t m_tmpBuffer[2];
    int16_t m_tmpBufferPos;
    int16_t m_payloadLength;
    int16_t m_payloadChecksum;
    uint8_t m_readPayload;

    OutputMessageHandler m_handler;
    MessageParser &m_parser;

    inline void _reset() {
      m_tmpBufferPos = 0;
      m_state = START_SEQUENCE;
    }
    inline void _next(int next) {
      m_tmpBufferPos = 0;
      m_state = next;
    }
    inline void _done() {
      if (m_handler && m_parser.done()) {
        (*m_handler)(m_parser.lastMessage());
      }
    }

    inline bool _checkPayloadChecksum(int cs) {
      return ((m_payloadChecksum & 0x7fff) == cs);
    }

    void _processStartSequence(int c) {
      if (m_tmpBufferPos == 0 && c == 0xa0) {
        m_tmpBufferPos = 1;
      } else
      if (m_tmpBufferPos == 1 && c == 0xa2) {
        _next(PAYLOAD_LENGTH);
      } else {
        _reset();
      }
    }

    void _processPayloadLength(int c) {
      if (m_tmpBufferPos == 0 && c <= 0x7f) {
        m_tmpBuffer[1] = c;
        m_tmpBufferPos = 1;
      } else
      if (m_tmpBufferPos == 1) {
        m_tmpBuffer[0] = c;
        m_payloadLength = *((uint16_t *)m_tmpBuffer);
        if (m_payloadLength < MAX_SIRF_PAYLOAD_LENGTH) {
          m_payloadChecksum = 0;
          m_readPayload = 1;
          m_parser.reset();
          _next(PAYLOAD);
        } else {
          _reset();
        }
      } else {
        _reset();
      }
    }

    void _processPayload(int c) {
      if (m_readPayload)
        m_readPayload = m_parser.parse(c);
      m_tmpBufferPos++;

      if (m_tmpBufferPos == m_payloadLength) {
        _next(CHECKSUM);
      }
    }

    void _processChecksum(int c) {
      if (m_tmpBufferPos == 0 && c <= 0x7f) {
        m_tmpBuffer[1] = c;
        m_tmpBufferPos = 1;
      } else
      if (m_tmpBufferPos == 1) {
        m_tmpBuffer[0] = c;
        int checksum = *((uint16_t *)m_tmpBuffer);
        if (_checkPayloadChecksum(checksum)) {
          _next(END_SEQUENCE);
        } else {
          _reset();
        }
      } else {
        _reset();
      }
    }

    void _processEndSequence(int c) {
      if (m_tmpBufferPos == 0 && c == 0xb0) {
        m_tmpBufferPos = 1;
      } else
      if (m_tmpBufferPos == 1 && c == 0xb3) {
        _done();
        _reset();
      } else {
        _reset();
      }
    }
  };

  inline void _swap16(void *io) {
    uint8_t *p = (uint8_t *)io;
    uint8_t t = p[0];
    p[0] = p[1];
    p[1] = t;
  }

  inline void _swap32(void *io) {
    uint8_t *p = (uint8_t *)io;
    uint8_t t = p[0];
    p[0] = p[3];
    p[3] = t;
    t = p[1];
    p[1] = p[2];
    p[2] = t;
  }

  inline bool
  MessageParser::_processGeodeticNavigationData(int c) {
    ((uint8_t *)&m_msg)[m_msgPos++] = c;
    if (m_msgPos == sizeof(GeodeticNavigationData) + 1) {
      _swap16(&m_msg.messageBody.geodeticNavigationData.navValid);
      _swap16(&m_msg.messageBody.geodeticNavigationData.navType);
      _swap16(&m_msg.messageBody.geodeticNavigationData.extendedWeekNumber);
      _swap32(&m_msg.messageBody.geodeticNavigationData.TOW);
      _swap16(&m_msg.messageBody.geodeticNavigationData.UTCYear);
      _swap16(&m_msg.messageBody.geodeticNavigationData.UTCSecond);
      _swap32(&m_msg.messageBody.geodeticNavigationData.satelliteIDList);
      _swap32(&m_msg.messageBody.geodeticNavigationData.latitude);
      _swap32(&m_msg.messageBody.geodeticNavigationData.longitude);
      _swap32(&m_msg.messageBody.geodeticNavigationData.altitudeFromEllipsoid);
      _swap32(&m_msg.messageBody.geodeticNavigationData.altitudeFromMSL);
      _swap16(&m_msg.messageBody.geodeticNavigationData.speedOverGround);
      _swap16(&m_msg.messageBody.geodeticNavigationData.courseOverGround);
      _swap16(&m_msg.messageBody.geodeticNavigationData.magneticVariation);
      _swap16(&m_msg.messageBody.geodeticNavigationData.climbRate);
      _swap16(&m_msg.messageBody.geodeticNavigationData.headingRate);
      _swap32(&m_msg.messageBody.geodeticNavigationData.estimatedHorizontalPositionError);
      _swap32(&m_msg.messageBody.geodeticNavigationData.estimatedVerticalPositionError);
      _swap32(&m_msg.messageBody.geodeticNavigationData.estimatedTimeError);
      _swap16(&m_msg.messageBody.geodeticNavigationData.estimatedHorizontalVelocityError);
      _swap32(&m_msg.messageBody.geodeticNavigationData.clockBias);
      _swap32(&m_msg.messageBody.geodeticNavigationData.clockBiasError);
      _swap32(&m_msg.messageBody.geodeticNavigationData.clockDrift);
      _swap32(&m_msg.messageBody.geodeticNavigationData.clockDriftError);
      _swap32(&m_msg.messageBody.geodeticNavigationData.distance);
      _swap16(&m_msg.messageBody.geodeticNavigationData.distanceError);
      _swap16(&m_msg.messageBody.geodeticNavigationData.headingError);

      m_done = true;

      return false;
    }
    return true;
  }

} /* SiRF */

} /* GPS */

#endif /* __GPS_sirf_h */
