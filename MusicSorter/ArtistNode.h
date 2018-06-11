#pragma once
#ifndef ArtistNode

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
protected:
	std::map<std::string, AlbumNode*> albums;
};

ArtistNode::ArtistNode(SongNode* s) :
	artistName(s->artist())
{
	addAlbum(s);
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
		std::cout << s->album() << " exists" << std::endl;
		return albums.at(s->album())->addSong(s);
	}
	else {
		AlbumNode* a = new AlbumNode(s);
		albums.insert(std::pair<std::string, AlbumNode*>(s->album(), a));
		std::cout << s->album() << " new" << std::endl;
		return true;
	}
}
#endif ArtistNode

