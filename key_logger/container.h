#ifndef _EVENT__CONTAINER_H
#define _EVENT__CONTAINER_H

#include <utility>
#include <queue>
#include <deque>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <sstream>

#include "utility.h"
#include "string.h"


namespace event {
namespace __detail {

template <typename _Traits>
struct __container_traits {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;
    typedef event_pair                          value_type;
    
    inline constexpr value_type
    operator()(const_key& __k, const_action& __a) {
        return std::make_pair(__k, __a);
    }
};

template <typename _Traits>
    requires __key_value_type<_Traits>
struct __container_traits<_Traits> {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef typename _Traits::value_type        value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;

    inline constexpr value_type
    operator()(const_key& __k, const_action&) {
        return __k;
    }
};

template <typename _Traits>
    requires __action_value_type<_Traits>
struct __container_traits<_Traits> {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef typename _Traits::value_type        value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;

    inline constexpr value_type
    operator()(const_key&, const_action& __a) {
        return __a;
    }
};

template <typename _Traits>
    requires __non_default_value_type<_Traits>
struct __container_traits<_Traits> {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef typename _Traits::value_type        value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;

    inline constexpr value_type
    operator()(const_key& __k, const_action& __a) {
        return _Traits()(__k, __a);
    }
};

template <typename _Traits>
struct __binary_container_traits {
    typedef typename _Traits::key_type          key_type;
    typedef typename _Traits::action_type       action_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    
    inline constexpr bool
    operator()(const_action& __a) {
        return _Traits()(__a);
    }
};


template <typename _Traits> using __array  = __detail::__null_type<_Traits>;
template <typename _Traits> using __bitset = __detail::__null_type<_Traits>;

} /* __datail */


template <
    template <typename...>
        typename _ConT,
    typename _Traits
>
class container {
protected:
    typedef __detail::__container_traits<_Traits>       __traits;
public:
    typedef typename __traits::key_type                 key_type;
    typedef typename __traits::action_type              action_type;
    typedef typename __traits::const_key                const_key;
    typedef typename __traits::const_action             const_action;
    typedef typename __traits::value_type               value_type;
    typedef typename __traits::event_pair               event_pair;
    
public:
    container() = delete;
    ~container() = delete;
};


template <typename _Traits>
class container<__detail::__null_type, _Traits> {
protected:
    typedef __detail::__container_traits<_Traits>       __traits;
public:
    typedef typename __traits::key_type                 key_type;
    typedef typename __traits::action_type              action_type;
    typedef typename __traits::const_key                const_key;
    typedef typename __traits::const_action             const_action;
    typedef typename __traits::event_pair               event_pair;
    typedef void                                        container_type;
    typedef void                                        split_container;
    
public:
    container() = default;
    ~container() = default;
    
protected:
    static constexpr void __push_(const_key&, const_action&) {}
};


template <typename _Traits>
class container<std::queue, _Traits> {
protected:
    typedef __detail::__container_traits<_Traits>       __traits;
public:
    typedef typename __traits::key_type                 key_type;
    typedef typename __traits::action_type              action_type;
    typedef typename __traits::value_type               value_type;
    typedef typename __traits::event_pair               event_pair;
    typedef typename __traits::const_key                const_key;
    typedef typename __traits::const_action             const_action;
    typedef std::queue<value_type>                      container_type;
    typedef std::vector<value_type>                     split_container;
    
protected:
    container_type __c_;
    
public:
    container() = default;
    ~container() = default;

    value_type front() const;
    value_type back() const;
    value_type pop();
    split_container get_all();
    
    void clear();
    bool empty();
    std::size_t size() const;
    
protected:
    inline void __push_(const_key&, const_action&);
};


template <typename _Traits>
typename container<std::queue, _Traits>::value_type
container<std::queue, _Traits>::front() const {
    return __c_.front();
}

template <typename _Traits>
typename container<std::queue, _Traits>::value_type
container<std::queue, _Traits>::back() const {
    return __c_.back();
}

template <typename _Traits>
typename container<std::queue, _Traits>::value_type
container<std::queue, _Traits>::pop() {
    auto __tmp = __c_.front();
    __c_.pop();
    return __tmp;
}

template <typename _Traits>
std::size_t
container<std::queue, _Traits>::size() const {
    return __c_.size();
}

template <typename _Traits>
bool
container<std::queue, _Traits>::empty() {
    return __c_.empty();
}

template <typename _Traits>
typename container<std::queue, _Traits>::split_container
container<std::queue, _Traits>::get_all() {
    std::vector<value_type> __v(__c_.size());
    for (auto& __e : __v) {
        __e = __c_.front();
        __c_.pop();
    }
    return __v;
}

template <typename _Traits>
void
container<std::queue, _Traits>::clear() {
    __c_ = container_type();
}

template <typename _Traits>
inline void
container<std::queue, _Traits>::__push_(const_key& __k,
                                      const_action& __a)
{
    __c_.push(__traits()(__k, __a));
}


template <typename _Traits>
class container<std::deque, _Traits> {
protected:
    typedef __detail::__container_traits<_Traits>       __traits;
public:
    typedef typename __traits::key_type                 key_type;
    typedef typename __traits::action_type              action_type;
    typedef typename __traits::value_type               value_type;
    typedef typename __traits::event_pair               event_pair;
    typedef typename __traits::const_key                const_key;
    typedef typename __traits::const_action             const_action;
    typedef const std::size_t&                          index_type;
    typedef std::deque<value_type>                      container_type;
    typedef std::vector<value_type>                     split_container;

protected:
    container_type __c_;

public:
    container() = default;
    ~container() = default;
    
    value_type front() const;
    value_type back() const;
    value_type pop();
    value_type pop_back();
    value_type at(index_type) const;
    split_container get_all();
    std::string get_string();

    void clear();
    void erase(index_type);
    bool empty();
    std::size_t size() const;

    value_type operator[](index_type) const;

protected:
    inline void __push_(const_key&, const_action&);
};


template <typename _Traits>
typename container<std::deque, _Traits>::value_type
container<std::deque, _Traits>::front() const {
    return __c_.front();
}

template <typename _Traits>
typename container<std::deque, _Traits>::value_type
container<std::deque, _Traits>::back() const {
    return __c_.back();
}

template <typename _Traits>
typename container<std::deque, _Traits>::value_type
container<std::deque, _Traits>::pop() {
    auto __tmp = __c_.front();
    __c_.pop_front();
    return __tmp;
}

template <typename _Traits>
typename container<std::deque, _Traits>::value_type
container<std::deque, _Traits>::pop_back() {
    auto __tmp = __c_.back();
    __c_.pop_back();
    return __tmp;
}

template <typename _Traits>
typename container<std::deque, _Traits>::value_type
container<std::deque, _Traits>::at(index_type __i) const {
    return __c_.at(__i);
}

template <typename _Traits>
void
container<std::deque, _Traits>::erase(index_type __i) {
    __c_.erase(__c_.begin() + __i);
}

template <typename _Traits>
bool
container<std::deque, _Traits>::empty() {
    return __c_.empty();
}

template <typename _Traits>
std::size_t
container<std::deque, _Traits>::size() const {
    return __c_.size();
}

template <typename _Traits>
typename container<std::deque, _Traits>::split_container
container<std::deque, _Traits>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _Traits>
std::string
container<std::deque, _Traits>::get_string() {
    std::stringstream __s;
    for (auto& __e : __c_) {
        __s << __e;
    }
    return __s.str();
}

template <typename _Traits>
void
container<std::deque, _Traits>::clear() {
    __c_.resize(0);
}

template <typename _Traits>
typename container<std::deque, _Traits>::value_type
container<std::deque, _Traits>::operator[](index_type& __i) const {
    return __c_.at(__i);
}

template <typename _Traits>
inline void
container<std::deque, _Traits>::__push_(const_key& __k,
                                      const_action& __a)
{
    __c_.push_back(__traits()(__k, __a));
}


template <typename _Traits>
class container<std::vector, _Traits> {
protected:
    typedef __detail::__container_traits<_Traits>       __traits;
public:
    typedef typename __traits::key_type                 key_type;
    typedef typename __traits::action_type              action_type;
    typedef typename __traits::value_type               value_type;
    typedef typename __traits::event_pair               event_pair;
    typedef typename __traits::const_key                const_key;
    typedef typename __traits::const_action             const_action;
    typedef const std::size_t&                          index_type;
    typedef std::vector<value_type>                     container_type;
    typedef std::vector<value_type>                     split_container;

protected:
    container_type __c_;

public:
    container() = default;
    ~container() = default;

    value_type pop_back();
    value_type at(index_type) const;
    split_container pop_back(index_type);
    split_container at(index_type, index_type) const;
    split_container get_all();
    std::string get_string();

    void clear();
    void erase(index_type, index_type);
    bool empty();
    std::size_t size() const;

    value_type operator[](index_type) const;

protected:
    inline void __push_(const_key&, const_action&);
};


template <typename _Traits>
typename container<std::vector, _Traits>::value_type
container<std::vector, _Traits>::pop_back() {
    auto __tmp = *(--__c_.end());
    __c_.pop_back();
    return __tmp;
}

template <typename _Traits>
typename container<std::vector, _Traits>::split_container
container<std::vector, _Traits>::pop_back(index_type __i) {
    split_container __v {__c_.end() - __i, __c_.end()};
    __v.erase(__c_.end() - __i, __c_.end());
    return __v;
}

template <typename _Traits>
typename container<std::vector, _Traits>::value_type
container<std::vector, _Traits>::at(index_type __i) const {
    return __c_.at(__i);
}

template <typename _Traits>
typename container<std::vector, _Traits>::split_container
container<std::vector, _Traits>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _Traits>
std::string
container<std::vector, _Traits>::get_string() {
    std::stringstream __s;
    for (auto& ele : __c_) {
        __s << ele;
    }
    return __s.str();
}

template <typename _Traits>
typename container<std::vector, _Traits>::split_container
container<std::vector, _Traits>::at(index_type __1,
                                  index_type __2) const
{
    return split_container {__c_.begin() + __1, __c_.begin() + __2};
}

template <typename _Traits>
bool
container<std::vector, _Traits>::empty() {
    return __c_.empty();
}

template <typename _Traits>
void
container<std::vector, _Traits>::clear() {
    __c_.clear();
}

template <typename _Traits>
std::size_t
container<std::vector, _Traits>::size() const {
    __c_.size();
}

template <typename _Traits>
void
container<std::vector, _Traits>::erase(index_type __1,
                                     index_type __2)
{
    __c_.erase(__c_.begin() + __1, __c_.begin() + __2);
}

template <typename _Traits>
typename container<std::vector, _Traits>::value_type
container<std::vector, _Traits>::operator[](index_type __i) const {
    return this->at(__i);
}

template <typename _Traits>
inline void
container<std::vector, _Traits>::__push_(const_key& __k,
                                       const_action& __a)
{
    __c_.push_back(__traits()(__k, __a));
}


template <typename _Traits>
class container<__detail::__bitset, _Traits> {
protected:
    typedef __detail::__binary_container_traits<_Traits>  __traits;
public:
    typedef typename __traits::key_type                 key_type;
    typedef typename __traits::action_type              action_type;
    typedef bool                                        value_type;
    typedef typename __traits::const_key                const_key;
    typedef typename __traits::const_action             const_action;
    typedef const key_type&                             index_type;
    typedef std::bitset<key_type::code_size>            container_type;

protected:
    container_type __c_;

protected:
    container();
    ~container() = default;

public:
    void clear();
    value_type at(index_type) const;
    constexpr std::size_t size() const;

    bool operator[](index_type) const;
    
protected:
    inline void __push_(const_key&, const_action&);
};


template <typename _Traits>
container<__detail::__bitset, _Traits>::container() {
    clear();
}

template <typename _Traits>
typename container<__detail::__bitset, _Traits>::value_type
container<__detail::__bitset, _Traits>::at(index_type __i) const {
    return __c_[(std::size_t)__i];
}

template <typename _Traits>
void
container<__detail::__bitset, _Traits>::clear() {
    __c_.reset();
}

template <typename _Traits>
constexpr std::size_t
container<__detail::__bitset, _Traits>::size() const {
    return key_type::code_size;
}

template <typename _Traits>
bool
container<__detail::__bitset, _Traits>::operator[](index_type __i) const {
    return this->at(__i);
}

template <typename _Traits>
void
container<__detail::__bitset, _Traits>::__push_(const_key& __k,
                                              const_action& __a)
{
    __c_[(std::size_t)__k] = __traits()(__a);
}


template <typename _Traits>
class container<__detail::__array, _Traits> {
protected:
    typedef __detail::__container_traits<_Traits>       __traits;
public:
    typedef typename __traits::key_type                 key_type;
    typedef typename __traits::action_type              action_type;
    typedef typename __traits::value_type               value_type;
    typedef typename __traits::event_pair               event_pair;
    typedef typename __traits::const_key                const_key;
    typedef typename __traits::const_action             const_action;
    typedef const value_type&                           index_type;
    typedef std::array<value_type, key_type::code_size> container_type;

protected:
    container_type __c_;

protected:
    container();
    ~container() = default;

public:
    void clear();
    value_type at(index_type) const;
    constexpr std::size_t size() const;

    value_type operator[](index_type) const;
    
protected:
    inline void __push_(const_key&, const_action&);
};


template <typename _Traits>
container<__detail::__array, _Traits>::container() {
    clear();
}

template <typename _Traits>
typename container<__detail::__array, _Traits>::value_type
container<__detail::__array, _Traits>::at(index_type __i) const {
    return __c_.at((std::size_t)__i);
}

template <typename _Traits>
void
container<__detail::__array, _Traits>::clear() {
    __c_.fill((action_type)0);
}

template <typename _Traits>
constexpr std::size_t
container<__detail::__array, _Traits>::size() const {
    return key_type::code_size;
}

template <typename _Traits>
typename container<__detail::__array, _Traits>::value_type
container<__detail::__array, _Traits>::operator[](index_type __i) const {
    return this->at(__i);
}

template <typename _Traits>
void
container<__detail::__array, _Traits>::__push_(const_key& __k,
                                             const_action& __a)
{
    __c_.at((std::size_t)__k) = __traits()(__k, __a);
}


template <typename _Traits> using queue   = container<std::queue, _Traits>;
template <typename _Traits> using deque   = container<std::deque, _Traits>;
template <typename _Traits> using vector  = container<std::vector, _Traits>;
template <typename _Traits> using array   = container<__detail::__array, _Traits>;
template <typename _Traits> using bitset  = container<__detail::__bitset, _Traits>;
template <typename _Traits> using null_container = container<__detail::__null_type, _Traits>;

} /* event */
#endif /* _EVENT__CONTAINER_H */
