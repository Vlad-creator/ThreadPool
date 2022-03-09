
//TEST 1
void foo(int a , int b , int& sum)
{
	sum = a + b;
}

int foo_sum(int a , int b)
{
	return a + b;
}

int foo_mul(int a , int b)
{
	return a * b;
}

TEST(first , test)
{
	try
	{
		my_pool::ThreadPool pool(2);
		int a = 1;
		int b = 3;
		int sum = 5;
		auto res1 = pool.push(foo , a , b , std::ref(sum));
		auto res2 = pool.push<int>(foo_sum , a , b);
		auto res3 = pool.push<int>(foo_mul , a , b);
		while(!res1->ready) {};
		while(!res2->ready) {};
		while(!res3->ready) {};
		EXPECT_EQ(1 , ((sum == 4) && (res2->data == 4) && (res3->data == 3)));
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//TEST 2
TEST(lambda , test)
{
	try
	{
		my_pool::ThreadPool pool(2);
		int sum = 0;
		int res1;
	    auto res_1 = pool.push([&](int a , int b){res1 = a + b;} , 3 , 4);
	    auto res_2 = pool.push<int>([](int a , int b){return a + b;} , 3 , 4);
	    auto res_3 = pool.push([](int a , int b , int& sum){sum = a + b;} , 3 , 4 , std::ref(sum));
	    while(!res_1->ready) {};
		while(!res_2->ready) {};
		while(!res_3->ready) {};
		EXPECT_EQ(1 , ((res1 == 7) && (res_2->data == 7) && (sum == 7)));
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//TEST 3
template<typename T = int>
class sum
{
public:
	T operator() (const T a , const T b)
	{
		return a + b;
	}
};
TEST(functor , test)
{
	my_pool::ThreadPool pool(2);
	sum test;
	auto res = pool.push<int>(test , 3 , 4);
	while(!res->ready) {};
	EXPECT_EQ(1 , (res->data == 7));
}

//TEST 4
void sum_test(int a , int b , int& sum)
{
	sum = a + b;
}

void main_sum(int a , int b , int& sum)
{
	sum_test(a , b , sum);
}

TEST(second , test)
{
	try
	{
		my_pool::ThreadPool pool(2);
		int test;
		auto res = pool.push(main_sum , 3 , 4 , std::ref(test));
		while(!res->ready) {};
		EXPECT_EQ(1 , (test == 7));
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//TEST 5
TEST(exep , test)
{
	std::string res;
	try
	{
		my_pool::ThreadPool pool(-3);
	}
	catch (std::exception const& e)
	{
		res = e.what();
	}
	EXPECT_EQ(1 , "error" == res);
}
