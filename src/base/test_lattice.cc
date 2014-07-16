#include "lattice.cc"
#include <stdio.h>
int main(){
  SLevel * l = new SLevel();
  l->name = "low";
  SLevel * h = new SLevel();
  h->name = "high";

  Lattice::addL(l);
  if(Lattice::isLeq(l,l))
    fprintf(stderr, "l <= l\n");
  else
    fprintf(stderr, " l NLEQ l?!?!?\n");

  Lattice::addL(h);
  if(Lattice::isLeq(l,h))
    fprintf(stderr, "l <= h to early!\n");
  else
    fprintf(stderr, "l and h are nleq by default\n");

  Lattice::setLeq(l,h);
  if(Lattice::isLeq(l,h))
    fprintf(stderr, "l <= h\n");
  else
    fprintf(stderr, " l nleq h even after set!!\n");
}
