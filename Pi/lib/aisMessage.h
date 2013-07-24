#ifndef _AIS_MESSAGE_H
#define _AIS_MESSAGE_H 

#include <string>

class AISMessage {
public:
	AISMessage(const std::string &rawString);

	unsigned char* getBits(int start, int length);
	int getInt(int start, int length);
	int getUInt(int start, int length);
	std::string getString();
private:
	unsigned char* bitstream;
	int streamSize;
};

#endif