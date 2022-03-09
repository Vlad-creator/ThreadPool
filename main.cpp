#include "includes/thread_pool.h"

#ifdef TEST
	#include <gtest/gtest.h>
	#include "tests/gtests.h"
#endif


void inf_loop()
{
	while(true) {};
}
void print()
{
	std::cout << "error" << std::endl;
}

int main(int argc , char* argv[])
{
	#ifdef TEST
	    ::testing::InitGoogleTest(&argc, argv);
	    return RUN_ALL_TESTS();
	#endif
	try
	{
		my_pool::ThreadPool pool(-1);
		std::cout << pool.num_thr_ << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}