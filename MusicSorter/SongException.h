#pragma once
#ifndef SongException
#include <iostream>

class SongException : public std::exception {
public:
	int errorType;

	SongException(int x) :
	errorType(x)
	{}

	virtual const char* what() const throw() {
		if ( errorType == 1 )
			return "Audio file is missing some or all crucial ID3 data (artist, album, title, track #)";
		if ( errorType == 2 )
			return "File has no ID3 data";
	}
};

#endif SongException

