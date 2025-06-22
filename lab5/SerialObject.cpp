#include "SerialObject.h"

double SerialObject::symuluj(double u){
  for(auto o: m_child){
    u = o->symuluj(u);
  }
  return u;
};

  void SerialObject::Add(Komponent *component){
    m_child.push_back( component );
  }
  void SerialObject::Remove(Komponent *component){
    m_child.remove(component);
  }


  Komponent* SerialObject::getChild(int n){
	  if (n >= 0 && n < static_cast<int>(m_child.size())) {
		  auto it = m_child.begin();
		  std::advance(it, n);
		  return *it;
	  }
	  return nullptr;
  }


  size_t SerialObject::getChildrenCount() const{
	  return m_child.size();
  }
