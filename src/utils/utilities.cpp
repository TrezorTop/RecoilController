#include "utilities.h"

bool utilities::GetKeyState(int vKey) {
  return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}
