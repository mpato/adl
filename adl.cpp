#include <stdlib.h> 

#define MAX_LOCAL_FRAMES 100
#define MAX_AUTOMATA_FRAMES 100
#define FINAL_RETURN_ADDR -1

struct automata_sf
{
  int return_address;
  int return_asf;
  int return_sf;
  int id;
  int frame_size;
  local_sf frames[1];
};
struct local_sf
{
  int return_address;
  int return_sf;
  int frame_size;
  char data[1];
};

enum value_types{
  VALUE_INT,
  VALUE_DOUBLE,
  VALUE_STRING,
  VALUE_TUPLE,
  VALUE_ARRAY,
  VALUE_POINTER,
};

struct value 
{
  int type;
  union {
    int Int;
    double Double;
    char *String;
    struct value *Tuple;
    struct value *Pointer;
    struct { 
      int type, size;
      void *ptr;
    } Array;
  } Value;
};

enum operation
{
  SUM,
  MUL,
  DIV,
  SUML_TUPLE,
  DIFF_TUPLE,
  MUL_TUPLE,
  START,
  MOVETO,
  CALL,
  IF,
  JUMP
};

struct instruction
{
  operation op;
  value operand1, operand2;
  value result;
}

automata_sf *memory;

int init()
{
  memory = (automata_sf *)malloc(sizeof(automata_sf) * MAX_AUTOMATA_FRAMES 
		  + sizeof(local_sf) * MAX_AUTOMATA_FRAMES * MAX_LOCAL_FRAMES);
  return memory != NULL;
}

void run_machine()
{
  automata_sf *automata;
  local_sf *frame;
  instruction inst;
  int current_automata;
  int current_procedure;
  int program_counter = 0;
  
  while (1){
    
  }
}


