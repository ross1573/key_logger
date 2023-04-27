#ifndef _EVENT__STRING_H
#define _EVENT__STRING_H

#include <string>
#include <sstream>
#include <ostream>
#include <utility>

#include "event.h"
#include "utility.h"


namespace event {
    
std::string string(const action::code& __a) {
    switch (__a) {
        case action::key_up:        return "KEY_UP";
        case action::key_down:      return "KEY_DOWN";
        case action::key_system:    return "KEY_SYSTEM";
        default: break;
    }
    return "UNKNOWN";
}
    
std::string string(const key::code& __k) {
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
            
        case key::up:                   return "up";
        case key::down:                 return "down";
        case key::left:                 return "left";
        case key::right:                return "right";
        case key::space:                return "space";
        case key::enter:                return "enter";
        case key::tab:                  return "tab";
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
} /* event */


std::ostream& operator<<(std::ostream& __o, event::key::code __k) {
    switch (__k) {
        case event::key::_0: __o << "0"; break;
        case event::key::_1: __o << "1"; break;
        case event::key::_2: __o << "2"; break;
        case event::key::_3: __o << "3"; break;
        case event::key::_4: __o << "4"; break;
        case event::key::_5: __o << "5"; break;
        case event::key::_6: __o << "6"; break;
        case event::key::_7: __o << "7"; break;
        case event::key::_8: __o << "8"; break;
        case event::key::_9: __o << "9"; break;
        case event::key::a: __o << "A"; break;
        case event::key::b: __o << "B"; break;
        case event::key::c: __o << "C"; break;
        case event::key::d: __o << "D"; break;
        case event::key::e: __o << "E"; break;
        case event::key::f: __o << "F"; break;
        case event::key::g: __o << "G"; break;
        case event::key::h: __o << "H"; break;
        case event::key::i: __o << "I"; break;
        case event::key::j: __o << "J"; break;
        case event::key::k: __o << "K"; break;
        case event::key::l: __o << "L"; break;
        case event::key::m: __o << "M"; break;
        case event::key::n: __o << "N"; break;
        case event::key::o: __o << "O"; break;
        case event::key::p: __o << "P"; break;
        case event::key::q: __o << "Q"; break;
        case event::key::r: __o << "R"; break;
        case event::key::s: __o << "S"; break;
        case event::key::t: __o << "T"; break;
        case event::key::u: __o << "U"; break;
        case event::key::v: __o << "V"; break;
        case event::key::w: __o << "W"; break;
        case event::key::x: __o << "X"; break;
        case event::key::y: __o << "Y"; break;
        case event::key::z: __o << "Z"; break;
            
        case event::key::enter:                __o << "\n"; break;
        case event::key::space:                __o <<  " "; break;
        case event::key::equal:                __o <<  "="; break;
        case event::key::minus:                __o <<  "-"; break;
        case event::key::right_square_bracket: __o <<  "]"; break;
        case event::key::left_square_bracket:  __o <<  "["; break;
        case event::key::quote:                __o <<  "'"; break;
        case event::key::semi:                 __o <<  ";"; break;
        case event::key::slash:                __o <<  "/"; break;
        case event::key::backslash:            __o << "\\"; break;
        case event::key::comma:                __o <<  ","; break;
        case event::key::dot:                  __o <<  "."; break;
            
#ifdef _APPL
        case event::key::tilde:                __o <<  "~"; break;
            
            //        case key::clear:                return "clear";
            //        case key::del:                  return "delete";
            //        case key::insert:               return "insert";
            //        case key::home:                 return "home";
            //        case key::end:                  return "end";
            //        case key::function:             return "function";
            //        case key::left_command:         return "left command";
            //        case key::right_command:        return "right command";
            //        case key::left_option:          return "left option";
            //        case key::right_option:         return "right option";
            //        case key::right_control:        return "right control";
            
#elif defined _WIN
            //        case key::left_window:          return "left window";
            //        case key::right_window:         return "right window";
            //        case key::alt:                  return "alternative";
#endif
        default: break;
    }
    return __o;
}


template <
    template <typename...>
        typename _T,
    typename... _TT
> requires event::contain_key_type<_T<_TT...>>::value
std::ostream& operator<<(std::ostream& __o, const _T<_TT...>& __e) {
    __o << std::get<typename event::get_key_type<_TT...>::type>(__e);
    return __o;
}

#endif /* _EVENT__STRING_H */
