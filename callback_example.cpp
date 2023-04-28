#include "key_logger/keyboard.h"


void function_ptr_example(event::key::code key, event::action::code action) {
    if (action == event::action::key_down)
        std::cout << "This is from function pointer, key: " << key << std::endl;
}


int main() {
    /*
        [ Function pointer example ]
        - callback type: function pointer(void(*))
        - container type: null_container
        - parameter type: default(key_action_type)
        - mask type: default(key_down, key_up, key_system)
     */
    event::keyboard::logger<
        decltype(&function_ptr_example),
        event::null_container
    > function_logger;
        
    function_logger.set_callback(&function_ptr_example);
    function_logger.start();
    
    
    /*
        [ Member function example ]
        - callback type: member function pointer(void(class::*))
        - container type: null_container
        - parameter type: action_type
        - mask type: default(key_down, key_up, key_system)
     */
    struct mem_fn_example {
        std::string str = "This is from member function, action: ";
        
        void some_function(event::action::code action) {
            if (action == event::action::key_down)
                std::cout << str << action << std::endl;
        }
    };
    
    event::keyboard::logger<
        decltype(&mem_fn_example::some_function),
        event::null_container,
        event::keyboard::action_type
    > mem_fn_logger;
    
    mem_fn_example mem_fn;
    mem_fn_logger.set_callback(&mem_fn, &mem_fn_example::some_function);
    mem_fn_logger.start();
    
    
    /*
        [ Lambda function example ]
        - callback type: lambda function
        - container type: null_container
        - parameter type: key_type
        - mask type: key_up
     */
    std::string str("This is from lambda function, key: ");
    auto lambda_func = [&str](event::key::code key) -> void {
        std::cout << str << key << std::endl;
    };
    
    event::keyboard::logger<
        decltype(lambda_func),
        event::null_container,
        event::keyboard::key_type,
        event::action::key_up
    > lambda_logger;
    
    lambda_logger.set_callback(lambda_func);
    lambda_logger.start();
    
    
    /*
        [ std function example ]
        - callback type: std::function
        - container type: deque(std::deque<key_type>)
        - parameter type: key_type
        - mask type: key_down
     */
    event::keyboard::logger<
        event::key::code,
        event::deque,
        event::keyboard::key_type,
        event::action::key_down
    > std_function_logger;
    
    std_function_logger.set_callback([](event::key::code key) -> void {
        std::cout << "This is from std::function, key: " << key << std::endl;
    });
    std_function_logger.start();
    
    
    /*
        [ Do other stuff... ]
        - Logger runs on other thread
        - It does not block other threads
     
        - Other threads can be blocked only on following function calls
          |- start() -> this function inserts the callback function to __logger_base
          |- stop()  -> this function removes the callback function from __logger_base
     */
    while (function_logger.is_running()) {
        if (not std_function_logger.empty()) {
            auto input = std_function_logger.pop_back();
            
            if (input == event::key::code::escape)
                break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    
    /*
        [ Clean up ]
        - logger(__logger_base) is stopped when there is no callback to invoke
        - logger(__logger_base) restarts when callback is needed
        - logger(__logger_base) is destroyed on program exit(could not be destroyed during program run)
     */
    function_logger.stop();
    mem_fn_logger.stop();
    lambda_logger.stop();
    std_function_logger.stop();
}
