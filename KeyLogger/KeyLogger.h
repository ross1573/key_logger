#ifndef __KEYBOARD_INPUT_H__
#define __KEYBOARD_INPUT_H__

#include <functional>
#include <thread>
#include <array>
#include <assert.h>
#include <iostream>
#include <string>
#include <atomic>

#include "KeyCode.h"

#ifdef  __APPLE__
#include <ApplicationServices/ApplicationServices.h>
typedef __darwin_::__key_code KEY_CODE;
#elif _WIN32
#define NOMINMAX
#include <windows.h>
typedef __win32_::__key_code KEY_CODE;
#endif


class KeyLogger {
    typedef KeyLogger* _Ip;
    typedef KEY_CODE _Key;
    typedef std::function<void(_Key)> _Fp;
    typedef std::array<std::atomic<bool>, KEY_CODE_SIZE> _Arr;
    
private:
    static _Ip instance;
    static _Fp press_callback;
    static _Fp release_callback;
    static _Arr key_state;
    static bool running;
    std::thread thread;
#ifdef __APPLE__
    CFRunLoopRef loop;
#elif _WIN32
    HHOOK hook;
    MSG message;
#endif
    
private:
    KeyLogger();
    ~KeyLogger();

public:
    static _Ip getInstance();
    static void reset(_Key = KEY_CODE::KEY_CODE_NULL);
    
    static _Ip setPressCallback(_Fp);
    static _Ip setReleaseCallback(_Fp);
    _Ip run(bool = true);
    _Ip stop();
    
    static bool isRunning();
    static bool isKeyPressed(_Key);
    static bool isKeyReleased(_Key);
    
    static std::string convertToString(_Key);
    
private:
#ifdef __APPLE__
    void __init_darwin();
    static CGEventRef __callback_darwin(CGEventTapProxy, CGEventType, CGEventRef, void*);
#elif _WIN32
    void __init_win32();
    static LRESULT CALLBACK __callback_win32(int, WPARAM, LPARAM);
#endif
};

#endif /* __KEYBOARD_INPUT_H__ */
