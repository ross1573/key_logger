#include "key_logger"


int main() {
    bool should_end = false;
    auto logger = key_logger<void, void(event::key), event::action::key_down>();
    
    logger.set_callback([&should_end](event::key key) mutable {
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
