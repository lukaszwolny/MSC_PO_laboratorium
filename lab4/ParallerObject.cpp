#include "ParallerObject.h"

double ParallerObject::symuluj(double u){
    double tmp = 0;
    for(auto o: m_child){
        tmp += o->symuluj(u);
    }
    return tmp;
};

void ParallerObject::Add(Komponent *component){
    m_child.push_back( component );
}
void ParallerObject::Remove(Komponent *component){
    m_child.remove(component);
}