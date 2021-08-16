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

				seq++;
				p_list_reader = shared_list_move_forward(list_key, p_list_reader);
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
	};
}
