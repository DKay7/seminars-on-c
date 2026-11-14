
#include "number.h"

int main() {

  signed char *digits_str_lhs = NULL;
  readDigits(&digits_str_lhs);

  number_t num_lhs = {}; 
  numCtor(&num_lhs, digits_str_lhs);

  // ------------

  signed char *digits_str_rhs = NULL;
  readDigits(&digits_str_rhs);
  
  number_t num_rhs = {}; 
  numCtor(&num_rhs, digits_str_rhs);
  


}


