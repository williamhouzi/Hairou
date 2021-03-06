#ifndef __SHARED_DATA_HANDLER_H__
#define __SHARED_DATA_HANDLER_H__

//#include "stm32f4xx.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "shared_data_structure.h"

extern Shared_Dict global_shared_dict;
extern Shared_Area global_shared_area;


// -------------------------------Init.--------------------------------

void shared_area_init(void);

Shared_MCB_Wrapper shared_basic_value_init(int16_t key, Shared_Value_Type value_type);

Shared_MCB_Wrapper shared_struct_init(int16_t key, uint16_t value_length, Shared_Value_Type value_type);

Shared_MCB_Wrapper shared_array_init(int16_t key, uint16_t max_size, Shared_Value_Type container_type, Shared_Value_Type node_type);

Shared_MCB_Wrapper shared_list_init(int16_t key, uint16_t max_list_size, Shared_Value_Type container_type, Shared_Value_Type node_type);

Shared_MCB_Wrapper shared_fifo_init(int16_t key, uint16_t max_fifo_size, Shared_Value_Type container_type, Shared_Value_Type node_type);


// ---------------------------Update/Push_back--------------------------
Shared_MCB_Wrapper shared_basic_value_update(int16_t key, void* p_source, uint16_t value_length);

Shared_MCB_Wrapper update_shared_struct(int16_t key, void * p_source, uint16_t struct_length);

uint8_t* shared_array_update(int16_t key, uint8_t seq, void* p_value, uint16_t value_length);

void shared_list_push_back(int16_t key, void* p_value, uint16_t value_length);

void shared_fifo_push_back(int16_t key, void* p_value, uint16_t value_length);

// ---------------------------Pop/Remove.--------------------------
// Return next node pointer.
// TODO:
uint8_t* shared_list_remove(int16_t key, uint8_t* p_node);

void shared_fifo_pop(int16_t key);

// ---------------------------Get Value.--------------------------
uint8_t* get_shared_basic_value(int16_t key);

uint8_t* get_shared_array_value(int16_t key, uint16_t seq);

uint8_t* shared_list_move_forward(int16_t key, uint8_t* p_current_node);

uint8_t* shared_fifo_front(int16_t key);

uint8_t* shared_fifo_back(int16_t key);

// ---------------------------Get Ref.--------------------------

uint8_t* ref_shared_basic_value(int16_t key);

uint8_t* ref_shared_struct(int16_t key);

uint8_t* ref_shared_array(int16_t key);

uint8_t* ref_shared_array_value(int16_t key, uint16_t seq);

uint8_t* ref_shared_list(int16_t key);

uint8_t* ref_shared_fifo(int16_t key);

// ---------------------------look up dict-------------------------
Shared_MCB_Wrapper get_shared_mcb_info(int16_t key);

// Check if key is existed. Return true if successed to find key.
// Argus: int16_t key.
bool shared_key_existed(int16_t key);

// ---------------------------Tools.--------------------------
// Get vlaue length 
uint16_t get_basic_type_length(Shared_Value_Type value_type);

// Get available size in basic dict. Return size.
uint16_t available_global_basic_dict_size(void);

// Get available size in container dict. Return size.
uint16_t available_container_dict_size(void);

// Get available size in shared area. Return size.
uint16_t available_shared_area_size(void);


// ---------------------------Private----------------------------------
// ---------------------------Dict Control.--------------------------
uint16_t shared_basic_dict_append(Shared_Basic_Pair new_pair);
uint16_t shared_container_dict_append(Shared_Container_Pair new_pair);
// ---------------------------Area Control.--------------------------
uint8_t* shared_malloc(uint16_t size);
// TODO:
void shared_free(void* pointer);
void* shared_area_cpy(void* p_dest, void* p_source, uint16_t value_length);

#endif



