#include "Komponent.h"

Komponent* Komponent::getChild(int n) {
    return this->component;
}

size_t Komponent::getChildrenCount() const {
    return (component != nullptr) ? 1 : 0;
}