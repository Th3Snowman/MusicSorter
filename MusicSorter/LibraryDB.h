#pragma once
#ifndef LibraryDB

#include "ArtistNode.h"
#include "AlbumNode.h"
#include <map>
#include <fileref.h>
#include <tag.h>
#include <boost\filesystem.hpp>
#include <string>
#include <iostream>

class LibraryDB {
public:
	LibraryDB(boost::filesystem::path);
	bool addArtist(SongNode*);
private:
	std::map<std::string, ArtistNode*> artists;
	bool checkArtist(SongNode*);
};

LibraryDB::LibraryDB(boost::filesystem::path p) {
	for (boost::filesystem::recursive_directory_iterator library(p);
		library != boost::filesystem::recursive_directory_iterator();
		library++) {
		if (!boost::filesystem::is_directory(library->path())) {
			SongNode *currentSong = new SongNode(library->path());
			addArtist(currentSong);
		}
	}
}

bool LibraryDB::checkArtist(SongNode* s) {
	std::string artistName = s->artist();
	if (artists.find(artistName) == artists.end())
		return false;
	else
		return true;
}

bool LibraryDB::addArtist(SongNode* s) {
	if (checkArtist(s)) {
		std::cout << s->artist() << " exists" << std::endl;
		return artists.at(s->artist())->addAlbum(s);
	}
	else {
		ArtistNode* a = new ArtistNode(s);
		artists.insert(std::pair<std::string, ArtistNode*>(s->artist(), a));
		std::cout << s->artist() << " new" << std::endl;
		return true;
	}
}
#endif AlbumList