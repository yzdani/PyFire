#ifndef Radio_H
#define Radio_H
 
#include <Arduino.h>
 
class Radio {
public:
        Radio();
        ~Radio();
		void init();
        String send(String radioText);
        void setChannel(int channel);
		int rssi();
		String read();
};
 
#endif