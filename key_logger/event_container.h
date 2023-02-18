#ifndef __EVENT_CONTAINER_H__
#define __EVENT_CONTAINER_H__

#include <utility>
#include <queue>
#include <deque>
#include <vector>
#include <bitset>
#include <array>
#include <string>

#include "key_code.h"
#include "key_str.h"


template <typename _T>
_T
convert_event(const event::key&, const event::action&) {}

template <>
event::key
convert_event<event::key>(const event::key& __k, const event::action&) {
    return __k;
}

template <>
event::action
convert_event<event::action>(const event::key&, const event::action& __a) {
    return __a;
}

template <>
std::pair<event::key, event::action>
convert_event<std::pair<event::key, event::action>>(const event::key& __k, const event::action& __a) {
    return std::make_pair(__k, __a);
}

template <>
std::pair<event::action, event::key>
convert_event<std::pair<event::action, event::key>>(const event::key& __k, const event::action& __a) {
    return std::make_pair(__a, __k);
}


template <typename _T>
class __event_container {
public:
    typedef _T value_type;
    
protected:
    __event_container() = default;
    ~__event_container() = default;
    __event_container(__event_container const &) = delete;
    
    void push(const event::key&, const event::action&);
    virtual void __push_(value_type&&) = 0;
};


template <typename _T>
void
__event_container<_T>::push(const event::key& __k, const event::action& __a) {
    __push_(convert_event<_T>(__k, __a));
}


template <>
class __event_container <void> {
public:
    typedef void value_type;
    
protected:
    __event_container() = default;
    ~__event_container() = default;
    __event_container(__event_container const &) = delete;
};


template <>
class __event_container <bool> {
public:
    typedef bool value_type;
    
protected:
    __event_container() = default;
    ~__event_container() = default;
    __event_container(__event_container const &) = delete;
    
    virtual void push(const event::key&, const event::action&) = 0;
};


template <typename _ConT>
class event_container {
protected:
    event_container() = delete;
    ~event_container() = delete;
};


template <>
class event_container<void> : public __event_container<void> {
protected:
    typedef __event_container<std::nullptr_t> _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef void                        container_type;
    
protected:
    event_container() = default;
    virtual ~event_container() = default;
};


template <typename _T> _REQUIRES_EVENT_TYPE(_T)
class event_container<std::queue<_T>>
: public __event_container<_T> {
protected:
    typedef __event_container<_T>       _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::queue<value_type>      container_type;
    typedef std::vector<value_type>     split_container;
    
protected:
    container_type __c_;
    
protected:
    event_container() = default;
    virtual ~event_container() = default;
    
public:
    value_type pop();
    container_type& container();
    split_container get_all();
    
    bool empty();
    void clear();
    std::size_t size();
    
protected:
    void __push_(value_type&&);
};


template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::queue<_T>>::value_type
event_container<std::queue<_T>>::pop() {
    auto __tmp = __c_.front();
    __c_.pop();
    return __tmp;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::queue<_T>>::container_type&
event_container<std::queue<_T>>::container() {
    return __c_;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
std::size_t
event_container<std::queue<_T>>::size() {
    return __c_.size();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
bool
event_container<std::queue<_T>>::empty() {
    return __c_.empty();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::queue<_T>>::split_container
event_container<std::queue<_T>>::get_all() {
    std::vector<value_type> __v(__c_.size());
    while (!__c_.empty()) {
        __v.emplace_back(__c_.front());
        __c_.pop();
    }
    return __v;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::queue<_T>>::clear() {
    __c_ = container_type();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::queue<_T>>::__push_(value_type&& __v) {
    __c_.push(__v);
}


template <typename _T> _REQUIRES_EVENT_TYPE(_T)
class event_container<std::deque<_T>>
: public __event_container<_T> {
protected:
    typedef __event_container<_T>       _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::deque<value_type>      container_type;
    typedef std::vector<value_type>     split_container;
    
protected:
    container_type __c_;
    
protected:
    event_container() = default;
    virtual ~event_container() = default;
    
public:
    value_type pop();
    value_type pop_back();
    value_type at(std::size_t);
    container_type& container();
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


template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::deque<_T>>::value_type
event_container<std::deque<_T>>::pop() {
    auto __tmp = __c_.front();
    __c_.pop_front();
    return __tmp;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::deque<_T>>::value_type
event_container<std::deque<_T>>::pop_back() {
    auto __tmp = __c_.back();
    __c_.pop_back();
    return __tmp;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::deque<_T>>::value_type
event_container<std::deque<_T>>::at(std::size_t __i) {
    return __c_.at(__i);
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::deque<_T>>::erase(std::size_t __i) {
    __c_.erase(__c_.begin() + __i);
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::deque<_T>>::container_type&
event_container<std::deque<_T>>::container() {
    return __c_;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
bool
event_container<std::deque<_T>>::empty() {
    return __c_.empty();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
std::size_t
event_container<std::deque<_T>>::size() {
    return __c_.size();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::deque<_T>>::split_container
event_container<std::deque<_T>>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
std::string
event_container<std::deque<_T>>::get_string() {
    std::string __s;
    for (auto& ele : __c_) {
        __s.append(event::string(ele));
    }
    return __s;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::deque<_T>>::clear() {
    __c_.resize(0);
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
const typename event_container<std::deque<_T>>::value_type
event_container<std::deque<_T>>::operator[](std::size_t __i) const {
    return at(__i);
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::deque<_T>>::__push_(value_type&& __v) {
    __c_.push_back(__v);
}


template <typename _T> _REQUIRES_EVENT_TYPE(_T)
class event_container<std::vector<_T>>
: public __event_container<_T> {
protected:
    typedef __event_container<_T>       _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::vector<value_type>     container_type;
    typedef std::vector<value_type>     split_container;
    
protected:
    container_type __c_;
    
protected:
    event_container() = default;
    virtual ~event_container() = default;
    
public:
    split_container pop(std::size_t);
    value_type at(std::size_t);
    container_type& container();
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


template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::vector<_T>>::split_container
event_container<std::vector<_T>>::pop(std::size_t __i) {
    split_container __v {__c_.begin(), __c_.begin() + __i};
    __v.erase(__c_.begin(), __c_.begin() + __i);
    return __v;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::vector<_T>>::value_type
event_container<std::vector<_T>>::at(std::size_t __i) {
    return __c_.at(__i);
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::vector<_T>>::container_type&
event_container<std::vector<_T>>::container() {
    return __c_;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::vector<_T>>::split_container
event_container<std::vector<_T>>::get_all() {
    return split_container {__c_.begin(), __c_.end()};
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
std::string
event_container<std::vector<_T>>::get_string() {
    std::string __s;
    for (auto& ele : __c_) {
        __s.append(event::string(ele));
    }
    return __s;
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
typename event_container<std::vector<_T>>::split_container
event_container<std::vector<_T>>::at(std::size_t __1, std::size_t __2) {
    return split_container {__c_.begin() + __1, __c_.begin() + __2};
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
bool
event_container<std::vector<_T>>::empty() {
    return __c_.empty();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::vector<_T>>::clear() {
    __c_.clear();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
std::size_t
event_container<std::vector<_T>>::size() {
    __c_.size();
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::vector<_T>>::erase(std::size_t __1, std::size_t __2) {
    __c_.erase(__c_.begin() + __1, __c_.begin() + __2);
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
const typename event_container<std::vector<_T>>::value_type
event_container<std::vector<_T>>::operator[](std::size_t __i) const {
    return at(__i);
}

template <typename _T> _REQUIRES_EVENT_TYPE(_T)
void
event_container<std::vector<_T>>::__push_(value_type&& __v) {
    __c_.push_back(__v);
}


template <>
class event_container<std::bitset<event::code_size>>
: public __event_container<bool> {
protected:
    typedef __event_container<bool>         _Base;
public:
    typedef typename _Base::value_type      value_type;
    typedef std::bitset<event::code_size>   container_type;
    
protected:
    container_type __c_;
    
protected:
    event_container() = default;
    virtual ~event_container() = default;
    
public:
    value_type at(std::size_t);
    void clear();
    constexpr std::size_t size() const;
    
    const value_type operator[](std::size_t) const;
};


typename event_container<std::bitset<event::code_size>>::value_type
event_container<std::bitset<event::code_size>>::at(std::size_t __i) {
    return __c_[__i];
}

void
event_container<std::bitset<event::code_size>>::clear() {
    __c_.reset();
}

constexpr std::size_t
event_container<std::bitset<event::code_size>>::size() const {
    return event::code_size;
}

const typename event_container<std::bitset<event::code_size>>::value_type
event_container<std::bitset<event::code_size>>::operator[](std::size_t __i) const {
    return __c_[__i];
}


template <std::size_t _I>
class event_container<std::array<bool, _I>>
: public __event_container<bool> {
protected:
    typedef __event_container<bool>     _Base;
public:
    typedef typename _Base::value_type  value_type;
    typedef std::array<value_type, _I>  container_type;
    
protected:
    container_type __c_;
    
protected:
    event_container() = default;
    virtual ~event_container() = default;
    
public:
    value_type at(std::size_t);
    void clear();
    constexpr std::size_t size() const;
    
    const value_type operator[](std::size_t) const;
};


template <std::size_t _I>
typename event_container<std::array<bool, _I>>::value_type
event_container<std::array<bool, _I>>::at(std::size_t __i) {
    return __c_.at(__i);
}

template <std::size_t _I>
void
event_container<std::array<bool, _I>>::clear() {
    __c_.fill(false);
}

template <std::size_t _I>
constexpr std::size_t
event_container<std::array<bool, _I>>::size() const {
    return __c_.size();
}

template <std::size_t _I>
const typename event_container<std::array<bool, _I>>::value_type
event_container<std::array<bool, _I>>::operator[](std::size_t __i) const {
    return __c_.at(__i);
}

#endif /* __EVENT_CONTAINER_H__ */
