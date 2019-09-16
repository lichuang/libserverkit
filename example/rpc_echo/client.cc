#include "serverkit.h"
#include "echo.pb.h"
#include "base/wait.h"

using namespace serverkit;

void echo_done(EchoResponse* resp, WaitGroup* wait) {
	Info() << "response: " << resp->echo_msg();
	wait->Done();
}

void create_channel_done(RpcChannel* channel) {
	EchoRequest request;
	EchoResponse response;
	RpcController controller;
	WaitGroup wait;

	request.set_msg("hello");
	
	EchoService_Stub stub(channel);

	wait.Add(1);

	stub.Echo(&controller, &request, &response, 
		gpb::NewCallback(::echo_done, &response, &wait));

	Info() << "client waiting";

	wait.Wait();

  if (controller.Failed()) {
    Error() << "rpc fail: " << controller.ErrorText();
  }

	delete channel;
}

int main(int argc, char *args[]) {
	serverkit::ServerkitInit(argc, args, ServerkitOption());
	CreateRpcChannel(Endpoint("127.0.0.1", 22222), create_channel_done);

	RunServer();

	return 0;
}