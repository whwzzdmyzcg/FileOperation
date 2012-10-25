/*
 * =====================================================================================
 *
 *       Filename:  DelRepetition.cc
 *
 *    Description:  查找文件中的重复条目，删除之。
 *
 *		Q：读取文件(用的是istream_iterator)时存在空格问题，如“黑豹乐队-Don't Break My Heart”，读取出来进行相同项判断后，再写入，变成了“黑豹乐队-Don't”，“Break”，“My”，“Heart”各占一行。
 *		A：参考http://hi.baidu.com/shouzhewei/item/8282b3a18f13e02d8919d3de
 *		   参考http://blog.csdn.net/bichenggui/article/details/4710996
 *		对于istream_iterator 与istreambuf_iterator，
 *		前者会忽略空格，回车等字符；若想原文拷贝（与原文件一模一样）的话，就需使用后者了。
 *
 *		好吧，其实是我想多了(其实我的目标就是读取行，再对各行进行比较，根本不是要一个字符一个字符地读取)，
 *		折腾来折腾去，最后直接用getline就搞定了。
 *
 *        Version:  1.0
 *        Created:  08/31/2012 08:07:59 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Rachel
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <cstring> // strlen
#include <fstream> // ifstream,ofstream
#include <vector>

using std::ofstream;
using std::ifstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;

void DelRepetition(char *file);

/*
 * 查找文件中的相同项，删除相同项。
 * @input:文件流
 * 无返回值
 */
void DelRepetition(char *file)
{
	string str;
	vector<string> content; // 存放读出的文件内容
	vector<string>::iterator iter, it;

	ifstream input(file);	// <==> ifstream input; input.open(file);

	while (!input.eof())
	{
		// QA文件内容太大怎么办？
		// A：用循环队列
		// to do...
		getline(input, str); // 一行一行读出文件内容
		content.push_back(str);	// 将文件内容存入vector中
	}

	// 查找重复条目
//	for (iter = content.begin(); iter != content.end(); iter++)
//	{
//		// cout << "内容：" << *iter << endl;
//		for (it = iter+1; it != content.end(); it++)
//		{
//			if (*iter == *it)	// 相同项
//			{
//				cout << "Delete -->" << *iter << endl;
//				*iter = "";	// 将要删除内容对应的迭代器清空
//			}
//		}
//	}

	// 查找重复条目
	// 比上一种方法高效一些
	iter = content.begin();
	while (iter != content.end())
	{
		for (it = iter+1; it != content.end(); it++)
		{
			if (*iter == *it)	// 相同项
			{
				cout << "Delete -->" << *iter << endl;
				*iter = "";	// 将要删除内容对应的迭代器清空
				break;
			}
		}
		iter++;
	}

	// 重新写回文件
	ofstream output(file);
	for (iter = content.begin(); iter != content.end(); iter++)
	{
		if (*iter != "")
			output << *iter << endl;
	}

	// close file stream
	input.close();
	output.close();
}

int main(int argc, char **argv)
{
	// 参数检查
	if (argc < 2 || strlen(argv[1]) < 1)
	{
		cout << "call error!" << endl << "Usage: ./" << argv[0] << " FileName" << endl;
		return -1;
	}

	DelRepetition(argv[1]);

	return 0;
}
