#ifndef _EVENT__LOGGER_H
#define _EVENT__LOGGER_H

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <atomic>

#include "utility.h"
#include "container.h"
#include "callback.h"
#include "string.h"


namespace event {
namespace __detail {

template <std::size_t... _Mask>
class __logger_common;


template <std::size_t... _Mask>
class __logger_base {
private:
    typedef __logger_base<_Mask...>*                _InsP;
    struct __callback_type;
public:
#ifdef _APPL
    typedef CGEventType                             event_type;
    typedef CGEventRef                              event_ref;
    typedef CGEventField                            event_field;
#elif defined _WIN
    typedef int                                     event_field;
    typedef WPARAM                                  event_type;
    typedef LPARAM                                  event_ref;
#endif
#ifdef __RECORD_CHRONO_TIME
    typedef std::chrono::high_resolution_clock      clock_type;
    typedef std::chrono::time_point<clock_type>     time_point;
#elif defined __RECORD_RDTSC_TIME
    typedef volatile std::uint64_t                  time_point;
#endif
    
    typedef __callback_type                         callback;
    typedef typename callback::instance_type        instance_type;
    typedef typename callback::instance_ptr         instance_ptr;
    typedef typename callback::func_type            func_type;
    typedef typename callback::id_type              id_type;
    typedef std::vector<callback>                   callback_vec;
    typedef typename callback_vec::iterator         callback_iter;
    
private:
    struct __callback_type {
        typedef std::size_t                         id_type;
        typedef __logger_common<_Mask...>           instance_type;
        typedef instance_type*                      instance_ptr;
        typedef void (instance_type::*              func_type)
            (const event_type&, const event_ref&);
        
        id_type __id_;
        instance_ptr __i_;
        func_type __f_;
        
        __callback_type() = delete;
        __callback_type(id_type __id, instance_ptr __i, func_type __f)
            : __id_(__id), __i_(__i), __f_(__f) {}
    };
    
    static _InsP __i_;
    callback_vec __v_;
    id_type __c_;

    std::atomic<bool> __s_;
    std::mutex __m_;
    std::thread __t_;
    event_field __f_;
#ifdef _APPL
    CFRunLoopRef __l_;
#elif defined _WIN
    HHOOK __h_;
    MSG __ms;
#endif
#if defined __RECORD_CHRONO_TIME || \
    defined __RECORD_RDTSC_TIME
     time_point __tp = 0;
#endif
    
protected:
    __logger_base();
    ~__logger_base();
    
public:
    void set_field(event_field);
    void remove_callback(const id_type&);
    id_type submit_callback(const instance_ptr&, const func_type&);
    [[nodiscard]] static auto callback_invoked_timepoint();
    
private:
    void __init_();
    void __start_();
    void __stop_();
    
    static void __terminate();
    static constexpr std::uint64_t __create_mask_bit();
    
#ifdef _APPL
    void __init_darwin(int);
    inline static CGEventRef __callback_darwin
        (CGEventTapProxy, CGEventType, CGEventRef, void*);
#elif defined _WIN
    void __init_win32();
    static LRESULT CALLBACK __callback_win32(int, WPARAM, LPARAM);
#endif
    
    friend class __logger_common<_Mask...>;
};


template <std::size_t... _Mask>
typename __logger_base<_Mask...>::_InsP
__logger_base<_Mask...>::__i_ = new __logger_base<_Mask...>();

template <std::size_t... _Mask>
__logger_base<_Mask...>::__logger_base() {
    std::atexit(&__logger_base::__terminate);
}

template <std::size_t... _Mask>
__logger_base<_Mask...>::~__logger_base() {
    __stop_();
}


template <std::size_t... _Mask>
void __logger_base<_Mask...>::__start_() {
    __t_ = std::thread(&__logger_base::__init_, this);
    __t_.detach();
    __s_ = true;
    __c_ = 1;
}

template <std::size_t... _Mask>
typename __logger_base<_Mask...>::id_type
__logger_base<_Mask...>::submit_callback(const instance_ptr& __i,
                                         const func_type& __f)
{
    if (__v_.size() == 0)
        __start_();
    
    __m_.lock();
    __v_.emplace_back(__c_, __i, __f);
    __m_.unlock();
    return __c_++;
}

template <std::size_t... _Mask>
auto
__logger_base<_Mask...>::callback_invoked_timepoint() {
    return __i_->__tp;
}

template <std::size_t... _Mask>
void __logger_base<_Mask...>::remove_callback(const id_type& __i) {
    auto __it = __v_.begin();
    for (; __it != __v_.end(); __it++) {
        if (__it->__id_ == __i)
            break;
    }
    
    if (__it == __v_.end())
        return;
    
    __m_.lock();
    std::swap(*__it, *--__v_.end());
    __v_.erase(--__v_.end());
    __m_.unlock();
    
    if (__v_.empty())
        __stop_();
}

template <std::size_t... _Mask>
void __logger_base<_Mask...>::set_field(event_field __f) {
    __f_ = __f;
}

template <std::size_t... _Mask>
void __logger_base<_Mask...>::__stop_() {
    if (!__s_)
        return;
    
#ifdef _APPL
    CFRunLoopStop(__l_);
#endif
    __s_ = false;
}

template <std::size_t... _Mask>
void __logger_base<_Mask...>::__init_() {
#ifdef _APPL
    __init_darwin(1);
#elif defined _WIN
    __init_win32();
#endif
}

template <std::size_t... _Mask>
void __logger_base<_Mask...>::__terminate() {
    if (__i_ == nullptr)
        return;
    
    delete __i_;
    __i_ = nullptr;
}

template <std::size_t... _Mask>
constexpr std::uint64_t __logger_base<_Mask...>::__create_mask_bit() {
    std::uint64_t __mb = 0;
    for (auto& __e : __event_mask<_Mask...>::value) {
        __mb |= _EVENT_MASK_BIT(__e);
    }
    return __mb;
}

#ifdef _APPL
template <std::size_t... _Mask>
void __logger_base<_Mask...>::__init_darwin(int __attempt) {
    CFMachPortRef __e = CGEventTapCreate(kCGAnnotatedSessionEventTap,
        kCGHeadInsertEventTap, kCGEventTapOptionListenOnly,
        __create_mask_bit(), &__logger_base::__callback_darwin, nullptr);
    
    if (!__e) {
        if (__attempt == 1) {
            fprintf(stderr, "Failed to create event tap\n");
            fprintf(stderr, "Waiting");
        }
        else if (__attempt == _EVENT_CREATE_ATTEMPT) {
            fprintf(stderr, "Timeout, Exiting...\n");
            exit(1);
        }
        
        auto __step = std::chrono::milliseconds(_EVENT_CREATE_STEP);
        std::this_thread::sleep_for(__step);
        fprintf(stderr, ".");
        __init_darwin(++__attempt);
    }
    
    if (__attempt > 1)
        fprintf(stdout, "\nEvent tap created\n");
    
    __l_ = CFRunLoopGetCurrent();
    auto __a = kCFAllocatorDefault;
    
    CFRunLoopSourceRef __s = CFMachPortCreateRunLoopSource(__a, __e, 0);
    CFRunLoopAddSource(__l_, __s, kCFRunLoopDefaultMode);
    CGEventTapEnable(__e, true);
    CFRunLoopRun();
}


template <std::size_t... _Mask>
CGEventRef __logger_base<_Mask...>
::__callback_darwin(CGEventTapProxy, CGEventType __t,
                    CGEventRef __e, void*)
{
#ifdef __RECORD_CHRONO_TIME
    __i_->__tp = std::chrono::high_resolution_clock::now();
#elif defined __RECORD_RDTSC_TIME
    __i_->__tp = __rdtsc();
#endif
    __i_->__m_.lock();
    for (const callback& __c : __i_->__v_) {
        (__c.__i_->*__c.__f_)(__t, __e);
    }
    __i_->__m_.unlock();
    return __e;
}

#elif defined _WIN
template <std::size_t... _Mask>
void __logger_base<_Mask...>::__init_win32() {
    __h_ = SetWindowsHookEx(__f_, &__logger_base
                            ::__callback_win32, nullptr, 0);
    
    if (!__h_) {
        std::cout << "Failed to create hook : " << GetLastError();
        return;
    }
    while (!GetMessage(&__ms, nullptr, 0, 0) && __s_) {
        TranslateMessage(&__ms);
        DispatchMessage(&__ms);
    }
    UnhookWindowsHookEx(__h_);
}

template <std::size_t... _Mask>
LRESULT CALLBACK __logger_base<_Mask...>
::__callback_win32(int __c, WPARAM __w, LPARAM __l) {
#ifdef __RECORD_CHRONO_TIME
    __i_->__tp = std::chrono::high_resolution_clock::now();
#elif defined __RECORD_RDTSC_TIME
    __i_->__tp = __rdtsc();
#endif

    if (__c != HC_ACTION)
        return CallNextHookEx(nullptr, __c, __w, __l);
    
    if (__create_mask_bit() & _EVENT_MASK_BIT(__w)) {
        __i_->__m_.lock();
        for (auto& __c : __i_->__v_) {
            (__c.__i_->*__c.__f_)(__w, __l);
        }
        __i_->__m_.unlock();
    }
    return CallNextHookEx(nullptr, __c, __w, __l);
}
#endif


template <std::size_t... _Mask>
class __logger_common {
private:
    typedef __logger_base<_Mask...>             _Base;
    typedef __logger_common                     _This;
public:
    typedef typename _Base::callback::id_type   id_type;
    typedef typename _Base::event_field         event_field;
    typedef typename _Base::event_type          event_type;
    typedef typename _Base::event_ref           event_ref;
    typedef _Base                               logger_type;
    
protected:
    id_type __id_;
    bool __s_;
    
protected:
    __logger_common() = default;
    __logger_common(const _This&) = delete;
    __logger_common(_This&&) = delete;
    _This& operator=(const _This&) = delete;
    _This& operator=(_This&&) = delete;
    virtual ~__logger_common();
    
public:
    void start();
    void stop();
    void restart();
    [[nodiscard]] inline bool is_running();
    
protected:
    void __set_field(event_field);
    virtual void __callback_(const event_type&, const event_ref&) = 0;
};


template <std::size_t... _Mask>
__logger_common<_Mask...>::~__logger_common() {
    stop();
}

template <std::size_t... _Mask>
void __logger_common<_Mask...>::start() {
    __id_ = _Base::__i_->submit_callback(this,
        &__logger_common<_Mask...>::__callback_);
    __s_ = true;
}

template <std::size_t... _Mask>
void __logger_common<_Mask...>::stop() {
    if (__s_) {
        _Base::__i_->remove_callback(__id_);
        __s_ = false;
    }
}

template <std::size_t... _Mask>
void __logger_common<_Mask...>::restart() {
    
}

template <std::size_t... _Mask>
bool __logger_common<_Mask...>::is_running() {
    return __s_;
}

template <std::size_t... _Mask>
void __logger_common<_Mask...>::__set_field(event_field __f) {
    _Base::__i_->set_field(__f);
}


struct __event_default_type {
    typedef __logger_base<>::event_type key_type;
    typedef __logger_base<>::event_ref  action_type;
    typedef void(*func_type)(const key_type&, const action_type&);
};

} /* __detail */


template <std::size_t... _Mask>
class logger
: public __detail::__logger_common<_Mask...>,
  public callback<typename __detail::__event_default_type::func_type,
                  __detail::__event_default_type> {
private:
    typedef typename __detail::__event_default_type::func_type  _FunctionT;
    typedef __detail::__event_default_type                      _Type;
    typedef __detail::__logger_common<_Mask...>                 _Base;
    typedef callback<_FunctionT, _Type>                         _Callback;
public:
    typedef typename _Base::event_field                         event_field;
    typedef typename _Base::event_type                          event_type;
    typedef typename _Base::event_ref                           event_ref;
    typedef typename _Base::logger_type                         logger_type;
    
public:
    logger() = default;
    virtual ~logger() = default;
    
private:
    void __callback_(const event_type&, const event_ref&) final;
};

template <std::size_t... _Mask>
void logger<_Mask...>
::__callback_(const event_type& __t, const event_ref& __r) {
    _Callback::__invoke_(__t, __r);
}

}
#endif /* _EVENT__LOGGER_H */
