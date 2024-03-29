#ifndef _EVENT__CALLBACK_H
#define _EVENT__CALLBACK_H

#include <type_traits>
#include <tuple>

#include "utility.h"


namespace event {
namespace __detail {

template <typename _Traits>
struct __callback_traits {
public:
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::tuple<key_type, action_type>   value_type;
    
    inline constexpr value_type
    operator()(const_key& __k, const_action& __a) {
        return value_type(__k, __a);
    }
};

template <typename _Traits>
    requires __key_value_type<_Traits>
struct __callback_traits<_Traits> {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef typename _Traits::value_type        value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;

    inline constexpr value_type
    operator()(const_key& __k, const_action&) {
        return __k;
    }
};

template <typename _Traits>
    requires __action_value_type<_Traits>
struct __callback_traits<_Traits> {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef typename _Traits::value_type        value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;

    inline constexpr value_type
    operator()(const_key&, const_action& __a) {
        return __a;
    }
};

template <typename _Traits>
    requires __non_default_value_type<_Traits>
struct __callback_traits<_Traits> {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef typename _Traits::value_type        value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;

    inline constexpr value_type
    operator()(const_key& __k, const_action& __a) {
        return _Traits()(__k, __a);
    }
};

} /* __detail */


template <
    typename _FunctionT,
    typename _Traits
>
class callback {
protected:
    typedef __detail::__callback_traits<_Traits>            __traits;
    typedef __detail::__callable_type_wrapper<_FunctionT>   __invoker;
public:
    typedef typename __traits::key_type                     key_type;
    typedef typename __traits::action_type                  action_type;
    typedef typename __traits::value_type                   value_type;
    typedef typename __traits::const_key                    const_key;
    typedef typename __traits::const_action                 const_action;
    typedef typename __invoker::func_type                   func_type;
    typedef typename __invoker::return_type                 return_type;
    
protected:
    func_type __c_;
    
public:
    callback() = default;
    ~callback() = default;
    
    void set_callback(const func_type& __c) {
        __c_ = __c;
    }
    
protected:
    inline return_type
    __invoke_(const_key& __k, const_action& __a) {
        return __c_(__traits()(__k, __a));
    }
    
    inline return_type
    __invoke_(const_key& __k, const_action& __a)
    requires (__detail::__is_std_tuple<typename __traits::value_type>::value)
    {
        return std::apply(__c_, __traits()(__k, __a));
    }
};


template <typename _Traits>
class callback<__detail::__null_type<>, _Traits> {
protected:
    typedef __detail::__callback_traits<_Traits>            __traits;
public:
    typedef typename __traits::key_type                     key_type;
    typedef typename __traits::action_type                  action_type;
    typedef typename __traits::value_type                   value_type;
    typedef typename __traits::const_key                    const_key;
    typedef typename __traits::const_action                 const_action;
    
public:
    callback() = default;
    ~callback() = default;
    
protected:
    static constexpr void __invoke_(const key_type&, const action_type&) {}
};


template <
    typename _FunctionT,
    typename _Traits
> requires __detail::__call_operator_type<_FunctionT>
class callback<_FunctionT, _Traits> {
protected:
    typedef __detail::__callback_traits<_Traits>            __traits;
    typedef __detail::__callable_type_wrapper<_FunctionT>   __invoker;
public:
    typedef typename __traits::key_type                     key_type;
    typedef typename __traits::action_type                  action_type;
    typedef typename __traits::value_type                   value_type;
    typedef typename __traits::const_key                    const_key;
    typedef typename __traits::const_action                 const_action;
    typedef typename __invoker::instance_type               instance_type;
    typedef typename __invoker::return_type                 return_type;
    typedef std::unique_ptr<instance_type>                  instance_pointer;

protected:
    instance_pointer __i_;

public:
    callback() = default;
    ~callback() = default;
    
    template <typename _InstanceT>
        requires std::is_same_v<instance_type, std::decay_t<_InstanceT>>
    void set_callback(_InstanceT&& __i) {
        __i_ = std::make_unique<instance_type>(std::forward<_InstanceT>(__i));
    }

protected:
    inline return_type
    __invoke_(const_key& __k, const_action& __a) {
        return (*__i_)(__traits()(__k, __a));
    }
    
    inline return_type
    __invoke_(const_key& __k, const_action& __a)
    requires (__detail::__is_std_tuple<typename __traits::value_type>::value)
    {
        return std::apply(*__i_, __traits()(__k, __a));
    }
};


template <
    typename _FunctionT,
    typename _Traits
> requires __detail::__member_function_type<_FunctionT>
class callback<_FunctionT, _Traits> {
protected:
    typedef __detail::__callback_traits<_Traits>            __traits;
    typedef __detail::__callable_type_wrapper<_FunctionT>   __invoker;
public:
    typedef typename __traits::key_type                     key_type;
    typedef typename __traits::action_type                  action_type;
    typedef typename __traits::value_type                   value_type;
    typedef typename __traits::const_key                    const_key;
    typedef typename __traits::const_action                 const_action;
    typedef typename __invoker::func_type                   func_type;
    typedef typename __invoker::instance_type               instance_type;
    typedef typename __invoker::return_type                 return_type;
    typedef instance_type*                                  instance_pointer;
    
protected:
    instance_pointer __i_;
    func_type __f_;
    
public:
    callback() = default;
    ~callback() = default;
    
    void set_callback(const instance_pointer& __i,
                      const func_type& __f)
    {
        __i_ = __i;
        __f_ = __f;
    }
    
    void set_callback(const instance_type& __i,
                      const func_type& __f)
    {
        __i_ = &__i;
        __f_ = __f;
    }
    
    void set_callback(const instance_pointer& __i) {
        __i_ = __i;
    }
    
    void set_callback(const instance_type& __i) {
        __i_ = &__i;
    }
    
    void set_callback(const func_type& __f) {
        __f_ = __f;
    }
    
protected:
    inline return_type
    __invoke_(const_key& __k, const_action& __a) {
        return (__i_->*__f_)(__traits()(__k, __a));
    }
    
    inline return_type
    __invoke_(const_key& __k, const_action& __a)
    requires (__detail::__is_std_tuple<typename __traits::value_type>::value)
    {
        return std::apply(__f_,
            std::tuple_cat(std::make_tuple(__i_), __traits()(__k, __a)));
    }
};


using null_callback = __detail::__null_type<>;

} /* event */
#endif /* _EVENT__CALLBACK_H */
