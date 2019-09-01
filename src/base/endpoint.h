/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_ENDPOINT_H__
#define __SERVERKIT_BASE_ENDPOINT_H__

#include <string>
#include "base/string.h"

using namespace std;

namespace serverkit {

class Endpoint {
public:
	Endpoint(const string& addr, int port)
		: addr_(addr), port_(port), str_("") {
	}

	Endpoint() 
		: addr_(""), port_(0), str_("") {
	}

  Endpoint(const Endpoint& ep) {
		*this = ep;
	}

  void operator=(const Endpoint& ep);

	void Init(const string& addr, int port) {
		this->addr_ = addr;
		this->port_ = port;
	}

	const string& Address() const {
		return addr_;
	}

	int Port() const {
		return port_;
	}

	const string& String() {
		if (str_.empty()) {
			Stringf(&this->str_, "%s:%d", addr_.c_str(), port_);
		}
 		return str_;
	}

private:
	string addr_;
	int port_;
	string str_;
};

inline void 
Endpoint::operator=(const Endpoint& ep) {
	this->addr_ = ep.addr_;
	this->port_ = ep.port_;
	this->str_ = ep.str_;
}

};  // namespace serverkit

#endif // __SERVERKIT_BASE_ENDPOINT_H__