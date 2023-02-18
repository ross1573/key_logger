#ifndef __EVENT_CALLBACK_H__
#define __EVENT_CALLBACK_H__

#include <functional>

#include "event_logger.h"


template <typename _FuncT, std::size_t... _Mask>
class event_callback {
public:
    typedef std::function<_FuncT>   callback;
    typedef event::key              key;
    typedef event::action           action;
protected:
    typedef std::array<callback, sizeof...(_Mask)> _CallT;
    
protected:
    _CallT __f_;
    
protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    virtual void invoke_callback(const key&, const action&) = 0;
    
public:
    template <std::size_t _I>
    inline void set_callback(callback);
};


template <typename _FuncT, std::size_t... _Mask>
template <std::size_t _I>
void event_callback<_FuncT, _Mask...>::set_callback(callback __c) {
    __f_[_I] = __c;
}


template <std::size_t... _Mask>
class event_callback<void, _Mask...> {
public:
    typedef event::key      key;
    typedef event::action   action;
    typedef void            callback;
    
protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    inline void invoke_callback(const key&, const action&) { /* Undefined */ }
};


template <std::size_t... _Mask>
class event_callback<void(typename event::key), _Mask...> {
public:
    typedef event::key              key;
    typedef event::action           action;
    typedef std::function<void(key)> callback;
protected:
    typedef std::array<callback, sizeof...(_Mask)> _CallT;
    
protected:
    _CallT __f_;
    
protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    void invoke_callback(const key&, const action&);
    
public:
    template <std::size_t _I>
    inline void set_callback(callback);
};


template <std::size_t... _Mask>
template <std::size_t _I>
void event_callback<void(typename event::key), _Mask...>
::set_callback(callback __c) {
    __f_[_I] = __c;
}

template <std::size_t... _Mask>
void event_callback<void(typename event::key), _Mask...>
::invoke_callback(const key& __k, const action& __a) {
    for (int i = 0; i < sizeof...(_Mask); i++) {
        if (callback_mask<_Mask...>::value[i] == __a) {
            if (__f_[i] != nullptr) {
                __f_[i](__k);
            }
            break;
        }
    }
}


template <std::size_t... _Mask>
class event_callback<void(typename event::action), _Mask...> {
public:
    typedef event::key                  key;
    typedef event::action               action;
    typedef std::function<void(action)> callback;
protected:
    typedef std::array<callback, sizeof...(_Mask)> _CallT;
    
protected:
    _CallT __f_;
    
protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    void invoke_callback(const key&, const action&);
    
public:
    template <std::size_t _I>
    inline void set_callback(callback);
};


template <std::size_t... _Mask>
template <std::size_t _I>
void event_callback<void(typename event::action), _Mask...>
::set_callback(callback __c) {
    __f_[_I] = __c;
}

template <std::size_t... _Mask>
void event_callback<void(typename event::action), _Mask...>
::invoke_callback(const key& __k, const action& __a) {
    for (int i = 0; i < sizeof...(_Mask); i++) {
        if (callback_mask<_Mask...>::value[i] == __a) {
            if (__f_[i] != nullptr) {
                __f_[i](__a);
            }
            break;
        }
    }
}


template <std::size_t... _Mask>
class event_callback<void(typename event::key,
                          typename event::action), _Mask...> {
public:
    typedef event::key                          key;
    typedef event::action                       action;
    typedef std::function<void(key, action)>    callback;
protected:
    typedef callback _CallT;
    
protected:
    _CallT __f_;
    
protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    inline void invoke_callback(const key&, const action&);
    
public:
    inline void set_callback(callback);
};


template <std::size_t... _Mask>
void event_callback<void(typename event::key,
                         typename event::action), _Mask...>
::set_callback(callback __c) {
    __f_ = __c;
}

template <std::size_t... _Mask>
void event_callback<void(typename event::key,
                         typename event::action), _Mask...>
::invoke_callback(const key& __k, const action& __a) {
    __f_(__k, __a);
}


template <std::size_t... _Mask>
class event_callback<void(typename event::action,
                          typename event::key), _Mask...> {
public:
    typedef event::key                          key;
    typedef event::action                       action;
    typedef std::function<void(action, key)>    callback;
protected:
    typedef callback _CallT;
    
protected:
    _CallT __f_;
    
protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    inline void invoke_callback(const key&, const action&);
    
public:
    inline void set_callback(callback);
};


template <std::size_t... _Mask>
void event_callback<void(typename event::action,
                         typename event::key), _Mask...>
::set_callback(callback __c) {
    __f_ = __c;
}

template <std::size_t... _Mask>
void event_callback<void(typename event::action,
                         typename event::key), _Mask...>
::invoke_callback(const key& __k, const action& __a) {
    __f_(__a, __k);
}


template <std::size_t _Mask>
class event_callback<void(typename event::key), _Mask> {
public:
    typedef event::key              key;
    typedef event::action           action;
    typedef std::function<void(key)> callback;
protected:
    typedef callback                _CallT;
    
protected:
    _CallT __f_;
    
protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    void invoke_callback(const key&, const action&);
    
public:
    inline void set_callback(callback);
};


template <std::size_t _Mask>
void event_callback<void(typename event::key), _Mask>
::set_callback(callback __c) {
    __f_ = __c;
}

template <std::size_t _Mask>
void event_callback<void(typename event::key), _Mask>
::invoke_callback(const key& __k, const action& __a) {
    __f_(__k);
}


template <std::size_t _Mask>
class event_callback<void(typename event::action), _Mask> {
public:
    typedef event::key                  key;
    typedef event::action               action;
    typedef std::function<void(action)> callback;
protected:
    typedef callback                    _CallT;

protected:
    _CallT __f_;

protected:
    event_callback() = default;
    virtual ~event_callback() = default;
    void invoke_callback(const key&, const action&);

public:
    inline void set_callback(callback);
};


template <std::size_t _Mask>
void event_callback<void(typename event::action), _Mask>
::set_callback(callback __c) {
    __f_ = __c;
}

template <std::size_t _Mask>
void event_callback<void(typename event::action), _Mask>
::invoke_callback(const key& __k, const action& __a) {
    __f_(__a);
}

#endif /* __EVENT_CALLBACK_H__ */
