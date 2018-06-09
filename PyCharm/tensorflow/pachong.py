# !/usr/bin/env python
# -*- coding:utf-8 -*-
import requests
import json
# 定义一个请求函数,接收页面参数
def get_page(page):
    # 把页面参数添加在url的字符串当中
    url = 'https://image.baidu.com/search/acjson?tn=resultjson_com&ipn=rj&ct=201326592&is=&fp=result&queryWord=美女&cl=2&lm=-1&ie=utf-8&oe=utf-8&adpicid=&st=-1&z=&ic=0&word=美女&s=&se=&tab=&width=&height=&face=0&istype=2&qc=&nc=1&fr=&cg=girl&pn={}&rn=30&gsm=1e'.format(
        page)
    # 请求网站,并且得到网站的响应
    response = requests.get(url)
    # 判断状态的状况
    if response.status_code == 200:
        # 返回文本文件信息
        return response.text
def json_load(text):
    # 把文本文件处理成字典格式
    jsondict = json.loads(text)
    # 创建一个空的合集,作用是去重
    urlset = set()
    # 检查字典里面是否包含了data这个值
    if 'data' in jsondict.keys():
        # 从jsondict中取出data这个字典里面的东西,依次赋值给items!
        for items in jsondict.get('data'):
            # 异常处理,不是每一行数据都包含thumbURL这个数据的
            try:
                urlset.add(items['thumbURL'])
            except:
                pass
    return urlset
def down_cont(url):
    response = requests.get(url)
    name = url.split(',')[-1].split('&')[0]
    if response.status_code == 200:
        # 表示,如果文件名字相同,就删除当前文件,然后再创建一个一样名字的文件
        with open('./images/%s.jpg' % name, 'wb') as f:
            print('正在下载当前图片: ' + url)
            # 以二进制的方法写入到本地
            f.write(response.content)
def main():
    for p in range(5):
        print('正在下载 %s页 的图片' % p)
        page = p * 30
        text = get_page(page)
        urlset = json_load(text)
        for url in urlset:
            down_cont(url)
if __name__ == '__main__':
    main()