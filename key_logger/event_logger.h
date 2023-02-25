#ifndef __EVENT_LOGGER_H__
#define __EVENT_LOGGER_H__

#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

#include "event.h"


namespace event {

template <std::size_t... _Mask>
struct callback_mask {
    static const constexpr std::array<std::size_t, sizeof...(_Mask)> value{ _Mask... };
};


template <typename _Key, typename _Act, std::size_t... _Mask>
class __logger_base {
protected:
    typedef __logger_base<_Key, _Act, _Mask...>* _InsP;
public:
#ifdef _APPL
    typedef CGEventType                     event_type;
    typedef CGEventRef                      event_ref;
    typedef CGEventField                    event_field;
#elif defined _WIN
    typedef int                             event_field;
    typedef WPARAM                          event_type;
    typedef LPARAM                          event_ref;
#endif
    typedef std::function<void(const event_type&, const event_ref&)> callback;
    typedef std::size_t                     id_type;
    typedef std::vector<id_type>            id_vec;
    typedef std::pair<id_type, callback>    callback_id_pair;
    typedef std::vector<callback_id_pair>   callback_vec;
    
private:
    static _InsP __i_;
    callback_vec __v_;
    id_type __c_;
    id_vec __d_;
    
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
    __logger_base() = default;
    ~__logger_base();
    
public:
    static _InsP get_instance();
    id_type submit_callback(const callback&);
    void remove_callback(const id_type&);
    void set_field(event_field);
    
private:
    void __init_();
    void __start_();
    void __stop_();
    void __remove_callback();
    
    static const constexpr long long __create_mask_bit();
    
#ifdef _APPL
    void __init_darwin();
    static CGEventRef __callback_darwin(CGEventTapProxy, CGEventType, CGEventRef, void*);
#elif defined _WIN
    void __init_win32();
    static LRESULT CALLBACK __callback_win32(int, WPARAM, LPARAM);
#endif
};


template <typename _Key, typename _Act, std::size_t... _Mask>
typename __logger_base<_Key, _Act, _Mask...>::_InsP
__logger_base<_Key, _Act, _Mask...>::__i_ = nullptr;

template <typename _Key, typename _Act, std::size_t... _Mask>
__logger_base<_Key, _Act, _Mask...>::~__logger_base<_Key, _Act, _Mask...>() {
    __stop_();
}

template <typename _Key, typename _Act, std::size_t... _Mask>
typename __logger_base<_Key, _Act, _Mask...>::_InsP
__logger_base<_Key, _Act, _Mask...>::get_instance() {
    if (__i_ == nullptr) {
        __i_ = new __logger_base<_Key, _Act, _Mask...>();
        __i_->__start_();
    }
    return __i_;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::__start_() {
    __t_ = std::thread(&__logger_base::__init_, this);
    __t_.detach();
    __s_ = true;
    __c_ = 1;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
typename __logger_base<_Key, _Act, _Mask...>::id_type
__logger_base<_Key, _Act, _Mask...>::submit_callback(const callback& __c) {
    __v_.push_back(std::make_pair(__c_, std::move(__c)));
    return __c_++;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::remove_callback(const id_type& __i) {
    __d_.push_back(__i);
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::__remove_callback() {
    if (__d_.empty()) return;
    
    std::sort(__d_.begin(), __d_.end());
    auto __v_i = __v_.begin();
    auto __d_i = __d_.begin();
    
    for (; __v_i != __v_.end(); __v_i++) {
        if (__v_i->first == *__d_i) {
            __v_.erase(__v_i);
            if (__d_i++ == __d_.end())
                return;
        }
    }
    
    __d_.clear();
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::set_field(event_field __f) {
    __f_ = __f;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::__stop_() {
    if (!__s_) return;
#ifdef _APPL
    CFRunLoopStop(__l_);
#endif
    __s_ = false;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::__init_() {
#ifdef _APPL
    __init_darwin();
#elif defined _WIN
    __init_win32();
#endif
}

template <typename _Key, typename _Act, std::size_t... _Mask>
const constexpr long long __logger_base<_Key, _Act, _Mask...>::__create_mask_bit() {
    long long __mb = 0;
    for (auto& __e : callback_mask<_Mask...>::value) {
        __mb |= _EVENT_MASK_BIT(__e);
    }
    return __mb;
}

#ifdef _APPL
template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::__init_darwin() {
    CFMachPortRef __e = CGEventTapCreate(kCGSessionEventTap,
                                         kCGHeadInsertEventTap,
                                         kCGEventTapOptionDefault,
                                         __create_mask_bit(),
                                         &__logger_base::__callback_darwin,
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
CGEventRef __logger_base<_Key, _Act, _Mask...>
::__callback_darwin(CGEventTapProxy __p, CGEventType __t, CGEventRef __e, void*) {
    for (const callback_id_pair& __c : __i_->__v_) {
        __c.second(__t, __e);
    }
    __i_->__remove_callback();
    return __e;
}

#elif defined _WIN
template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger_base<_Key, _Act, _Mask...>::__init_win32() {
    __h_ = SetWindowsHookEx(__f_, &__logger_base::__callback_win32, nullptr, 0);
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
LRESULT CALLBACK __logger_base<_Key, _Act, _Mask...>
::__callback_win32(int __c, WPARAM __w, LPARAM __l) {
    if (__c != HC_ACTION)
        return CallNextHookEx(NULL, __c, __w, __l);

    if (__create_mask_bit() & _EVENT_MASK_BIT(__w)) {
        for (auto& __c : __i_->__v_) {
            __c.second(__w, __l);
        }
    }
    
    __i_->__remove_callback();
    return CallNextHookEx(NULL, __c, __w, __l);
}
#endif


template <typename _Key, typename _Act, std::size_t... _Mask>
class __logger {
private:
    typedef __logger_base<_Key, _Act, _Mask...> _Base;
protected:
    typedef typename _Base::callback            callback;
    typedef typename _Base::id_type             id_type;
    typedef typename _Base::event_field         event_field;
    typedef typename _Base::event_type          event_type;
    typedef typename _Base::event_ref           event_ref;
    
private:
    id_type __i_;
    
protected:
    bool __s_;
    
protected:
    __logger() = default;
    virtual ~__logger() = default;
    
public:
    void start();
    void stop();
    
protected:
    void __set_field(event_field);
    virtual void __callback_(const event_type&, const event_ref&) = 0;
};


template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger<_Key, _Act, _Mask...>::start() {
    callback __f = std::bind(&__logger<_Key, _Act, _Mask...>::__callback_,
                             this,
                             std::placeholders::_1,
                             std::placeholders::_2);
    __i_ = _Base::get_instance()->submit_callback(std::move(__f));
    __s_ = true;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger<_Key, _Act, _Mask...>::stop() {
    _Base::get_instance()->remove_callback(__i_);
    __s_ = false;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __logger<_Key, _Act, _Mask...>::__set_field(event_field __f) {
    _Base::get_instance()->set_field(__f);
}


template <typename _Key, typename _Act, std::size_t... _Mask>
class logger : public __logger<_Key, _Act, _Mask...> {
private:
    typedef __logger<_Key, _Act, _Mask...> _Base;
public:
    typedef typename _Base::callback callback;
    
private:
    callback __c_;
    
public:
    logger() = default;
    virtual ~logger() = default;
    
    void set_callback(callback);
    
private:
    void __callback_(const _Key&, const _Act&) final;
};


template <typename _Key, typename _Act, std::size_t... _Mask>
void logger<_Key, _Act, _Mask...>::__callback_(const _Key& __k, const _Act& __a) {
    __c_(__k, __a);
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void logger<_Key, _Act, _Mask...>::set_callback(callback __c) {
    __c_ = __c;
}

}
#endif /* __EVENT_LOGGER_H__ */
