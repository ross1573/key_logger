#ifndef __EVENT_LOGGER_H__
#define __EVENT_LOGGER_H__

#include <thread>
#include <iostream>

#include "key_code.h"


template <std::size_t... _Mask>
struct callback_mask {
    static const constexpr std::array<std::size_t, sizeof...(_Mask)> value{ _Mask... };
};


template <typename _Key, typename _Act, std::size_t... _Mask>
class __event_logger {
protected:
    bool __s_;
    std::thread __t_;
#ifdef _APPL
    CFRunLoopRef __l_;
#elif defined _WIN
    HHOOK __h_;
    MSG __m_;
#endif
    
protected:
    __event_logger() = default;
    virtual ~__event_logger() = default;
    
public:
    void start(bool = true);
    void restart(bool = true);
    void stop();
    
protected:
    virtual void __event_logger_callback(const _Key&, const _Act&) = 0;
    
private:
    void __init_();
    
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
void __event_logger<_Key, _Act, _Mask...>::start(bool __d) {
    __t_ = std::thread(&__event_logger::__init_, this);
    __s_ = true;
    if (__d) {
        __t_.detach();
    }
    else {
        __t_.join();
    }
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::stop() {
    if (!__s_) return;
#ifdef _APPL
    CFRunLoopStop(__l_);
#endif
    __s_ = false;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::restart(bool __d) {
    stop();
    start(__d);
}


template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::__init_() {
#ifdef _APPL
    __init_darwin();
#elif defined _WIN
    __init_win32();
#endif
}

#ifdef _APPL
template <typename _Key, typename _Act, std::size_t... _Mask>
constexpr long long __event_logger<_Key, _Act, _Mask...>::__create_mask_bit() const {
    long long __mb = 0;
    for (auto& e : callback_mask<_Mask...>::value) {
        __mb |= CGEventMaskBit(e);
    }
    return __mb;
}

template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::__init_darwin() {
    CFMachPortRef __e = CGEventTapCreate(kCGSessionEventTap,
                                         kCGHeadInsertEventTap,
                                         kCGEventTapOptionDefault,
                                         __create_mask_bit(),
                                         &__event_logger::__callback_darwin,
                                         this);
    
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
CGEventRef __event_logger<_Key, _Act, _Mask...>::__callback_darwin(CGEventTapProxy __p, CGEventType __t, CGEventRef __e, void* __i) {
    CGKeyCode __kc = (CGKeyCode)CGEventGetIntegerValueField(__e, kCGKeyboardEventKeycode);
    const _Key __k = (_Key)__kc;
    const _Act __a = (_Act)__t;
    ((__event_logger*)__i)->__event_logger_callback(__k, __a);
    return __e;
}

#elif defined _WIN
template <typename _Key, typename _Act, std::size_t... _Mask>
void __event_logger<_Key, _Act, _Mask...>::__init_win32() {
    __h_ = SetWindowsHookEx(WH_KEYBOARD_LL, &__event_logger::__callback_win32, nullptr, 0);
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
LRESULT CALLBACK __event_logger<_Key, _Act, _Mask...>::__callback_win32(int __c, WPARAM __w, LPARAM __l) {
    if (__c != HC_ACTION) return CallNextHookEx(NULL, __c, __w, __l);
    _Key __k = (_Key)((PKBDLLHOOKSTRUCT)__l)->vkCode;
    _Act __a = (_Act)__w;
    //__event_logger_callback(__k, __a);
    return CallNextHookEx(NULL, __c, __w, __l);
}
#endif


template <typename _Key, typename _Act, std::size_t... _Mask>
class event_logger : public __event_logger<_Key, _Act, _Mask...> {
public:
    typedef std::function<void(_Key, _Act)> callback;
    
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
