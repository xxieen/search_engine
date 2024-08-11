### 一款在Linux环境下用c++实现的定制化内容搜索的搜索引擎

#### 运行前期准备：

1.将需要搜索的内容按如下格式生成

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<rss version="2.0">
    <channel>
        <title><![CDATA[汽车频道]]></title>
        <image>
            <title><![CDATA[汽车频道]]></title>
            <link>http://auto.people.cn</link>
            <url>http://www.people.cn/img/2014peoplelogo/rmw_logo.gif</url>
        </image>
        <description><![CDATA[汽车新闻]]></description>
        <link>http://auto.people.cn</link>
        <language>zh-cn</language>
        <generator>www.people.cn</generator>
        <copyright><![CDATA[Copyright 1997-2016 by www.people.com.cn. all rights reserved]]></copyright>
        <pubDate>2021-05-18 17:26:42</pubDate>

        ...
        
        <item>
            <title><![CDATA[车门锁栓存隐患 福特召回千辆进口林肯MKZ]]></title>
            <link>http://auto.people.com.cn/n1/2021/0208/c1005-32025697.html</link>
            <pubDate>2021-02-08</pubDate>
            <author>人民网</author>
            <description><![CDATA[<p style="text-indent: 2em;"> 近日，福特汽车（中国）有限公司根据《缺陷汽车产品召回管理条例》和《缺陷汽车产品召回管理条例实施办法》要求，向国家市场监督管理总局备案召回计划，决定自2021年3月1日起，召回2014年6月11日-2015年2月12日生产的部分2015年款进口林肯MKZ汽车，共计2290辆。</p> <p style="text-indent: 2em;"> 本次召回范围内的车辆，由于供应商原因，部分车门锁栓的爪簧调整片底座经过长时间使用可能会出现断裂现象，导致车门无法上锁；或上锁的车门可能在车辆行驶时出现解锁的情况，增加乘员受伤风险，存在安全隐患。</p> <p style="text-indent: 2em;"> 福特汽车（中国）有限公司将委托林肯品牌授权经销商，为召回范围内的车辆免费更换经过设计改善的车门锁栓，以消除安全隐患。（刘洋 刘晓梦）</p> ]]></description>
        </item>
	</channel>
</rss>
```

2.将其放入`项目目录/data/xml`中

3.分别在 `项目目录/src/DictProducer`，`项目目录/src/PagePreprocessor`,`项目目录/client`和`项目目录`下执行make命令

4.在bin目录下，首先运行`Pageprocessor`生成网页库，再运行`Cutter`生成词典库

5.修改conf文件下的`server.conf`内的相关配置信息(server_ip等 )

#### 程序启动 ：

```bash
#在bin目录下
./SEserver

#在另一窗口下
./client
```

#### 内容分布：

```xml
#bin: 可执行文件
#client: 客户端代码文件
#src：保存源码文件
#include 保存服务器端头文件
#bin: 可执行文件
#conf: 配置信息文件
#lib: 第三方库文件
#obj: 编译生成的中间文件
```



