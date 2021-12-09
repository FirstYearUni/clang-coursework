#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *registerNames[] =
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

int *getRegIndexes(int hex)
{

  int regIndexes[2] = {0, 0};
  regIndexes[0] = hex / 0x10;
  regIndexes[1] = hex % 0x10;
  if (hex == 0x82 ||
      hex == 0x15 ||
      hex == 0x64)
  {
    printf("%x %s %s\n", hex, registerNames[regIndexes[1]], registerNames[regIndexes[0]]);
  }
  // printf("%s %d %s %d \n", registerNames[regIndexes[0]], regIndexes[0], registerNames[regIndexes[1], regIndexes[1]]);
  return regIndexes;
}

char *convertIntToString(long src)
{
  char str[100];
  sprintf(str, "%ld", src);
  char *result = str;
  return result;
}

long ConvertLittleEndian(int bytes[])
{
  long normalNumber = (bytes[0]) + (bytes[1] << 8) + (bytes[2] << 16) +
                      (bytes[3] << 24);
  return normalNumber;
}
char *getY86Instruction(unsigned char instruction[])
{
  char opcode[100];
  strcpy(opcode, getOpCode(instruction[0]));
  strcat(opcode, " ");
  char *finalInstruction = opcode;
  // instructions with no operands
  if (instruction[0] == 0x10 || instruction[0] == 0x90 || instruction[0] == 0x00)
  {
    return opcode;
  }
  // instructions with registers as operands
  else if (
      (instruction[0] >= 0x20 && instruction[0] <= 0x26) ||
      (instruction[0] >= 0x60 && instruction[0] <= 0x63))
  {
    int *regIndexes = getRegIndexes(instruction[1]);
    char regA[10];
    strcpy(regA, registerNames[regIndexes[0]]);
    char regB[10];
    strcpy(regB, registerNames[regIndexes[1]]);
    // attach the different parts of the instuction
    strcat(finalInstruction, regA);
    strcat(finalInstruction, ", ");
    strcat(finalInstruction, regB);
    return finalInstruction;
  }
  else if (instruction[0] == 0xA0 || instruction[0] == 0xB0)
  {
    int *regIndexes = getRegIndexes(instruction[1]);
    char regA[10];
    strcpy(regA, registerNames[regIndexes[0]]);
    strcat(finalInstruction, regA);
    return finalInstruction;
  }
  // instructions with intermediate values as operands
  else if (
      instruction[0] == 0x30 ||
      instruction[0] == 0x40 ||
      instruction[0] == 0x50)
  {
    // movement instructions with intermediate values
    int *regIndexes = getRegIndexes(instruction[1]);
    // print the size of regIndexes
    printf("size: %lu\n", sizeof(regIndexes)/ sizeof(1));
    printf("\n");
    char regA[10];
    printf("opcode: %x regs: %x\n", instruction[0], instruction[1]);
    printf("%d\n", regIndexes[0]);
    printf("%d\n", regIndexes[1]);
    strcpy(regA, registerNames[regIndexes[0]]);
    char regB[10];
    strcpy(regB, registerNames[regIndexes[1]]);
    int bytesToParse[4] = {instruction[2], instruction[3], instruction[4], instruction[5]};
    long number = ConvertLittleEndian(bytesToParse);
    char numberAsString[100];
    strcpy(numberAsString, convertIntToString(number));
    if (instruction[0] == 0x30)
    {
      strcat(numberAsString, ", ");
      strcat(finalInstruction, numberAsString);
      strcat(finalInstruction, regB);
      return finalInstruction;
    }
    else if (instruction[0] == 0x40)
    {
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
    }
    else if (instruction[0] == 0x50)
    {
      strcat(finalInstruction, numberAsString);
      strcat(finalInstruction, "(");
      strcat(finalInstruction, regB);
      strcat(finalInstruction, "), ");
      strcat(finalInstruction, regA);
      return finalInstruction;
    }
  }
  else if (
      (instruction[0] >= 70 && instruction[0] <= 0x76) ||
      instruction[0] == 0x80)
  {
    int bytesToParse[] = {instruction[1],
                          instruction[2],
                          instruction[3],
                          instruction[4]};

    long destination = ConvertLittleEndian(bytesToParse);
    char destAsString[100];
    strcpy(destAsString, convertIntToString(destination));
    strcat(finalInstruction, destAsString);
    return finalInstruction;
  }
  return "TODO: undisassembled opcode and operands";
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
    char *fullInstruction = getY86Instruction(instruction);
    printf("%s\n", fullInstruction);
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