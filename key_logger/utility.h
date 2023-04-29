#ifndef _EVENT__UTILITY_H
#define _EVENT__UTILITY_H

#include <type_traits>
#include <utility>

#include "event.h"


namespace event {
namespace __detail {

template <typename...>
struct __null_type {};

template <typename _T>
struct __is_std_function : std::false_type {};

template <typename _T>
struct __is_std_function<std::function<_T>> : std::true_type {};

template <typename _T>
struct __is_std_tuple : std::false_type {};

template <typename... _T>
struct __is_std_tuple<std::tuple<_T...>> : std::true_type {};

template <std::size_t _Val, std::size_t... _Mask>
concept __contain_value = ((_Val == _Mask) or ...);


template <typename _T>
concept __has_call_operator =
    std::is_class_v<_T> and
    requires { &_T::operator(); };

template <typename _T>
concept __call_operator_type =
    __has_call_operator<_T> and
    not __is_std_function<_T>::value;

template <typename _T>
concept __member_function_type =
    std::is_member_function_pointer_v<_T>;


template <typename _T>
struct __callable_type_wrapper
: __callable_type_wrapper<std::function<void(_T)>> {};

template <>
struct __callable_type_wrapper<__null_type<>> {
    typedef void return_type;
    typedef void func_type;
};

template <typename _Ret, typename... _Param>
struct __callable_type_wrapper<_Ret(*)(_Param...)> {
    typedef _Ret return_type;
    typedef _Ret (*func_type)(_Param...);
};

template <typename _Ret, typename... _Param>
struct __callable_type_wrapper<_Ret(_Param...)>
: __callable_type_wrapper<std::function<_Ret(_Param...)>> {};

template <typename _Ret, typename... _Param>
struct __callable_type_wrapper<std::function<_Ret(_Param...)>> {
    typedef _Ret return_type;
    typedef std::function<_Ret(_Param...)> func_type;
};

template <typename _T>
struct __callable_instance_helper {
    typedef void return_type;
    typedef std::nullptr_t instance_type;
};

template <typename _T, typename _Ret, typename... _Param>
struct __callable_instance_helper<_Ret(_T::*)(_Param...)> {
    typedef _Ret return_type;
    typedef _T instance_type;
};

template <typename _T, typename _Ret, typename... _Param>
struct __callable_instance_helper<_Ret(_T::*)(_Param...) const> {
    typedef _Ret return_type;
    typedef _T instance_type;
};

template <typename _T>
    requires __call_operator_type<_T>
struct __callable_type_wrapper<_T>
: __callable_instance_helper<decltype(&_T::operator())> {};

template <typename _T>
    requires __member_function_type<_T>
struct __callable_type_wrapper<_T>
: __callable_instance_helper<_T> {
    typedef _T func_type;
};


template <
    typename _T,
    typename key_type = typename _T::key_type,
    typename value_type = typename _T::value_type
>
concept __key_value_type =
    std::is_same_v<key_type, value_type>;

template <
    typename _T,
    typename action_type = typename _T::action_type,
    typename value_type = typename _T::value_type
>
concept __action_value_type =
    std::is_same_v<action_type, value_type>;

template <
    typename _T,
    typename key_type = typename _T::key_type,
    typename action_type = typename _T::action_type
>
concept __non_default_value_type =
    not __key_value_type<_T> and
    not __action_value_type<_T> and
    requires(const key_type& __k, const action_type& __a) {
    {
        _T()(__k, __a)
    } -> std::convertible_to<typename _T::value_type>;
};


template <std::size_t... _Mask>
struct __event_mask {
    static constexpr std::size_t mask_size = sizeof...(_Mask);
    static constexpr std::array<std::size_t, mask_size> value{ _Mask... };
};

template <std::size_t... _Mask>
concept __mask_not_empty = requires {
    __event_mask<_Mask...>::mask_size not_eq 0;
};


template <typename _T>
concept __is_event_type =
    std::is_same_v<_T, event::key::code> or
    std::is_same_v<_T, event::action::code>;


template <typename _T> concept __is_key_type = std::is_same_v<_T, event::key::code>;
template <typename _T> concept __is_action_type = std::is_same_v<_T, event::action::code>;
template <typename... _T> concept __is_event_types = (__is_event_type<_T> and ...);
template <typename... _T> concept __is_key_types = (__is_key_type<_T> and ...);
template <typename... _T> concept __is_action_types = (__is_action_type<_T> and ...);
template <typename... _T> concept __contain_event_type = (__is_event_type<_T> or ...);
template <typename... _T> concept __contain_key_type = (__is_key_type<_T> or ...);
template <typename... _T> concept __contain_action_type = (__is_action_type<_T> or ...);

} /* __detail */


template <typename _T>
struct is_event_type : std::false_type {};

template <typename _T>
    requires __detail::__is_event_type<_T>
struct is_event_type<_T> : std::true_type {};

template <
    template <typename...>
        typename _T,
    typename... _TT
> requires __detail::__is_event_types<_TT...>
struct is_event_type<_T<_TT...>> : std::true_type {};

template <typename _T>
struct is_key_type : std::false_type {};

template <typename _T>
    requires __detail::__is_key_type<_T>
struct is_key_type<_T> : std::true_type {};

template <
    template <typename...>
        typename _T,
    typename... _TT
> requires __detail::__is_key_types<_TT...>
struct is_key_type<_T<_TT...>> : std::true_type {};

template <typename _T>
struct is_action_type : std::false_type {};

template <typename _T>
    requires __detail::__is_action_type<_T>
struct is_action_type<_T> : std::true_type {};

template <
    template <typename...>
        typename _T,
    typename... _TT
> requires __detail::__is_action_types<_TT...>
struct is_action_type<_T<_TT...>> : std::true_type {};

template <typename _T>
struct contain_event_type : std::false_type {};

template <
    template <typename...>
        typename _T,
    typename... _TT
> requires __detail::__contain_event_type<_TT...>
struct contain_event_type<_T<_TT...>> : std::true_type {};

template <typename _T>
struct contain_key_type : std::false_type {};

template <
    template <typename...>
        typename _T,
    typename... _TT
> requires __detail::__contain_key_type<_TT...>
struct contain_key_type<_T<_TT...>> : std::true_type {};

template <typename _T>
struct contain_action_type : std::false_type {};

template <
    template <typename...>
        typename _T,
    typename... _TT
> requires __detail::__contain_action_type<_TT...>
struct contain_action_type<_T<_TT...>> : std::true_type {};


template <typename... _T>
struct get_event_type {
    typedef std::nullptr_t type;
};

template <
    typename _1,
    typename... _T
>
struct get_event_type<_1, _T...>
: std::conditional_t<__detail::__is_event_type<_1>,
    _1, get_event_type<_T...>> {};

template <typename _T>
struct get_event_type<_T> {
    typedef std::nullptr_t type;
};

template <typename... _T>
struct get_key_type {
    typedef std::nullptr_t type;
};

template <
    typename _1,
    typename... _T
>
struct get_key_type<_1, _T...>
: std::conditional<__detail::__is_key_type<_1>,
    _1, get_key_type<_T...>> {};

template <typename _T>
struct get_key_type<_T> {
    typedef std::nullptr_t type;
};

template <typename... _T>
struct get_action_type {
    typedef std::nullptr_t type;
};

template <
    typename _1,
    typename... _T
>
struct get_action_type<_1, _T...>
: std::conditional<__detail::__is_action_type<_1>,
    _1, get_action_type<_T...>> {};

template <typename _T>
struct get_action_type<_T> {
    typedef std::nullptr_t type;
};


                          using null_type = __detail::__null_type<>;
template <typename... _T> using null_template_type = __detail::__null_type<_T...>;
template <typename... _T> using get_event_type_t = typename get_event_type<_T...>::type;
template <typename... _T> using get_key_type_t = typename get_key_type<_T...>::type;
template <typename... _T> using get_action_type_t = typename get_action_type<_T...>::type;

template <typename _T> inline constexpr bool is_event_type_v = is_event_type<_T>::value;
template <typename _T> inline constexpr bool is_key_type_v = is_key_type<_T>::value;
template <typename _T> inline constexpr bool is_action_type_v = is_action_type<_T>::value;
template <typename _T> inline constexpr bool contain_event_type_v = contain_event_type<_T>::value;
template <typename _T> inline constexpr bool contain_key_type_v = contain_key_type<_T>::value;
template <typename _T> inline constexpr bool contain_action_type_v = contain_action_type<_T>::value;

} /* event */
#endif /* _EVENT__UTILITY_H */
