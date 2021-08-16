#ifndef __SHARED_DATA_STRUCTURE_H__
#define __SHARED_DATA_STRUCTURE_H__

//#include "stm32f4xx.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

// visual studio C++
#include <stdint.h>

#define STRUCT_ALIGNMENT 1
#pragma pack(STRUCT_ALIGNMENT)    

#define SHARED_AREA_MAX_SIZE 10000
#define SHARED_BASIC_DICT_MAX_SIZE 500
#define SHARED_CONTAINER_DICT_MAX_SIZE 100


typedef enum
{
	unknown_type = 0,
	shared_int8_t,
	shared_int16_t,
	shared_int32_t,
	shared_int64_t,
	shared_uint8_t,
	shared_uint16_t,
	shared_uint32_t,
	shared_uint64_t,
	shared_float_t,
	shared_doublt_t,
	shared_list_t,
	shared_array_t,
	shared_fifo_t,
	shared_struct_t
} Shared_Value_Type;

typedef enum
{
	unknown_dict = 0,
	shared_basic_dict,
	shared_container_dict
} Shared_Dict_Type;


typedef struct Shared_Basic_MCB
{
	uint8_t* p_value_begin;
	Shared_Value_Type value_type;
} Shared_Basic_MCB;

typedef struct Shared_Container_MCB
{
	uint8_t* 		    p_value_begin;
	Shared_Value_Type 	container_type;
	Shared_Value_Type 	node_type;
	uint8_t 			container_size;
	uint8_t 			container_capacity;
} Shared_Container_MCB;

typedef struct Shared_Basic_Pair
{
	int16_t key;
	Shared_Basic_MCB mcb;
} Shared_Basic_Pair;

typedef struct Shared_Container_Pair
{
	int16_t key;
	Shared_Container_MCB mcb;
} Shared_Container_Pair;

typedef struct Shared_Basic_Dict
{
	Shared_Basic_Pair shared_dict[SHARED_BASIC_DICT_MAX_SIZE];
	uint16_t dict_size;  //number of key-world;
	uint16_t dict_capacity;
} Shared_Basic_Dict; //index

typedef struct Shared_Container_Dict
{
	Shared_Container_Pair shared_dict[SHARED_CONTAINER_DICT_MAX_SIZE];
	uint16_t dict_size;  //number of key-world;
	uint16_t dict_capacity;
} Shared_Container_Dict; //index


// design to get return mcb value
typedef struct Shared_MCB_Wrapper
{
	int16_t             unique_key;
	uint8_t*            p_value_begin;
	Shared_Dict_Type  	dict_type;
	uint16_t            key_seq_in_dict;
	Shared_Value_Type 	value_type;
	Shared_Value_Type	item_type;
	uint8_t 		    container_size;
	uint8_t 			container_capacity;
} Shared_MCB_Wrapper;


typedef struct Shared_Dict
{
	Shared_Basic_Dict shared_basic_dict;
	Shared_Container_Dict shared_container_dict;
} Shared_Dict;



// ------------------Shared List----------------------
typedef struct Shared_List_Director
{
	uint8_t* p_free_hub;
	uint8_t* p_active_hub;
} Shared_List_Director;

//typedef struct Shared_List_Reader_Node
//{
//	void* p_curr_value;
//	uint16_t value_length;
//	struct Shared_List_Reader_Node* p_next_node;
//} Shared_List_Reader_Node;
//
//typedef struct Shared_List_Reader
//{
//	uint16_t node_num;
//	Shared_List_Reader_Node* p_reader_header;
//} Shared_List_Reader;

// fifo
typedef struct Shared_FIFO_Director
{
	uint8_t head_seq;
	uint8_t tail_seq;
} Shared_FIFO_Director;






// struct

// version 2
//typedef struct Shared_Value_Info_Node
//{
//	void *p_begin;
//	Shared_Value_Type value_type;
//	Shared_Value_Type element_value_type;
//	struct Shared_Value_Info_Node * next;
//} Shared_Value_Info_Node;

//typedef struct Shared_Value_Info_List
//{
//	uint8_t nodeSeq;
//	Shared_Value_Info_Node value_info;
//} Shared_Value_Info_List;


typedef struct Shared_Area
{
	uint8_t shared_area[SHARED_AREA_MAX_SIZE]; //10k area
	uint16_t area_size;
	uint16_t area_capacity;
} Shared_Area; //10k area




#endif

