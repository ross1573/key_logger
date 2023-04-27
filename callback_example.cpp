#include "key_logger/keyboard.h"


void function_ptr_example(event::key::code key, event::action::code action) {
    if (action == event::action::key_down)
        std::cout << "This is from function pointer, key: " << key << std::endl;
}


int main() {
    /* Function pointer example */
    event::keyboard::logger<
        decltype(&function_ptr_example),
        event::null_container
    > function_logger;
    
    function_logger.set_callback(&function_ptr_example);
    function_logger.start();
    
    
    /* Member function example */
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
    
    
    /* Lambda function example */
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
    
    
    /* std::function example */
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
    
    
    /* Do other stuff... */
    while (function_logger.is_running()) {
        if (not std_function_logger.empty()) {
            auto input = std_function_logger.pop_back();
            
            if (input == event::key::code::escape)
                break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    
    /* Clean up */
    function_logger.stop();
    mem_fn_logger.stop();
    lambda_logger.stop();
    std_function_logger.stop();
}
