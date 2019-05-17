#pragma once
#ifndef LibraryDB
#define DEBUGGING true

#include "ArtistNode.h"
#include "AlbumNode.h"
#include "SongException.h"
#include <map>
#include <taglib\fileref.h>
#include <taglib\tag.h>
#include <boost\filesystem.hpp>
#include <string>
#include <iostream>

class LibraryDB {
public:
	LibraryDB(boost::filesystem::path);
	bool addArtist(SongNode*);
	~LibraryDB();
private:
	std::map<std::string, ArtistNode*> artists;
	bool checkArtist(SongNode*);
};

LibraryDB::LibraryDB(boost::filesystem::path p) {
	for (boost::filesystem::recursive_directory_iterator library(p);
		library != boost::filesystem::recursive_directory_iterator();
		library++) {
		if (!boost::filesystem::is_directory(library->path())) {
			try {
				SongNode *currentSong = new SongNode(library->path());
				addArtist(currentSong);
			}
			catch (SongException e) {
				std::cout << e.what() << std::endl;
			}
		}
	}
}

LibraryDB::~LibraryDB() {
	for (std::map<std::string, ArtistNode*>::iterator it = artists.begin(); it != artists.end(); it++) {
		delete(it->second);
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
		if (DEBUGGING)
			std::cout << s->artist() << " exists" << std::endl;
		return artists.at(s->artist())->addAlbum(s);
	}
	else {
		ArtistNode* a = new ArtistNode(s);
		artists.insert(std::pair<std::string, ArtistNode*>(s->artist(), a));
		if (DEBUGGING)
			std::cout << s->artist() << " new" << std::endl;
		return true;
	}
}
#endif AlbumList