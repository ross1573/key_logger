#ifndef _EVENT__KEY_LOGGER_H
#define _EVENT__KEY_LOGGER_H

#include "logger.h"


namespace event {
namespace keyboard {
namespace __detail {

struct __key_action_type {
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

struct __key_type : __key_action_type {
    typedef key::code       value_type;
};

struct __action_type : __key_action_type {
    typedef action::code    value_type;
};


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
void __logger<_Mask...>::__callback_(const event_type& __t, const event_ref& __e) {
#ifdef _APPL
    CGKeyCode __kc = (CGKeyCode)CGEventGetIntegerValueField(__e, kCGKeyboardEventKeycode);
    const key::code __k = (key::code)__kc;
    const action::code __a = (action::code)__t;
#elif defined _WIN
    const key::code __k = (key::code)((PKBDLLHOOKSTRUCT)__e)->vkCode;
    const action::code __a = (action::code)__t;
#endif
    __key_callback(__k, __a);
}
    
} /* __detail */

    
template <
    typename _FunctionT,
    template <typename...>
        typename _ContainerT,
    typename _Type = __detail::__key_action_type,
    std::size_t... _Mask
>
class logger;
 

template <
    typename _FunctionT,
    template <typename...>
        typename _ContainerT,
    typename _Type,
    std::size_t... _Mask
>
class logger
: public __detail::__logger<_Mask...>,
  public callback<_FunctionT, _Type>,
  public _ContainerT<_Type> {
private:
    typedef __detail::__logger<_Mask...>        _Base;
    typedef callback<_FunctionT, _Type>         _Callback;
    typedef _ContainerT<_Type>                  _Container;
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
    typename _Type,
    std::size_t... _Mask>
void logger<_FuncT, _ConT, _Type, _Mask...>
::__key_callback(const_key& __k, const_action& __a) {
    _Callback::__invoke_(__k, __a);
    _Container::__push_(__k, __a);
}


template <
    typename _FunctionT,
    template <typename...>
        typename _ContainerT,
    typename _Type
>
class logger<_FunctionT, _ContainerT, _Type>
: public __detail::__logger<event::action::key_system,
                            event::action::key_down,
                            event::action::key_up>,
  public callback<_FunctionT, _Type>,
  public _ContainerT<_Type> {
private:
    typedef __detail::__logger<event::action::key_system,
                               event::action::key_down,
                               event::action::key_up> _Base;
    typedef callback<_FunctionT, _Type>         _Callback;
    typedef _ContainerT<_Type>                  _Container;
      
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
    typename _Type
>
void
logger<_FuncT, _ConT, _Type>
::__key_callback(const_key& __k, const_action& __a) {
    _Callback::__invoke_(__k, __a);
    _Container::__push_(__k, __a);
}


using key_action_type   = __detail::__key_action_type;
using key_type          = __detail::__key_type;
using action_type       = __detail::__action_type;

} /* key */
} /* event */
#endif /* _EVENT__KEY_LOGGER_H */
