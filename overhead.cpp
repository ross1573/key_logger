#define __RECORD_RDTSC_TIME

#include <numeric>
#include "key_logger/keyboard.h"


auto make_lambda(auto& vec) {
    return [&](const event::key::code&) {
        volatile auto end = __rdtsc();
        auto diff = end - event::logger<event::action::key_down>
                               ::logger_type::callback_invoked_timepoint();
        vec.push_back((std::uint64_t)diff);
    };
}

static constexpr std::size_t logger_size = 100;
using time_vec = std::vector<std::uint64_t>;
using time_vec_arr = std::array<time_vec, logger_size>;
using lambda_type = decltype(make_lambda(time_vec_arr()[-1]));
using logger_type = event::keyboard::logger<lambda_type, event::null_container,
    event::keyboard::key_type, event::action::key_down>;


int main() {
    std::size_t counter = 0;
    std::array<time_vec, logger_size> vec_arr;
    std::array<logger_type, logger_size> logger_arr;
    
    for (auto& logger : logger_arr) {
        auto& vec = vec_arr[counter++];
        vec.reserve(1024);
        
        logger.set_callback(make_lambda(vec));
        logger.start();
    }
    counter = 0;
    
    auto logger2 = event::keyboard::logger<event::null_callback, event::deque,
        event::keyboard::key_type, event::action::key_down>();
    logger2.start();

    while(logger2.is_running()) {
        if (!logger2.empty()) {
            auto input = logger2.back();
            if (input == event::key::code::escape) {
                logger2.stop();
                break;
            }
            
            if (input == event::key::enter) {
                std::cout << logger2.get_string() << std::flush;
                logger2.clear();
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    for (auto& logger : logger_arr) {
        logger.stop();
        auto& vec = vec_arr[counter++];
        
        auto cold_cache_ignore_size = 1;
        auto begin = vec.begin() + cold_cache_ignore_size;
        auto end = vec.end();
        auto vec_size = vec.size() - (std::size_t)cold_cache_ignore_size;
        
        if (vec_size <= 0)
            return 0;

        std::sort(begin, end);
        std::size_t size = (std::size_t)((double)vec_size * 0.97);
        std::size_t left = vec_size - size;

        auto mean_90 = std::accumulate(begin, end - (long)left, (std::uint64_t)0) / size;
        auto mean = std::accumulate(begin, end, (std::uint64_t)0) / vec_size;
        auto worst = *std::max_element(begin, end);
        
        std::cout << "\n[ Logger " << counter << " ]" << std::endl;
        std::cout << "Input size: " << vec_size << '\n'
            << "Mean time: " << mean << '\n'
            << "Mean time(90%): " << mean_90 << '\n'
            << "Worst time: " << worst << '\n' << std::endl;
    }
}

