#include "stdafx.h"
#include <string>
#include <boost\filesystem.hpp>
#include <tag.h>

class songsFolder {
public:
	std::string songFileName;
	boost::filesystem::path songPath;
	songsFolder* previous = NULL;
	songsFolder* next = NULL;
};

class albumsFolder {
public:
	TagLib::String albumName;
	songsFolder* songHead = NULL;
	albumsFolder* previous = NULL;
	albumsFolder* next = NULL;
};


class artistsFolder {
public:
	TagLib::String artistName;
	albumsFolder* albumHead = NULL;
	artistsFolder* previous = NULL;
	artistsFolder* next = NULL;
};
