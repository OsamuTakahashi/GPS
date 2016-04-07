CC=gcc

CUNIT_INCLUDE=/opt/local/include
CUNIT_LIB=/opt/local/lib

CFLAGS=-I../src -I$(CUNIT_INCLUDE) -D__UNIT_TEST__

.cpp.o:
	$(CC) -c $(CFLAGS) $<

HEADERS=../src/GPS/nmea.h	\
				../src/GPS/util.h

OBJECTS=test.o	\
				nmea.o	\
				lexertest.o	\
				parsertest.o	\
				utiltest.o

test:	$(OBJECTS) $(HEADERS)
	$(CC) -L$(CUNIT_LIB) -lcunit -o test $(OBJECTS)

test.o: $(HEADERS)
lexertest.o:	$(HEADERS)
parsertest.o: $(HEADERS)
utiltest.o:		$(HEADERS)

nmea.o:	../src/nmea.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) ../src/nmea.cpp


clean:
	-rm *.o
