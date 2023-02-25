# KeyLogger
Multipurpose key logger for Windows and macOS writhen in C++  
Requires C++ 20, tested on apple clang 14.0.0 and VC++ 14.30  
Performance is about 0~2 mircoseconds(on full optimization -Ofast/-O2)
~~~
On macOS, please add 'ApplicationServices.framework' to the project
~~~
-----
## Usage  
1. create an object of class key_logger
```c++
auto example_logger = event::key::logger<container_type, callback_type, event_mask...>();
```
2. set callback function  
```c++
example_logger.set_callback([=](event::key::code key) mutable {
    std::cout << "Key Pressed: " << event::string(key) << std::endl;
});
```
3. start
 ```c++
example_logger.start();
```
4. cleanup
 ```c++
example_logger.stop();
```
-----
## Supported Features  
### Container Types
- Stored Containers
  - vector
    ```c++
    key_vector = std::vector<event::key::code>
    action_vector = std::vector<event::action::code>
    ```
  - queue
    ```c++
    key_queue = std::queue<event::key::code>
    action_queue = std::queue<event::action::code>
    ```
  - deque
    ```c++
    key_deque = std::deque<event::key::code>
    action_deque = std::deque<event::action::code>
    ```
- Table Containers
  - array
    ```c++
    key_array = std::array<bool, event::key::code_size>
    ```
  - bitset
    ```c++
    key_bitset = std::bitset<event::key::code_size>
    ```
  
### Callback Function Types
- void
  ~~~
  cannot register callback function, non callback function will be invoked
  ~~~
- single parameter
  ```c++
  void(event::key::code)
  void(event::action::code)
  ```
- two parameter
  ```c++
  void(event::key::code, event::action::code)
  ```
  
### Event Masks
- Key press
  ```c++
  event::action::key_down
  ```
- key release
  ```c++
  event::action::key_up
  ```

-----
## Windows

https://user-images.githubusercontent.com/44907014/218326797-0e62f713-73d0-4514-9d4e-2e62cf3a7782.mov  
  
## macOS

https://user-images.githubusercontent.com/44907014/218326907-1e9c2974-ac6f-4a4e-9428-49386e50e61b.mov
