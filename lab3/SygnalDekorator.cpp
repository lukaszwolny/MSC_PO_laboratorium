#include "SygnalDekorator.h"

SygnalDekorator::SygnalDekorator(SygnalGenerator* sygnalgenerator) : sygnalgenerator_(sygnalgenerator) {}

double SygnalDekorator::generuj() {
    return this->sygnalgenerator_->generuj();
}