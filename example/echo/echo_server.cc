#include "serverkit.h"

int main(int argc, char* args[]) {
	serverkit::ServerkitInit(argc, args);

	Info() << "hello world";

	serverkit::Server s(1);
	s.Run();

	return 0;
}