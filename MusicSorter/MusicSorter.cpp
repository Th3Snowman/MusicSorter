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



using namespace TagLib;
using namespace std;
using namespace boost::filesystem;


int main()
{
	TagLib::FileRef f("F:\meh.flac");
	Tag *plz = f.tag();
	cout << plz->artist() << endl;
	cout << file_size("F:\meh.flac");

	Sleep(1000);
    return 0;
}

