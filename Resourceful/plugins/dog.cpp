#include <iostream>

extern "C"
void sayHello(std::ostream& os) {
  os << "woof";
}