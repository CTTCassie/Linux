#include"crawler.h"
#include<boost/lexical_cast.hpp>

size_t WriteData(char *ptr,size_t size,size_t nmemb,void *userdata)
{
    if(ptr == NULL || userdata == NULL){
        return 0;
    }
    string *str=(string *)userdata;
    //不能写成赋值，因为libcurl是一块一块写数据
    //WriteData函数会被多次调用，需要把这些零碎的块组合在一起
    str->append(ptr,size*nmemb);
    return size*nmemb;
}

//类似RAII的处理方式，防止因异常而出发释放文件句柄的情况
class ScopedHandler
{
public:
    ScopedHandler(CURL *handler)
       :_handler(handler)
    {}
    ~ScopedHandler()
    {
        curl_easy_cleanup(_handler);
    }
private:
    ScopedHandler(const ScopedHandler&);
    ScopedHandler& operator=(const ScopedHandler&);
private:
    CURL *_handler;
};

bool GetHtml(const string& url,const string *html)
{
    CURL *handler=curl_easy_init();    //创建文件句柄

    ScopedHandler sh(handler);

    curl_easy_setopt(handler,CURLOPT_URL,url.c_str());
    curl_easy_setopt(handler,CURLOPT_WRITEFUNCTION,WriteData);
    curl_easy_setopt(handler,CURLOPT_WRITEDATA,html);

    CURLcode success=curl_easy_perform(handler);
    if(success != CURLE_OK){
        Log(ERROR) << "GetHtml error!code" << success << endl;
        return false;
    }
    return true;
}

bool GetUrlList(const string& html,vector<string> *urlList)
{
    boost::regex reg("jyxxnr\\.asp\\?id=\\d*");
    boost::smatch result;
    //定义起始和结束的迭代器
    string::const_iterator begin=html.begin();
    string::const_iterator end=html.end();
    //regex_search()是boost中进行正则匹配
    while(boost::regex_search(begin,end,result,reg))
    {
        string url=string("http://job.xatu.edu.cn/");
        string id=result[0];
        urlList->push_back(url+id);

        begin=result[0].second;
    }
    return true;
}

bool GetTitle(const string& html,string *title)   //获取标题
{
    //<strong></strong>之间的是标题
    size_t strong_begin=html.find("<strong>");
    size_t strong_end=html.find("</strong>");
    if(strong_begin == string::npos || strong_end == string::npos)
    {
        Log(ERROR) << "GetTitle not find title" << html << endl;
        return false;
    }
    size_t begin=strong_begin+strlen("<strong>");
    if(begin >= strong_end){
        Log(ERROR) << "GetTitle title begin error" << begin << endl;
        return false;
    }
    //开始正确截取标题
    *title=html.substr(begin,strong_end-begin);
    return true;
}

bool Save(const string& path,int num,const string& file,const string& html)
{
    //string seq=boost::lexical_cast<string>(num);    //将整形的num转成字符型
    string full_path=path+boost::lexical_cast<string>(num)+"."+file;
    ofstream filename(full_path.c_str());
    if(!filename.is_open()){
        Log(ERROR) << "Save open failed" << full_path << endl;
        return false;
    }
    filename << html;
    filename.close();
    return true;
}

//完成字符编码的转化工作，将gb2312转换成utf8
//iconv -f gb2312 -t utf8
int CodeConvert(const char *from,const char *to,char *inbuf,size_t inLen,char *outbuf,size_t outLen)
{
    char **pin=&inbuf;
    char **pout=&outbuf;
    iconv_t cd=iconv_open(to,from);
    if(cd == 0){
        Log(ERROR) << "iconv_open failed!" << endl;
        return -1;
    }
    memset(outbuf,0,outLen);
    if(iconv(cd,pin,&inLen,pout,&outLen) == -1){
        Log(ERROR) << "iconv failed" << errno << endl;
        return -1;
    }
    iconv_close(cd);
    return 0;
}

//爬虫的入口函数
void Run()
{
    int num=1;     //存储到磁盘上所用的序号
    //1、获取到西安工业大学就业信息网主页的html
    Log(INFO) << "Crawler XATU Start!" << endl;
    string html;
    string url="http://job.xatu.edu.cn/jyxx.asp";     //定义爬虫的起始页面
    bool res=GetHtml(url,&html);
    if(!res){
        Log(ERROR) << "Get XATU entry html failed!" << url << endl;
        return;
    }
    Log(INFO) << "Get XATU entry html done!" << endl;
    //2、解析html，拿到所有的详情页url的列表
    vector<string> userList;
    res=GetUrlList(html,&userList);
    if(!res){
        Log(ERROR) << "Get XATU detail_url failed!" << html << endl;
        return;
    }
    Log(INFO) << "Get XATU detail done!" << endl;
    //3、遍历这个列表
    for(size_t i=0;i<userList.size();i++)
    {
        //4、分别获取到每个详情页的html
        string detail_url;
        res=GetHtml(userList[i],&detail_url);
        if(!res){
            Log(ERROR) << "Get XATU detail_url failed!" << detail_url << endl;
            continue;
        }
        Log(INFO) << "Get XATU detail done!" << endl;

        //string detail;
        //detail.resize(1024 * 100);
        char detail[SIZE];
        CodeConvert("gbk","utf8",(char *)detail_url.c_str(),detail_url.size(),detail,SIZE);

        //5、解析出详情页的标题
        string title;
        res=GetTitle(detail,&title);
        if(!res){
            Log(ERROR) << "Get XATU detail_titile failed!" << endl;
            continue;;
        }
        Log(INFO) << "Get XATU detail_titile done!" << endl;
        //6、把详情页的html按照标题作为文件名，存储在磁盘上
        string path="../title/";
        string file=title+".html";
        res=Save(path,num,file,detail);
        if(!res){
            Log(ERROR) << "Get XATU Save failed!" << endl;
            continue;
        }
        num++;
        Log(INFO) << "Get XATU Save done!" << title << endl;
    }
    Log(INFO) << "Crawler all done!" << endl;
    return;
}
