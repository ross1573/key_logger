#ifndef __KEY_STR_H__
#define __KEY_STR_H__

#include <string>

#include "key_code.h"


namespace event {
std::string string(action __a) {
    switch (__a) {
        case action::key_up:        return "KEY_UP";
        case action::key_down:      return "KEY_DOWN";
        default: break;
    }
    return "UNKNOWN";
}

std::string string(key __k) {
    switch (__k) {
        case key::_0: return "0";
        case key::_1: return "1";
        case key::_2: return "2";
        case key::_3: return "3";
        case key::_4: return "4";
        case key::_5: return "5";
        case key::_6: return "6";
        case key::_7: return "7";
        case key::_8: return "8";
        case key::_9: return "9";
        case key::a: return "A";
        case key::b: return "B";
        case key::c: return "C";
        case key::d: return "D";
        case key::e: return "E";
        case key::f: return "F";
        case key::g: return "G";
        case key::h: return "H";
        case key::i: return "I";
        case key::j: return "J";
        case key::k: return "K";
        case key::l: return "L";
        case key::m: return "M";
        case key::n: return "N";
        case key::o: return "O";
        case key::p: return "P";
        case key::q: return "Q";
        case key::r: return "R";
        case key::s: return "S";
        case key::t: return "T";
        case key::u: return "U";
        case key::v: return "V";
        case key::w: return "W";
        case key::x: return "X";
        case key::y: return "Y";
        case key::z: return "Z";
            
        case key::up:               return "UP";
        case key::down:             return "DOWN";
        case key::left:             return "LEFT";
        case key::right:            return "RIGHT";
        case key::tab:              return "TAB";
        case key::enter:            return "ENTER";
        case key::escape:           return "ESCAPE";
        case key::backspace:        return "BACKSPACE";
            
#ifdef _APPL
        case key::clear:            return "CLEAR";
        case key::space:            return "SPACE";
        case key::del:              return "DELETE";
        case key::ret:              return "RETURN";
        case key::insert:           return "INSERT";
        case key::home:             return "HOME";
        case key::end:              return "END";
            
        case key::equal:            return "=";
        case key::minus:            return "-";
        case key::rightbracket:     return ">";
        case key::leftbracket:      return "<";
        case key::quote:            return "'";
        case key::semi:             return ";";
        case key::slash:            return "|";
        case key::comma:            return ",";
        case key::dot:              return ".";
            
        case key::f1:               return "F1";
        case key::f2:               return "F2";
        case key::f3:               return "F3";
        case key::f4:               return "F4";
        case key::f5:               return "F5";
        case key::f6:               return "F6";
        case key::f7:               return "F7";
        case key::f8:               return "F8";
        case key::f9:               return "F9";
        case key::f10:              return "F10";
        case key::f12:              return "F12";
#elif defined _WIN
        case key::shift:            return "SHIFT";
        case key::ctrl:             return "CTRL";
        case key::alt:              return "ALT";
#endif
        default: break;
    }
    return "UNKNOWN";
}
}

#endif /* __KEY_STR_H__ */
