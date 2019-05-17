#pragma once
#ifndef AlbumNode
#define DEBUGGING true
#include <string>
#include "SongNode.h"
#include <map>

class AlbumNode {
public:
	AlbumNode(SongNode*);
	std::string albumName;
	bool checkSong(SongNode*);
	bool addSong(SongNode*);
	~AlbumNode();
protected:
	std::map<std::string, SongNode*> songs;
};

AlbumNode::AlbumNode(SongNode* s) :
albumName(s->album())
{
	addSong(s);
}

AlbumNode::~AlbumNode() {
	for (std::map<std::string, SongNode*>::iterator it = songs.begin(); it != songs.end(); it++) {
		delete(it->second);
	}
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
		if (DEBUGGING)
			std::cout << s->name() << " exists" << std::endl;
		return false;
	}
	else {
		songs.insert(std::pair<std::string, SongNode*>(s->name(), s));
		if (DEBUGGING)
			std::cout << s->name() << " new" << std::endl;
		return true;
	}
}

#endif AlbumNode
