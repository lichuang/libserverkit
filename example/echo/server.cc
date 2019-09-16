#include "serverkit.h"
#include "base/status.h"

using namespace serverkit;

int main(int argc, char* args[]) {
	serverkit::ServerkitInit(argc, args, ServerkitOption());

	Info() << "hello world";

	RunServer();

	return 0;
}