#include <stdio.h>

#include <stdlib.h>

#include <string.h>


int convertStrToByteCode(const char * str, unsigned char inst[], int size);
void printOpCode(int opCodeHex);

int main(int argc, char ** argv) {
  FILE * pFile = NULL;

  char buffer[15];

  if (argc < 2) {
    pFile = fopen("./test1.txt", "r");
  } else {
    pFile = fopen(argv[1], "r");
  }

  if (pFile == NULL) {
    printf("Error open test file, please make sure they exist.\n");

    return 0;
  }

  while (fgets(buffer, 15, pFile)) {
    //This unsigned char array stores an instruction read from the file
    //As the largest y86 instruction is 6 bytes, there are 6 unsigned char in the array where
    //each represents a byte.
    unsigned char instruction[6] = {
      0,
      0,
      0,
      0,
      0,
      0
    };
    convertStrToByteCode(buffer, instruction, 6);
    // From here, your task is to complete the implementation so that all y86 opcodes can be disassembled.
    //Any undisassembled opcode should display as "TODO: undisaasembled opcode"
    printOpCode(instruction[0]);
    // printf("%c",*getOpCode(instruction[0]));
    // printf("TODO: undisaasembled opcode. The first byte of the instruction is 0x%X\n", instruction[0]);
  }

  fclose(pFile);

  return 0;
}

void printOpCode(int opCodeHex) {
  /* 00 & 10 */
  char simple[][10] = {
    "halt",
    "nop"
  }; // 00 - 10
  /* 20 - 26 */
  char move[][10] = {
    "rrmovl",
    "cmovle",
    "cmovl",
    "cmove",
    "cmovne",
    "cmovge",
    "cmovg",
    "irmovl",
    "rmmovl",
    "mrmovl"
  };
  /* 30 - 50 */
  char move_intermediate[][10] = {
    "irmovl",
    "rmmovl",
    "mrmovl"
  };
  /* A0 - B0 */
  char stack[][10] = {
    "pushl",
    "popl"
  };
  /* 60 - 63 */
  char al[][10] = {
    "addl",
    "subl",
    "andl",
    "xorl"
  };
  /* 70 - 76 */
  char jump[][10] = {
    "jmp",
    "jle",
    "jl",
    "je",
    "jne",
    "jge",
    "jg"
  };
  /* 80 - 90 */
  char fn[][10] = {
    "call",
    "ret"
  };

  if (opCodeHex >= 0x00 && opCodeHex <= 0x10) {
    if (opCodeHex == 0x00) {
      printf("%s\n", simple[0]);
    } else if (opCodeHex == 0x10) {
      printf("%s\n", simple[1]);
    }
  } else if (opCodeHex >= 0x20 && opCodeHex <= 0x26) {
    printf("%s\n", move[opCodeHex - 0x20]);
  } else if (opCodeHex >= 0x30 && opCodeHex <= 0x50) {
      if (opCodeHex == 0x30) {
        printf("%s\n", move_intermediate[0]);
      } else if (opCodeHex == 0x40) {
        printf("%s\n", move_intermediate[1]);
      } else if (opCodeHex == 0x50) {
        printf("%s\n", move_intermediate[2]);
      }      
  } else if (opCodeHex >= 0xA0 && opCodeHex <= 0xB0) {
    
    if (opCodeHex == 0xA0) {
      printf("%s\n", stack[0]);
    } else if (opCodeHex == 0xB0) {
      printf("%s\n", stack[1]);
    }
  } else if (opCodeHex >= 0x60 && opCodeHex <= 0x63) {
    printf("%s\n", al[opCodeHex - 0x60]);
  } else if (opCodeHex >= 0x70 && opCodeHex <= 0x76) {
    printf("%s\n", jump[opCodeHex - 0x70]);
  } else if (opCodeHex >= 0x80 && opCodeHex <= 0x90) {
    if (opCodeHex == 0x80) {
      printf("%s\n", fn[0]);
    } else if (opCodeHex == 0x90) {
      printf("%s\n", fn[1]);
    }
  } else {
    printf("TODO: undisaasembled opcode");
  }
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode(const char * str, unsigned char inst[], int size) {
  int numHexDigits = 0;
  char * endstr;
  //Each instruction should consist of at most 12 hex digits
  numHexDigits = strlen(str) - 1;
  //Convert the string to integer, N.B. this integer is in decimal
  long long value = strtol(str, & endstr, 16);

  int numBytes = numHexDigits >> 1;
  int byteCount = numHexDigits >> 1;

  while (byteCount > 0) {
    unsigned long long mask = 0xFF;
    unsigned long shift = (numBytes - byteCount) << 3;

    inst[byteCount - 1] = (value & (mask << shift)) >> shift;
    byteCount--;
  }

  //Return the size of the instruction in bytes
  return numHexDigits >> 1;
}