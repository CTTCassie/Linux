#include"MyVector.h"

class TestVector:public testing::Test
{
public:
	virtual void SetUp()
	{
		cout<<"SetUp"<<endl;
		v.PushBack(2);
		v.PushBack(4);
		v.PushBack(6);
		v.PushBack(8);
		v.PushBack(0);
		v.PushBack(3);
	}
	virtual void TearDown()
	{
		cout<<"TearDown"<<endl;
		v.Clear();
	}
	MyVector<int> v;
};


TEST_F(TestVector,Copy)
{
	MyVector<int> v1(5,10);
	for(int i=0;i<5;i++){
		ASSERT_EQ(v1[i],10);
	}
	MyVector<int> v2(v);
	ASSERT_FALSE(v2.Size() != v.Size()) << "v.Size() != v2.Size()";
	v1=v2;
	ASSERT_FALSE(v2.Size() != v1.Size()) << "v.Size() != v2.Size()";
}

class TestPushBack:public testing::TestWithParam<int>
{};
TEST_P(TestPushBack,PushBack)
{
	MyVector<int> v1;
	int n=GetParam();
	v1.PushBack(n);
	EXPECT_EQ(v1.Back(),n);
}
INSTANTIATE_TEST_CASE_P(TrueReturn,TestPushBack,testing::Values(10,12,14,16));

TEST_F(TestVector,PopBack)
{
	for(int i=v.Size()-1;i>=0;--i){
		v.PopBack();
		ASSERT_EQ(v.Size(),i);
	}
	ASSERT_TRUE(v.Empty()) << "v is empty";
}

TEST_F(TestVector,Insert)
{
	v.Insert(2,10);
	ASSERT_NE(v[2],6) << "v[2] != 6";
	ASSERT_EQ(v[2],10) << "v[2] = 10";
	v.Insert(v.Size()-1,20);
	ASSERT_EQ(v[v.Size()-1],20) << "v[last]=20";
}

TEST_F(TestVector,Erase)
{
	int data=v[0];
	v.Erase(0);
	ASSERT_NE(v[0],data);
	ASSERT_EQ(v.Size(),5);

	data=v[v.Size()-1];
	v.Erase(v.Size()-1);
	ASSERT_NE(v[v.Size()-1],data);
	ASSERT_EQ(v.Size(),4);
	
	data=v[2];
	v.Erase(2);
	ASSERT_NE(v[2],data);
	ASSERT_EQ(v.Size(),3);
}

TEST_F(TestVector,Resize)
{
	v.Resize(3);
	for(int i=0;i<3;i++){
		ASSERT_TRUE(v[i] != 0);
	}
	v.Resize(6,100);
	for(int i=3;i<6;i++){
		ASSERT_EQ(v[i],100);
	}
	v.Resize(12);
	for(int i=6;i<12;i++){
		ASSERT_TRUE(v[i] == 0);
	}
}

TEST_F(TestVector,Reverse)
{
	v.Reverse(10);
	ASSERT_EQ(v.Capacity(),10);
}

int main(int argc,char *argv[])
{
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
