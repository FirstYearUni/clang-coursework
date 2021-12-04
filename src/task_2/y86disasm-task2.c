#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *register_names[] =
    {
        "%eax",
        "%ecx",
        "%edx",
        "%ebx",
        "%esp",
        "%ebp",
        "%esi",
        "%edi",
        "UNKNOWN_REGSITER"};
/* 00 & 10 */
char *simple[] = {
    "halt",
    "nop"}; // 00 - 10
/* 20 - 26 */
char *move[] = {
    "rrmovl",
    "cmovle",
    "cmovl",
    "cmove",
    "cmovne",
    "cmovge",
    "cmovg",
    "irmovl",
    "rmmovl",
    "mrmovl"};
/* 30 - 50 */
char *move_intermediate[] = {
    "irmovl",
    "rmmovl",
    "mrmovl"};
/* A0 - B0 */
char *stack[] = {
    "pushl",
    "popl"};
/* 60 - 63 */
char *al[] = {
    "addl",
    "subl",
    "andl",
    "xorl"};
/* 70 - 76 */
char *jump[] = {
    "jmp",
    "jle",
    "jl",
    "je",
    "jne",
    "jge",
    "jg"};
/* 80 - 90 */
char *fn[] = {
    "call",
    "ret"};

char *getOpCode(int opCodeHex)
{

  if (opCodeHex >= 0x00 && opCodeHex <= 0x10)
  {
    if (opCodeHex == 0x00)
    {
      return (simple[0]);
    }
    else if (opCodeHex == 0x10)
    {
      return (simple[1]);
    }
  }
  else if (opCodeHex >= 0x20 && opCodeHex <= 0x26)
  {
    return (move[opCodeHex - 0x20]);
  }
  else if (opCodeHex >= 0x30 && opCodeHex <= 0x50)
  {
    if (opCodeHex == 0x30)
    {
      return (move_intermediate[0]);
    }
    else if (opCodeHex == 0x40)
    {
      return (move_intermediate[1]);
    }
    else if (opCodeHex == 0x50)
    {
      return (move_intermediate[2]);
    }
  }
  else if (opCodeHex >= 0xA0 && opCodeHex <= 0xB0)
  {

    if (opCodeHex == 0xA0)
    {
      return (stack[0]);
    }
    else if (opCodeHex == 0xB0)
    {
      return (stack[1]);
    }
  }
  else if (opCodeHex >= 0x60 && opCodeHex <= 0x63)
  {
    return (al[opCodeHex - 0x60]);
  }
  else if (opCodeHex >= 0x70 && opCodeHex <= 0x76)
  {
    return (jump[opCodeHex - 0x70]);
  }
  else if (opCodeHex >= 0x80 && opCodeHex <= 0x90)
  {
    if (opCodeHex == 0x80)
    {
      return (fn[0]);
    }
    else if (opCodeHex == 0x90)
    {
      return (fn[1]);
    }
  }
  return "TODO: undisaasembled opcode";
}
//This is an array of register mnemonics in y86

int *getRegIndexes(int hex) {
  int regIndexes[2] = {0, 0};
  regIndexes[0] = hex / 0x10;
  regIndexes[1] = hex % 0x10;
  return regIndexes;
}
char *printY86Instruction(unsigned char instruction[])
{
  char *opcode = getOpCode(instruction[0]);
  if (instruction[0] == 0x10 || instruction[0] == 0x90 || instruction[0] == 0x00)
  {
    return opcode;
  }
  else if (
      (instruction[0] >= 20 && instruction[0] <= 26) ||
      (instruction[0] >= 0x60 && instruction[0] <= 0x63))
  {

  }
}
int convertStrToByteCode(const char *str, unsigned char inst[], int size);

int main(int argc, char **argv)
{
  FILE *pFile = NULL;

  char buffer[15];

  if (argc < 2)
  {
    pFile = fopen("./test1.txt", "r");
  }
  else
  {
    pFile = fopen(argv[1], "r");
  }

  if (pFile == NULL)
  {
    printf("Error open test file, please make sure they exist.\n");

    return 0;
  }

  while (fgets(buffer, 15, pFile) && strlen(buffer) > 1)
  {
    //This unsigned char array stores an instruction read from the file
    //As the largest y86 instruction is 6 bytes, there are 6 unsigned char in the array where
    //each represents a byte.
    unsigned char instruction[6] = {0, 0, 0, 0, 0, 0};
    convertStrToByteCode(buffer, instruction, 6);

    //TODO: From here, your task is to complete the implementation so that all y86 opcode and operands can be disassembled.
    //Any undisassembled opcode should display as "TODO: undisassembled opcode and operands"
    char *opcode = getOpCode(instruction[0]);
    printf("TODO: undisassembled opcode and operands. The first byte of the instruction is 0x%X %s\n", instruction[4], opcode);
  }

  fclose(pFile);

  return 0;
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode(const char *str, unsigned char inst[], int size)
{
  int numHexDigits = 0;
  char *endstr;
  //Each instruction should consist of at most 12 hex digits
  numHexDigits = strlen(str) - 1;
  //Convert the string to integer, N.B. this integer is in decimal
  long long value = strtol(str, &endstr, 16);

  int numBytes = numHexDigits >> 1;
  int byteCount = numHexDigits >> 1;

  while (byteCount > 0)
  {
    unsigned long long mask = 0xFF;
    unsigned long shift = (numBytes - byteCount) << 3;

    inst[byteCount - 1] = (value & (mask << shift)) >> shift;
    byteCount--;
  }

  //Return the size of the instruction in bytes
  return numHexDigits >> 1;
}