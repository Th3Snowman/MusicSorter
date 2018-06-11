#pragma once
#ifndef SongNode

#include <fileref.h>
#include <tag.h>
#include <boost\filesystem.hpp>
#include <string>

class SongNode {
public:
	SongNode(boost::filesystem::path p);
	boost::filesystem::path songPath;
	bool operator==(const SongNode& s) const;
	std::string name();
	std::string artist();
	std::string album();
private:
	TagLib::String songName;
	TagLib::Tag* songTag;
	TagLib::String albumName;
	TagLib::String artistName;
	int trackNumber;
	int yearMade;
};


SongNode::SongNode(boost::filesystem::path p):
songPath(p)
{
	TagLib::FileRef song(songPath.c_str());
	songTag = song.tag();
	if (songTag->isEmpty())
		delete this;
	songName = songTag->title();
	albumName = songTag->album();
	artistName = songTag->artist();
	trackNumber = songTag->track();
	if (songName == TagLib::String::null || albumName == TagLib::String::null || artistName == TagLib::String::null || trackNumber == 0)
		delete this;
}

bool SongNode::operator==(const SongNode& s) const {
	if (this->songName == s.songName && this->albumName == s.albumName && this->artistName == s.artistName && this->trackNumber == s.trackNumber)
		return true;
	else
		return false;
}

std::string SongNode::name() {
	return std::string(songName.toCString());
}

std::string SongNode::album() {
	return std::string(albumName.toCString());
}

std::string SongNode::artist() {
	return std::string(artistName.toCString());
}
#endif SongNode
