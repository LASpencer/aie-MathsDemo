#include "Application2D.h"

int main() {
	
	//HACK TESTING

	//HACK end tests

	// allocation
	auto app = new Application2D();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}