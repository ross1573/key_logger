#include "key_logger/keyboard.h"


int main() {
    /* Queue example */
    event::keyboard::logger<
        event::null_callback,
        event::queue
    > queue_logger;
    queue_logger.start();
    
    
    /* Deque example */
    event::keyboard::logger<
        event::null_callback,
        event::deque,
        event::keyboard::key_type,
        event::action::key_down
    > deque_logger;
    deque_logger.start();
    
    
    /* Vector example */
    event::keyboard::logger<
        event::null_callback,
        event::vector,
        event::keyboard::action_type,
        event::action::key_down
    > vector_logger;
    vector_logger.start();
    
    
    /* Array example */
    event::keyboard::logger<
        event::null_callback,
        event::array,
        event::keyboard::action_type
    > array_logger;
    array_logger.start();
    
    
    /* Bitset example */
    event::keyboard::logger<
        event::null_callback,
        event::bitset
    > bitset_logger;
    bitset_logger.start();
    
    
    /* Do other stuff... */
    while (deque_logger.is_running()) {
        if (not deque_logger.empty()) {
            auto input = deque_logger.back();
            
            switch (input) {
                case event::key::code::escape:
                    deque_logger.stop();
                    break;
                case event::key::code::enter:
                    std::cout << deque_logger.get_string();
                    deque_logger.clear();
                    break;
                case event::key::code::q:
                    std::cout << "\nExiting vector logger...";
                    vector_logger.stop();
                    deque_logger.pop_back();
                    break;
                case event::key::code::backspace:
                    deque_logger.pop_back();
                    deque_logger.pop_back();
                default: break;
            };
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    
    /* Clean up */
    queue_logger.stop();
    deque_logger.stop();
    vector_logger.stop();
    array_logger.stop();
    bitset_logger.stop();
}
