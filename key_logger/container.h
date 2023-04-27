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

template <typename _Type>
struct __container_type_wrapper {
    typedef typename _Type::key_type            key_type;
    typedef typename _Type::action_type         action_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;
    typedef event_pair                          value_type;
    
    inline constexpr value_type
    operator()(const_key& __k, const_action& __a) {
        return std::make_pair(__k, __a);
    }
};

template <typename _Type>
    requires __key_value_type<_Type>
struct __container_type_wrapper<_Type> {
    typedef typename _Type::key_type            key_type;
    typedef typename _Type::action_type         action_type;
    typedef typename _Type::value_type          value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;

    inline constexpr value_type
    operator()(const_key& __k, const_action&) {
        return __k;
    }
};

template <typename _Type>
    requires __action_value_type<_Type>
struct __container_type_wrapper<_Type> {
    typedef typename _Type::key_type            key_type;
    typedef typename _Type::action_type         action_type;
    typedef typename _Type::value_type          value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;

    inline constexpr value_type
    operator()(const_key&, const_action& __a) {
        return __a;
    }
};

template <typename _Type>
    requires __non_default_value_type<_Type>
struct __container_type_wrapper<_Type> {
    typedef typename _Type::key_type            key_type;
    typedef typename _Type::action_type         action_type;
    typedef typename _Type::value_type          value_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    typedef std::pair<key_type, action_type>    event_pair;

    inline constexpr value_type
    operator()(const_key& __k, const_action& __a) {
        return _Type()(__k, __a);
    }
};

template <typename _Type>
struct __binary_container_helper {
    typedef typename _Type::key_type            key_type;
    typedef typename _Type::action_type         action_type;
    typedef const key_type                      const_key;
    typedef const action_type                   const_action;
    
    inline constexpr bool
    operator()(const_action& __a) {
        return _Type()(__a);
    }
};


template <typename _Type> using __array  = __detail::__null_type<_Type>;
template <typename _Type> using __bitset = __detail::__null_type<_Type>;

} /* __datail */


template <
    template <typename...>
        typename _ConT,
    typename _Type
>
class container {
protected:
    typedef __detail::__container_type_wrapper<_Type>   __type;
public:
    typedef typename __type::key_type                   key_type;
    typedef typename __type::action_type                action_type;
    typedef typename __type::const_key                  const_key;
    typedef typename __type::const_action               const_action;
    typedef typename __type::value_type                 value_type;
    typedef typename __type::event_pair                 event_pair;
    
public:
    container() = delete;
    ~container() = delete;
};


template <typename _Type>
class container<__detail::__null_type, _Type> {
protected:
    typedef __detail::__container_type_wrapper<_Type>   __type;
public:
    typedef typename __type::key_type                   key_type;
    typedef typename __type::action_type                action_type;
    typedef typename __type::const_key                  const_key;
    typedef typename __type::const_action               const_action;
    typedef typename __type::event_pair                 event_pair;
    typedef void                                        container_type;
    typedef void                                        split_container;
    
public:
    container() = default;
    ~container() = default;
    
protected:
    static constexpr void __push_(const_key&, const_action&) {}
};


template <typename _Type>
class container<std::queue, _Type> {
protected:
    typedef __detail::__container_type_wrapper<_Type>   __type;
public:
    typedef typename __type::key_type                   key_type;
    typedef typename __type::action_type                action_type;
    typedef typename __type::value_type                 value_type;
    typedef typename __type::event_pair                 event_pair;
    typedef typename __type::const_key                  const_key;
    typedef typename __type::const_action               const_action;
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


template <typename _Type>
typename container<std::queue, _Type>::value_type
container<std::queue, _Type>::front() const {
    return __c_.front();
}

template <typename _Type>
typename container<std::queue, _Type>::value_type
container<std::queue, _Type>::back() const {
    return __c_.back();
}

template <typename _Type>
typename container<std::queue, _Type>::value_type
container<std::queue, _Type>::pop() {
    auto __tmp = __c_.front();
    __c_.pop();
    return __tmp;
}

template <typename _Type>
std::size_t
container<std::queue, _Type>::size() const {
    return __c_.size();
}

template <typename _Type>
bool
container<std::queue, _Type>::empty() {
    return __c_.empty();
}

template <typename _Type>
typename container<std::queue, _Type>::split_container
container<std::queue, _Type>::get_all() {
    std::vector<value_type> __v(__c_.size());
    for (auto& __e : __v) {
        __e = __c_.front();
        __c_.pop();
    }
    return __v;
}

template <typename _Type>
void
container<std::queue, _Type>::clear() {
    __c_ = container_type();
}

template <typename _Type>
inline void
container<std::queue, _Type>::__push_(const_key& __k,
                                      const_action& __a)
{
    __c_.push(__type()(__k, __a));
}


template <typename _Type>
class container<std::deque, _Type> {
protected:
    typedef __detail::__container_type_wrapper<_Type>   __type;
public:
    typedef typename __type::key_type                   key_type;
    typedef typename __type::action_type                action_type;
    typedef typename __type::value_type                 value_type;
    typedef typename __type::event_pair                 event_pair;
    typedef typename __type::const_key                  const_key;
    typedef typename __type::const_action               const_action;
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


template <typename _Type>
typename container<std::deque, _Type>::value_type
container<std::deque, _Type>::front() const {
    return __c_.front();
}

template <typename _Type>
typename container<std::deque, _Type>::value_type
container<std::deque, _Type>::back() const {
    return __c_.back();
}

template <typename _Type>
typename container<std::deque, _Type>::value_type
container<std::deque, _Type>::pop() {
    auto __tmp = __c_.front();
    __c_.pop_front();
    return __tmp;
}

template <typename _Type>
typename container<std::deque, _Type>::value_type
container<std::deque, _Type>::pop_back() {
    auto __tmp = __c_.back();
    __c_.pop_back();
    return __tmp;
}

template <typename _Type>
typename container<std::deque, _Type>::value_type
container<std::deque, _Type>::at(index_type __i) const {
    return __c_.at(__i);
}

template <typename _Type>
void
container<std::deque, _Type>::erase(index_type __i) {
    __c_.erase(__c_.begin() + __i);
}

template <typename _Type>
bool
container<std::deque, _Type>::empty() {
    return __c_.empty();
}

template <typename _Type>
std::size_t
container<std::deque, _Type>::size() const {
    return __c_.size();
}

template <typename _Type>
typename container<std::deque, _Type>::split_container
container<std::deque, _Type>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _Type>
std::string
container<std::deque, _Type>::get_string() {
    std::stringstream __s;
    for (auto& __e : __c_) {
        __s << __e;
    }
    return __s.str();
}

template <typename _Type>
void
container<std::deque, _Type>::clear() {
    __c_.resize(0);
}

template <typename _Type>
typename container<std::deque, _Type>::value_type
container<std::deque, _Type>::operator[](index_type& __i) const {
    return __c_.at(__i);
}

template <typename _Type>
inline void
container<std::deque, _Type>::__push_(const_key& __k,
                                      const_action& __a)
{
    __c_.push_back(__type()(__k, __a));
}


template <typename _Type>
class container<std::vector, _Type> {
protected:
    typedef __detail::__container_type_wrapper<_Type>   __type;
public:
    typedef typename __type::key_type                   key_type;
    typedef typename __type::action_type                action_type;
    typedef typename __type::value_type                 value_type;
    typedef typename __type::event_pair                 event_pair;
    typedef typename __type::const_key                  const_key;
    typedef typename __type::const_action               const_action;
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


template <typename _Type>
typename container<std::vector, _Type>::value_type
container<std::vector, _Type>::pop_back() {
    auto __tmp = *(--__c_.end());
    __c_.pop_back();
    return __tmp;
}

template <typename _Type>
typename container<std::vector, _Type>::split_container
container<std::vector, _Type>::pop_back(index_type __i) {
    split_container __v {__c_.end() - __i, __c_.end()};
    __v.erase(__c_.end() - __i, __c_.end());
    return __v;
}

template <typename _Type>
typename container<std::vector, _Type>::value_type
container<std::vector, _Type>::at(index_type __i) const {
    return __c_.at(__i);
}

template <typename _Type>
typename container<std::vector, _Type>::split_container
container<std::vector, _Type>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _Type>
std::string
container<std::vector, _Type>::get_string() {
    std::stringstream __s;
    for (auto& ele : __c_) {
        __s << ele;
    }
    return __s.str();
}

template <typename _Type>
typename container<std::vector, _Type>::split_container
container<std::vector, _Type>::at(index_type __1,
                                  index_type __2) const
{
    return split_container {__c_.begin() + __1, __c_.begin() + __2};
}

template <typename _Type>
bool
container<std::vector, _Type>::empty() {
    return __c_.empty();
}

template <typename _Type>
void
container<std::vector, _Type>::clear() {
    __c_.clear();
}

template <typename _Type>
std::size_t
container<std::vector, _Type>::size() const {
    __c_.size();
}

template <typename _Type>
void
container<std::vector, _Type>::erase(index_type __1,
                                     index_type __2)
{
    __c_.erase(__c_.begin() + __1, __c_.begin() + __2);
}

template <typename _Type>
typename container<std::vector, _Type>::value_type
container<std::vector, _Type>::operator[](index_type __i) const {
    return this->at(__i);
}

template <typename _Type>
inline void
container<std::vector, _Type>::__push_(const_key& __k,
                                       const_action& __a)
{
    __c_.push_back(__type()(__k, __a));
}


template <typename _Type>
class container<__detail::__bitset, _Type> {
protected:
    typedef __detail::__binary_container_helper<_Type>  __type;
public:
    typedef typename __type::key_type                   key_type;
    typedef typename __type::action_type                action_type;
    typedef bool                                        value_type;
    typedef typename __type::const_key                  const_key;
    typedef typename __type::const_action               const_action;
    typedef const key_type&                             index_type;
    typedef std::bitset<key_type::code_size>            container_type;

protected:
    container_type __c_;

protected:
    container() = default;
    ~container() = default;

public:
    void clear();
    value_type at(index_type) const;
    constexpr std::size_t size() const;

    bool operator[](index_type) const;
    
protected:
    inline void __push_(const_key&, const_action&);
};


template <typename _Type>
typename container<__detail::__bitset, _Type>::value_type
container<__detail::__bitset, _Type>::at(index_type __i) const {
    return __c_[(std::size_t)__i];
}

template <typename _Type>
void
container<__detail::__bitset, _Type>::clear() {
    __c_.reset();
}

template <typename _Type>
constexpr std::size_t
container<__detail::__bitset, _Type>::size() const {
    return key_type::code_size;
}

template <typename _Type>
bool
container<__detail::__bitset, _Type>::operator[](index_type __i) const {
    return this->at(__i);
}

template <typename _Type>
void
container<__detail::__bitset, _Type>::__push_(const_key& __k,
                                              const_action& __a)
{
    __c_[(std::size_t)__k] = __type()(__a);
}


template <typename _Type>
class container<__detail::__array, _Type> {
protected:
    typedef __detail::__container_type_wrapper<_Type>   __type;
public:
    typedef typename __type::key_type                   key_type;
    typedef typename __type::action_type                action_type;
    typedef typename __type::value_type                 value_type;
    typedef typename __type::event_pair                 event_pair;
    typedef typename __type::const_key                  const_key;
    typedef typename __type::const_action               const_action;
    typedef const value_type&                           index_type;
    typedef std::array<value_type, key_type::code_size> container_type;

protected:
    container_type __c_;

protected:
    container() = default;
    ~container() = default;

public:
    void clear();
    value_type at(index_type) const;
    constexpr std::size_t size() const;

    value_type operator[](index_type) const;
    
protected:
    inline void __push_(const_key&, const_action&);
};


template <typename _Type>
typename container<__detail::__array, _Type>::value_type
container<__detail::__array, _Type>::at(index_type __i) const {
    return __c_.at((std::size_t)__i);
}

template <typename _Type>
void
container<__detail::__array, _Type>::clear() {
    __c_.fill((action_type)0);
}

template <typename _Type>
constexpr std::size_t
container<__detail::__array, _Type>::size() const {
    return key_type::code_size;
}

template <typename _Type>
typename container<__detail::__array, _Type>::value_type
container<__detail::__array, _Type>::operator[](index_type __i) const {
    return this->at(__i);
}

template <typename _Type>
void
container<__detail::__array, _Type>::__push_(const_key& __k,
                                             const_action& __a)
{
    __c_.at((std::size_t)__k) = __type()(__k, __a);
}


template <typename _Type> using queue   = container<std::queue, _Type>;
template <typename _Type> using deque   = container<std::deque, _Type>;
template <typename _Type> using vector  = container<std::vector, _Type>;
template <typename _Type> using array   = container<__detail::__array, _Type>;
template <typename _Type> using bitset  = container<__detail::__bitset, _Type>;
template <typename _Type> using null_container = container<__detail::__null_type, _Type>;

} /* event */
#endif /* _EVENT__CONTAINER_H */
