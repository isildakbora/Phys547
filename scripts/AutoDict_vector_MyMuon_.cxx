#include "vector"
#include "MyMuon.h"
#ifdef __CINT__ 
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#pragma link C++ class vector<MyMuon>+;
#pragma link C++ class vector<MyMuon>::*;
#ifdef G__VECTOR_HAS_CLASS_ITERATOR
#pragma link C++ operators vector<MyMuon>::iterator;
#pragma link C++ operators vector<MyMuon>::const_iterator;
#pragma link C++ operators vector<MyMuon>::reverse_iterator;
#endif
#endif
