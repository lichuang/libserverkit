#include "serverkit.h"
#include "echo.pb.h"

using namespace serverkit;

void echo_done(EchoResponse* resp) {
	Info() << "response: " << resp->echo_msg();

	delete resp;
}

void create_channel_done(RpcChannel* channel) {
	EchoRequest request;
	EchoResponse* response = new EchoResponse();
	RpcController controller;

	request.set_msg("hello world");
	
	EchoService_Stub stub(channel);

	stub.Echo(&controller, &request, response, 
		gpb::NewCallback(::echo_done, response));

  if (controller.Failed()) {
    Error() << "rpc fail: " << controller.ErrorText();
  }

	//delete channel;
}

int main(int argc, char *args[]) {
	serverkit::ServerkitInit(argc, args, ServerkitOption());
	CreateRpcChannel(Endpoint("127.0.0.1", 22222), create_channel_done);

	RunServer();

	return 0;
}