// MusicSorter.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#define TAGLIB_STATIC 
#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>
#include <Windows.h>
#include <boost\filesystem.hpp>
#include <string>
#include "ErrorFile.cpp"
#include "FolderTemplate.cpp"
#include <string.h>



using namespace TagLib;
using namespace std;
using namespace boost::filesystem;

void printFolderTree(artistsFolder *folderHead);
songsFolder * newSongEntry(string currentSong, path currentSongPath);
albumsFolder * newAlbumEntry(String currentAlbum, string currentSong, path currentSongPath);
artistsFolder * newArtistEntry(String currentArtist, String currentAlbum, string currentSong, path currentSongPath);
void addArtistNodeToBack(artistsFolder *nodeToadd, artistsFolder *headOfTree);
void addAlbumNodeToBack(albumsFolder *nodeToadd, albumsFolder *headOfTree);

int main(int argc, char *argv[])
{
	errorClass errors;
	if (argc == 3) {
		path initalFolder(argv[1]); //path to target file
		path libraryFolder(argv[2]); //path to target folder

		if (is_directory(initalFolder) && is_directory(libraryFolder)) { // if no exception and the file at the following path exists
			artistsFolder *folderHead = NULL;
			bool artistFound = false;
			bool albumFound = false;
			artistsFolder *currentArtistNode = NULL;
			albumsFolder *currentAlbumNode = NULL;
			songsFolder *currentSongNode = NULL;

			for (recursive_directory_iterator originFolder(initalFolder); //directory iterator for folder full of random song files
				originFolder != recursive_directory_iterator();
				originFolder++) {
				FileRef currentSongRef((originFolder->path()).string().c_str()); //file ref to extract id3 tag data from files
				if (!currentSongRef.isNull()) {
					Tag *songTag = currentSongRef.tag();
					String currentArtist = songTag->artist();
					String currentAlbum = songTag->album();
					string currentSongFileName = originFolder->path().filename().string(); //filename of song with extension included
					path currentSongPath = path(originFolder->path().string()); //creating path that leads to current song with extension on file
					artistFound = false;
					albumFound = false;

					if (folderHead == NULL) {
						//cout << "Starting folder chain" << endl;
						folderHead = newArtistEntry(currentArtist, currentAlbum, currentSongFileName, currentSongPath);
					}
					else {
						currentArtistNode = folderHead;
						while (artistFound == false && currentArtistNode != NULL) {
							if (currentArtist == currentArtistNode->artistName) {
								//cout << "Found artist" << endl;
								artistFound = true;
							}
							else {
								//cout << "Artist not found" << endl;
								currentArtistNode = currentArtistNode->next;
							}
						}
						if (artistFound) {
							currentAlbumNode = currentArtistNode->albumHead;
							while (albumFound == false && currentAlbumNode != NULL) {
								if (currentAlbum == currentAlbumNode->albumName) {
									//cout << "Found album" << endl;
									albumFound = true;
								}
								else
									currentAlbumNode = currentAlbumNode->next;
							}
							if (albumFound) {
								currentSongNode = currentAlbumNode->songHead;
								cout << originFolder->path().filename().string() << endl;
								songsFolder *newSong = newSongEntry(currentSongFileName, currentSongPath);
								newSong->next = currentSongNode;
								currentAlbumNode->songHead = newSong;
							}
							else
								addAlbumNodeToBack(newAlbumEntry(currentAlbum, currentSongFileName, currentSongPath), currentArtistNode->albumHead);
						}
						else
							addArtistNodeToBack(newArtistEntry(currentArtist, currentAlbum, currentSongFileName, currentSongPath), folderHead);
					}
				}
			}
			currentArtistNode = folderHead;
			while(currentArtistNode != NULL) {
				artistFound = false;
				directory_iterator targetArtistFolder(libraryFolder);
				while (targetArtistFolder != directory_iterator() && artistFound == false){
					cout << "Artist Name?" << currentArtistNode->artistName.toCString() << endl;
					if (!strcmp(targetArtistFolder->path().string().substr(libraryFolder.string().length()+1).c_str(),currentArtistNode->artistName.toCString())) {
						artistFound = true;
						cout << "Found Artist " << currentArtistNode->artistName << endl;
					}
					targetArtistFolder++;
				}
				path libraryArtistFolder;
				if (!artistFound) {
					libraryArtistFolder = path(libraryFolder.string() + '/' + currentArtistNode->artistName.toCString());
					try {
						create_directory(libraryArtistFolder);
					}
					catch (filesystem_error e) {
						cout << e.what() << endl;
					}
				}
				else
					libraryArtistFolder = path(libraryFolder.string() + '/' + currentArtistNode->artistName.toCString());
				currentAlbumNode = currentArtistNode->albumHead;
				while (currentAlbumNode != NULL) {
					albumFound = false;
					//string artistPath = libraryFolder.string() + '/' + currentArtistNode->artistName.toCString();
					//path libraryArtistFolder(artistPath);
					directory_iterator targetAlbumFolder(libraryArtistFolder);
					while (targetAlbumFolder != directory_iterator() && albumFound == false) {
						if (!strcmp(targetAlbumFolder->path().string().substr(libraryArtistFolder.string().length() + 1).c_str(), currentAlbumNode->albumName.toCString())) {
							cout << "Album Found" << endl;
							albumFound = true;
						}
						targetAlbumFolder++;
					}
					string albumPath;
					if (!albumFound) {
						path newAlbumFolder(libraryFolder.string() + '/' + currentAlbumNode->albumName.toCString());
						albumPath = newAlbumFolder.string();
						try {
							create_directory(newAlbumFolder);
						}
						catch (filesystem_error e) {
							cout << e.what() << endl;
						}
					}
					else
						albumPath = libraryFolder.string() + '/' + currentArtistNode->artistName.toCString() + '/' + currentAlbumNode->albumName.toCString();
					currentSongNode = currentAlbumNode->songHead;
					while (currentSongNode != NULL) {
						path songDestinationPath(albumPath + '/' + currentSongNode->songFileName);
						cout << currentSongNode->songPath.string() << " " << songDestinationPath.string() << endl;
						try {
							copy_file(currentSongNode->songPath, songDestinationPath);
						}
						catch (filesystem_error e) {
							cout << e.what() << endl;
						}
						currentSongNode = currentSongNode->next;
					}
					currentAlbumNode = currentAlbumNode->next;
				}
				currentArtistNode = currentArtistNode->next;
			}
			printFolderTree(folderHead);
		}
		else
			errors.errorFunction(2);
	}
	else
		errors.errorFunction(1);
	Sleep(10000);
	return 0;
}

artistsFolder * newArtistEntry(String currentArtist, String currentAlbum, string currentSongFileName, path currentSongPath) {
	artistsFolder *start = new artistsFolder();
	start->artistName = currentArtist;
	start->albumHead = newAlbumEntry(currentAlbum, currentSongFileName, currentSongPath);
	return start;
}

albumsFolder * newAlbumEntry(String currentAlbum, string currentSongFileName, path currentSongPath) {
	albumsFolder *start = new albumsFolder();
	start->albumName = currentAlbum;
	start->songHead = new songsFolder();
	start->songHead = newSongEntry(currentSongFileName, currentSongPath);
	return start;
}

songsFolder * newSongEntry (string currentSong, path currentSongPath) {
	songsFolder *start = new songsFolder();
	start->songFileName = currentSong;
	start->songPath = currentSongPath;
	return start;
}

void printFolderTree(artistsFolder *folderHead) {
	artistsFolder *currentArtistNode;
	albumsFolder *currentAlbumNode;
	songsFolder *currentSongNode;
	
	currentArtistNode = folderHead;
	while (currentArtistNode != NULL) {
		cout << currentArtistNode->artistName << endl;
		currentAlbumNode = currentArtistNode->albumHead;
		while (currentAlbumNode != NULL) {
			cout << "	" << currentAlbumNode->albumName << endl;
			currentSongNode = currentAlbumNode->songHead;
			while (currentSongNode != NULL) {
				cout << "		" << currentSongNode->songFileName << endl;
				currentSongNode = currentSongNode->next;
			}
			currentAlbumNode = currentAlbumNode->next;
		}
		currentArtistNode = currentArtistNode->next;
	}
}

void addArtistNodeToBack(artistsFolder *nodeToadd, artistsFolder *headOfTree) {
	while (headOfTree->next != NULL) {
		headOfTree = headOfTree->next;
	}
	headOfTree->next = nodeToadd;
}

void addAlbumNodeToBack(albumsFolder *nodeToadd, albumsFolder *headOfTree) {
	while (headOfTree->next != NULL) {
		headOfTree = headOfTree->next;
	}
	headOfTree->next = nodeToadd;
}
