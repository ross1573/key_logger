#include "key_logger/keyboard.h"


int main() {
    /*
        [ Queue example ]
        - callback type: null_callback
        - container type: queue(std::queue)
        - value type: default(key_action_type)
        - mask type: default(key_down, key_up, key_system)
     */
    event::keyboard::logger<
        event::null_callback,
        event::queue
    > queue_logger;
    queue_logger.start();
    
    
    /*
        [ Deque example ]
        - callback type: null_callback
        - container type: deque(std::deque)
        - value type: key_type
        - mask type: key_down, key_system
     */
    event::keyboard::logger<
        event::null_callback,
        event::deque,
        event::keyboard::traits::key,
        event::action::key_down, event::action::key_system
    > deque_logger;
    deque_logger.start();
    
    
    /*
        [ Vector example ]
        - callback type: null_callback
        - container type: vector(std::vector)
        - value type: action_type
        - mask type: key_down
     */
    event::keyboard::logger<
        event::null_callback,
        event::vector,
        event::keyboard::traits::key_action,
        event::action::key_down
    > vector_logger;
    vector_logger.start();
    
    
    /*
        [ Array example ]
        - callback type: null_callback
        - container type: array(std::array)
        - container size: key_type::code_size
        - value type: action_type
        - mask type: key_down
     */
    event::keyboard::logger<
        event::null_callback,
        event::array,
        event::keyboard::traits::action
    > array_logger;
    array_logger.start();
    
    
    /*
        [ Bitset example ]
        - callback type: null_callback
        - container type: bitset(std::bitset)
        - container size: key_type::code_size
        - value type: bool
        - mask type: key_down
     */
    event::keyboard::logger<
        event::null_callback,
        event::bitset
    > bitset_logger;
    bitset_logger.start();
    
    
    /*
        [ Do other stuff... ]
        - Logger runs on other thread
        - It does not block other threads
     
        - Other threads can be blocked only on following function calls
          |- start() -> insert callback function to __logger_base
          |- stop()  -> remove callback function from __logger_base
     */
    while (deque_logger.is_running()) {
        if (not deque_logger.empty()) {
            auto input = deque_logger.back();
            
            if (vector_logger.is_running() && not vector_logger.empty()) {
                std::cout << "Vector Logger: " << vector_logger.get_string() << std::endl;
                vector_logger.clear();
            }
            
            switch (input) {
                case event::key::code::escape:
                    deque_logger.stop();
                    break;
                case event::key::code::enter:
                    std::cout << deque_logger.get_string();
                    vector_logger.clear();
                    deque_logger.clear();
                    break;
                case event::key::code::q:
                    std::cout << "\nExiting vector logger...";
                    vector_logger.stop();
                    vector_logger.clear();
                    deque_logger.pop_back();
                    break;
                case event::key::code::s:
                    std::cout << "\nStarting vector logger...";
                    vector_logger.start();
                    deque_logger.pop_back();
                    break;
                case event::key::code::backspace:
                    deque_logger.pop_back();
                    if (not deque_logger.empty())
                        deque_logger.pop_back();
                default: break;
            };
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    
    /*
        [ Clean up ]
        - logger(__logger_base) is stopped when there is no callback to invoke
        - logger(__logger_base) restarts when callback is newly submited
        - logger(__logger_base) is destroyed on program exit
     */
    queue_logger.stop();
    deque_logger.stop();
    vector_logger.stop();
    array_logger.stop();
    bitset_logger.stop();
}
