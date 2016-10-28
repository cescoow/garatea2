#ifndef GPS_h
#define GPS_h

#include <Arduino.h>

class GPS{
private:
	char cDataBuffer[500], northsouth, eastwest;
	bool has_read_GGA, has_read_RMC;
	int numbersatelites, date, fix_quality;
	float latitude, longitude, timefix, speed, altitude, HDOP;
public:
	void readGPS(void);
	void parse(char *cmd);
	int get_numbersatelites();
	int get_date();
	int get_fix_quality();
	float get_latitude();
	float get_longitude();
	float get_timefix();
	float get_speed();
	float get_altitude();
	float get_HDOP();
	char get_northsouth();
	char get_eastwest();
};


#endif