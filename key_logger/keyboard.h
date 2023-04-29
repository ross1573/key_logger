#ifndef _EVENT__KEY_LOGGER_H
#define _EVENT__KEY_LOGGER_H

#include "logger.h"


namespace event {
namespace keyboard {
namespace __detail {

struct __keyboard_traits {
    typedef key::code       key_type;
    typedef action::code    action_type;
    
    inline constexpr bool
    operator()(const action_type& __a) {
        switch (__a) {
            case action_type::key_down: return true;
            case action_type::key_up:
            case action_type::key_system: return false;
        }
    }
};

struct __key_action_traits : __keyboard_traits {
    typedef key::code       key_type;
    typedef action::code    action_type;
    typedef std::pair<key_type, action_type> value_type;
};

struct __key_traits : __keyboard_traits {
    typedef key::code       key_type;
    typedef action::code    action_type;
    typedef key::code       value_type;
};

struct __action_traits : __keyboard_traits {
    typedef key::code       key_type;
    typedef action::code    action_type;
    typedef action::code    value_type;
};


inline constexpr event::key::code
__shift_converter(const event::key::code& __k) {
    switch (__k) {
        case event::key::code::a:
        case event::key::code::s:
        case event::key::code::d:
        case event::key::code::f:
        case event::key::code::h:
        case event::key::code::g:
        case event::key::code::z:
        case event::key::code::x:
        case event::key::code::c:
        case event::key::code::v:
        case event::key::code::b:
        case event::key::code::q:
        case event::key::code::w:
        case event::key::code::e:
        case event::key::code::r:
        case event::key::code::y:
        case event::key::code::t:
        case event::key::code::_1:
        case event::key::code::_2:
        case event::key::code::_3:
        case event::key::code::_4:
        case event::key::code::_6:
        case event::key::code::_5:
        case event::key::code::equal:
        case event::key::code::_9:
        case event::key::code::_7:
        case event::key::code::minus:
        case event::key::code::_8:
        case event::key::code::_0:
        case event::key::code::right_square_bracket:
        case event::key::code::o:
        case event::key::code::u:
        case event::key::code::left_square_bracket:
        case event::key::code::i:
        case event::key::code::p:
        case event::key::code::l:
        case event::key::code::j:
        case event::key::code::apostrophe:
        case event::key::code::k:
        case event::key::code::semicolon:
        case event::key::code::backslash:
        case event::key::code::comma:
        case event::key::code::slash:
        case event::key::code::n:
        case event::key::code::m:
        case event::key::code::dot: break;
        default: return __k;
    };
    return (event::key::code)(0b11000000 | (char)__k);
}


template <std::size_t... _Mask>
class __logger
: public event::__detail::__logger_common<_Mask...> {
private:
    typedef event::__detail::__logger_common<_Mask...>  _Base;
public:
    typedef typename _Base::event_field                 event_field;
    typedef typename _Base::event_type                  event_type;
    typedef typename _Base::event_ref                   event_ref;
    typedef key::code                                   key_type;
    typedef action::code                                action_type;
    typedef const key_type                              const_key;
    typedef const action_type                           const_action;
        
protected:
    __logger();
    virtual ~__logger() = default;
    virtual void __key_callback(const_key&, const_action&) = 0;
        
private:
    void __callback_(const event_type&, const event_ref&) final;
};
    

template <std::size_t... _Mask>
__logger<_Mask...>::__logger() {
#ifdef _WIN
    _Base::__set_field(WH_KEYBOARD_LL);
#endif
}
    
template <std::size_t... _Mask>
void __logger<_Mask...>::__callback_(const event_type& __t,
                                     const event_ref& __e)
{
#ifdef _APPL
    int64_t __kc = CGEventGetIntegerValueField(__e, kCGKeyboardEventKeycode);
    const key::code __k = (key::code)__kc;
    const action::code __a = (action::code)__t;
#elif defined _WIN
    const key::code __k = (key::code)((PKBDLLHOOKSTRUCT)__e)->vkCode;
    const action::code __a = (action::code)__t;
#endif
    __key_callback(__k, __a);
}


template <std::size_t... _Mask>
    requires event::__detail::__contain_value<action::key_system, _Mask...>
class __logger <_Mask...>
: public event::__detail::__logger_common<_Mask...> {
private:
    typedef event::__detail::__logger_common<_Mask...>  _Base;
public:
    typedef typename _Base::event_field                 event_field;
    typedef typename _Base::event_type                  event_type;
    typedef typename _Base::event_ref                   event_ref;
    typedef key::code                                   key_type;
    typedef action::code                                action_type;
    typedef const key_type                              const_key;
    typedef const action_type                           const_action;
    
private:
    bool __sh = false;
        
protected:
    virtual ~__logger() = default;
    virtual void __key_callback(const_key&, const_action&) = 0;
    
    __logger() {
#ifdef _WIN
        _Base::__set_field(WH_KEYBOARD_LL);
#endif
    }
        
private:
    void __callback_(const event_type& __t, const event_ref& __e)
    {
#ifdef _APPL
        int64_t __kc = CGEventGetIntegerValueField(__e, kCGKeyboardEventKeycode);
        if (__sh)
            __kc = (int64_t)__shift_converter((key::code)__kc);

        const key::code __k = (key::code)__kc;
        const action::code __a = (action::code)__t;
        __process_system_keys(__k, __a);
#elif defined _WIN
        const key::code __k = (key::code)((PKBDLLHOOKSTRUCT)__e)->vkCode;
        const action::code __a = (action::code)__t;
#endif
        __key_callback(__k, __a);
    }
    
    inline constexpr void __process_system_keys(const key::code& __k,
                                                const action::code& __a)
    {
        if (__a != action::key_system)
            return;
        
        switch (__k) {
            case key::code::left_shift:
            case key::code::right_shift:    __sh = (!__sh); break;
            default: break;
        }
    }
};
    
} /* __detail */

    
template <
    typename _FunctionT,
    template <typename...>
        typename _ContainerT,
    typename _Traits = __detail::__key_action_traits,
    std::size_t... _Mask
>
class logger;
 

template <
    typename _FunctionT,
    template <typename...>
        typename _ContainerT,
    typename _Traits,
    std::size_t... _Mask
>
class logger
: public __detail::__logger<_Mask...>,
  public callback<_FunctionT, _Traits>,
  public _ContainerT<_Traits> {
private:
    typedef __detail::__logger<_Mask...>        _Base;
    typedef callback<_FunctionT, _Traits>       _Callback;
    typedef _ContainerT<_Traits>                _Container;
  public:
    typedef typename _Base::key_type            key_type;
    typedef typename _Base::action_type         action_type;
    typedef typename _Base::const_key           const_key;
    typedef typename _Base::const_action        const_action;
    
public:
    logger() = default;
    virtual ~logger() = default;
    
protected:
    void __key_callback(const_key&, const_action&) final;
};

template <
    typename _FuncT,
    template <typename...>
        typename _ConT,
    typename _Traits,
    std::size_t... _Mask>
void logger<_FuncT, _ConT, _Traits, _Mask...>
::__key_callback(const_key& __k, const_action& __a) {
    _Callback::__invoke_(__k, __a);
    _Container::__push_(__k, __a);
}


template <
    typename _FunctionT,
    template <typename...>
        typename _ContainerT,
    typename _Traits
>
class logger<_FunctionT, _ContainerT, _Traits>
: public __detail::__logger<event::action::key_system,
                            event::action::key_down,
                            event::action::key_up>,
  public callback<_FunctionT, _Traits>,
  public _ContainerT<_Traits> {
private:
    typedef __detail::__logger<event::action::key_system,
                               event::action::key_down,
                               event::action::key_up> _Base;
    typedef callback<_FunctionT, _Traits>       _Callback;
    typedef _ContainerT<_Traits>                _Container;
      
public:
    typedef typename _Base::key_type            key_type;
    typedef typename _Base::action_type         action_type;
    typedef typename _Base::const_key           const_key;
    typedef typename _Base::const_action        const_action;
    
public:
    logger() = default;
    virtual ~logger() = default;
    
protected:
    void __key_callback(const_key&, const_action&) final;
};

template <
    typename _FuncT,
    template <typename...>
        typename _ConT,
    typename _Traits
>
void
logger<_FuncT, _ConT, _Traits>
::__key_callback(const_key& __k, const_action& __a) {
    _Callback::__invoke_(__k, __a);
    _Container::__push_(__k, __a);
}


namespace traits {
using key_action    = __detail::__key_action_traits;
using key           = __detail::__key_traits;
using action        = __detail::__action_traits;
} /* traits */

} /* key */
} /* event */
#endif /* _EVENT__KEY_LOGGER_H */
