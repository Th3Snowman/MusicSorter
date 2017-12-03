#include "stdafx.h"
#include <iostream>

class errorClass {
	public:
		static void errorFunction(int errorCode) {
			switch (errorCode) {
			case 1:
				std::cout << "Error (1): Incorrect number of arguments" << std::endl;
				break;
			case 2:
				std::cout << "Error (2): one or both or paths provided are not directories" << std::endl;
				break;
			}
		}
};