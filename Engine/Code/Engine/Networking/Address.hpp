#pragma once

#define NETWORKING
#include "Engine/Core/Win.hpp"

#include <string>

namespace Net {

class Address {
public:
    Address() = default;
    Address(const Address& other) = default;
    Address(Address&& other) = default;
    Address& operator=(const Address& rhs) = default;
    Address& operator=(Address&& rhs) = default;
    ~Address() = default;
    explicit Address(const std::string& value);
    
    bool operator==(const Address& rhs) const;
    bool operator!=(const Address& rhs) const;

protected:
private:
    union address_t {
        uint32_t ipv6[4] = { 0u, 0u, 0u, 0u };
        uint32_t ipv4;
        ~address_t() {};
    };
    address_t address{};
    uint16_t port = 0;
};

}