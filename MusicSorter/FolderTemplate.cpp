#include "stdafx.h"
#include <string>
#include <boost\filesystem.hpp>
#include <tag.h>

class songsFolder {
public:
	std::string songFileName;
	boost::filesystem::path songPath; // the path of the song file in the inital folder, is used when copying the song into its target location in the specified album folder
	songsFolder* next = NULL;
	songsFolder(std::string currentSong, boost::filesystem::path currentSongPath) {
		songFileName = currentSong;
		songPath = currentSongPath;
	}

};

class albumsFolder {
public:
	TagLib::String albumName;
	songsFolder* songHead = NULL; // head of song LL that extends from each album node
	albumsFolder* next = NULL;
	albumsFolder(TagLib::String currentAlbum, std::string currentSongFileName, boost::filesystem::path currentSongPath) {
		albumName = currentAlbum;
		songHead = new songsFolder(currentSongFileName, currentSongPath); // whenever a new album node is needed, a new song node is needed as well
	}
};

class artistsFolder {
public:
	TagLib::String artistName;
	albumsFolder* albumHead = NULL; // head of album LL that extends from each artist node
	artistsFolder* next = NULL;
	artistsFolder(TagLib::String currentArtist, TagLib::String currentAlbum, std::string currentSongFileName, boost::filesystem::path currentSongPath) {
		artistName = currentArtist;
		albumHead = new albumsFolder(currentAlbum, currentSongFileName, currentSongPath); // whenever a new artist node is needed, a new album node is needed as well
	}
};
