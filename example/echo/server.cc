#include "serverkit.h"
#include "base/status.h"

using namespace serverkit;

int main(int argc, char* args[]) {
	serverkit::ServerkitInit(argc, args);

	Info() << "hello world";

	Assert(1 == 0) << "hahahahh";

	//serverkit::Server s(1);
	//s.Run();

	return 0;
}