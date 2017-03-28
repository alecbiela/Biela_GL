/*Biela_GL, an openGL engine by Alec Bielanos
 *There are still many structural issues to work out,
 *Like tidying up the encapsulation and reworking movement.
 *But, for now, we have v1.0 as a submission for HW5 in IGME309*/

#include "stdafx.h"
#include "Engine.h"
#include <ctime>

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));

	//creates a new GL engine
	Engine* e = new Engine();

	//if we failed to initialize, quit
	if (!e->Init()) {
		delete e; return -1;
	}

	//if we failed to buffer the models, quit
	if (!e->BufferModels()) {
		delete e; return -1;
	}

	e->Texture("");

	//if we failed to use the shaders, quit
	if (!e->UseShaders()) {
		delete e; return -1;
	}

	//run the game loop
	e->GameLoop();


	//clear memory and exit
	delete e;

	return 0;
}

