#include "KeyLogger.h"


void press_callback(KEY_CODE key) {
    std::cout << "Key pressed: " << KeyLogger::convertToString(key) << std::endl;
}


int main() {
    KeyLogger::getInstance()->setPressCallback(press_callback)->run(false);
}
