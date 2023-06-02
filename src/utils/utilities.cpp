#include "utilities.h"

bool utilities::GetKeyState(const int vKey) {
  return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}
