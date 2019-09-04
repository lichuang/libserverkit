/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_METHOD_META_H__
#define __SERVERKIT_RPC_METHOD_META_H__

#include "google/protobuf/descriptor.h"
#include "google/protobuf/service.h"

namespace serverkit {

namespace gpb = ::google::protobuf;

// RPC Method meta info class
class MethodMeta {
public:
	MethodMeta(gpb::Service*, const gpb::MethodDescriptor* desp)
		: service_(service), methodDesp_(desp) {}

	gpb::Service* GetService() {
  	return service_;
	}

	const gpb::MethodDescriptor& GetMethodDescriptor() const {
  	return *methodDesp_;
	}

	const gpb::Message& GetRequestPrototype() const {
  return service_->GetRequestPrototype(methodDesp_);
	}

	const gpb::Message& GetResponsePrototype() const {
  	return service_->GetResponsePrototype(methodDesp_);
	}

private:
	gpb::Service* service_;
	gpb::MethodDescriptor methodDesp_;
};

};  // namespace serverkit

#endif // __SERVERKIT_RPC_METHOD_META_H__