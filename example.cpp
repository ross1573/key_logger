#include "key_logger/key_logger"


int main() {
    bool should_end = false;
    
    auto logger_vec = event::key::logger<event::key::vector, void(event::key::code), event::action::key_down>();
    logger_vec.set_callback([&logger_vec](event::key::code key) mutable {
        std::cout << "This callback is from logger_vec: " << event::string(key) << std::endl;
        if (key == event::key::q) {
            logger_vec.stop();
        }
    });
    logger_vec.start();
    
    auto logger_queue = event::key::logger<std::queue<int>, void(const event::key::code&), event::action::key_down>();
    logger_queue.set_callback([&should_end](const event::key::code& key) mutable {
        std::cout << "This callback is from logger_queue: " << event::string(key) << std::endl;
        if (key == event::key::escape) {
            should_end = true;
        }
    });
    logger_queue.start();
    
    auto logger_array = event::key::logger<std::queue<int>, void(const event::key::code&), event::action::key_down>();
    logger_array.set_callback([=](const event::key::code& key) mutable {
        std::cout << "This callback is from logger_array: " << event::string(key) << std::endl;
    });
    logger_array.start();
    
    while (!should_end) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    logger_vec.stop();
    logger_queue.stop();
    logger_array.stop();
}
