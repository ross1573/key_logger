#include "KeyLogger.h"


KeyLogger::_Ip KeyLogger::instance = nullptr;
KeyLogger::_Fp KeyLogger::press_callback = nullptr;
KeyLogger::_Fp KeyLogger::release_callback = nullptr;
KeyLogger::_Arr KeyLogger::key_state;
bool KeyLogger::running = false;


KeyLogger::KeyLogger() {
    assert(!instance);
    for (auto& key : key_state) {
        key.store(false);
    }
}

KeyLogger::~KeyLogger() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

KeyLogger::_Ip KeyLogger::getInstance() {
    if (!instance) instance = new KeyLogger();
    return instance;
}

void KeyLogger::reset(_Key key) {
    if (key == KEY_CODE::KEY_CODE_NULL) {
        for (auto& key : key_state)
            key.store(false);
    }
    else key_state[key] = false;
}


KeyLogger::_Ip KeyLogger::run(bool detach) {
#ifdef __APPLE__
    thread = std::thread(&KeyLogger::__init_darwin, instance);
#elif _WIN32
    thread = std::thread(&KeyLogger::__init_win32, instance);
#endif
    running = true;
    if (detach) thread.detach();
    else thread.join();
    return instance;
}

KeyLogger::_Ip KeyLogger::setPressCallback(_Fp press_callback) {
    getInstance()->press_callback = press_callback;
    return instance;
}

KeyLogger::_Ip KeyLogger::setReleaseCallback(_Fp release_callback) {
    getInstance()->release_callback = release_callback;
    return instance;
}


KeyLogger::_Ip KeyLogger::stop() {
    if (!running) return instance;
#ifdef __APPLE__
    CFRunLoopStop(loop);
#endif
    running = false;
    return instance;
}


bool KeyLogger::isRunning() {
    return running;
}

bool KeyLogger::isKeyPressed(_Key key) {
    return key_state[key];
}

bool KeyLogger::isKeyReleased(_Key key) {
    return !key_state[key];
}

std::string KeyLogger::convertToString(_Key key) {
    switch (key) {
        case KEY_CODE::KEY_CODE_0: return "0";
        case KEY_CODE::KEY_CODE_1: return "1";
        case KEY_CODE::KEY_CODE_2: return "2";
        case KEY_CODE::KEY_CODE_3: return "3";
        case KEY_CODE::KEY_CODE_4: return "4";
        case KEY_CODE::KEY_CODE_5: return "5";
        case KEY_CODE::KEY_CODE_6: return "6";
        case KEY_CODE::KEY_CODE_7: return "7";
        case KEY_CODE::KEY_CODE_8: return "8";
        case KEY_CODE::KEY_CODE_9: return "9";
        case KEY_CODE::KEY_CODE_A: return "A";
        case KEY_CODE::KEY_CODE_B: return "B";
        case KEY_CODE::KEY_CODE_C: return "C";
        case KEY_CODE::KEY_CODE_D: return "D";
        case KEY_CODE::KEY_CODE_E: return "E";
        case KEY_CODE::KEY_CODE_F: return "F";
        case KEY_CODE::KEY_CODE_G: return "G";
        case KEY_CODE::KEY_CODE_H: return "H";
        case KEY_CODE::KEY_CODE_I: return "I";
        case KEY_CODE::KEY_CODE_J: return "J";
        case KEY_CODE::KEY_CODE_K: return "K";
        case KEY_CODE::KEY_CODE_L: return "L";
        case KEY_CODE::KEY_CODE_M: return "M";
        case KEY_CODE::KEY_CODE_N: return "N";
        case KEY_CODE::KEY_CODE_O: return "O";
        case KEY_CODE::KEY_CODE_P: return "P";
        case KEY_CODE::KEY_CODE_Q: return "Q";
        case KEY_CODE::KEY_CODE_R: return "R";
        case KEY_CODE::KEY_CODE_S: return "S";
        case KEY_CODE::KEY_CODE_T: return "T";
        case KEY_CODE::KEY_CODE_U: return "U";
        case KEY_CODE::KEY_CODE_V: return "V";
        case KEY_CODE::KEY_CODE_W: return "W";
        case KEY_CODE::KEY_CODE_X: return "X";
        case KEY_CODE::KEY_CODE_Y: return "Y";
        case KEY_CODE::KEY_CODE_Z: return "Z";
            
        case KEY_CODE::KEY_CODE_UP:     return "UP";
        case KEY_CODE::KEY_CODE_DOWN:   return "DOWN";
        case KEY_CODE::KEY_CODE_LEFT:   return "LEFT";
        case KEY_CODE::KEY_CODE_RIGHT:  return "RIGHT";
        case KEY_CODE::KEY_CODE_TAB:    return "TAB";
        case KEY_CODE::KEY_CODE_ENTER:  return "ENTER";
        case KEY_CODE::KEY_CODE_ESCAPE: return "ESCAPE";
            
#ifdef __APPLE__
        case KEY_CODE::KEY_CODE_CLEAR:  return "CLEAR";
        case KEY_CODE::KEY_CODE_SPACE:  return "SPACE";
        case KEY_CODE::KEY_CODE_DELETE: return "DELETE";
        case KEY_CODE::KEY_CODE_RETURN: return "RETURN";
#elif _WIN32
        case KEY_CODE::KEY_CODE_BACKSPACE:  return "BACKSPACE";
        case KEY_CODE::KEY_CODE_SHIFT:      return "SHIFT";
        case KEY_CODE::KEY_CODE_CTRL:       return "CTRL";
        case KEY_CODE::KEY_CODE_ALT:        return "ALT";
#endif
        default: break;
    }
    return "UNKNOWN";
}


#ifdef __APPLE__
void KeyLogger::__init_darwin() {
    CFMachPortRef event = CGEventTapCreate(kCGSessionEventTap,
                                         kCGHeadInsertEventTap,
                                         kCGEventTapOptionDefault,
                                         CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp),
                                         &KeyLogger::__callback_darwin,
                                         NULL);
    
    if (!event) {
        fprintf(stderr, "Failed to create event tap\n");
        exit(1);
    }
    
    loop = CFRunLoopGetCurrent();
    CFRunLoopSourceRef loop_src = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event, 0);
    CFRunLoopAddSource(loop, loop_src, kCFRunLoopCommonModes);
    CGEventTapEnable(event, true);
    CFRunLoopRun();
}

CGEventRef KeyLogger::__callback_darwin(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void*) {
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp)) return event;
    CGKeyCode key_code = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    if (type == kCGEventKeyDown) {
        key_state[key_code] = true;
        if (press_callback)
            press_callback((KEY_CODE)key_code);
    }
    else if (type == kCGEventKeyUp) {
        key_state[key_code] = false;
        if (release_callback)
            release_callback((KEY_CODE)key_code);
    }
    return event;
}

#elif _WIN32
void KeyLogger::__init_win32() {
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, &KeyLogger::__callback_win32, NULL, 0);
    if (!hook) {
        std::cout << "Failed to create hook : " << GetLastError();
        return;
    }
    while (!GetMessage(&message, NULL, 0, 0) && running) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    UnhookWindowsHookEx(hook);
}

LRESULT CALLBACK KeyLogger::__callback_win32(int c, WPARAM w, LPARAM l) {
    if (c != HC_ACTION) return CallNextHookEx(NULL, c, w, l);
    auto key = ((PKBDLLHOOKSTRUCT)l)->vkCode;
    if (w == WM_KEYDOWN) {
        key_state[key] = true;
        if (press_callback)
            press_callback((KEY_CODE)key);
    }
    else if (w == WM_KEYUP) {
        key_state[key] = false;
        if (release_callback)
            release_callback((KEY_CODE)key);
    }
    
    return CallNextHookEx(NULL, c, w, l);
}
#endif
