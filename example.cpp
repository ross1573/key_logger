#include "key_logger"


int main() {
    bool should_end = false;
    auto logger = event::key::logger<std::vector<int>, void(event::key::code), event::action::key_down, event::action::key_up>();
    
    logger.set_callback<0>([&should_end](event::key::code key) mutable {
        std::cout << "Key Pressed: " << event::string(key) << ", " << key << std::endl;
        if (key == event::key::q) {
            should_end = true;
        }
    });
    logger.start();
    
    while (!should_end) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
