#ifndef _EVENT__H
#define _EVENT__H

#ifdef  __APPLE__
#   define _APPL
#   define _EVENT_MASK_BIT(x) CGEventMaskBit(x)
#   include <ApplicationServices/ApplicationServices.h>

#   ifndef _EVENT_CREATE_ATTEMPT
#       define _EVENT_CREATE_ATTEMPT 30
#   endif
#   ifndef _EVENT_CREATE_STEP
#       define _EVENT_CREATE_STEP 1000
#   endif

#elif _WIN32
#   define _WIN
#   define _EVENT_MASK_BIT(x) ((__int64)1 << (x & 0x00FF))
#   define NOMINMAX
#   include <windows.h>
#endif


#ifdef _APPL
namespace event {
namespace action {
enum code {
    key_down        = kCGEventKeyDown,
    key_up          = kCGEventKeyUp,
    key_system      = kCGEventFlagsChanged
};
} /* action */

namespace key {
enum code {
    a                       =   0,
    s                       =   1,
    d                       =   2,
    f                       =   3,
    h                       =   4,
    g                       =   5,
    z                       =   6,
    x                       =   7,
    c                       =   8,
    v                       =   9,
    b                       =  11,
    q                       =  12,
    w                       =  13,
    e                       =  14,
    r                       =  15,
    y                       =  16,
    t                       =  17,
    _1                      =  18,
    _2                      =  19,
    _3                      =  20,
    _4                      =  21,
    _6                      =  22,
    _5                      =  23,
    equal                   =  24,
    _9                      =  25,
    _7                      =  26,
    minus                   =  27,
    _8                      =  28,
    _0                      =  29,
    right_square_bracket    =  30,
    o                       =  31,
    u                       =  32,
    left_square_bracket     =  33,
    i                       =  34,
    p                       =  35,
    enter                   =  36,
    l                       =  37,
    j                       =  38,
    quote                   =  39,
    k                       =  40,
    semi                    =  41,
    backslash               =  42,
    comma                   =  43,
    slash                   =  44,
    n                       =  45,
    m                       =  46,
    dot                     =  47,
    tab                     =  48,
    space                   =  49,
    tilde                   =  50,
    backspace               =  51,
    escape                  =  53,
    right_command           =  54,
    left_command            =  55,
    left_shift              =  56,
    capslock                =  57,
    left_option             =  58,
    left_control            =  59,
    right_shift             =  60,
    right_option            =  61,
    right_control           =  62,
    clear                   =  71,
    f5                      =  96,
    f6                      =  97,
    f7                      =  98,
    f3                      =  99,
    f8                      = 100,
    f9                      = 101,
    f11                     = 103,
    f13                     = 105,
    f16                     = 106,
    f14                     = 107,
    f10                     = 109,
    f12                     = 111,
    f15                     = 113,
    insert                  = 114,
    home                    = 115,
    del                     = 117,
    f4                      = 118,
    end                     = 119,
    f2                      = 120,
    f1                      = 122,
    left                    = 123,
    right                   = 124,
    down                    = 125,
    up                      = 126,
    function                = 179,
    null                    =  -1,
    code_size               = 255
};

} /* key */
/* _APPL */


#elif defined _WIN
namespace event {
namespace action {
enum code {
    key_down        = WM_KEYDOWN,
    key_up          = WM_KEYUP,
    key_system      = WM_SYSKEYDOWN
};
} /* action */

namespace key {
enum code {
    backspace               =   8,
    tab                     =   9,
    enter                   =  13,
    shift                   =  16,
    ctrl                    =  17,
    alt                     =  18,
    escape                  =  27,
    space                   =  32,
    left                    =  37,
    up                      =  38,
    right                   =  39,
    down                    =  40,
    _0                      =  48,
    _1                      =  49,
    _2                      =  50,
    _3                      =  51,
    _4                      =  52,
    _5                      =  53,
    _6                      =  54,
    _7                      =  55,
    _8                      =  56,
    _9                      =  57,
    a                       =  65,
    b                       =  66,
    c                       =  67,
    d                       =  68,
    e                       =  69,
    f                       =  70,
    g                       =  71,
    h                       =  72,
    i                       =  73,
    j                       =  74,
    k                       =  75,
    l                       =  76,
    m                       =  77,
    n                       =  78,
    o                       =  79,
    p                       =  80,
    q                       =  81,
    r                       =  82,
    s                       =  83,
    t                       =  84,
    u                       =  85,
    v                       =  86,
    w                       =  87,
    x                       =  88,
    y                       =  89,
    z                       =  90,
    left_window             =  91,
    right_window            =  92,
    left_shift              = 160,
    right_shift             = 161,
    left_control            = 162,
    semi                    = 186,
    equal                   = 187,
    comma                   = 188,
    minus                   = 189,
    dot                     = 190,
    slash                   = 191,
    left_square_bracket     = 219,
    backslash               = 220,
    right_square_bracket    = 221,
    quote                   = 222,
    null                    =  -1,
    code_size               = 255
};

} /* key */
#endif /* _WIN32 */

} /* event */
#endif /* _EVENT__H */
