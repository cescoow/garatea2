#include "GPS.h"

void GPS::readGPS(void){

	// Implementacao renovada, provavelmente com o uso de String e strcmp ao invez de char por char
	char c;
    has_read_GGA = false;
    has_read_RMC = false;

    while(!has_read_GGA || !has_read_RMC) { // stopreading
        if(Serial.available() > 0) {
            //pc.printf("esta readable1\r\n");
            c = Serial.read();
            if(c == '$') {
                //pc.printf("esta readable2\r\n");
                cDataBuffer[0] = c;
                c = Serial.read();
                if (c == 'G') {
                    //pc.printf("esta readable3\r\n");
                    cDataBuffer[1] = c;
                    c = Serial.read();
                    if (c == 'P') {
                        //pc.printf("esta readable4\r\n");
                        cDataBuffer[2] = c;
                        c = Serial.read();
                        if (c == 'G') {
                            //pc.printf("esta readable5\r\n");
                            cDataBuffer[3] = c;
                            c = Serial.read();
                            if (c == 'G') {
                                //pc.printf("esta readable6\r\n");
                                cDataBuffer[4] = c;
                                cDataBuffer[5] = Serial.read();
                                cDataBuffer[6] = Serial.read();
                                has_read_GGA = true;
                                for(unsigned int i=7; i <sizeof(cDataBuffer); i++) {
                                    c = Serial.read();
                                    if( c == '\r' ) { // aparentemente o gps manda um \r para sinalizar final da sentence (pode ser que seja \n)
                                        //pc.printf("%s\n", cDataBuffer);
                                        parse(cDataBuffer);
                                        i = sizeof(cDataBuffer);
                                    } else {
                                        cDataBuffer[i] = c;
                                    }
                                }
                            }
                        } else if (c == 'R' && !has_read_RMC) { // RMC
                            cDataBuffer[3] = c;
                            c = Serial.read();
                            if (c == 'M') {
                                cDataBuffer[4] = c;
                                cDataBuffer[5] = Serial.read();
                                cDataBuffer[6] = Serial.read();
                                has_read_RMC = true;
                                for(unsigned int i=7; i<sizeof(cDataBuffer); i++) {
                                    c = Serial.read();
                                    if( c == '\r' ) { // aparentemente o gps manda um \r para sinalizar final da sentence (pode ser que seja \n)
                                        //pc.printf("%s\n", cDataBuffer);
                                        parse(cDataBuffer);
                                        i = sizeof(cDataBuffer);
                                    } else {
                                        cDataBuffer[i] = c;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //has_read = true;
        }//readable if

    }//while
}

void GPS::parse(char *cmd)
{
    // Global Positioning System Fix Data
    if(strncmp(cmd,"$GPGGA", 6) == 0) {
        sscanf(cmd, "$GPGGA,%f,%f,%c,%f,%c,%d,%d,%f,%f", &timefix, &latitude, &northsouth, &longitude, &eastwest, &fix_quality, &numbersatelites, &HDOP, &altitude);
        //pc.printf(" ALOALO GGA  *** %d;%d;%.0f;%f;%c;%f;%c;%f;%f;%d;%.1f\r\n", numbersatelites, date, timefix, latitude, northsouth, longitude, eastwest, altitude, speed, fix_quality, HDOP);
    }

    // Geographic position, Latitude and Longitude
    if(strncmp(cmd,"$GPRMC", 6) == 0) {
        sscanf(cmd, "$GPRMC,%*f,%*c,%*f,%*c,%*f,%*c,%f,%*f,%d", &speed, &date); // ignores all other data, except speed and date
        //pc.printf("   ALOU ALOU ESTOU ESCANEANDO RMC    \r\n");
    }
    speed = speed * 0.51444; // conversao de knots para m/s
}

int GPS::get_numbersatelites(){
	return this->numbersatelites;
}
int GPS::get_date(){
	return this->date;
}
int GPS::get_fix_quality(){
	return this->fix_quality;
}
float GPS::get_latitude(){
	return this->latitude;
}
float GPS::get_longitude(){
	return this->longitude;
}
float GPS::get_timefix(){
	return this->timefix;
}
float GPS::get_speed(){
	return this->speed;
}
float GPS::get_altitude(){
	return this->altitude;
}
float GPS::get_HDOP(){
	return this->HDOP;
}
char GPS::get_northsouth(){
	return this->northsouth;
}
char GPS::get_eastwest(){
	return this->eastwest;
}














