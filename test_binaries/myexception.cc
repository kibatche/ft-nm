#include <iostream>

void test() {
    throw std::runtime_error("Erreur !");
}

int main() {
    try {
        test();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}