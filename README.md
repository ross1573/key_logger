# KeyLogger
Multipurpose key logger for Windows and macOS writhen in C++  
Requires C++ 20, tested on apple clang 14 and VC++ 14  
Overhead is about 1~2 mircoseconds(on first callback invocation)  
~~~
On macOS, please add 'ApplicationServices.framework' to the project
~~~
-----
## Usage  
1. create an logger object
```c++
auto example_logger = event::keyboard::logger<callback, container, traits, event_mask...>();
```  
2. set callback function
```c++
example_logger.set_callback(callback_function);
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
- Null container
  ```c++
  event::null_container = event::null_template_type
  ```  
- Stored Containers  
  ```c++
  event::vector -> std::vector  
  event::queue  -> std::queue  
  event::deque  -> std::deque  
  ```  
- Table Containers  
  ```c++
  event::array  -> std::array
  event::bitset -> std::bitset
  ```    
  
### Callback Function Types  
- Null callback
  ```c++
  event::null_container = event::null_type
  ```  
- Function types
  ```c++
  type                      -> std::function<void(type)>  
  return_type(param...)     -> std::function<return_type(parameter...)>  
  return_type(*)(param...)        
  return_type(class::*)(param...) 
  std::function<return_type(param)...>  
  lambda  
  ```  
  
### Event Masks
- Key press
  ```c++
  event::action::key_down
  ```
- Key release
  ```c++
  event::action::key_up
  ```  
- System keys
  ```c++
  event::action::key_system
  ```  

-----
## Windows

https://user-images.githubusercontent.com/44907014/218326797-0e62f713-73d0-4514-9d4e-2e62cf3a7782.mov  
  
## macOS

https://user-images.githubusercontent.com/44907014/218326907-1e9c2974-ac6f-4a4e-9428-49386e50e61b.mov
