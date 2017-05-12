#include "Application2D.h"
#include "Testing.h"
//HACK For tests


int main() {

	//run tests
	runUnitTests();

	// allocation
	auto app = new Application2D();

	// initialise and loop
	app->run("lasmath Maths Library Demo", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}