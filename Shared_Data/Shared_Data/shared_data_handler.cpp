#include "shared_data_handler.h"
#include "shared_data_structure.h"



//Shared_Basic_Value_Info init_basic_info = {NULL,unknown_type};
//Shared_Basic_Pair init_pair= {0,{NULL,unknown_type}};

//Shared_Container_Value_Info init_container_info = {NULL, unknown_type, unknown_type, 0, 0};
//Shared_Container_Pair init_container_pair = {-1,{NULL, unknown_type, unknown_type, 0, 0}};

//Shared_Basic_Dict t_shared_basic_dict = {{-1,{NULL,unknown_type}},0};
//Shared_Container_Dict init_container_dict = {{-1,{NULL, unknown_type, unknown_type, 0, 0}},0};

// Global Shared data def
//Shared_Dict global_shared_dict = {{{-1,{NULL,unknown_type}},0}, {{-1,{NULL, unknown_type, unknown_type, 0, 0}},0}};
//Shared_Area global_shared_area = { {0},0, global_shared_area.shared_area, global_shared_area.shared_area };
Shared_Dict global_shared_dict;
Shared_Area global_shared_area;

// ??how to init and allocate array?? 
// allocate index and area;
// set pRead and pWrite;


// ------------------------------------------------Init.-------------------------------------------------------
void shared_area_init(void)
{
	global_shared_dict.shared_basic_dict.dict_capacity = SHARED_BASIC_DICT_MAX_SIZE;
	global_shared_dict.shared_container_dict.dict_capacity = SHARED_CONTAINER_DICT_MAX_SIZE;
	global_shared_dict.shared_basic_dict.dict_size = 0;
	global_shared_dict.shared_container_dict.dict_size = 0;
	for (uint16_t i = 0; i < global_shared_dict.shared_basic_dict.dict_capacity; i++)
	{
		global_shared_dict.shared_basic_dict.shared_dict[i].key = -1;
	}
	for (uint16_t i = 0; i < global_shared_dict.shared_container_dict.dict_capacity; i++)
	{
		global_shared_dict.shared_container_dict.shared_dict[i].key = -1;
	}

	global_shared_area.area_capacity = SHARED_AREA_MAX_SIZE;
	global_shared_area.area_size = 0;
	//global_shared_area.p_begin = global_shared_area.shared_area;
	//global_shared_area.p_end = global_shared_area.shared_area + 1;
}


Shared_MCB_Wrapper shared_basic_value_init(int16_t key, Shared_Value_Type value_type)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (curr_mcb_info.unique_key == -1) // no key
	{
		if (available_global_basic_dict_size() > 0)
		{
			uint16_t value_length = get_basic_type_length(value_type);

			if (value_length != 0)
			{
				uint8_t* p_dest = shared_malloc(value_length);
				if (p_dest != NULL)
				{
					Shared_Basic_MCB new_value_info = { p_dest, value_type };
					Shared_Basic_Pair new_pair = { key, new_value_info };

					int16_t key_seq = shared_basic_dict_append(new_pair);

					curr_mcb_info.unique_key = key;
					curr_mcb_info.p_value_begin = p_dest;
					curr_mcb_info.value_type = value_type;
					curr_mcb_info.dict_type = shared_basic_dict;
					curr_mcb_info.key_seq_in_dict = key_seq;

					printf("success to init new key_value.\n");
				}
				else
				{
					printf("fail to init: failed to allocate memory\n");
				}
			}
			else
			{
				printf("fail to init: not a basic type. need to specify length of value\n");
			}
		}
		else
		{
			printf("fail to init: no available field.\n");
		}
	}
	else
	{
		printf("fail to init: key existed.\n");
	}

	return curr_mcb_info;
}

Shared_MCB_Wrapper shared_struct_init(int16_t key, uint16_t value_length, Shared_Value_Type value_type)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (curr_mcb_info.unique_key == -1) // no key
	{
		if (available_global_basic_dict_size() > 0)
		{
			if (value_type == shared_struct_t)
			{
				uint8_t* p_dest = shared_malloc(value_length);
				if (p_dest != NULL)
				{
					Shared_Basic_MCB new_value_info = { p_dest, shared_struct_t };
					Shared_Basic_Pair new_pair = { key, new_value_info };

					int16_t key_seq = shared_basic_dict_append(new_pair);

					curr_mcb_info.unique_key = key;
					curr_mcb_info.p_value_begin = p_dest;
					curr_mcb_info.value_type = shared_struct_t;
					curr_mcb_info.dict_type = shared_basic_dict;
					curr_mcb_info.key_seq_in_dict = key_seq;

					printf("success to init new key_value.\n");
				}
				else
				{
					printf("fail to init: failed to allocate memory\n");
				}
			}
			else
			{
				printf("fail to init: not struct type.\n");
			}
		}
		else
		{
			printf("fail to init: no available field.\n");
		}
	}
	else
	{
		printf("fail to init: key existed.\n");
	}

	return curr_mcb_info;
}

Shared_MCB_Wrapper shared_array_init(int16_t key, uint16_t array_size, Shared_Value_Type container_type, Shared_Value_Type node_type)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (container_type == shared_array_t)
	{
		if (curr_mcb_info.unique_key == -1) // no key
		{
			if (available_container_dict_size() > 0)
			{
				uint16_t node_length = get_basic_type_length(node_type);
				if (node_length != 0)
				{
					uint16_t array_length = node_length * array_size;

					//TODO: allocate.
					uint8_t* p_dest = shared_malloc(array_length);

					if (p_dest != NULL)
					{
						// TODO: Write data in area
						//if (write_in_shared_area(p_dest, p_array, array_length))
						Shared_Container_MCB container_mcb = { p_dest, container_type, node_type, array_size, array_size };
						Shared_Container_Pair container_pair = { key, container_mcb };

						int16_t key_seq = shared_container_dict_append(container_pair);
						curr_mcb_info.unique_key = key;
						curr_mcb_info.p_value_begin = p_dest;
						curr_mcb_info.value_type = container_type;
						curr_mcb_info.item_type = node_type;
						curr_mcb_info.dict_type = shared_container_dict;
						curr_mcb_info.key_seq_in_dict = key_seq;

						printf("success to update new key_value.\n");

					}
					else
					{
						printf("failed to init array: failed to allocate memory\n");
					}
				}
				else
				{
					printf("failed to init array: node type is not basic.\n");
				}

			}
			else
			{
				printf("failed to init array: no available field.\n");
			}
		}
		else // existed
		{
			printf("failed to init array: key existed\n");
		}
	}
	else
	{
		printf("failed to init array: expected type is array.\n");
	}

	return curr_mcb_info;
}

Shared_MCB_Wrapper shared_list_init(int16_t key, uint16_t max_list_size, Shared_Value_Type container_type, Shared_Value_Type node_type)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (container_type == shared_list_t)
	{
		if (curr_mcb_info.unique_key == -1) // no key
		{
			if (available_container_dict_size() > 0)
			{

				if (get_basic_type_length(node_type) != 0)
				{
					// value + p_next;
					uint16_t list_node_length = get_basic_type_length(node_type) + sizeof(uint8_t*);
					uint16_t list_length = list_node_length * max_list_size;

					// p_free + p_used + list_length
					Shared_List_Director p_director;
					uint16_t mem_length = sizeof(p_director) + list_length;

					//TODO: allocate.
					uint8_t* p_dest = shared_malloc(mem_length);

					if (p_dest != NULL)
					{
						uint8_t* p_writer = p_dest;
						p_director.p_free_hub = p_dest + sizeof(p_director);
						p_director.p_active_hub = NULL;
						shared_area_cpy(p_dest, &p_director, sizeof(p_director));

						uint16_t curr_seq = 0;
						uint8_t* p_curr = p_director.p_free_hub;
						uint8_t* p_next = p_curr + list_node_length;
						while (curr_seq < max_list_size)
						{
							uint8_t empty = 0;
							// TODO: equal to 0;
							uint16_t node_type_lens = get_basic_type_length(node_type);
							uint8_t* p_cursor = p_curr;
							while (node_type_lens-- > 0)
							{
								shared_area_cpy(p_cursor, &empty, 1);
								p_cursor++;
							}

							if (curr_seq == (max_list_size - 1))
							{
								p_next = NULL;
							}
							shared_area_cpy(p_cursor, &p_next, sizeof(p_next));
							
							p_curr = p_next;
							p_next = p_next + list_node_length;

							curr_seq += 1;
						}

						Shared_Container_MCB container_mcb = { p_dest, container_type, node_type,  0 , max_list_size };
						Shared_Container_Pair container_pair = { key, container_mcb };
						int16_t key_seq = shared_container_dict_append(container_pair);

						curr_mcb_info.unique_key = key;
						curr_mcb_info.p_value_begin = p_dest;
						curr_mcb_info.value_type = container_type;
						curr_mcb_info.item_type = node_type;
						curr_mcb_info.dict_type = shared_container_dict;
						curr_mcb_info.key_seq_in_dict = key_seq;

						printf("success to update new key_value.\n");

					}
					else
					{
						printf("failed to init list: failed to allocate memory\n");
					}
				}
				else
				{
					printf("failed to init list: node type is not basic.\n");
				}
			}
			else
			{
				printf("failed to init list: no available field.\n");
			}
		}
		else // existed
		{
			printf("failed to init list: existed key.");
		}
	}
	else
	{
		printf("failed to init list: expected type is list.\n");
	}


	return curr_mcb_info;
}

Shared_MCB_Wrapper shared_fifo_init(int16_t key, uint16_t max_fifo_size, Shared_Value_Type container_type, Shared_Value_Type node_type)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (container_type == shared_fifo_t)
	{
		if (curr_mcb_info.unique_key == -1) // no key
		{
			if (available_container_dict_size() > 0)
			{
				uint16_t node_length = get_basic_type_length(node_type);

				if (node_length != 0)
				{
					uint16_t fifo_length = node_length * max_fifo_size;

					Shared_FIFO_Director fifo_director = { 0 };
					uint16_t memo_length = sizeof(fifo_director) + fifo_length;

					//TODO: allocate.
					uint8_t* p_dest = shared_malloc(memo_length);

					if (p_dest != NULL)
					{

						shared_area_cpy(p_dest, &fifo_director, sizeof(fifo_director));
						// TODO: Write data in area

						Shared_Container_MCB container_mcb = { p_dest, container_type, node_type, 0 , max_fifo_size };
						Shared_Container_Pair container_pair = { key, container_mcb };
						int16_t key_seq = shared_container_dict_append(container_pair);

						curr_mcb_info.unique_key = key;
						curr_mcb_info.p_value_begin = p_dest;
						curr_mcb_info.value_type = container_type;
						curr_mcb_info.item_type = node_type;
						curr_mcb_info.dict_type = shared_container_dict;
						curr_mcb_info.key_seq_in_dict = key_seq;

						printf("success to update new key_value.\n");

					}
					else
					{
						printf("failed to init fifo: failed to allocate memory\n");
					}
				}
				else
				{
					printf("failed to init fifo: node type is not basic.\n");
				}
			}
			else
			{
				printf("failed to init fifo: no available field.\n");
			}
		}
		else // existed
		{
			printf("failed to init fifo: key existed\n");
		}
	}
	else
	{
		printf("failed to init fifo: expected type is fifo.\n");
	}

	return curr_mcb_info;
}


// -------------------------------------------------Update.-------------------------------------------------------
Shared_MCB_Wrapper shared_basic_value_update(int16_t key, void* p_source, Shared_Value_Type type)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (curr_mcb_info.unique_key != -1) // no key
	{
		// key existed.
		if (curr_mcb_info.dict_type == shared_basic_dict && curr_mcb_info.value_type == type)
		{
			uint16_t value_length = (uint16_t)(get_basic_type_length(type));
			if (value_length != 0)
			{
				void* p_dest = (void*)(curr_mcb_info.p_value_begin);
				//TODO: allocate.
				if (shared_area_cpy(p_dest, p_source, value_length))
				{
					printf("success to update existed key_value.\n");
				}
			}
			else
			{
				printf("failed to update:  unknown type\n");
			}
		}
		else
		{
			printf("failed to update: error Value Type: expected type is %d.\n", curr_mcb_info.value_type);
		}
	}
	else
	{
		printf("failed to update: no key\n");
	}

	return curr_mcb_info;
}

Shared_MCB_Wrapper update_shared_struct(int16_t key, void * p_source, uint16_t struct_length)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (curr_mcb_info.unique_key != -1) // no key
	{
		// key existed.
		if (curr_mcb_info.dict_type == shared_basic_dict && curr_mcb_info.value_type == shared_struct_t)
		{
			uint16_t value_length = struct_length;
			if (value_length != 0)
			{
				void* p_dest = curr_mcb_info.p_value_begin;
				//TODO: allocate.
				if (shared_area_cpy(p_dest, p_source, value_length))
				{
					printf("success to update existed key_value.\n");
				}
			}
			else
			{
				printf("failed to update:  unknown type\n");
			}
		}
		else
		{
			printf("failed to update: error Value Type: expected type is %d.\n", curr_mcb_info.value_type);
		}
	}
	else
	{
		printf("failed to update: no key\n");
	}

	return curr_mcb_info;
}

uint8_t * shared_array_update(int16_t key, uint8_t seq, void * p_value, uint16_t value_length)
{
	uint8_t* result = NULL;
	//Shared_MCB_Wrapper result =
	//{
	//	-1,           	// int16_t unique_key
	//	NULL,         	// void *p_begin
	//	unknown_dict, 	// Shared_Dict_Type dict_type
	//	0,           	// int16_t key_seq
	//	unknown_type, 	// Shared_Value_Type value_type
	//	unknown_type, 	// Shared_Value_Type item_type
	//	0,            	// uint8_t container_size
	//	0             	// uint8_t container_capacity
	//};
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_array_t)
		{
			if (value_length == get_basic_type_length(curr_mcb_info.item_type))
			{
				if (seq < curr_mcb_info.container_size)
				{
					uint16_t node_length = get_basic_type_length(curr_mcb_info.item_type);
					uint8_t* p_dest = curr_mcb_info.p_value_begin + seq * node_length;

					shared_area_cpy(p_dest, p_value, value_length);

					result = p_dest;

				}
				else
				{
					printf("failed to set: overflow. array size is %d.\n", curr_mcb_info.container_size);
				}
			}
			else
			{
				printf("failed to update: node type error.\n");
			}

		}
		else
		{
			printf("failed to set: container type error.\n");
		}
	}
	else
	{
		printf("failed to set: no key\n");
	}

	return result;

}

uint8_t* shared_list_move_forward(int16_t key, uint8_t* p_current_node)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (p_current_node != NULL)
	{
		if (curr_mcb_info.unique_key != -1) // key existed
		{
			if (curr_mcb_info.value_type == shared_list_t)
			{
				uint16_t value_length = get_basic_type_length(curr_mcb_info.item_type);
				uint8_t* p_cursor = p_current_node + value_length;
				uint32_t* p_test = (uint32_t*)(p_cursor);
				uint32_t test = *p_test;
				result = (uint8_t*)(test);

			}
			else
			{
				printf("failed to set: container type error.\n");
			}
		}
		else
		{
			printf("failed to move: no key\n");
		}
	}
	else
	{
		printf("failed to move: have moved to end already.\n");
	}

	return result;
}


uint8_t* shared_fifo_front(int16_t key)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_fifo_t)
		{
			if (curr_mcb_info.container_size != 0)
			{
				uint8_t* p_begin = curr_mcb_info.p_value_begin;

				Shared_FIFO_Director* p_fifo_director = (Shared_FIFO_Director*)(p_begin);
				uint8_t* p_reader = p_begin + sizeof(Shared_FIFO_Director);

				uint16_t node_length = get_basic_type_length(curr_mcb_info.item_type);
				result = p_reader + node_length * (p_fifo_director->head_seq);
			}
			else
			{
				printf("failed to get: fifo is empty.\n");
			}
		}
		else
		{
			printf("failed to get: container type error.\n");
		}
	}
	else
	{
		printf("failed to set: no key\n");
	}
	return result;
}


uint8_t* shared_fifo_back(int16_t key)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_fifo_t)
		{
			if (curr_mcb_info.container_size != 0)
			{
				uint8_t* p_begin = curr_mcb_info.p_value_begin;

				Shared_FIFO_Director* p_fifo_director = (Shared_FIFO_Director*)(p_begin);
				uint8_t* p_reader = p_begin + sizeof(Shared_FIFO_Director);

				uint16_t node_length = get_basic_type_length(curr_mcb_info.item_type);
				uint16_t tail_seq = (p_fifo_director->head_seq + curr_mcb_info.container_size - 1) % curr_mcb_info.container_capacity;
				result = p_reader + node_length * tail_seq;
			}
			else
			{
				printf("failed to get: fifo is empty.\n");
			}
		}
		else
		{
			printf("failed to get: container type error.\n");
		}
	}
	else
	{
		printf("failed to set: no key\n");
	}
	return result;
}

// ------------------------------------------------Append.-------------------------------------------------------

void shared_list_push_back(int16_t key, void* p_value, uint16_t value_length)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_list_t)
		{
			if (value_length == get_basic_type_length(curr_mcb_info.item_type))
			{
				if (curr_mcb_info.container_capacity > curr_mcb_info.container_size)
				{
					uint8_t* p_begin = curr_mcb_info.p_value_begin;
					uint16_t list_size = curr_mcb_info.container_size;

					// TODO: read from area.
					Shared_List_Director* p_director = (Shared_List_Director*)(p_begin);

					uint8_t* p_write_dest = p_director->p_free_hub;
					uint8_t* p_write_dest_next = shared_list_move_forward(key, p_write_dest);
					p_director->p_free_hub = p_write_dest_next;


					// write new value.
					shared_area_cpy(p_write_dest, p_value, value_length);
					uint8_t* p_end = NULL;
					shared_area_cpy(p_write_dest + value_length, &p_end, sizeof(uint8_t*));


					uint8_t* p_active_curr = p_director->p_active_hub;
					if (p_active_curr == NULL)
					{
						p_director->p_active_hub = p_write_dest;
					}
					else
					{
						uint8_t* p_active_next = shared_list_move_forward(key, p_active_curr);
						while (p_active_next != NULL)
						{
							p_active_curr = p_active_next;
							p_active_next = shared_list_move_forward(key, p_active_curr);
						}
						uint8_t* p_active_previous_end = p_active_curr;
						shared_area_cpy(p_active_previous_end + value_length, &p_write_dest, sizeof(uint8_t*));
					}

					global_shared_dict.shared_container_dict.shared_dict[curr_mcb_info.key_seq_in_dict].mcb.container_size += 1;
				}
				else
				{
					printf("failed to push back: list is full.\n");
				}
			}
			else
			{
				printf("failed to update: node type error.\n");
			}

		}
		else
		{
			printf("failed to get: container type error.\n");
		}
	}
	else
	{
		printf("failed to set: no key\n");
	}
}

void shared_fifo_push_back(int16_t key, void* p_value, uint16_t value_length)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_fifo_t)
		{
			if (value_length == get_basic_type_length(curr_mcb_info.item_type))
			{
				if (curr_mcb_info.container_capacity >= curr_mcb_info.container_size)
				{
					uint8_t* p_begin = curr_mcb_info.p_value_begin;

					Shared_FIFO_Director* p_fifo_director = (Shared_FIFO_Director*)(p_begin);


					uint16_t head_seq = (uint16_t)(p_fifo_director->head_seq);
					//uint16_t tail_seq = (uint16_t)(p_fifo_director->tail_seq);
					uint16_t capacity = (uint16_t)(curr_mcb_info.container_capacity);

					uint8_t* p_fifo_begin = p_begin + sizeof(Shared_FIFO_Director);
					uint16_t node_length = get_basic_type_length(curr_mcb_info.item_type);
					uint16_t new_value_seq = (head_seq + curr_mcb_info.container_size) % capacity;
					uint8_t* p_dest = p_fifo_begin + node_length * new_value_seq;
					shared_area_cpy(p_dest, p_value, value_length);


					if (curr_mcb_info.container_capacity == curr_mcb_info.container_size)
					{
						uint8_t new_head_seq = (uint8_t)((head_seq + 1) % capacity);
						shared_area_cpy(p_begin, &new_head_seq, sizeof(uint8_t));
						printf("success to append the latest node: oldest node is covered.\n");
					}
					else
					{
						global_shared_dict.shared_container_dict.shared_dict[curr_mcb_info.key_seq_in_dict].mcb.container_size += 1;
					}
				}
				else
				{
					printf("failed to get: fifo is empty.\n");
				}
			}
			else
			{
				printf("failed to update: node type error.\n");
			}

		}
		else
		{
			printf("failed to get: container type error.\n");
		}
	}
	else
	{
		printf("failed to set: no key\n");
	}
}
// ------------------------------------------------Pop/Remove.---------------------------------------------------
uint8_t* shared_list_remove(int16_t key, uint8_t* p_node)
{
	uint8_t* result = NULL;
	if (p_node == NULL)
		return result;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_list_t)
		{
			if (curr_mcb_info.container_size > 0)
			{
				Shared_List_Director* p_list_dir = (Shared_List_Director*)(curr_mcb_info.p_value_begin);
				uint8_t* p_active_curr = p_list_dir->p_active_hub;
				uint8_t* p_active_next = shared_list_move_forward(key, p_active_curr);
				uint8_t* p_free_header = p_list_dir->p_free_hub;

				if (p_active_curr == p_node)
				{
					p_list_dir->p_active_hub = p_active_next;

					uint8_t* p_node_next_pointer = p_node + get_basic_type_length(curr_mcb_info.item_type);
					shared_area_cpy(p_node_next_pointer, &p_free_header, sizeof(p_free_header));
					p_list_dir->p_free_hub = p_node;

					uint8_t empty = 0;
					// TODO: equal to 0;
					uint16_t node_type_lens = get_basic_type_length(curr_mcb_info.item_type);
					uint8_t* p_cursor = p_node;
					while (node_type_lens-- > 0)
					{
						shared_area_cpy(p_cursor, &empty, 1);
						p_cursor++;
					}

					result = p_active_next;

					global_shared_dict.shared_container_dict.shared_dict[curr_mcb_info.key_seq_in_dict].mcb.container_size -= 1;
				}
				else
				{
					bool find_node = false;
					uint16_t list_size = curr_mcb_info.container_size;
					while (list_size-- > 0)
					{
						if (p_active_next == p_node)
						{
							uint8_t* p_next_node_pointer = shared_list_move_forward(key, p_node);

							uint8_t* p_active_curr_next_pointer = p_active_curr + get_basic_type_length(curr_mcb_info.item_type);
							shared_area_cpy(p_active_curr_next_pointer, &p_next_node_pointer, sizeof(p_active_curr_next_pointer));

							uint8_t* p_node_next_pointer = p_node + get_basic_type_length(curr_mcb_info.item_type);
							shared_area_cpy(p_node_next_pointer, &p_free_header, sizeof(p_free_header));
							p_list_dir->p_free_hub = p_node;

							find_node = true;
							result = p_next_node_pointer;

							global_shared_dict.shared_container_dict.shared_dict[curr_mcb_info.key_seq_in_dict].mcb.container_size -= 1;

							break;
						}
						else
						{
							p_active_curr = p_active_next;
							p_active_next = shared_list_move_forward(key, p_active_curr);
						}
					}



					if (find_node == false)
					{
						printf("failed to remove: fail to find the node.\n");
					}
					else
					{
						uint8_t empty = 0;
						// TODO: equal to 0;
						uint16_t node_type_lens = get_basic_type_length(curr_mcb_info.item_type);
						uint8_t* p_cursor = p_node;
						while (node_type_lens-- > 0)
						{
							shared_area_cpy(p_cursor, &empty, 1);
							p_cursor++;
						}
					}
				}
			}
			
		}
		else
		{
			printf("failed to remove: container type error.\n");
		}
	}
	else
	{
		printf("failed to remove: no key\n");
	}
	return result;
}

void shared_fifo_pop(int16_t key)
{
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_fifo_t)
		{
			if (curr_mcb_info.container_size > 0)
			{
				Shared_FIFO_Director* p_fifo_director = (Shared_FIFO_Director*)(curr_mcb_info.p_value_begin);
				uint8_t head_seq = p_fifo_director->head_seq;
				uint8_t capacity = curr_mcb_info.container_capacity;
				uint8_t new_head_seq = (uint8_t)((head_seq + 1) % capacity);
				shared_area_cpy(curr_mcb_info.p_value_begin, &new_head_seq, sizeof(new_head_seq));

				uint8_t empty = 0;
				// TODO: equal to 0;
				uint16_t node_type_lens = get_basic_type_length(curr_mcb_info.item_type);
				uint8_t* p_cursor = curr_mcb_info.p_value_begin + sizeof(Shared_FIFO_Director) + node_type_lens * head_seq;
				while (node_type_lens-- > 0)
				{
					shared_area_cpy(p_cursor, &empty, 1);
					p_cursor++;
				}

				global_shared_dict.shared_container_dict.shared_dict[curr_mcb_info.key_seq_in_dict].mcb.container_size -= 1;
			}
			else
			{
				printf("failed to pop: fifo is empty.\n");
			}
		}
		else
		{
			printf("failed to pop: container type error.\n");
		}
	}
	else
	{
		printf("failed to pop: no key\n");
	}
}


// -------------------------------------------------look up dict------------------------------------------------
Shared_MCB_Wrapper get_shared_mcb_info(int16_t shared_key)
{
	Shared_MCB_Wrapper result =
	{
		-1,           	// int16_t unique_key
		NULL,         	// void *p_begin
		unknown_dict, 	// Shared_Dict_Type dict_type
		0,           	// int16_t key_seq
		unknown_type, 	// Shared_Value_Type value_type
		unknown_type, 	// Shared_Value_Type item_type
		0,            	// uint8_t container_size
		0             	// uint8_t container_capacity
	};

	bool find_key = false;
	uint16_t curr_num = 0;

	// go through basic_dict.
	while (find_key == false && curr_num < global_shared_dict.shared_basic_dict.dict_size)
	{
		if ((int16_t)(shared_key) == global_shared_dict.shared_basic_dict.shared_dict[curr_num].key)
		{
			result.unique_key = shared_key;
			result.p_value_begin = global_shared_dict.shared_basic_dict.shared_dict[curr_num].mcb.p_value_begin;
			result.dict_type = shared_basic_dict;
			result.key_seq_in_dict = curr_num;
			result.value_type = global_shared_dict.shared_basic_dict.shared_dict[curr_num].mcb.value_type;

			find_key = true;
		}
		curr_num++;
	}

	curr_num = 0;
	// go through container_dict.
	while (find_key == false && curr_num < global_shared_dict.shared_container_dict.dict_size)
	{
		if ((int16_t)(shared_key) == global_shared_dict.shared_container_dict.shared_dict[curr_num].key)
		{
			result.unique_key = shared_key;
			result.p_value_begin = global_shared_dict.shared_container_dict.shared_dict[curr_num].mcb.p_value_begin;
			result.dict_type = shared_container_dict;
			result.key_seq_in_dict = curr_num;
			result.value_type = global_shared_dict.shared_container_dict.shared_dict[curr_num].mcb.container_type;
			result.item_type = global_shared_dict.shared_container_dict.shared_dict[curr_num].mcb.node_type;
			result.container_size = global_shared_dict.shared_container_dict.shared_dict[curr_num].mcb.container_size;
			result.container_capacity = global_shared_dict.shared_container_dict.shared_dict[curr_num].mcb.container_capacity;

			find_key = true;
		}
		curr_num++;
	}

	return result;
}

bool shared_key_existed(int16_t shared_key)
{
	bool result = false;
	Shared_MCB_Wrapper value_info = get_shared_mcb_info(shared_key);

	if (value_info.unique_key != -1)
		result = true;

	return result;
}

uint8_t * get_shared_array_value(int16_t key, uint16_t seq)
{
	uint8_t* result = NULL;
	//Shared_MCB_Wrapper result =
	//{
	//	-1,           	// int16_t unique_key
	//	NULL,         	// void *p_begin
	//	unknown_dict, 	// Shared_Dict_Type dict_type
	//	0,           	// int16_t key_seq
	//	unknown_type, 	// Shared_Value_Type value_type
	//	unknown_type, 	// Shared_Value_Type item_type
	//	0,            	// uint8_t container_size
	//	0             	// uint8_t container_capacity
	//};
	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);

	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_array_t)
		{
			if (seq < curr_mcb_info.container_size)
			{
				uint16_t node_length = get_basic_type_length(curr_mcb_info.item_type);
				uint8_t* p_dest = curr_mcb_info.p_value_begin + seq * node_length;

				result = p_dest;

			}
			else
			{
				printf("failed to set: overflow. array size is %d.\n", curr_mcb_info.container_size);
			}

		}
		else
		{
			printf("failed to set: container type error.\n");
		}
	}
	else
	{
		printf("failed to set: no key\n");
	}

	return result;

}

// ------------------------------------------------Get Value.---------------------------------------------------

uint8_t* ref_shared_basic_value(int16_t key)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		uint16_t value_length = get_basic_type_length(curr_mcb_info.value_type);
		if (value_length != 0)
		{
			result = curr_mcb_info.p_value_begin;
		}
		else
		{
			printf("failed to get ref:type error.\n");
		}
	}
	else
	{
		printf("failed to get ref: no key\n");
	}
	return result;
}

uint8_t* ref_shared_struct(int16_t key)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if(curr_mcb_info.value_type == shared_struct_t)
		{
			result = curr_mcb_info.p_value_begin;
		}
		else
		{
			printf("failed to get ref:type error.\n");
		}
	}
	else
	{
		printf("failed to get ref: no key\n");
	}
	return result;
}

uint8_t* ref_shared_array(int16_t key)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_array_t)
		{
			result = curr_mcb_info.p_value_begin;
		}
		else
		{
			printf("failed to get ref:type error.\n");
		}
	}
	else
	{
		printf("failed to get ref: no key\n");
	}
	return result;
}

uint8_t* ref_shared_array_value(int16_t key, uint16_t seq)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_array_t)
		{
			uint16_t node_length = get_basic_type_length(curr_mcb_info.item_type);
			if (node_length != 0)
			{
				result = curr_mcb_info.p_value_begin + seq * node_length;
			}
			else
			{
				printf("failed to get ref: node type error.\n");
			}
		}
		else
		{
			printf("failed to get ref:type error.\n");
		}
	}
	else
	{
		printf("failed to get ref: no key\n");
	}
	return result;
}

uint8_t* ref_shared_list(int16_t key)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_list_t)
		{
			uint8_t* p_begin = curr_mcb_info.p_value_begin;

			// TODO: read from area.
			Shared_List_Director* p_director = (Shared_List_Director*)(p_begin);

			result = p_director->p_active_hub;
		}
		else
		{
			printf("failed to get ref:type error.\n");
		}
	}
	else
	{
		printf("failed to get ref: no key\n");
	}
	return result;
}

uint8_t* ref_shared_fifo(int16_t key)
{
	uint8_t* result = NULL;

	Shared_MCB_Wrapper curr_mcb_info = get_shared_mcb_info(key);
	if (curr_mcb_info.unique_key != -1) // key existed
	{
		if (curr_mcb_info.value_type == shared_fifo_t)
		{
			uint8_t* p_begin = curr_mcb_info.p_value_begin;

			result = p_begin + sizeof(Shared_FIFO_Director);
		}
		else
		{
			printf("failed to get ref:type error.\n");
		}
	}
	else
	{
		printf("failed to get ref: no key\n");
	}
	return result;
}

// -------------------------------------------------Get Ref.----------------------------------------------------


// ---------------------------Dict Control.--------------------------
uint16_t shared_basic_dict_append(Shared_Basic_Pair new_pair)
{
	uint16_t append_pos = 0;

	if (available_global_basic_dict_size() > 0)
	{
		append_pos = global_shared_dict.shared_basic_dict.dict_size;
		global_shared_dict.shared_basic_dict.shared_dict[global_shared_dict.shared_basic_dict.dict_size] = new_pair;
		global_shared_dict.shared_basic_dict.dict_size += 1;
	}

	return append_pos;
}

uint16_t shared_container_dict_append(Shared_Container_Pair new_pair)
{
	uint16_t append_pos = 0;

	if (available_container_dict_size() > 0)
	{
		append_pos = global_shared_dict.shared_container_dict.dict_size;
		global_shared_dict.shared_container_dict.shared_dict[global_shared_dict.shared_container_dict.dict_size] = new_pair;
		global_shared_dict.shared_container_dict.dict_size += 1;
	}

	return append_pos;
}

// -------------------------------------------------Area Control.----------------------------------------------------
uint8_t* shared_malloc(uint16_t size)
{
	uint8_t* result = NULL;
	if (size + global_shared_area.area_size <= global_shared_area.area_capacity)
	{
		result = global_shared_area.shared_area + global_shared_area.area_size;
		global_shared_area.area_size += size;
	}
	else
	{
		printf("failed to malloc: insufficent area.\n");
	}
	return result;
}

void shared_free(void * pointer)
{
				
}

void* shared_area_cpy(void* p_dest, void* p_source, uint16_t value_length)
{
	void* result = NULL;
	if (p_dest == NULL || p_source == NULL || value_length == 0)
		return NULL;

	uint8_t* p_area_dest = (uint8_t*)(p_dest);
	uint8_t* p_value_source = (uint8_t*)(p_source);
	if ((p_area_dest >= global_shared_area.shared_area) && ((p_area_dest + value_length) <= (global_shared_area.shared_area + global_shared_area.area_capacity)))
	{
		while (value_length-- > 0)
		{
			*p_area_dest++ = *p_value_source++;
		}
	}
	else
	{
		printf("failed to write in shared area: insufficent area.\n");
	}

	return p_area_dest;
}


// -------------------------------------------------Tools----------------------------------------------------
uint16_t get_basic_type_length(Shared_Value_Type value_type)
{
	uint16_t result = 0;

	switch (value_type)
	{
	case shared_int8_t:
	case shared_uint8_t:
		result = 1;
		break;

	case shared_int16_t:
	case shared_uint16_t:
		result = 2;
		break;

	case shared_int32_t:
	case shared_uint32_t:
	case shared_float_t:
		result = 4;
		break;

	case shared_int64_t:
	case shared_uint64_t:
	case shared_doublt_t:
		result = 8;
		break;


	default:
		break;
	}


	return result;
}


uint16_t available_global_basic_dict_size()
{
	return SHARED_BASIC_DICT_MAX_SIZE - global_shared_dict.shared_basic_dict.dict_size;
}

// Get available size in container dict.
uint16_t available_container_dict_size()
{
	return SHARED_CONTAINER_DICT_MAX_SIZE - global_shared_dict.shared_container_dict.dict_size;
}


uint16_t available_shared_area_size()
{
	return SHARED_AREA_MAX_SIZE - global_shared_area.area_size;
}


// -------------------------------------------------Tmp----------------------------------------------------

Shared_MCB_Wrapper update_shared_container_value(int16_t key, void* p_source, uint16_t length, Shared_Value_Type container_type, Shared_Value_Type node_type)
{
	Shared_MCB_Wrapper result = { -1, NULL, unknown_dict, 0, unknown_type, unknown_type, 0, 0 };

	return result;
}

int16_t shared_dict_array_append(int16_t key, void * p_array, uint16_t array_size, Shared_Value_Type container_type, Shared_Value_Type node_type)
{
	int16_t result = -1;
	if (available_container_dict_size() > 0)
	{
		if (!shared_key_existed(key))
		{
			Shared_Container_MCB mcb = {};
			Shared_Container_Pair a = {};
		}
		else
		{
			printf("fail to append dict: key existed \n");
		}
	}

	return result;
}