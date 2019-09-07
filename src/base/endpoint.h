/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_ENDPOINT_H__
#define __SERVERKIT_BASE_ENDPOINT_H__

#include <stdint.h>
#include <string>
#include "base/string.h"

using namespace std;

namespace serverkit {

class Endpoint {
public:
	Endpoint(const string& addr, uint16_t port)
		: addr_(addr), port_(port), str_("") {
	}

	Endpoint() 
		: addr_(""), port_(0), str_("") {
	}

  Endpoint(const Endpoint& ep) {
		*this = ep;
	}

  void operator=(const Endpoint& ep);
	bool operator==(const Endpoint& ep);

	void Init(const string& addr, uint16_t port) {
		this->addr_ = addr;
		this->port_ = port;
	}

	const string& Address() const {
		return addr_;
	}

	uint16_t Port() const {
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
	uint16_t port_;
	string str_;
};

inline void 
Endpoint::operator=(const Endpoint& ep) {
	this->addr_ = ep.addr_;
	this->port_ = ep.port_;
	this->str_ = ep.str_;
}

inline bool 
Endpoint::operator==(const Endpoint& ep) {
	return this->port_ == ep.port_ && this->str_ == ep.str_;
}

};  // namespace serverkit

#endif // __SERVERKIT_BASE_ENDPOINT_H__