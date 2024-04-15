//
// Created by carson on 11/15/21.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assembler.h"

char *ASM_ERROR_UNKNOWN_INSTRUCTION = "Unknown Assembly Instruction";
char *ASM_ERROR_ARG_REQUIRED = "Argument Required";
char *ASM_ERROR_BAD_LABEL = "Bad Label";
char *ASM_ERROR_OUT_OF_RANGE = "Number is out of range";

//=========================================================
//  All the instructions available on the LMSM architecture
//=========================================================
const char *INSTRUCTIONS[28] =
        {"ADD", "SUB", "LDA", "STA", "BRA", "BRZ", "BRP", "INP", "OUT", "HLT", "COB", "DAT",
         "LDI",
         "JAL", "CALL", "RET",
         "SPUSH", "SPUSHI", "SPOP", "SDUP", "SDROP", "SSWAP", "SADD", "SSUB", "SMAX", "SMIN", "SMUL", "SDIV"
        };
const int INSTRUCTION_COUNT = 28;

//===================================================================
//  All the instructions that require an arg on the LMSM architecture
//===================================================================
const char *ARG_INSTRUCTIONS[11] =
        {"ADD", "SUB", "LDA", "STA", "BRA", "BRZ", "BRP", "DAT",
         "LDI",
         "CALL",
         "SPUSHI"
        };
const int ARG_INSTRUCTION_COUNT = 11;

//======================================================
// Constructors/Destructors
//======================================================

asm_instruction * asm_make_instruction(char* type, char *label, char *label_reference, int value, asm_instruction * predecessor) {
    asm_instruction *new_instruction = calloc(1, sizeof(asm_instruction));
    new_instruction->instruction = type;
    new_instruction->label = label;
    new_instruction->label_reference = label_reference;
    new_instruction->value = value;
    new_instruction->next = NULL;
    if (predecessor != NULL) {
        predecessor->next = new_instruction;
        new_instruction->offset = predecessor->offset + predecessor->slots;
    } else {
        new_instruction->offset = 0;
    }
    // TODO: set the number of slots for the instruction into the slots field
    // need to look at instruction, use IF to check for number of slots
    return new_instruction;
}

void asm_delete_instruction(asm_instruction *instruction) {
    if (instruction == NULL) {
        return;
    }
    asm_delete_instruction(instruction->next);
    free(instruction);
}

asm_compilation_result * asm_make_compilation_result() {
    asm_compilation_result *result = calloc(1, sizeof(asm_compilation_result));
    return result;
}

void asm_delete_compilation_result(asm_compilation_result *result) {
    asm_delete_instruction(result->root);
    free(result);
}

//======================================================
// Helpers
//======================================================
int asm_is_instruction(char * token) {
    for (int i = 0; i < INSTRUCTION_COUNT; ++i) {
        if (strcmp(token, INSTRUCTIONS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int asm_instruction_requires_arg(char * token) {
    for (int i = 0; i < ARG_INSTRUCTION_COUNT; ++i) {
        if (strcmp(token, ARG_INSTRUCTIONS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int asm_is_num(char * token){
    if (*token == '-') { // allow a leading negative
        token++;
        if (*token == '\0') {
            return 0;
        }
    }
    while (*token != '\0') {
        if (*token < '0' || '9' < *token) {
            return 0;
        }
        token++;
    }
    return 1;
}

int asm_find_label(asm_instruction *root, char *label) {
    // TODO - scan the linked list for the given label, return -1 if not found
    return -1;
}


//======================================================
// Assembly Parsing/Scanning
//======================================================

void asm_parse_src(asm_compilation_result * result, char * original_src){

    // copy over so strtok can mutate
    char * src = calloc(strlen(original_src) + 1, sizeof(char));
    strcat(src, original_src);
    asm_instruction * last_instruction = NULL;
    asm_instruction * current_instruction = NULL;

    char *current = strtok(src, " \n");
    // [LABEL] <INST> [LABEL_REF | VALUE}
    // FOO INP
    // INP
    // ADD FOO
    // BAR ADD FOO
    // BAR ADD

    while(current != NULL) {
        char *type = NULL;
        char *label = NULL;
        char *label_ref = NULL;
        int value = 0;

        if(asm_is_instruction(current)) { // if the first input is an instruction, set label to current
            label = current;
            current = strtok(NULL, " \n");
        } // need to do NULL checks after this if statement for other checks on strtok()

        // use asm_instruction_requires_arg() to check if its direct indexing instruction
        // use asm_is_num() to check for a value in parsing

        //asm_instruction *inst = asm_make_instruction(type, label, label_ref, value, last_instruction);

        // TODO: assign current instruction to last instruction (sets up LL of inst)

        // TODO: check if asm_compilation_result *root is null, if YES, assign current instruction to it
        //                                                      else, just keep looping
        current = strtok(NULL, " \n");
    }


    //TODO - generate a linked list of instructions and store the first into
    //       the result->root
    //
    //       generate the following errors as appropriate:
    //
    //       ASM_ERROR_UNKNOWN_INSTRUCTION - when an unknown instruction is encountered
    //       ASM_ERROR_ARG_REQUIRED        - when an instruction does not have a proper argument passed to it
    //       ASM_ERROR_OUT_OF_RANGE        - when a number argument is out of range (-999 to 999)
    //
    //       store the error in result->error
}

//======================================================
// Machine Code Generation
//======================================================

void asm_gen_code_for_instruction(asm_compilation_result  * result, asm_instruction *instruction) {
    //TODO - generate the machine code for the given instruction
    //
    // note that some instructions will take up multiple slots
    //
    // note that if the instruction has a label reference rather than a raw number reference
    // you will need to look it up with `asm_find_label` and, if the label does not exist,
    // report the error as ASM_ERROR_BAD_LABEL

    int value_for_instruction = instruction->value;

    if (instruction->label_reference) { // need to turn the label into the offset / call asm_find_label
        value_for_instruction = asm_find_label(result->root, instruction->label_reference);
    }

    // some instructions have multiple offsets
    if (strcmp("ADD", instruction->instruction) == 0) {
        result->code[instruction->offset] = 100 + value_for_instruction;
    } else if (strcmp("SUB", instruction->instruction) == 0) {
        result->code[instruction->offset] = 200 + value_for_instruction;
    } else {
        result->code[instruction->offset] = 0;
    }

}

void asm_gen_code(asm_compilation_result * result) {
    asm_instruction * current = result->root;
    while (current != NULL) {
        asm_gen_code_for_instruction(result, current);
        current = current->next;
    }
}

//======================================================
// Main API
//======================================================

asm_compilation_result * asm_assemble(char *src) {
    asm_compilation_result * result = asm_make_compilation_result();
    asm_parse_src(result, src);
    asm_gen_code(result);
    return result;
}
