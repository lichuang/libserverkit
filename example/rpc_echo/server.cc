#include "serverkit.h"
#include "base/string.h"
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
        
        response->set_echo_msg("world");
        string content;
        response->SerializeToString(&content);

        EchoResponse msg;
        msg.ParseFromString(content);
        string str = StringToHex(content);
        Info() << "in EchoService::Echo:" << str;

        done->Run();
    }
};

int main(int argc, char *args[]) {
    serverkit::ServerkitInit(argc, args, ServerkitOption());

    EchoServiceImpl echo_service_impl;

    AddRpcService(Endpoint("127.0.0.1", 22222), &echo_service_impl);

    RunServer();
}