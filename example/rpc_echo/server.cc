#include "serverkit.h"
#include "echo.pb.h"

using namespace serverkit;

class EchoServiceImpl : public EchoService {
public:
    EchoServiceImpl() {};
    virtual ~EchoServiceImpl() {};
    virtual void Echo(google::protobuf::RpcController* cntl,
                      const EchoRequest* request,
                      EchoResponse* response,
                      google::protobuf::Closure* done) {
        Info() << "in EchoService::Echo";
        response->set_echo_msg("world");
        done->Run();
    }
};

int main(int argc, char *args[]) {
    serverkit::ServerkitInit(argc, args, ServerkitOption());

    EchoServiceImpl echo_service_impl;

    AddRpcService(Endpoint("127.0.0.1", 22222), &echo_service_impl);

    RunServer();
}