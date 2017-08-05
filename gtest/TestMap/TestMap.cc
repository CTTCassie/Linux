#include<iostream>
using namespace std;
#include<map>
#include<gtest/gtest.h>

class GlobalTest:public testing::Environment
{
public:
    virtual void SetUp()
	{
		cout<<"SetUp"<<endl;
	}
	virtual void TearDown()
	{
		cout<<"TearDown"<<endl;
	}
};
int main(int argc,char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	testing::Environment* env = new GlobalTest();
	testing::AddGlobalTestEnvironment(env);
	return RUN_ALL_TESTS();
}









class TestMap:public testing::Test
{
public:
	//添加日志
	static void SetUpTestCase()
	{
		cout<<"SetUpTestCase"<<endl;
	}
	static void TearDownTestCase()
	{
		cout<<"TearDownTestCase"<<endl;
	}
	virtual void SetUp()   //TEST跑之前会执行SetUp
	{
		cout<<"SetUp"<<endl;
    	test_map.insert(make_pair(1,0));
    	test_map.insert(make_pair(2,1));
    	test_map.insert(make_pair(3,2));
    	test_map.insert(make_pair(4,3));
    	test_map.insert(make_pair(5,4));
	}
	virtual void TearDown() //TEST跑完之后会执行TearDown
	{
		cout<<"TearDown"<<endl;
		test_map.clear();
	}
	map<int,int> test_map;
};
TEST_F(TestMap,Find)
{
	map<int,int>::iterator it=test_map.find(1);
	ASSERT_NE(it,test_map.end());
}
TEST_F(TestMap,Size)
{
	ASSERT_EQ(test_map.size(),5);
}

int main(int argc,char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
