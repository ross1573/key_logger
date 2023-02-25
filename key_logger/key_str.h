#ifndef __KEY_STR_H__
#define __KEY_STR_H__

#include <string>

#include "event.h"


namespace event {
std::string string(action::code __a) {
    switch (__a) {
        case action::key_up:        return "KEY_UP";
        case action::key_down:      return "KEY_DOWN";
        default: break;
    }
    return "UNKNOWN";
}

std::string string(key::code __k) {
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
            
        case key::space:                return "space";
        case key::up:                   return "up";
        case key::down:                 return "down";
        case key::left:                 return "left";
        case key::right:                return "right";
        case key::tab:                  return "tab";
        case key::enter:                return "enter";
        case key::escape:               return "escape";
        case key::backspace:            return "backspace";
        case key::left_shift:           return "left shift";
        case key::right_shift:          return "right shift";
        case key::left_control:         return "left control";

        case key::equal:                return "=";
        case key::minus:                return "-";
        case key::right_square_bracket: return "]";
        case key::left_square_bracket:  return "[";
        case key::quote:                return "'";
        case key::semi:                 return ";";
        case key::slash:                return "/";
        case key::backslash:            return "\\";
        case key::comma:                return ",";
        case key::dot:                  return ".";

            
#ifdef _APPL
        case key::clear:                return "clear";
        case key::del:                  return "delete";
        case key::insert:               return "insert";
        case key::home:                 return "home";
        case key::end:                  return "end";
        case key::function:             return "function";
        case key::left_command:         return "left command";
        case key::right_command:        return "right command";
        case key::left_option:          return "left option";
        case key::right_option:         return "right option";
        case key::right_control:        return "right control";
            
        case key::tilde:                return "~";
            
        case key::f1:                   return "F1";
        case key::f2:                   return "F2";
        case key::f3:                   return "F3";
        case key::f4:                   return "F4";
        case key::f5:                   return "F5";
        case key::f6:                   return "F6";
        case key::f7:                   return "F7";
        case key::f8:                   return "F8";
        case key::f9:                   return "F9";
        case key::f10:                  return "F10";
        case key::f11:                  return "F11";
        case key::f12:                  return "F12";
        case key::f13:                  return "F13";
        case key::f14:                  return "F14";
        case key::f15:                  return "F15";
        case key::f16:                  return "F16";

#elif defined _WIN
        case key::left_window:          return "left window";
        case key::right_window:         return "right window";
        case key::alt:                  return "alternative";
#endif
        default: break;
    }
    return "UNKNOWN";
}
}

#endif /* __KEY_STR_H__ */
