#pragma once
#ifndef AlbumNode

#include <string>
#include "SongNode.h"
#include <map>

class AlbumNode {
public:
	AlbumNode(SongNode*);
	std::string albumName;
	bool checkSong(SongNode*);
	bool addSong(SongNode*);
protected:
	std::map<std::string, SongNode*> songs;
};

AlbumNode::AlbumNode(SongNode* s) :
albumName(s->album())
{
	addSong(s);
}

bool AlbumNode::checkSong(SongNode* s) {
	std::string songName = s->name();
	if (songs.find(songName) == songs.end())
		return false;
	else
		return true;
}

bool AlbumNode::addSong(SongNode* s) {
	if (checkSong(s)) {
		std::cout << s->name() << " exists" << std::endl;
		return false;
	}
	else {
		songs.insert(std::pair<std::string, SongNode*>(s->name(), s));
		std::cout << s->name() << " new" << std::endl;
		return true;
	}
}

#endif AlbumNode
