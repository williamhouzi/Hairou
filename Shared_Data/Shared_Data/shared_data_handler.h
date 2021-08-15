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
// dict init
// shared init
void shared_area_init(void);

Shared_MCB_Wrapper shared_basic_value_init(uint16_t key, Shared_Value_Type value_type);

// TODO:
Shared_MCB_Wrapper shared_struct_init(uint16_t key, uint16_t value_length, Shared_Value_Type value_type);

Shared_MCB_Wrapper shared_array_init(uint16_t key, uint16_t max_size, Shared_Value_Type container_type, Shared_Value_Type node_type);

Shared_MCB_Wrapper shared_list_init(uint16_t key, uint16_t max_list_size, Shared_Value_Type container_type, Shared_Value_Type node_type);

Shared_MCB_Wrapper shared_fifo_init(uint16_t key, uint16_t max_fifo_size, Shared_Value_Type container_type, Shared_Value_Type node_type);


// ---------------------------Update.--------------------------
//Critical Method
// Append new pair in basic dict. Return key's seq if successed. And return -1 if not.
// Argus: key. 
//        p_value. start pointer of source value in share.
//        type.    type of source value.
Shared_MCB_Wrapper shared_basic_value_update(uint16_t key, void* p_source, Shared_Value_Type type);
// TODO
// Append Struct
Shared_MCB_Wrapper update_shared_struct(uint16_t key, void* p_source, uint16_t struct_length, Shared_Value_Type struct_type);

uint8_t* shared_array_update(uint16_t key, uint8_t seq, uint8_t* p_value, uint16_t value_length);

uint8_t* shared_list_move_forward(uint16_t key, uint8_t* p_current_node);

//uint8_t* update_shared_list_value(uint16_t key, uint8_t seq, uint8_t* p_value, uint16_t value_length);

uint8_t* shared_fifo_front(uint16_t key);

uint8_t* shared_fifo_back(uint16_t key);

//uint8_t * shared_fifo_update_value(uint16_t key, uint8_t seq, uint8_t * p_value, uint16_t value_length);

// ---------------------------Push_back.--------------------------
// TODO: delete
Shared_MCB_Wrapper update_shared_array(uint16_t key, void* p_array, uint16_t array_size, Shared_Value_Type container_type, Shared_Value_Type node_type);

void shared_list_push_back(uint16_t key, uint8_t* p_value, uint16_t value_length);

void shared_fifo_push_back(uint16_t key, uint8_t* p_value, uint16_t value_length);

// ---------------------------Pop/Remove.--------------------------
// Return next node pointer.
// TODO:
uint8_t* shared_list_remove(uint16_t key, uint8_t* p_node);

void shared_fifo_pop(uint16_t key);


// ---------------------------look up dict-------------------------
Shared_MCB_Wrapper get_shared_mcb_info(uint16_t shared_key);

// Check if key is existed. Return true if successed to find key.
// Argus: uint16_t key.
bool shared_key_existed(uint16_t shared_key);

// ---------------------------Get Value.--------------------------

uint8_t* get_shared_array_value(uint16_t key, uint16_t seq);
//
//uint8_t* get_shared_list_value(uint16_t key, uint16_t seq);
//
//uint8_t* get_shared_fifo_value(uint16_t key, uint16_t seq);


// ---------------------------Get Ref.--------------------------

uint8_t* ref_shared_basic_value(uint16_t key);

uint8_t* ref_shared_struct(uint16_t key);

uint8_t* ref_shared_array(uint16_t key);

uint8_t* ref_shared_array_value(uint16_t key, uint16_t seq);

uint8_t* ref_shared_list(uint16_t key);

uint8_t* ref_shared_fifo(uint16_t key);


// ---------------------------Dict Control.--------------------------
uint16_t shared_basic_dict_append(Shared_Basic_Pair new_pair);

uint16_t shared_container_dict_append(Shared_Container_Pair new_pair);



// ---------------------------Area Control.--------------------------
// TODO:
uint8_t* shared_malloc(uint16_t size);

// TODO:
void shared_free(void* pointer);

// Write shared area.
// Argus: p_dest.
//				p_source.
//        value_length.
void* write_shared_area(void* p_dest, void* p_source, uint16_t value_length);


// ---------------------------Tools.--------------------------
// Get vlaue length 
uint16_t get_basic_type_length(Shared_Value_Type value_type);

// Get available size in basic dict. Return size.
uint16_t available_global_basic_dict_size(void);

// Get available size in container dict. Return size.
uint16_t available_container_dict_size(void);

// Get available size in shared area. Return size.
uint16_t available_shared_area_size(void);

#endif



