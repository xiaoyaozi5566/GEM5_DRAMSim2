#include <stddef.h>
#include <string>
#include <map>

class SLevel {
  public:
  std::string name;
};

class Lattice {
  private:
  // assoc arr of the form level l -> level -r -> (l <= r)?
  std::map< SLevel*, std::map< SLevel*, bool > > lmap;
  Lattice();
  static Lattice * instance;
  
  public:
  static Lattice * get();
  static bool isLeq(SLevel * l, SLevel * r);
  static void addL(SLevel * s);
  static void setLeq(SLevel * l, SLevel * r);

  // Will only be written if needed
  // SLevel supremum();
  // SLevel infimum();
};
