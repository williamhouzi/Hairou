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




	};
}
