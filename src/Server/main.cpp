#include "Application.hpp"


#include <stdlib.h> // system

// #pragma comment (lib, "Mswsock.lib")
// #pragma comment(linker, "/MERGE:.rdata=.text")
// #pragma comment(linker, "/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")
// #pragma comment(linker, "/ENTRY:WinMain")

int __cdecl main(int argc, char **argv)
{
	Application app;
	app.init();
		
	app.run();
	
	system("pause");
	return 0;
}

