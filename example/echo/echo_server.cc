#include "base/log.h"
#include "base/log_thread.h"
#include "core/server.h"

int main() {
	serverkit::Singleton<serverkit::LogThread>::Instance();

	Info() << "hello world";

	serverkit::Server s(1);
	s.Run();

	return 0;
}