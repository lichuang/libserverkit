#include "serverkit.h"
#include "echo.pb.h"

using namespace serverkit;

void echo_done(EchoResponse* resp) {
	Info() << "response: " << resp->echo_msg();
}

int main(int argc, char *args[]) {
	serverkit::ServerkitInit(argc, args);

	EchoRequest request;
	EchoResponse response;
	RpcController controller;

	request.set_msg("hello");
	
	RpcChannel channel(Endpoint("127.0.0.1", 22222));
	EchoService_Stub stub(&channel);

	stub.Echo(&controller, &request, &response, 
		gpb::NewCallback(::echo_done, &response));

	RunServer(ServerOption());

	return 0;
}