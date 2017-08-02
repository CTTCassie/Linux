from scrapy.spider import BaseSpider
from scrapy.selector import HtmlXPathSelector
from scrapy.item import Item
from douban.items import DoubanItem
import re

class GroupTestSpider(BaseSpider):
	name = 'douban'
	allowed_domains = ["douban.com"]
	start_urls = ["http://www.douban.com/group/WHV/",]
	def __get_id_from_group_url(self,url):
		urls = re.search("^http://www.douban.com/group/([^/]+)/$", url)
		if(urls):
			return urls.group(1)
		else:
			return 0

	def parse(self,response):
		self.log("Fetch douban homepage page: %s" % response.url)
		hxs = HtmlXPathSelector(response)
		item = DoubanItem()

		item['groupName'] = hxs.select('//h1/text()').re("^\s+(.*)\s+$")[0]

		item['groupURL'] = response.url
		groupid = self.__get_id_from_froup_url(response.url)

		members_url="http://www.douban.com/group/%s/members" % groupid
		members_text=hxs.select('//a[contains(@href, "%s")]/text()' % members_url).re("\((\d+)\)")
		item['totalNumber']=members_text[0]

		item['RelativeGroups']=[]
		groups=hxs.select('//div[contains(@class, "group-list-item")]')
		for group in groups:
		    url=group.select('div[contains(@class, "title")]/a/@href').extract()[0]
		    item['RelativeGroups'].append(url)
		return item

		    
		    




