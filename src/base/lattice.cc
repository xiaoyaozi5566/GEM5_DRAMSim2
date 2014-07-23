#include "lattice.hh"

Lattice* Lattice::instance = NULL;
Lattice::Lattice(){
  lmap = *(new std::map< SLevel*, std::map< SLevel*, bool > >());
}

Lattice * Lattice::get(){
  if(!instance)
    instance = new Lattice();
  return instance;
}

bool Lattice::isLeq(SLevel* l, SLevel* r){
  return get()->lmap[l][r];
}

void Lattice::addL(SLevel* s){
  // Add the new level to the entry for each existing level, defaulting at nleq
  for(std::map< SLevel*, std::map< SLevel*, bool> >::iterator it =
      get()->lmap.begin(); it!=get()->lmap.end(); ++it){
    get()->lmap[it->first][s] = false;
  }

  // Add an entry for the new level.
  get()->lmap[s] = *(new std::map< SLevel*, bool >());

  // Add each existing level to the new entry, defaulting at nleq
  for(std::map< SLevel*, std::map< SLevel*, bool> >::iterator it =
      get()->lmap.begin(); it!=get()->lmap.end(); ++it){
    get()->lmap[s][it->first] = false;
  }

  // Make the entry leq itself
  get()->lmap[s][s] = true;
}

void Lattice::setLeq(SLevel* l, SLevel* r){
  get()->lmap[l][r] = true;
}
