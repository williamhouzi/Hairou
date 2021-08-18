#include "pch.h"
#include "CppUnitTest.h"
#include "../Shared_Data/shared_data_handler.h"
#include "../Shared_Data/shared_data_structure.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//#define ALIGNMENT 1
//#pragma pack(ALIGNMENT)   

namespace SharedDataUT
{
	TEST_CLASS(SharedDataUT)
	{
	public:
		TEST_METHOD(Test1_Struct_Length)
		{
			if (STRUCT_ALIGNMENT == 4)
			{
				int len = 0;
				len = sizeof(Shared_Value_Type);
				Assert::AreEqual(4, len);

				len = sizeof(Shared_Dict_Type);
				Assert::AreEqual(4, len);

				len = sizeof(Shared_Basic_MCB);
				Assert::AreEqual(8, len);

				len = sizeof(Shared_Container_MCB);
				Assert::AreEqual(16, len);

				len = sizeof(Shared_Basic_Pair);
				Assert::AreEqual(12, len);

				len = sizeof(Shared_Container_Pair);
				Assert::AreEqual(20, len);

				len = sizeof(Shared_Basic_Dict);
				Assert::AreEqual(6004, len);

				len = sizeof(Shared_Container_Dict);
				Assert::AreEqual(2004, len);

				len = sizeof(Shared_MCB_Wrapper);
				Assert::AreEqual(28, len);

				len = sizeof(Shared_Dict);
				Assert::AreEqual(8008, len);

				len = sizeof(Shared_List_Director);
				Assert::AreEqual(8, len);

				len = sizeof(Shared_FIFO_Director);
				Assert::AreEqual(2, len);

				len = sizeof(Shared_Area);
				Assert::AreEqual(10012, len);
			}

			if (STRUCT_ALIGNMENT == 1)
			{
				int len = 0;
				len = sizeof(Shared_Value_Type);
				Assert::AreEqual(4, len);

				len = sizeof(Shared_Dict_Type);
				Assert::AreEqual(4, len);

				len = sizeof(Shared_Basic_MCB);
				Assert::AreEqual(8, len);

				len = sizeof(Shared_Container_MCB);
				Assert::AreEqual(14, len);

				len = sizeof(Shared_Basic_Pair);
				Assert::AreEqual(10, len);

				len = sizeof(Shared_Container_Pair);
				Assert::AreEqual(16, len);

				len = sizeof(Shared_Basic_Dict);
				Assert::AreEqual(5004, len);

				len = sizeof(Shared_Container_Dict);
				Assert::AreEqual(1604, len);

				len = sizeof(Shared_MCB_Wrapper);
				Assert::AreEqual(22, len);

				len = sizeof(Shared_Dict);
				Assert::AreEqual(6608, len);

				len = sizeof(Shared_List_Director);
				Assert::AreEqual(8, len);

				len = sizeof(Shared_FIFO_Director);
				Assert::AreEqual(2, len);

				len = sizeof(Shared_Area);
				Assert::AreEqual(10004, len);
			}


		}

		TEST_METHOD(Test2_Type_Length)
		{
			Shared_Value_Type type = shared_int32_t;
			uint16_t lens = get_basic_type_length(type);
			Assert::AreEqual(4, static_cast<int>(lens));
		}


		TEST_METHOD(Test3_Malloc_Write_Read)
		{
			shared_area_init();
			uint64_t testValue = 0x0f00ffff0000ffff;

			uint8_t* p_curr_area = global_shared_area.shared_area;
			uint8_t* p_dest1 = shared_malloc(sizeof(uint64_t));
			Assert::AreEqual(p_dest1, p_curr_area);
			shared_area_cpy(p_dest1, &testValue, sizeof(testValue));
			Assert::AreEqual(8, (int)(global_shared_area.area_size));
			uint64_t* p_reader = (uint64_t*)(p_dest1);
			uint64_t reader = *p_reader;
			Assert::AreEqual(testValue, reader);
			Assert::AreEqual(testValue, *p_reader);


			struct Test_Value2 {
				uint8_t v1;
				uint16_t v2;
				int32_t v3;
			};

			Test_Value2 testValue2 = { 0x10, 0x2220, 0x33303330 };
			p_curr_area = global_shared_area.shared_area+global_shared_area.area_size;
			uint8_t* p_dest2 = shared_malloc(sizeof(testValue2));
			shared_area_cpy(p_dest2, &testValue2, sizeof(testValue2));
			Assert::AreEqual(p_dest2, p_curr_area);
			Assert::AreEqual((int)(sizeof(testValue) + sizeof(testValue2)), (int)(global_shared_area.area_size));
			Test_Value2* t2_reader = (Test_Value2*)(p_dest2);
			Test_Value2 t2_expected = { 0x10, 0x2220, 0x33303330 };
			Assert::AreEqual((uint32_t)(t2_expected.v1), (uint32_t)(t2_reader->v1));
			Assert::AreEqual((uint32_t)(t2_expected.v2), (uint32_t)(t2_reader->v2));
			Assert::AreEqual(t2_expected.v3, t2_reader->v3);
		}

		TEST_METHOD(Test4_Shared_List_Append_Read_Remove)
		{
			shared_area_init();
			int basic_dict_size = global_shared_dict.shared_basic_dict.dict_size;
			int container_dict_size = global_shared_dict.shared_container_dict.dict_size;
			int16_t list_key = 0;
			uint16_t list_cap = 5;
			Shared_Value_Type node_type = shared_int32_t;


			// list init
			shared_list_init(list_key, list_cap, shared_list_t, node_type);
			
			// list push back
			int32_t node_value[4] = { 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD };
			shared_list_push_back(list_key, node_value, sizeof(int32_t));
			shared_list_push_back(list_key, node_value+1, sizeof(int32_t));
			shared_list_push_back(list_key, node_value+2, sizeof(int32_t));
			shared_list_push_back(list_key, node_value+3, sizeof(int32_t));


			// read list mcb info
			Shared_MCB_Wrapper mcb_list = get_shared_mcb_info(list_key);
			uint16_t list_size = mcb_list.container_size;
			Assert::AreEqual((uint32_t)(4), (uint32_t)(list_size));
			
			
			// traverse list
			int seq = 0;
			uint8_t* p_list_reader = ref_shared_list(list_key);
			while (list_size-- > 0)
			{
				int32_t* p_node = (int32_t*)(p_list_reader);
				int32_t value = *p_node;
				Assert::AreEqual(value, node_value[seq]);
				p_list_reader = shared_list_move_forward(list_key, p_list_reader);

				seq++;
			}

			// remove all
			p_list_reader = ref_shared_list(list_key);
			list_size = mcb_list.container_size;

			uint8_t* test_next = shared_list_move_forward(list_key, p_list_reader);

			while (list_size-- > 0)
			{
				p_list_reader = shared_list_remove(list_key, p_list_reader);
			}
			mcb_list = get_shared_mcb_info(list_key);
			list_size = mcb_list.container_size;
			Assert::AreEqual((uint32_t)(0), (uint32_t)(list_size));


			// all again.
			shared_list_push_back(list_key, node_value, sizeof(int32_t));
			shared_list_push_back(list_key, node_value + 1, sizeof(int32_t));
			shared_list_push_back(list_key, node_value + 2, sizeof(int32_t));
			shared_list_push_back(list_key, node_value + 3, sizeof(int32_t));
			
			
			// traverse list
			mcb_list = get_shared_mcb_info(list_key);
			list_size = mcb_list.container_size;
			Assert::AreEqual((uint32_t)(4), (uint32_t)(list_size));
			seq = 0;
			p_list_reader = ref_shared_list(list_key);
			while (list_size-- > 0)
			{
				int32_t* p_node = (int32_t*)(p_list_reader);
				int32_t value = *p_node;
				Assert::AreEqual(value, node_value[seq]);

				seq++;
				p_list_reader = shared_list_move_forward(list_key, p_list_reader);
			}


			// remove last 3
			p_list_reader = ref_shared_list(list_key);
			p_list_reader = shared_list_move_forward(list_key, p_list_reader);

			test_next = shared_list_move_forward(list_key, p_list_reader);
			while (p_list_reader != NULL)
			{
				p_list_reader = shared_list_remove(list_key, p_list_reader);
			}

			// add last 3
			shared_list_push_back(list_key, node_value + 1, sizeof(int32_t));
			shared_list_push_back(list_key, node_value + 2, sizeof(int32_t));
			shared_list_push_back(list_key, node_value + 3, sizeof(int32_t));

			// traverse list
			mcb_list = get_shared_mcb_info(list_key);
			list_size = mcb_list.container_size;
			Assert::AreEqual((uint32_t)(4), (uint32_t)(list_size));
			seq = 0;
			p_list_reader = ref_shared_list(list_key);
			while (list_size-- > 0)
			{
				int32_t* p_node = (int32_t*)(p_list_reader);
				int32_t value = *p_node;
				Assert::AreEqual(value, node_value[seq]);

				seq++;
				p_list_reader = shared_list_move_forward(list_key, p_list_reader);
			}

 		}
		TEST_METHOD(Test5_Shared_Array_Update_Get)
		{
			shared_area_init();
			int16_t array_key = 1;
			shared_array_init(0, 3, shared_array_t, shared_int32_t);
			shared_array_init(array_key, 3, shared_array_t, shared_int32_t);

			int32_t array_value[3] = { 1,2,3 };

			for (int i = 0; i < 3; i++)
			{
				shared_array_update(array_key, (uint8_t)(i), array_value + i, sizeof(int32_t));
			}

			Shared_MCB_Wrapper mcb_arr = get_shared_mcb_info(array_key);
			uint16_t value_length = get_basic_type_length(mcb_arr.item_type);
			for (int i = 0; i < 3; i++)
			{
				uint8_t* p_value = get_shared_array_value(array_key, i);
				int32_t value = *(int32_t*)(p_value);
				Assert::AreEqual(value, array_value[i]);
			}
		}

		TEST_METHOD(Test6_Shared_FIFO_Update_Get)
		{
			shared_area_init();

			int16_t fifo_key = 2;
			uint16_t fifo_cap = 3;
			Shared_MCB_Wrapper init_mcb = shared_fifo_init(fifo_key, fifo_cap, shared_fifo_t, shared_uint16_t);

			uint16_t fifo_value[6] = { 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666 };
			shared_fifo_push_back(fifo_key, fifo_value, sizeof(*fifo_value));
			shared_fifo_push_back(fifo_key, fifo_value + 1, sizeof(*fifo_value));
			shared_fifo_push_back(fifo_key, fifo_value + 2, sizeof(*fifo_value));
			shared_fifo_push_back(fifo_key, fifo_value + 3, sizeof(*fifo_value));
			shared_fifo_push_back(fifo_key, fifo_value + 4, sizeof(*fifo_value));
			shared_fifo_push_back(fifo_key, fifo_value + 5, sizeof(*fifo_value));

			uint16_t* front_value = (uint16_t*)(shared_fifo_front(fifo_key));
			Assert::AreEqual((uint32_t)(fifo_value[3]), (uint32_t)(*front_value));

			uint16_t* back_value = (uint16_t*)(shared_fifo_back(fifo_key));
			Assert::AreEqual((uint32_t)(fifo_value[5]), (uint32_t)(*back_value));



			shared_fifo_pop(fifo_key);
			front_value = (uint16_t*)(shared_fifo_front(fifo_key));
			Assert::AreEqual((uint32_t)(fifo_value[4]), (uint32_t)(*front_value));
			
			Shared_MCB_Wrapper fifo_mcb = get_shared_mcb_info(fifo_key);
			Assert::AreEqual((uint32_t)(2), (uint32_t)(fifo_mcb.container_size));
		}

		TEST_METHOD(Test7_Basic_Update_Get)
		{
			shared_area_init();

			shared_basic_value_init(0, shared_int8_t);
			shared_basic_value_init(1, shared_uint16_t);
			shared_basic_value_init(2, shared_doublt_t);

			int8_t value1 = 0x11;
			uint16_t value2 = 0x2222;
			double value3 = 3.33;

			shared_basic_value_update(0, &value1, sizeof(value1));
			shared_basic_value_update(1, &value2, sizeof(value2));
			shared_basic_value_update(2, &value3, sizeof(value3));


			uint8_t* p_real_value1 = get_shared_basic_value(0);
			uint8_t* p_real_value2 = get_shared_basic_value(1);
			uint8_t* p_real_value3 = get_shared_basic_value(2);

			Assert::AreEqual((uint32_t)(value1), (uint32_t)(*(int8_t*)(p_real_value1)));
			Assert::AreEqual((uint32_t)(value2), (uint32_t)(*(uint16_t*)(p_real_value2)));
			Assert::AreEqual((double)(value3), *(double*)(p_real_value3));
		}

		TEST_METHOD(Test8_Struct_Update_Get)
		{
			shared_area_init();

			struct Test_Value
			{
				int v1;
				double v2;
				uint8_t v3;
			};
			int16_t struct_key = 3;

			Test_Value value_ut = { 1, 2.2, 0x33 };
			Shared_MCB_Wrapper init_mcb = shared_struct_init(struct_key, sizeof(Test_Value), shared_struct_t);
			update_shared_struct(struct_key, &value_ut, sizeof(value_ut));

			uint8_t* ref_struct_v = ref_shared_struct(struct_key);
			Test_Value* p_value = (Test_Value*)(ref_struct_v);

			Assert::AreEqual(value_ut.v1, p_value->v1);
			Assert::AreEqual(value_ut.v2, p_value->v2);
			Assert::AreEqual((uint32_t)(value_ut.v3), (uint32_t)(p_value->v3));
		}

	};
}
