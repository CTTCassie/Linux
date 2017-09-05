#include<gtest/gtest.h>
#include"crawler.h"

//测试日志是否正确打印
TEST(Crawler,Log)
{
    Log(INFO) << "hello" << endl;
}

#ifdef _DEBUG_
#define _DEBUG__
//测试是否能够成功获得起始页的html
TEST(Crawler,getHtml)
{
    string url="http://job.xatu.edu.cn/jyxx.asp";     //定义爬虫的起始页面
    string html;
    ASSERT_TRUE(GetHtml(url,&html));
    ASSERT_GT(html.size(),1000);
    Log(DEBUG) << html << endl;
}

//测试是否能够正确获得起始页html中的url列表
TEST(Crawler,GetUrlList)
{
    string html="<td width=\"70%\" height=\"28\" align=\"left\" ><a href=\"jyxxnr.asp?id=3983&mnid=7726&classname=%B5%A5%CE%BB%D0%E8%C7%F3%D0%C5%CF%A2\" target=\"_blank\">昆明荣者光电科技发展有限公司（招聘启事）</a>&nbsp;<font color=\"#FF0000\"></font></td>";
    vector<string> urlList;
    ASSERT_TRUE(GetUrlList(html,&urlList));
    ASSERT_EQ(urlList.size(),1);
    string result="http://job.xatu.edu.cn/jyxxnr.asp?id=3983";   //自定义拼接出来的结果
    ASSERT_EQ(urlList[0],result);
}

//测试首页中拼接出来的其中一个html
TEST(Crawler,GetDetailHtml)
{
    string url="http://job.xatu.edu.cn/jyxxnr.asp?id=3983";
    string html;
    ASSERT_TRUE(GetHtml(url,&html));
    Log(DEBUG) << html << endl;
}
#endif

//测试获取是否能够正确获取标题
TEST(Crawler,GetTitle)
{
    string html="<td height=\"50\" align=\"center\" class=\"cu sb\"><strong>昆明荣者光电科技发展有限公司（招聘启事）</strong></td>";
    //string html="<td width=\"70%\" height=\"28\" align=\"left\" ><a href=\"jyxxnr.asp?id=4024&mnid=7726&classname=%B5%A5%CE%BB%D0%E8%C7%F3%D0%C5%CF%A2\" target=\"_blank\">展讯通信有限公司2018招聘公告</a>&nbsp;<font color=\"#FF0000\"></font></td>";
    string title;     
    ASSERT_TRUE(GetTitle(html,&title));
    ASSERT_EQ(title,"昆明荣者光电科技发展有限公司（招聘启事）");
}

//将获取到的标题保存下来
TEST(Crawel,Save)
{
    string path="../test_title/";
    string file="文件.html";
    string html="hello";
    int num=1;    //文件序号
    //期待的最终结果是 ../test_title/1.文件.html
    ASSERT_TRUE(Save(path,num,file,html));
}
