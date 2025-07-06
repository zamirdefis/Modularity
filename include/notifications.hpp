#pragma once
#include <string>
#define ERROR_PREFIX "[X]"
#define WARNING_PREFIX "[!]"
#define MSG_PREFIX "[#]"
namespace co {
extern void error(const std::string& str);
}

#define NE_1 "Incorrect argument"
#define NE_2 "Couldn't parse the argument"
#define NE_3 "It is impossible to get a number from an argument"
#define NE_4 "This argument does not support negative numbers"
#define NE_5 "An error occurred while obtaining the CPU load"
