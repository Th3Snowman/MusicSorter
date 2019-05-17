#pragma once
#ifndef ArtistNode
#define DEBUGGING true

#include <string>
#include "AlbumNode.h"
#include "SongNode.h"
#include <map>

class ArtistNode {
public:
	ArtistNode(SongNode*);
	std::string artistName;
	bool checkAlbum(SongNode*);
	bool addAlbum(SongNode*);
	~ArtistNode();
protected:
	std::map<std::string, AlbumNode*> albums;
};

ArtistNode::ArtistNode(SongNode* s) :
	artistName(s->artist())
{
	addAlbum(s);
}

ArtistNode::~ArtistNode() {
	for (std::map<std::string, AlbumNode*>::iterator it = albums.begin(); it != albums.end(); it++) {
		delete(it->second);
	}
}

bool ArtistNode::checkAlbum(SongNode* s) {
	std::string albumName = s->album();
	if (albums.find(albumName) == albums.end())
		return false;
	else
		return true;
}

bool ArtistNode::addAlbum(SongNode* s) {
	if (checkAlbum(s)) {
		if (DEBUGGING)
			std::cout << s->album() << " exists" << std::endl;
		return albums.at(s->album())->addSong(s);
	}
	else {
		AlbumNode* a = new AlbumNode(s);
		albums.insert(std::pair<std::string, AlbumNode*>(s->album(), a));
		if (DEBUGGING)
			std::cout << s->album() << " new" << std::endl;
		return true;
	}
}
#endif ArtistNode

