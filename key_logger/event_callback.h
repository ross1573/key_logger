#ifndef __EVENT_CALLBACK_H__
#define __EVENT_CALLBACK_H__

#include <functional>

#include "event_logger.h"

#define _REQUIRE_MASK_NOT_EMPTY \
    requires (sizeof...(_Mask) > 0)


namespace event {

template <typename _FuncT, std::size_t... _Mask>
class callback {
public:
    typedef std::function<_FuncT> callback_type;
protected:
    typedef std::array<callback_type, sizeof...(_Mask)> _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    virtual void invoke(const key::code&, const action::code&) = 0;
    
public:
    template <std::size_t _I>
    inline void set_callback(callback_type);
};


template <typename _FuncT, std::size_t... _Mask>
template <std::size_t _I>
void callback<_FuncT, _Mask...>
::set_callback(callback_type __c) {
    __f_[_I] = __c;
}


template <typename _Param, std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
class callback<void(_Param), _Mask...> {
public:
    typedef std::function<void(_Param)> callback_type;
protected:
    typedef std::array<callback_type, sizeof...(_Mask)> _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    virtual void invoke(const key::code&, const action::code&);
    
public:
    template <std::size_t _I>
    inline void set_callback(callback_type);
};


template <typename _Param, std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
void callback<void(_Param), _Mask...>
::invoke(const key::code& __k, const action::code& __a) {
    for (int i = 0; i < sizeof...(_Mask); i++) {
        if (callback_mask<_Mask...>::value[i] == __a) {
            if (__f_[i] != nullptr) {
                __f_[i]((_Param)__k);
            }
            break;
        }
    }
}

template <typename _Param, std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
template <std::size_t _I>
void callback<void(_Param), _Mask...>
::set_callback(callback_type __c) {
    __f_[_I] = __c;
}


template <typename _Param, std::size_t _Mask>
class callback<void(_Param), _Mask> {
public:
    typedef std::function<void(_Param)> callback_type;
protected:
    typedef callback_type _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    virtual void invoke(const key::code&, const action::code&);
    
public:
    inline void set_callback(callback_type);
};


template <typename _Param, std::size_t _Mask>
void callback<void(_Param), _Mask>
::invoke(const key::code& __k, const action::code& __a) {
    __f_((_Param)__k);
}

template <typename _Param, std::size_t _Mask>
void callback<void(_Param), _Mask>
::set_callback(callback_type __c) {
    __f_ = __c;
}


template <typename _1, typename _2, std::size_t _Mask>
class callback<void(_1, _2), _Mask> {
public:
    typedef std::function<void(_1, _2)> callback_type;
protected:
    typedef callback_type _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    virtual void invoke(const key::code&, const action::code&);
    
public:
    inline void set_callback(callback_type);
};


template <typename _1, typename _2, std::size_t _Mask>
void callback<void(_1, _2), _Mask>
::invoke(const key::code& __k, const action::code& __a) {
    __f_((_1)__k, (_2)__a);
}

template <typename _1, typename _2, std::size_t _Mask>
void callback<void(_1, _2), _Mask>
::set_callback(callback_type __c) {
    __f_ = __c;
}


template <typename _FuncT>
class callback<_FuncT> {
public:
    typedef std::function<_FuncT>   callback_type;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    virtual void invoke(const key::code&, const action::code&) { /* Undefined */ }
};


template <std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
class callback<void, _Mask...> {
public:
    typedef void            callback_type;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    inline void invoke(const key::code&, const action::code&) { /* Undefined */ }
};


template <std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
class callback<void(typename key::code,typename action::code), _Mask...> {
public:
    typedef std::function<void(key::code, action::code)> callback_type;
protected:
    typedef callback_type _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    inline void invoke(const key::code&, const action::code&);
    
public:
    inline void set_callback(callback_type);
};


template <std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
void callback<void(typename key::code, typename action::code), _Mask...>
::set_callback(callback_type __c) {
    __f_ = __c;
}

template <std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
void callback<void(typename key::code, typename action::code), _Mask...>
::invoke(const key::code& __k, const action::code& __a) {
    __f_(__k, __a);
}


template <std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
class callback<void(typename action::code, typename key::code), _Mask...> {
public:
    typedef std::function<void(action::code, key::code)> callback_type;
protected:
    typedef callback_type _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    inline void invoke(const key::code&, const action::code&);
    
public:
    inline void set_callback(callback_type);
};


template <std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
void callback<void(typename action::code, typename key::code), _Mask...>
::set_callback(callback_type __c) {
    __f_ = __c;
}

template <std::size_t... _Mask> _REQUIRE_MASK_NOT_EMPTY
void callback<void(typename action::code, typename key::code), _Mask...>
::invoke(const key::code& __k, const action::code& __a) {
    __f_(__a, __k);
}


template <std::size_t _Mask>
class callback<void(typename key::code,typename action::code), _Mask> {
public:
    typedef std::function<void(key::code, action::code)> callback_type;
protected:
    typedef callback_type _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    inline void invoke(const key::code&, const action::code&);
    
public:
    inline void set_callback(callback_type);
};


template <std::size_t _Mask>
void callback<void(typename key::code, typename action::code), _Mask>
::set_callback(callback_type __c) {
    __f_ = __c;
}

template <std::size_t _Mask>
void callback<void(typename key::code, typename action::code), _Mask>
::invoke(const key::code& __k, const action::code& __a) {
    __f_(__k, __a);
}


template <std::size_t _Mask>
class callback<void(typename action::code, typename key::code), _Mask> {
public:
    typedef std::function<void(action::code, key::code)> callback_type;
protected:
    typedef callback_type _CallT;
    
protected:
    _CallT __f_;
    
protected:
    callback() = default;
    virtual ~callback() = default;
    inline void invoke(const key::code&, const action::code&);
    
public:
    inline void set_callback(callback_type);
};


template <std::size_t _Mask>
void callback<void(typename action::code, typename key::code), _Mask>
::set_callback(callback_type __c) {
    __f_ = __c;
}

template <std::size_t _Mask>
void callback<void(typename action::code, typename key::code), _Mask>
::invoke(const key::code& __k, const action::code& __a) {
    __f_(__a, __k);
}

}
#endif /* __EVENT_CALLBACK_H__ */
