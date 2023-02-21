#ifndef __EVENT_LOGGER_H__
#define __EVENT_LOGGER_H__

#include <thread>
#include <iostream>
#include <vector>

#include "key_code.h"


template <std::size_t... _Mask>
struct callback_mask {
    static const constexpr std::array<std::size_t, sizeof...(_Mask)> value{ _Mask... };
};


template <typename _Key, typename _Act, std::size_t... _Mask>
class __event_logger_base {
protected:
    typedef __event_logger_base<_Key, _Act, _Mask...>*                  _InsP;
public:
#ifdef _APPL
    typedef CGEventType                                                 event_type;
    typedef CGEventRef                                                  event_ref;
    typedef CGEventField                                                event_field;
#elif defined _WIN
    typedef int                                                         event_field;
    typedef WPARAM                                                      event_type;
    typedef LPARAM                                                      event_ref;
#endif
    typedef std::function<void(const event_type&, const event_ref&)>    callback;
    typedef std::vector<callback>                                       callback_vec;
    typedef typename callback_vec::iterator                             iterator;
    
private:
    static _InsP __i_;
    callback_vec __v_;
    
protected:
    bool __s_;
    std::thread __t_;
    event_field __f_;
#ifdef _APPL
    CFRunLoopRef __l_;
#elif defined _WIN
    HHOOK __h_;
    MSG __m_;
#endif
    
protected:
    __event_logger_base() = default;
    ~__event_logger_base();
    
public:
    static _InsP get_instance();
    iterator submit_callback(callback&);
    void remove_callback(iterator&);
    void set_event_field(event_field);
    
private:
    void __init_();
    void __start_();
    void __stop_();
    
#ifdef _APPL
    constexpr long long __create_mask_bit() const;
    void __init_darwin();
    static CGEventRef __callback_darwin(CGEventTapProxy, CGEventType, CGEventRef, void*);
#elif defined _WIN
    void __init_win32();
    static LRESULT CALLBACK __callback_win32(int, WPARAM, LPARAM);
#endif
};


template <typename _Key, typename _Act, std::size_t... _Mask>
typename __event_logger_base<_Key, _Act, _Mask...>::_InsP
__event_logger_base<_Key, _Act, _Mask...>::__i_ = nullptr;

template <typename _Key, typename _Act, std::size_t... _Mask>
__event_logger_base<_Key, _Act, _Mask...>::~__event_logger_base<_Key, _Act, _Mask...>() {
    __stop_();
}

template <typename _Key, typename _Act, std::size_t... _Mask>
typename __event_logger_base<_Key, _Act, _Mask...>::_InsP
__event_logger_base<_Key, _Act, _Mask...>::get_instance() {
    if (__i_ == nullptr) {
        __i_ = new __event_logger_base<_Key, _Act, _Mask...>();
        __i_->__start_();
    }
    return __i_;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger_base<_Key, _Act, _Mask...>::__start_() {
    __t_ = std::thread(&__event_logger_base::__init_, this);
    __t_.detach();
    __s_ = true;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
typename __event_logger_base<_Key, _Act, _Mask...>::iterator
__event_logger_base<_Key, _Act, _Mask...>::submit_callback(callback& __c) {
    __v_.push_back(__c);
    return __v_.end()--;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger_base<_Key, _Act, _Mask...>::remove_callback(iterator& __i) {
    __v_.erase(__i);
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger_base<_Key, _Act, _Mask...>::set_event_field(event_field __f) {
    __f_ = __f;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger_base<_Key, _Act, _Mask...>::__stop_() {
    if (!__s_) return;
#ifdef _APPL
    CFRunLoopStop(__l_);
#endif
    __s_ = false;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger_base<_Key, _Act, _Mask...>::__init_() {
#ifdef _APPL
    __init_darwin();
#elif defined _WIN
    __init_win32();
#endif
}

#ifdef _APPL
template <typename _Key, typename _Act, std::size_t... _Mask>
constexpr long long __event_logger_base<_Key, _Act, _Mask...>::__create_mask_bit() const {
    long long __mb = sizeof...(_Mask) == 0 ? 0 : CGEventMaskBit(kCGEventFlagsChanged);
    for (auto& e : callback_mask<_Mask...>::value) {
        __mb |= CGEventMaskBit(e);
    }
    return __mb;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger_base<_Key, _Act, _Mask...>::__init_darwin() {
    CFMachPortRef __e = CGEventTapCreate(kCGSessionEventTap,
                                         kCGHeadInsertEventTap,
                                         kCGEventTapOptionDefault,
                                         __create_mask_bit(),
                                         &__event_logger_base::__callback_darwin,
                                         nullptr);
    
    if (!__e) {
        fprintf(stderr, "Failed to create event tap\n");
        exit(1);
    }
    
    __l_ = CFRunLoopGetCurrent();
    CFRunLoopSourceRef __s = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, __e, 0);
    CFRunLoopAddSource(__l_, __s, kCFRunLoopCommonModes);
    CGEventTapEnable(__e, true);
    CFRunLoopRun();
}


template <typename _Key, typename _Act, std::size_t... _Mask>
CGEventRef __event_logger_base<_Key, _Act, _Mask...>
::__callback_darwin(CGEventTapProxy __p, CGEventType __t, CGEventRef __e, void*) {
    for (auto& __c : __i_->__v_) {
        __c(__t, __e);
    }
    return __e;
}

#elif defined _WIN
template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger_base<_Key, _Act, _Mask...>::__init_win32() {
    __h_ = SetWindowsHookEx(__f_, &__event_logger_base::__callback_win32, nullptr, 0);
    if (!__h_) {
        std::cout << "Failed to create hook : " << GetLastError();
        return;
    }
    while (!GetMessage(&__m_, NULL, 0, 0) && __s_) {
        TranslateMessage(&__m_);
        DispatchMessage(&__m_);
    }
    UnhookWindowsHookEx(__h_);
}

template <typename _Key, typename _Act, std::size_t... _Mask>
LRESULT CALLBACK __event_logger_base<_Key, _Act, _Mask...>
::__callback_win32(int __c, WPARAM __w, LPARAM __l) {
    if (__c != HC_ACTION) return CallNextHookEx(NULL, __c, __w, __l); 
    _Act __a = (_Act)__w;
    bool __f = false;
    for (auto& __e : callback_mask<_Mask...>::value) {
        if (__a == __e) {
            __f = true;
            break;
        }
    }
    if (!__f) return CallNextHookEx(NULL, __c, __w, __l);

    for (auto& __c : __i_->__v_) {
        __c(__w, __l);
    }
    return CallNextHookEx(NULL, __c, __w, __l);
}
#endif


template <typename _Key, typename _Act, std::size_t... _Mask>
class __event_logger {
private:
    typedef __event_logger_base<_Key, _Act, _Mask...>   _Base;
protected:
    typedef typename _Base::callback                    callback;
    typedef typename _Base::callback_vec::iterator      iterator;
    typedef typename _Base::event_field                 event_field;
    typedef typename _Base::event_type                  event_type;
    typedef typename _Base::event_ref                   event_ref;
    
private:
    bool __s_;
    iterator __i_;
    
protected:
    __event_logger() = default;
    virtual ~__event_logger() = default;
    
public:
    void start();
    void stop();
    
protected:
    void __set_event_field(event_field);
    virtual void __event_callback(const event_type&, const event_ref&) = 0;
};


template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::start() {
    callback __f = std::bind(&__event_logger<_Key, _Act, _Mask...>::__event_callback,
                             this,
                             std::placeholders::_1,
                             std::placeholders::_2);
    __i_ = _Base::get_instance()->submit_callback(__f);
    __s_ = true;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::stop() {
    _Base::get_instance()->remove_callback(__i_);
    __s_ = false;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::__set_event_field(event_field __f) {
    _Base::get_instance()->set_event_field(__f);
}


template <typename _Key, typename _Act, std::size_t... _Mask>
class event_logger : public __event_logger<_Key, _Act, _Mask...> {
private:
    typedef __event_logger<_Key, _Act, _Mask...> _Base;
public:
    typedef typename _Base::callback callback;
    
private:
    callback __c_;
    
public:
    event_logger() = default;
    virtual ~event_logger() = default;
    
    void set_callback(callback);
    
private:
    void __event_logger_callback(const _Key&, const _Act&) final;
};


template <typename _Key, typename _Act, std::size_t... _Mask>
void event_logger<_Key, _Act, _Mask...>::__event_logger_callback(const _Key& __k, const _Act& __a) {
    __c_(__k, __a);
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void event_logger<_Key, _Act, _Mask...>::set_callback(callback __c) {
    __c_ = __c;
}

#endif /* __EVENT_LOGGER_H__ */
