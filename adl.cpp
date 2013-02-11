#include <stdlib.h> 

#define MAX_LOCAL_FRAMES 100
#define MAX_AUTOMATA_FRAMES 100
#define MAX_NUMBER_INSTRUCTIONS 5000
//#define NUMBER_REGISTERS 256
#define FINAL_RETURN_ADDR -1

typedef unsigned char u8_t; 

struct automata_frame_t
{
  int return_address;
  int return_asf;
  int return_sf;
  u8_t data[1];
};
struct local_frame_t
{
  int return_address;
  int return_sf;
  u8_t data[1];
};

enum var_types{
  VAR_INT,
  VAR_TUPLE,
  VAR_ARRAY,
  VAR_REFERENCE,
};

struct var_t 
{
  int type;
  union {
    int Int;
    struct var_t *Reference;
    struct {
      int size;
      u8_t data[1]; //vars 
    } Tuple;
    struct { 
      int type, size;
      u8_t data[1];
    } Array;
  } Value;
};

enum operand_types {
  OPR_LITERAL,
  OPR_CHAR_LITERAL,
  OPR_TEMP,
  OPR_AUTOMATA_TEMP,
  OPR_GLOBAL_TEMP,
  OPR_RET
};

struct operand_t
{
  int type;
  union{
    unsigned char char_literal;
    int literal;
    int temp;
    //  int reg;
  } operand;
};

struct arg_t
{
  int is_ref;
  operand_t opr;
};

enum operation
{
  INC,
  SUM,
  MUL,
  DIV,
  SUML_TUPLE,
  DIFF_TUPLE,
  MUL_TUPLE,
  INIT_AUTOMATA,
  INIT_STATE,
  INIT_FUNC,
  MOVETO,
  RETURN,
  IF,
  JUMP,
  LOAD,
  LOAD_ARRAY
};

struct instruction_t
{
  operation op;
  int size;
  u8_t operands[1];
};

struct state_t {
  automata_frame_t *automata;
  local_frame_t *frame;
  var_t ret;
  int program_counter;
};

#define ERROR_OK           0
#define ERROR_NOT_VAR      1
#define ERROR_EXPECTED_INT 2
#define ERROR_WRONG_ARGC 3

struct error_t {
  int program_counter;
  int error;
};

error_t machine_error = {0};

void set_error(int error, state_t &st)
{
  machine_error.program_counter = st.program_counter;
  machine_error.error = error;
}

u8_t *memory, *instructions;

int init()
{
  memory = (u8_t *) malloc(sizeof(automata_frame_t) * MAX_AUTOMATA_FRAMES 
			   + sizeof(local_frame_t) * MAX_AUTOMATA_FRAMES * MAX_LOCAL_FRAMES);
  instructions = (u8_t *) malloc(sizeof(var_t) * MAX_NUMBER_INSTRUCTIONS); 
  return memory != NULL && instructions != NULL;
}

var_t *get_var(operand_t *opr, state_t &st)
{
  var_t *ret;
  switch (opr->type){
  case OPR_TEMP:
    ret = (var_t *) &st.frame->data[opr->operand.temp];
    break;
  case OPR_AUTOMATA_TEMP:
    ret = (var_t *) &st.automata->data[opr->operand.temp];
    break;
  case OPR_GLOBAL_TEMP:
    ret = (var_t *) &memory[opr->operand.temp];
    break;
  default:
    set_error(ERROR_NOT_VAR, st);
    return NULL;
  }
  
  if (ret->type == VAR_REFERENCE)
    return ret->Value.Reference;
}

int get_int(operand_t *opr, state_t &st)
{
  var_t *var;
  var = get_var(opr, st);
  if (var == NULL)
    return 0;
  if (var->type != VAR_INT) {
    set_error(ERROR_EXPECTED_INT, st);
    return 0;
  }
  return var->Value.Int;
}

void run_machine()
{
  int current_automata = 0;
  int current_procedure = 0;
  state_t state = {0};
  instruction_t *instruction;
  int Int;
  var_t *var;

  /*
  SUM,
  MUL,
  DIV,
  SUML_TUPLE,
  DIFF_TUPLE,
  MUL_TUPLE,
  INIT_AUTOMATA,
  INIT_STATE,
  INIT_FUNC,
  MOVETO,
  RETURN,
  IF,
  JUMP,
  LOAD,
  LOAD_ARRAY
   */
  while (1) {
    instruction = (instruction_t *) &instructions[state.program_counter];
    switch (instruction->op) {
    case INC:
      if (instruction->size != 1) {
	set_error(ERROR_WRONG_ARGC, state);
	return;
      }
      var = get_var((operand_t *)instruction->operands, state);
      if (var->type != VAR_INT) {
	set_error(ERROR_EXPECTED_INT, state);
	return;
      }
      var->Value.Int++;
      break;
    }
  }
}
    
    




    
    
