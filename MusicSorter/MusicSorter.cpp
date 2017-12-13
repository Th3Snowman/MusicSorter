// MusicSorter.cpp : Defines the entry point for the console application
//

/*
	TODO:
		use destructors
*/

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

/*
This sorting works on the basis sorting all the songs using theie id3 tags from the specified inital folder into a 3d linked list with an inital LL of artists, from each artist node 
there extends a LL of albums corresponding to that artist, and from each album node there extends a LL of songs corresponding to that album. The first large portion of the code 
corresponds to building that LL and then the second large part of the code corresponds to using that LL to sort the songs from the inital folder into the specified library folder, building/sorting
the songs into a library in the format artist folders -> album folders -> songs. The function will search recursively into folders in the specified inital folders so it can also be used to simply
sort already existing libraries in any given folder format
*/

void printFolderTree(artistsFolder *folderHead);
songsFolder * newSongEntry(string currentSong, path currentSongPath);
albumsFolder * newAlbumEntry(String currentAlbum, string currentSong, path currentSongPath);
artistsFolder * newArtistEntry(String currentArtist, String currentAlbum, string currentSong, path currentSongPath);
void addArtistNodeToBack(artistsFolder *nodeToadd, artistsFolder *headOfTree);
void addAlbumNodeToBack(albumsFolder *nodeToadd, albumsFolder *headOfTree);
void deleteFolderTree(artistsFolder *folderHead);

int main(int argc, char *argv[])
{
	errorClass errors;
	if (argc == 3) {
		path initalFolder(argv[1]); //path to target file
		path libraryFolder(argv[2]); //path to target folder

		if (is_directory(initalFolder) && is_directory(libraryFolder)) { // if no exception and the file at the following path exists
			artistsFolder *folderHead = NULL;
			bool artistFound = false; // if we have found that a node already exists for the specified album/artist and therefore don't have to make a new node
			bool albumFound = false;
			artistsFolder *currentArtistNode = NULL; // references for scrolling through the linked list
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
					string currentSongFileName = originFolder->path().filename().string(); //filename of current song with extension included
					path currentSongPath = path(originFolder->path().string()); //path to current song
					artistFound = false;
					albumFound = false;

					if (folderHead == NULL) { // if this is the first entry and therefore the linked list isn't started
						folderHead = new artistsFolder(currentArtist, currentAlbum, currentSongFileName, currentSongPath);
					}
					else {
						currentArtistNode = folderHead; // start iterating through the artist LL looking for the artist of the current song
						while (artistFound == false && currentArtistNode != NULL) {
							if (currentArtist == currentArtistNode->artistName) {
								artistFound = true;
							}
							else {
								currentArtistNode = currentArtistNode->next;
							}
						}
						if (artistFound) { // if already existing artist node is found start iterating through the album LL looking for the album of the current song
							currentAlbumNode = currentArtistNode->albumHead; 
							while (albumFound == false && currentAlbumNode != NULL) { 
								if (currentAlbum == currentAlbumNode->albumName) {
									albumFound = true;
								}
								else
									currentAlbumNode = currentAlbumNode->next;
							}
							if (albumFound) { // if already existing album node is found then add the song into the LL extending from the album node
								currentSongNode = currentAlbumNode->songHead;
								songsFolder *newSong = new songsFolder(currentSongFileName, currentSongPath); // creating new song 
								newSong->next = currentSongNode; // adds it to the front of the LL (order is irrelevant since order of album is contained in id3 tags)
								currentAlbumNode->songHead = newSong;
							}
							else // if album node corresponding to current song isn't found then create a new album node and song node and attach them the LL extending from the correct artist node
								addAlbumNodeToBack(new albumsFolder(currentAlbum, currentSongFileName, currentSongPath), currentArtistNode->albumHead);
						}
						else // if artist node corresponding to current song isn't found then create a new album node and song node and attach them to the artist LL
							addArtistNodeToBack(new artistsFolder(currentArtist, currentAlbum, currentSongFileName, currentSongPath), folderHead);
					}
				}
				else { // if the file currently being read by the directory iterator doesn't have an id3 tag
					cout << "Non-music file found, skipping" << endl;
				}
			}
			currentArtistNode = folderHead; // we reuse the current<nodetype> pointers for scrolling through the LL looking for the correct folders in the library folder
			while(currentArtistNode != NULL) {
				artistFound = false;
				directory_iterator targetArtistFolder(libraryFolder);
				while (targetArtistFolder != directory_iterator() && artistFound == false){ // scroll through all the objects in the specified libraryFolder
					if ((!strcmp(targetArtistFolder->path().string().substr(libraryFolder.string().length()+1).c_str(),currentArtistNode->artistName.toCString())) && is_directory(targetArtistFolder->path())) { // checks if the current folder has 
						artistFound = true;																																										  // the same name as the currentArtist and is a directory
					}
					if (artistFound == false)
						targetArtistFolder++;
				}
				path libraryArtistFolder; // path that is for the current artist folder whether it already exists or is being created
				if (!artistFound) { // creating new folder
					libraryArtistFolder = path(libraryFolder.string() + '/' + currentArtistNode->artistName.toCString());
					try {
						create_directory(libraryArtistFolder);
					}
					catch (filesystem_error e) {
						cout << e.what() << endl;
					}
				}
				else // if folder already existed
					libraryArtistFolder = targetArtistFolder->path();
				currentAlbumNode = currentArtistNode->albumHead; // now checking for album folders inside the artist folder
				while (currentAlbumNode != NULL) {
					albumFound = false;
					directory_iterator targetAlbumFolder(libraryArtistFolder);
					while (targetAlbumFolder != directory_iterator() && albumFound == false) { // scroll through objects in the specified artistFolder
						if ((!strcmp(targetAlbumFolder->path().string().substr(libraryArtistFolder.string().length() + 1).c_str(), currentAlbumNode->albumName.toCString())) && is_directory(targetAlbumFolder->path())) { // checks if current folder has the same name as
							albumFound = true;																																									   // currentAlbum and is a directory
						}
						if (albumFound == false)
							targetAlbumFolder++;
					}
					path libraryAlbumFolder; // path (in string form) for album folder inside artist folder
					if (!albumFound) { // creating new album folder if not found
						path newAlbumFolder(libraryArtistFolder.string() + '/' + currentAlbumNode->albumName.toCString());
						libraryAlbumFolder = newAlbumFolder.string();
						try {
							create_directory(newAlbumFolder);
						}
						catch (filesystem_error e) {
							cout << e.what() << endl;
						}
					}
					else // if the album folder existed
						libraryAlbumFolder = targetAlbumFolder->path();
					currentSongNode = currentAlbumNode->songHead;
					while (currentSongNode != NULL) {
						path songDestinationPath(libraryAlbumFolder.string() + '/' + currentSongNode->songFileName); // create the theoretical path for the song in the album folder
						try {
							if (!exists(songDestinationPath)) // if the song doesn't already exist in that location
								copy_file(currentSongNode->songPath, songDestinationPath);
						}
						catch (filesystem_error e) {
							cout << e.what() << endl;
						}
						currentSongNode = currentSongNode->next; // iterate through song LL
					}
					currentAlbumNode = currentAlbumNode->next; // iterate through album LL
				}
				currentArtistNode = currentArtistNode->next; // iterate through artist LL
			}
			printFolderTree(folderHead); // print the LL
			deleteFolderTree(folderHead); // delete the LL
		}
		else
			errors.errorFunction(2);
	}
	else
		errors.errorFunction(1);
	Sleep(20000);
	return 0;
}

void printFolderTree(artistsFolder *folderHead) { // function iterates through the LL and prints it out in a nice format for the user to see what has been read in from the inital folder
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

void deleteFolderTree(artistsFolder *folderHead) {
	artistsFolder* currentArtistNode = folderHead;
	artistsFolder* previousArtistNode;
	albumsFolder* currentAlbumNode;
	albumsFolder* previousAlbumNode;
	songsFolder* currentSongNode;
	songsFolder* previousSongNode;

	while (currentArtistNode != NULL) {
		currentAlbumNode = currentArtistNode->albumHead;
		while (currentAlbumNode != NULL) {
			currentSongNode = currentAlbumNode->songHead;
			while (currentSongNode != NULL) {
				previousSongNode = currentSongNode;
				currentSongNode = previousSongNode->next;
				delete previousSongNode;
			}
			previousAlbumNode = currentAlbumNode;
			currentAlbumNode = previousAlbumNode->next;
			delete previousAlbumNode;
		}
		previousArtistNode = currentArtistNode;
		currentArtistNode = previousArtistNode->next;
		delete previousArtistNode;
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
