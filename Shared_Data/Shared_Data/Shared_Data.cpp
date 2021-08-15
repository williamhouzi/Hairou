#include <iostream>
#include "shared_data_handler.h"
#include "shared_data_structure.h"

using namespace std;


int main()
{
	int i = 0;
	shared_area_init();
	std::cout << global_shared_dict.shared_basic_dict.dict_size << endl;
	
	Shared_Value_Type type = shared_int8_t;
	uint16_t length = get_basic_type_length(shared_int8_t);
	cout << global_shared_area.area_size << endl;
	return 0;
}
