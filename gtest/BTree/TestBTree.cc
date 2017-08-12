#include"BTree.h"

class TestBTree:public testing::TestWithParam<int>
{};

TEST_P(TestBTree,Insert)
{
	BTree<int,int,3> bt;  //测试三阶的B树
	int num=GetParam();
	EXPECT_TRUE(bt.Insert(make_pair(num,100)));

}


TEST_P(TestBTree,FindNode)
{
	BTree<int,int,3> bt;  //测试三阶的B树
	int num=GetParam();
	EXPECT_TRUE(bt.Insert(make_pair(num,100)));

	pair<BTreeNode<int,int,3> *,int> res=bt.FindNode(num);
	ASSERT_NE(res.second,-1);
}

INSTANTIATE_TEST_CASE_P(InsertSuccess,TestBTree,testing::Values(53,75,139,49,145,36,101));

int main(int argc,char *argv[])
{
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
