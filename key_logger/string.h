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
            
        case key::a: return "a";
        case key::b: return "b";
        case key::c: return "c";
        case key::d: return "d";
        case key::e: return "e";
        case key::f: return "f";
        case key::g: return "g";
        case key::h: return "h";
        case key::i: return "i";
        case key::j: return "j";
        case key::k: return "k";
        case key::l: return "l";
        case key::m: return "m";
        case key::n: return "n";
        case key::o: return "o";
        case key::p: return "p";
        case key::q: return "q";
        case key::r: return "r";
        case key::s: return "s";
        case key::t: return "t";
        case key::u: return "u";
        case key::v: return "v";
        case key::w: return "w";
        case key::x: return "x";
        case key::y: return "y";
        case key::z: return "z";
#ifdef _APPL
        case key::A: return "A";
        case key::B: return "B";
        case key::C: return "C";
        case key::D: return "D";
        case key::E: return "E";
        case key::F: return "F";
        case key::G: return "G";
        case key::H: return "H";
        case key::I: return "I";
        case key::J: return "J";
        case key::K: return "K";
        case key::L: return "L";
        case key::M: return "M";
        case key::N: return "N";
        case key::O: return "O";
        case key::P: return "P";
        case key::Q: return "Q";
        case key::R: return "R";
        case key::S: return "S";
        case key::T: return "T";
        case key::U: return "U";
        case key::V: return "V";
        case key::W: return "W";
        case key::X: return "X";
        case key::Y: return "Y";
        case key::Z: return "Z";
#endif
            
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
        case key::semicolon:            return ";";
        case key::apostrophe:           return "'";
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
        case key::exclamation_mark:     return "!";
        case key::at_sign:              return "@";
        case key::sharp:                return "#";
        case key::dollor:               return "$";
        case key::percent:              return "%";
        case key::caret:                return "^";
        case key::ampersand:            return "&";
        case key::star:                 return "*";
        case key::left_round_bracket:   return "(";
        case key::right_round_bracket:  return ")";
        case key::underscore:           return "_";
        case key::plus:                 return "+";
        case key::left_curly_bracket:   return "{";
        case key::right_curly_bracket:  return "}";
        case key::vertical_bar:         return "|";
        case key::colon:                return ":";
        case key::quote:                return "\"";
        case key::left_angle_bracket:   return "<";
        case key::right_angle_bracket:  return ">";
        case key::question_mark:        return "?";
            
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
            
        case event::key::a: __o << "a"; break;
        case event::key::b: __o << "b"; break;
        case event::key::c: __o << "c"; break;
        case event::key::d: __o << "d"; break;
        case event::key::e: __o << "e"; break;
        case event::key::f: __o << "f"; break;
        case event::key::g: __o << "g"; break;
        case event::key::h: __o << "h"; break;
        case event::key::i: __o << "i"; break;
        case event::key::j: __o << "j"; break;
        case event::key::k: __o << "k"; break;
        case event::key::l: __o << "l"; break;
        case event::key::m: __o << "m"; break;
        case event::key::n: __o << "n"; break;
        case event::key::o: __o << "o"; break;
        case event::key::p: __o << "p"; break;
        case event::key::q: __o << "q"; break;
        case event::key::r: __o << "r"; break;
        case event::key::s: __o << "s"; break;
        case event::key::t: __o << "t"; break;
        case event::key::u: __o << "u"; break;
        case event::key::v: __o << "v"; break;
        case event::key::w: __o << "w"; break;
        case event::key::x: __o << "x"; break;
        case event::key::y: __o << "y"; break;
        case event::key::z: __o << "z"; break;
#ifdef _APPL
        case event::key::A: __o << "A"; break;
        case event::key::B: __o << "B"; break;
        case event::key::C: __o << "C"; break;
        case event::key::D: __o << "D"; break;
        case event::key::E: __o << "E"; break;
        case event::key::F: __o << "F"; break;
        case event::key::G: __o << "G"; break;
        case event::key::H: __o << "H"; break;
        case event::key::I: __o << "I"; break;
        case event::key::J: __o << "J"; break;
        case event::key::K: __o << "K"; break;
        case event::key::L: __o << "L"; break;
        case event::key::M: __o << "M"; break;
        case event::key::N: __o << "N"; break;
        case event::key::O: __o << "O"; break;
        case event::key::P: __o << "P"; break;
        case event::key::Q: __o << "Q"; break;
        case event::key::R: __o << "R"; break;
        case event::key::S: __o << "S"; break;
        case event::key::T: __o << "T"; break;
        case event::key::U: __o << "U"; break;
        case event::key::V: __o << "V"; break;
        case event::key::W: __o << "W"; break;
        case event::key::X: __o << "X"; break;
        case event::key::Y: __o << "Y"; break;
        case event::key::Z: __o << "Z"; break;
#endif
            
        case event::key::enter:                 __o << "\n"; break;
        case event::key::tab:                   __o << "\t"; break;
        case event::key::space:                 __o <<  " "; break;
        case event::key::equal:                 __o <<  "="; break;
        case event::key::minus:                 __o <<  "-"; break;
        case event::key::right_square_bracket:  __o <<  "]"; break;
        case event::key::left_square_bracket:   __o <<  "["; break;
        case event::key::apostrophe:            __o <<  "'"; break;
        case event::key::semicolon:             __o <<  ";"; break;
        case event::key::slash:                 __o <<  "/"; break;
        case event::key::backslash:             __o << "\\"; break;
        case event::key::comma:                 __o <<  ","; break;
        case event::key::dot:                   __o <<  "."; break;
            
#ifdef _APPL
        case event::key::tilde:                 __o <<  "~"; break;
        case event::key::exclamation_mark:      __o <<  "!"; break;
        case event::key::at_sign:               __o <<  "@"; break;
        case event::key::sharp:                 __o <<  "#"; break;
        case event::key::dollor:                __o <<  "$"; break;
        case event::key::percent:               __o <<  "%"; break;
        case event::key::caret:                 __o <<  "^"; break;
        case event::key::ampersand:             __o <<  "&"; break;
        case event::key::star:                  __o <<  "*"; break;
        case event::key::left_round_bracket:    __o <<  "("; break;
        case event::key::right_round_bracket:   __o <<  ")"; break;
        case event::key::underscore:            __o <<  "_"; break;
        case event::key::plus:                  __o <<  "+"; break;
        case event::key::left_curly_bracket:    __o <<  "{"; break;
        case event::key::right_curly_bracket:   __o <<  "}"; break;
        case event::key::vertical_bar:          __o <<  "|"; break;
        case event::key::colon:                 __o <<  ":"; break;
        case event::key::quote:                 __o <<  "\""; break;
        case event::key::left_angle_bracket:    __o <<  "<"; break;
        case event::key::right_angle_bracket:   __o <<  ">"; break;
        case event::key::question_mark:         __o <<  "?"; break;
            
#elif defined _WIN
            
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
