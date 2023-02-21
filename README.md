# KeyLogger
Simple key logger for Windows and macOS writhen in C++
~~~
On macOS, please add 'ApplicationServices.framework' to the project
~~~
-----
## Usage  
1. create an object of class key_logger
```c++
auto example_logger = key_logger<container_type, callback_type, event_mask>();
```
2. set callback function  
```c++
example_logger.set_callback([&should_end](event::key key) mutable {
    std::cout << "Key Pressed: " << event::string(key) << ", " << key << std::endl;
});
```
3. start
 ```c++
example_logger.start();
```
-----
## Supported Features  
### Container Types
- Stored Containers
  - vector
    ~~~
    key_vector = std::vector<event::key>
    action_vector = std::vector<event::action>
    ~~~
  - queue
    ~~~
    key_queue = std::queue<event::key>
    action_queue = std::queue<event::queue>
    ~~~
  - deque
    ~~~
    key_deque = std::deque<event::key>
    action_deque = std::deque<event::action>
    ~~~
- Table Containers
  - array
    ~~~
    key_array = std::array<bool, event::code_size>
    ~~~
  - bitset
    ~~~
    key_bitset = std::bitset<event::code_size>
    ~~~
  
### Callback Function Types
- void
  ~~~
  cannot register callback function, non callback function will be invoked
  ~~~
- single parameter
  ~~~
  void(event::key)
  void(event::action)
  ~~~
- two parameter
  ~~~
  void(event::key, event::action)
  ~~~
  
### Event Masks
- Key press
  ~~~
  event::action::key_down
  ~~~
- key release
  ~~~
  event::action::key_up
  ~~~

-----
## Windows

https://user-images.githubusercontent.com/44907014/218326797-0e62f713-73d0-4514-9d4e-2e62cf3a7782.mov  
  
## macOS

https://user-images.githubusercontent.com/44907014/218326907-1e9c2974-ac6f-4a4e-9428-49386e50e61b.mov
