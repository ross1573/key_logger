#ifndef __EVENT_CONTAINER_H__
#define __EVENT_CONTAINER_H__

#include <utility>
#include <queue>
#include <deque>
#include <vector>
#include <bitset>
#include <array>
#include <string>

#include "event.h"
#include "key_str.h"


namespace event {

template <typename _T>
_T
convert(const key::code& __k, const action::code&) {
    return (_T)__k;
}

template <>
std::nullptr_t
convert<std::nullptr_t>(const key::code&, const action::code&) {
    return nullptr;
}

template <>
key::code
convert<key::code>(const key::code& __k, const action::code&) {
    return __k;
}

template <>
action::code
convert<action::code>(const key::code&, const action::code& __a) {
    return __a;
}

template <>
std::pair<key::code, action::code>
convert<std::pair<key::code, action::code>>(const key::code& __k, const action::code& __a) {
    return std::make_pair(__k, __a);
}

template <>
std::pair<action::code, key::code>
convert<std::pair<action::code, key::code>>(const key::code& __k, const action::code& __a) {
    return std::make_pair(__a, __k);
}


template <typename _T>
class __container {
public:
    typedef _T value_type;
    
protected:
    __container() = default;
    ~__container() = default;
    __container(__container const &) = delete;
    
    virtual void push(const key::code&, const action::code&);
    virtual void __push_(value_type&&) = 0;
};


template <typename _T>
void
__container<_T>::push(const key::code& __k, const action::code& __a) {
    __push_(convert<_T>(__k, __a));
}


template <>
class __container <void> {
public:
    typedef void value_type;
    
protected:
    __container() = default;
    ~__container() = default;
    __container(__container const &) = delete;
};


template <>
class __container <bool> {
public:
    typedef bool value_type;
    
protected:
    __container() = default;
    ~__container() = default;
    __container(__container const &) = delete;
    
    virtual void push(const key::code&, const action::code&) = 0;
};


template <typename _ConT>
class container {
protected:
    container() = delete;
    ~container() = delete;
};


template <>
class container<void> : public __container<void> {
protected:
    typedef __container<std::nullptr_t> _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef void                        container_type;
    
protected:
    container() = default;
    virtual ~container() = default;
};


template <typename _T>
class container<std::queue<_T>>
: public __container<_T> {
protected:
    typedef __container<_T>             _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::queue<value_type>      container_type;
    typedef std::vector<value_type>     split_container;
    
protected:
    container_type __c_;
    
protected:
    container() = default;
    virtual ~container() = default;
    
public:
    value_type pop();
    container_type& get();
    split_container get_all();
    
    bool empty();
    void clear();
    std::size_t size();
    
protected:
    void __push_(value_type&&);
};


template <typename _T>
typename container<std::queue<_T>>::value_type
container<std::queue<_T>>::pop() {
    auto __tmp = __c_.front();
    __c_.pop();
    return __tmp;
}

template <typename _T>
typename container<std::queue<_T>>::container_type&
container<std::queue<_T>>::get() {
    return __c_;
}

template <typename _T>
std::size_t
container<std::queue<_T>>::size() {
    return __c_.size();
}

template <typename _T>
bool
container<std::queue<_T>>::empty() {
    return __c_.empty();
}

template <typename _T>
typename container<std::queue<_T>>::split_container
container<std::queue<_T>>::get_all() {
    std::vector<value_type> __v(__c_.size());
    while (!__c_.empty()) {
        __v.emplace_back(__c_.front());
        __c_.pop();
    }
    return __v;
}

template <typename _T>
void
container<std::queue<_T>>::clear() {
    __c_ = container_type();
}

template <typename _T>
void
container<std::queue<_T>>::__push_(value_type&& __v) {
    __c_.push(__v);
}


template <typename _T>
class container<std::deque<_T>>
: public __container<_T> {
protected:
    typedef __container<_T>             _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::deque<value_type>      container_type;
    typedef std::vector<value_type>     split_container;
    
protected:
    container_type __c_;
    
protected:
    container() = default;
    virtual ~container() = default;
    
public:
    value_type pop();
    value_type pop_back();
    value_type at(std::size_t);
    container_type& get();
    split_container get_all();
    std::string get_string();
    
    bool empty();
    void clear();
    std::size_t size();
    void erase(std::size_t);
    
    const value_type operator[](std::size_t) const;
    
protected:
    void __push_(value_type&&);
};


template <typename _T>
typename container<std::deque<_T>>::value_type
container<std::deque<_T>>::pop() {
    auto __tmp = __c_.front();
    __c_.pop_front();
    return __tmp;
}

template <typename _T>
typename container<std::deque<_T>>::value_type
container<std::deque<_T>>::pop_back() {
    auto __tmp = __c_.back();
    __c_.pop_back();
    return __tmp;
}

template <typename _T>
typename container<std::deque<_T>>::value_type
container<std::deque<_T>>::at(std::size_t __i) {
    return __c_.at(__i);
}

template <typename _T>
void
container<std::deque<_T>>::erase(std::size_t __i) {
    __c_.erase(__c_.begin() + __i);
}

template <typename _T>
typename container<std::deque<_T>>::container_type&
container<std::deque<_T>>::get() {
    return __c_;
}

template <typename _T>
bool
container<std::deque<_T>>::empty() {
    return __c_.empty();
}

template <typename _T>
std::size_t
container<std::deque<_T>>::size() {
    return __c_.size();
}

template <typename _T>
typename container<std::deque<_T>>::split_container
container<std::deque<_T>>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _T>
std::string
container<std::deque<_T>>::get_string() {
    std::string __s;
    for (auto& ele : __c_) {
        __s.append(event::string(ele));
    }
    return __s;
}

template <typename _T>
void
container<std::deque<_T>>::clear() {
    __c_.resize(0);
}

template <typename _T>
const typename container<std::deque<_T>>::value_type
container<std::deque<_T>>::operator[](std::size_t __i) const {
    return at(__i);
}

template <typename _T>
void
container<std::deque<_T>>::__push_(value_type&& __v) {
    __c_.push_back(__v);
}


template <typename _T>
class container<std::vector<_T>>
: public __container<_T> {
protected:
    typedef __container<_T>             _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::vector<value_type>     container_type;
    typedef std::vector<value_type>     split_container;
    
protected:
    container_type __c_;
    
protected:
    container() = default;
    virtual ~container() = default;
    
public:
    split_container pop(std::size_t);
    value_type at(std::size_t);
    container_type& get();
    split_container get_all();
    std::string get_string();
    split_container at(std::size_t, std::size_t);
    
    bool empty();
    void clear();
    std::size_t size();
    void erase(std::size_t, std::size_t);
    
    const value_type operator[](std::size_t) const;
    
protected:
    void __push_(value_type&&);
};


template <typename _T>
typename container<std::vector<_T>>::split_container
container<std::vector<_T>>::pop(std::size_t __i) {
    split_container __v {__c_.begin(), __c_.begin() + __i};
    __v.erase(__c_.begin(), __c_.begin() + __i);
    return __v;
}

template <typename _T>
typename container<std::vector<_T>>::value_type
container<std::vector<_T>>::at(std::size_t __i) {
    return __c_.at(__i);
}

template <typename _T>
typename container<std::vector<_T>>::container_type&
container<std::vector<_T>>::get() {
    return __c_;
}

template <typename _T>
typename container<std::vector<_T>>::split_container
container<std::vector<_T>>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _T>
std::string
container<std::vector<_T>>::get_string() {
    std::string __s;
    for (auto& ele : __c_) {
        __s.append(event::string(ele));
    }
    return __s;
}

template <typename _T>
typename container<std::vector<_T>>::split_container
container<std::vector<_T>>::at(std::size_t __1, std::size_t __2) {
    return split_container {__c_.begin() + __1, __c_.begin() + __2};
}

template <typename _T>
bool
container<std::vector<_T>>::empty() {
    return __c_.empty();
}

template <typename _T>
void
container<std::vector<_T>>::clear() {
    __c_.clear();
}

template <typename _T>
std::size_t
container<std::vector<_T>>::size() {
    __c_.size();
}

template <typename _T>
void
container<std::vector<_T>>::erase(std::size_t __1, std::size_t __2) {
    __c_.erase(__c_.begin() + __1, __c_.begin() + __2);
}

template <typename _T>
const typename container<std::vector<_T>>::value_type
container<std::vector<_T>>::operator[](std::size_t __i) const {
    return at(__i);
}

template <typename _T>
void
container<std::vector<_T>>::__push_(value_type&& __v) {
    __c_.push_back(__v);
}


template <std::size_t _Size>
class container<std::bitset<_Size>>
: public __container<bool> {
protected:
    typedef __container<bool>               _Base;
public:
    typedef typename _Base::value_type      value_type;
    typedef std::bitset<event::key::code_size> container_type;
    
protected:
    container_type __c_;
    
protected:
    container() = default;
    virtual ~container() = default;
    
public:
    value_type at(std::size_t);
    container_type get();
    void clear();
    constexpr std::size_t size() const;
    
    const value_type operator[](std::size_t) const;
};


template <std::size_t _Size>
typename container<std::bitset<_Size>>::value_type
container<std::bitset<_Size>>::at(std::size_t __i) {
    return __c_[__i];
}

template <std::size_t _Size>
typename container<std::bitset<_Size>>::container_type
container<std::bitset<_Size>>::get() {
    return __c_;
}

template <std::size_t _Size>
void
container<std::bitset<_Size>>::clear() {
    __c_.reset();
}

template <std::size_t _Size>
constexpr std::size_t
container<std::bitset<_Size>>::size() const {
    return _Size;
}

template <std::size_t _Size>
const typename container<std::bitset<_Size>>::value_type
container<std::bitset<_Size>>::operator[](std::size_t __i) const {
    return __c_[__i];
}


template <typename _T, std::size_t _I>
class container<std::array<_T, _I>>
: public __container<_T> {
protected:
    typedef __container<_T>             _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::array<value_type, _I>  container_type;
    
protected:
    container_type __c_;
    
protected:
    container() = default;
    virtual ~container() = default;
    
public:
    value_type at(std::size_t);
    container_type get();
    void clear();
    constexpr std::size_t size() const;
    
    const value_type operator[](std::size_t) const;
};


template <typename _T, std::size_t _I>
typename container<std::array<_T, _I>>::value_type
container<std::array<_T, _I>>::at(std::size_t __i) {
    return __c_.at(__i);
}

template <typename _T, std::size_t _I>
typename container<std::array<_T, _I>>::container_type
container<std::array<_T, _I>>::get() {
    return __c_;
}

template <typename _T, std::size_t _I>
void
container<std::array<_T, _I>>::clear() {
    __c_.fill((_T)0);
}

template <typename _T, std::size_t _I>
constexpr std::size_t
container<std::array<_T, _I>>::size() const {
    return _I;
}

template <typename _T, std::size_t _I>
const typename container<std::array<_T, _I>>::value_type
container<std::array<_T, _I>>::operator[](std::size_t __i) const {
    return __c_.at(__i);
}

}
#endif /* __EVENT_CONTAINER_H__ */
