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