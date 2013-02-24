/* 
 * File:   Utils.cpp
 * Author: radek
 * 
 * Created on 14 luty 2013, 17:06
 */

#include "Utils.h"
#include <sstream>

Utils::Utils() {
}
Utils::~Utils() {
}

std::string Utils::toStr(int id){
    std::stringstream s;
    s<<id;
    return s.str();
}