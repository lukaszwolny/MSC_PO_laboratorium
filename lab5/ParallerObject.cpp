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

Komponent* ParallerObject::getChild(int n) {
	if (n >= 0 && n < static_cast<int>(m_child.size())) {
		auto it = m_child.begin();
		std::advance(it, n);
		return *it;
	}
	return nullptr;
}

size_t ParallerObject::getChildrenCount() const {
	return m_child.size();
}