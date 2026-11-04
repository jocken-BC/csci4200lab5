#include <stdio.h>
#include <stdint.h>

// Helper function to print an 8-bit integer in true binary string format
void print_binary8(uint8_t num) {
  // Loop through all 8 bits from highest position (128) down to lowest (1)
  for (int i = 7; i >= 0; i--) {
    // Use an active bitmask to see if the bit at position 'i' is turned on
    uint8_t mask = (1 << i);
    if ((num & mask) != 0) {
      printf("1");
    } else {
      printf("0");
    }
  }
}

int main() {
  uint8_t a = 5;  // 00000101
  uint8_t b = 6;  // 00000110

  printf("\n\n=== Part 1: Bitwise Logic Gates ===\n");
  
  printf("Variable a:     "); print_binary8(a);     printf("  (Decimal: %d)\n", a);
  printf("Variable b:     "); print_binary8(b);     printf("  (Decimal: %d)\n", b);
  printf("-----------------------------------------\n");
    
  // AND Operation
  printf("AND (a & b):    "); print_binary8(a & b); printf("  (Decimal: %d)\n", a & b);
    
  // OR Operation
  printf("OR  (a | b):    "); print_binary8(a | b); printf("  (Decimal: %d)\n", a | b);
    
  // XOR Operation
  printf("XOR (a ^ b):    "); print_binary8(a ^ b); printf("  (Decimal: %d)\n", a ^ b);
    
  // NOT Operation (Inversion)
  printf("NOT (~a):       "); print_binary8(~a);    printf("  (Decimal: %d)\n", (uint8_t)~a);


  printf("\n\n=== Part 2: Bit Shifting Mechanics ===\n");

  uint8_t x = 3;  // 00000011
  printf("Original x:     "); print_binary8(x);     printf("  (Decimal: %d)\n", x);
  
  // Left Shift by 2
  uint8_t left_shift = x << 2;
  printf("Left (x << 2):  "); print_binary8(left_shift); printf("  (Decimal: %d)\n", left_shift);
    
  uint8_t y = 24; // 00011000
  printf("Original y:     "); print_binary8(y);     printf("  (Decimal: %d)\n", y);
    
  // Right Shift by 3
  uint8_t right_shift = y >> 3;
  printf("Right (y >> 3): "); print_binary8(right_shift); printf("  (Decimal: %d)\n", right_shift);

  printf("\n\n=== Part 3: Arithmetic vs. Logical Right Shifts ===\n");
    
  // A negative number in 8-bit two's complement (-16)
  // Bit pattern: 11110000 (The highest bit is 1, indicating negative)
  int8_t  signed_neg   = -16; 
  uint8_t unsigned_neg = 240; // Exactly the same bit pattern (11110000)
    
  printf("Signed Base (-16):   "); print_binary8((uint8_t)signed_neg);   printf("  (Decimal: %d)\n", signed_neg);
  printf("Unsigned Base (240): "); print_binary8(unsigned_neg);          printf("  (Decimal: %d)\n", unsigned_neg);
  printf("-----------------------------------------\n");

  // 1. ARITHMETIC SHIFT (On the signed variable)
  // In C, right-shifting a signed type performs an arithmetic shift.
  // It duplicates the sign bit (1) into the vacated high-order slots.
  int8_t arithmetic_result = signed_neg >> 2;
  printf("Arithmetic (>> 2):   "); print_binary8((uint8_t)arithmetic_result); printf("  (Decimal: %d)\n", arithmetic_result);

  // 2. LOGICAL SHIFT (On the unsigned variable)
  // In C, right-shifting an unsigned type performs a logical shift.
  // It always pads the vacated high-order slots with 0s, regardless of the sign bit.
  uint8_t logical_result = unsigned_neg >> 2;
  printf("Logical    (>> 2):   "); print_binary8(logical_result);     printf("  (Decimal: %d)\n", logical_result);
  printf("\n");

  return 0;
}