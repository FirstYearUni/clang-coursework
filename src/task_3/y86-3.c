#include <stdio.h>

#include <stdlib.h>

#include <string.h>
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

// defining a structure to contain a s string opCode and its size
typedef struct {
  int size;
  char * opCode;
}
opCodeStruct;

// function sets the size, opCode string of a passed hex number
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
      opSt -> size = 6;
      opSt -> opCode = move_intermediate[0];
      return *opSt;
      // return (move_intermediate[0]);
    } else if (opCodeHex == 0x40) {
      opSt -> size = 6;
      opSt -> opCode = move_intermediate[1];
      return *opSt;
      // return (move_intermediate[1]);
    } else if (opCodeHex == 0x50) {
      opSt -> size = 6;
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
    opSt -> size = 5;
    opSt -> opCode = jump[opCodeHex - 0x70];
    return *opSt;
    // return (jump[opCodeHex - 0x70]);
  } else if (opCodeHex >= 0x80 && opCodeHex <= 0x90) {
    if (opCodeHex == 0x80) {
      opSt -> size = 5;
      opSt -> opCode = fn[0];
      return *opSt;
      // return (fn[0]);
    } else if (opCodeHex == 0x90) {
      opSt -> size = 1;
      opSt -> opCode = fn[1];
      return *opSt;
      // return (fn[1]);
    }
  }
  opSt -> size = 0;
  opSt -> opCode = "halt";
  return *opSt;
}
//This array returns indexes of the  registermnemonics in the registerNames
// given the register hex number

int * getRegIndexes(int hex) {

  int regIndexes[2] = {
    0,
    0
  };
  regIndexes[0] = hex / 0x10;
  regIndexes[1] = hex % 0x10;
  // printf("%s %d %s %d \n", registerNames[regIndexes[0]], regIndexes[0], registerNames[regIndexes[1], regIndexes[1]]);
  return regIndexes;
}

char * convertIntToString(int src, char isJumpInstruction) {
  char str[100];
  sprintf(str, isJumpInstruction == 0 ? "%d" : "%u", src);
  char * result = str;
  return result;
}
long ConvertLittleEndian(int bytes[]) {
  long normalNumber = (bytes[0]) + (bytes[1] << 8) + (bytes[2] << 16) +
    (bytes[3] << 24);
  return normalNumber;
}
char * getY86Instruction(unsigned char instruction[]) {
  char opcode[100];
  opCodeStruct opCodeAsStruct;
  getOpCode(instruction[0], & opCodeAsStruct);
  strcpy(opcode, opCodeAsStruct.opCode);
  if (instruction[0] == 0x30) {
    strcat(opcode, " $");
  } else {
    strcat(opcode, " ");
  }

  char * finalInstruction = opcode;
  // instructions with no operands
  if (instruction[0] == 0x10 || instruction[0] == 0x90 || instruction[0] == 0x00) {
    printf("");
    return opcode;
  }
  // instructions with registers as operands
  else if (
    (instruction[0] >= 0x20 && instruction[0] <= 0x26) ||
    (instruction[0] >= 0x60 && instruction[0] <= 0x63)) {
    int * regIndexes = getRegIndexes(instruction[1]);
    char regA[100];
    strcpy(regA, registerNames[regIndexes[0]]);
    char regB[100];
    strcpy(regB, registerNames[regIndexes[1]]);
    // attach the different parts of the instuction
    strcat(finalInstruction, regA);
    strcat(finalInstruction, ", ");
    strcat(finalInstruction, regB);
    return finalInstruction;
  } else if (instruction[0] == 0xA0 || instruction[0] == 0xB0) {
    int * regIndexes = getRegIndexes(instruction[1]);
    char regA[10];
    strcpy(regA, registerNames[regIndexes[0]]);
    strcat(finalInstruction, regA);
    return finalInstruction;
  }
  // instructions with intermediate values as operands
  else if (
    instruction[0] == 0x30 ||
    instruction[0] == 0x40 ||
    instruction[0] == 0x50) {
    // movement instructions with intermediate values
    int * regIndexes = getRegIndexes(instruction[1]);
    char regA[100];
    strcpy(regA, registerNames[regIndexes[0]]);
    char regB[100];
    strcpy(regB, registerNames[regIndexes[1]]);
    int bytesToParse[4] = {
      instruction[2],
      instruction[3],
      instruction[4],
      instruction[5]
    };
    long number = ConvertLittleEndian(bytesToParse);
    char numberAsString[100];
    strcpy(numberAsString, convertIntToString(number, 0));
    if (instruction[0] == 0x30) {
      printf("");
      strcat(numberAsString, ", ");
      strcat(finalInstruction, numberAsString);
      strcat(finalInstruction, regB);
      return finalInstruction;
    } else if (instruction[0] == 0x40) {
      // format the given number with brackets
      strcat(numberAsString, "(");
      strcat(regB, ")");
      // join the parts into a value enclosed in brackets with another value.
      strcat(numberAsString, regB);
      // join opCode with space and regA as well as attaching the second attribute to get the full instruction.
      strcat(finalInstruction, regA);
      strcat(finalInstruction, ", ");
      strcat(finalInstruction, numberAsString);
      return finalInstruction;
    } else if (instruction[0] == 0x50) {
      strcat(finalInstruction, numberAsString);
      strcat(finalInstruction, "(");
      strcat(finalInstruction, regB);
      strcat(finalInstruction, "), ");
      strcat(finalInstruction, regA);
      return finalInstruction;
    }
  } else if (
    (instruction[0] >= 70 && instruction[0] <= 0x76) ||
    instruction[0] == 0x80) {
    int bytesToParse[] = {
      instruction[1],
      instruction[2],
      instruction[3],
      instruction[4]
    };

    unsigned int destination = (instruction[1]) + (instruction[2] << 8) + (instruction[3] << 16) +
      (instruction[4] << 24);
    char destAsString[100];
    strcpy(destAsString, convertIntToString(destination, 1));
    strcat(finalInstruction, destAsString);
    return finalInstruction;
  }
  return "halt";
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
    getOpCode(memory[PC], & tmp);
    unsigned char instruction[100];
    if (tmp.size > 0) {
      int limit = PC + tmp.size;
      // printf("PC: %d limit: %d, size: %d\n\n", PC, limit, tmp.size);
      for (int i = 0; i < tmp.size; i++) {
        // printf("PC: %d %02X\t", PC, memory[PC]);
        instruction[i] = memory[PC];
        PC++;
      }
      char * decodedInstruction = getY86Instruction(instruction);
      printf("%s\n", decodedInstruction);
    }
    // printf("PC: %d | %02X size: %d | opcode: %s |\n", PC, memory[PC], tmp.size, tmp.opCode);
    strcpy(instruction, "");
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