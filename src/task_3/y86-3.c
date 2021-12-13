#include <stdio.h>

#include <stdlib.h>

//This is the help function that reads y86 binary code from a file
unsigned int loadBinFromFile(const char * filename, unsigned char memory[], unsigned int memsize);
char * registerNames[] = {
  "%eax",
  "%ecx",
  "%edx",
  "%ebx",
  "%esp",
  "%ebp",
  "%esi",
  "%edi",
  "UNKOWN_REGISTER"
};
/* 00 & 10 */
char * simple[] = {
  "halt",
  "nop"
}; // 00 - 10
/* 20 - 26 */
char * move[] = {
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
char * move_intermediate[] = {
  "irmovl",
  "rmmovl",
  "mrmovl"
};
/* A0 - B0 */
char * stack[] = {
  "pushl",
  "popl"
};
/* 60 - 63 */
char * al[] = {
  "addl",
  "subl",
  "andl",
  "xorl"
};
/* 70 - 76 */
char * jump[] = {
  "jmp",
  "jle",
  "jl",
  "je",
  "jne",
  "jge",
  "jg"
};
/* 80 - 90 */
char * fn[] = {
  "call",
  "ret"
};

typedef struct {
  int size;
  char *opCode;
}
opCodeStruct;

opCodeStruct getOpCode(int opCodeHex, opCodeStruct * opSt) {

  if (opCodeHex >= 0x00 && opCodeHex <= 0x10) {
    if (opCodeHex == 0x00) {
      opSt -> size = 1;
      opSt -> opCode = simple[0];
      return *opSt;
    } else if (opCodeHex == 0x10) {
      opSt -> size = 1;
      opSt -> opCode = simple[1];
      return *opSt;
      // return (simple[1]);
    }
  } else if (opCodeHex >= 0x20 && opCodeHex <= 0x26) {
    opSt -> size = 2;
    opSt -> opCode = move[opCodeHex - 0x20];
    return *opSt;
    // return (move[opCodeHex - 0x20]);
  } else if (opCodeHex >= 0x30 && opCodeHex <= 0x50) {
    if (opCodeHex == 0x30) {
      opSt -> size = 5;
      opSt -> opCode = move_intermediate[0];
      return *opSt;
      // return (move_intermediate[0]);
    } else if (opCodeHex == 0x40) {
      opSt -> size = 5;
      opSt -> opCode = move_intermediate[1];
      return *opSt;
      // return (move_intermediate[1]);
    } else if (opCodeHex == 0x50) {
      opSt -> size = 5;
      opSt -> opCode = move_intermediate[2];
      return *opSt;
      // return (move_intermediate[2]);
    }
  } else if (opCodeHex >= 0xA0 && opCodeHex <= 0xB0) {

    if (opCodeHex == 0xA0) {
      opSt -> size = 2;
      opSt -> opCode = stack[0];
      return *opSt;
      // return (stack[0]);
    } else if (opCodeHex == 0xB0) {
      opSt -> size = 2;
      opSt -> opCode = stack[1];
      return *opSt;
      // return (stack[1]);
    }
  } else if (opCodeHex >= 0x60 && opCodeHex <= 0x63) {
    opSt -> size = 2;
    opSt -> opCode = al[opCodeHex - 0x60];
    return *opSt;

    // return (al[opCodeHex - 0x60]);
  } else if (opCodeHex >= 0x70 && opCodeHex <= 0x76) {
    opSt -> size = 2;
    opSt -> opCode = jump[opCodeHex - 0x70];
    return *opSt;
    // return (jump[opCodeHex - 0x70]);
  } else if (opCodeHex >= 0x80 && opCodeHex <= 0x90) {
    if (opCodeHex == 0x80) {
      opSt -> size = 2;
      opSt -> opCode = fn[0];
      return *opSt;
      // return (fn[0]);
    } else if (opCodeHex == 0x90) {
      opSt -> size = 2;
      opSt -> opCode = fn[1];
      return *opSt;
      // return (fn[1]);
    }
  }
  opSt -> size = 0;
  opSt -> opCode = "TODO: undisaasembled opcode";
  return *opSt;
}
int main(int argc, char ** argv) {
  unsigned int MAX_MEMSIZE = 4096;
  unsigned char memory[MAX_MEMSIZE]; //This array represents the 4KB memory space
  unsigned int PC = 0; //This is the initial program counter address

  if (argc < 2) {
    printf("Usage: please specify a y86 binary program file in the argument.\n");
    return -1;
  }

  unsigned int program_size = loadBinFromFile(argv[1], memory, MAX_MEMSIZE);

  if (program_size == 0)
    return 0;

  //TODO: Implement your disassembler from here
  //At the moment, it simply display each byte from the memory.
  while (PC < program_size) {
    opCodeStruct tmp;
    getOpCode(memory[PC], &tmp);
    printf("%02X size: %d opcode: %s\n", memory[PC], tmp.size, tmp.opCode);

    PC++;
  }

  return 0;
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function reads in a y86 machine bytecode from file and
store them in an unsigned char array.
******************************************************************************/
unsigned int loadBinFromFile(const char * filename, unsigned char memory[], unsigned int memsize) {
  unsigned int bytes_read = 0;

  unsigned int file_size = 0;

  FILE * pfile = fopen(filename, "rb");

  if (!pfile) {
    printf("Unable to load file %s, please check if the path and name are correct.\n", filename);
    return 0;
  }

  fseek(pfile, 0, SEEK_END);
  file_size = ftell(pfile);
  rewind(pfile);

  if (file_size > memsize) {
    printf("Program size exceeds memory size of %d.\n", memsize);
    return 0;
  }

  bytes_read = fread(memory, 1, file_size, pfile);

  if (bytes_read != file_size) {
    printf("Bytes read does not match the file size.\n");
    return 0;
  }

  fclose(pfile);

  return bytes_read;
}