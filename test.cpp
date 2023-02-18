#include <chrono>

#include "key_logger"


void example_press(event::key key) {
    std::cout << "Key Pressed: " << event::string(key) << ", " << key << std::endl;
}

void example_release(event::key key) {
    std::cout << "Key Released: " << event::string(key) << std::endl;
}


int main() {
    auto logger = key_logger<key_deque, void, event::action::key_down>();
    logger.start();
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << logger.get_string() << std::endl;
        logger.clear();
    }
}
