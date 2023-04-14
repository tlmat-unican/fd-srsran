#include <iostream>

union myUnion
{
  uint32_t type0_alloc;
  uint16_t type1_alloc;
  uint16_t type2_alloc;
};

int main(void)
{

  std::cout << "Sizeof my union = " << sizeof(myUnion)<< std::endl;
}
