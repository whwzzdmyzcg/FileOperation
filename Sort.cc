/*
 * =====================================================================================
 *
 *       Filename:  Sort-1.cc
 *
 *    Description:  把中文按拼音首字母排序
 *	参考http://blog.csdn.net/hnu_zxc/article/details/6746029
 *		vector排序
 *		没有实现按拼音排序的功能，但把相似项放到一起了(符号、数字、英文、中文)。
 *	参考http://hi.baidu.com/hehehehello/item/dcc44a4a6afc690e6dc2f08b
 *	参考http://hi.baidu.com/lpzsybgekvbdkrd/item/fe71b323b128834347996251
 *
 *        Version:  1.0
 *        Created:  09/01/2012 12:32:51 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Rachel
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>	// malloc; free
#include <cstring>	// strlen
#include <algorithm>	// sort
#include<wchar.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::vector;

bool lessmark(const string &str1, const string &str2)
{
	return str1 < str2;
}

bool greatermark(const string &str1, const string &str2)
{
	return str1 > str2;
}

void Sort(char *file)
{
	string str;
	vector<string> bakup;
	vector<string>::iterator iter;

	ifstream input(file);
	
	while (!input.eof())
	{
		getline(input, str);
		bakup.push_back(str);	// 将文件内容存入vector中
	}
	sort(bakup.begin(), bakup.end(), lessmark);	// 升序排序
//	sort(bakup.begin(), bakup.end(), greatermark);	// 降序排序

	ofstream output(file);
	for (iter = bakup.begin(); iter != bakup.end(); iter++)	// 重新写回文件
	{
		if (*iter != "")
			output << *iter << endl;
	}

	input.close();
	output.close();
}

/*
 * 读入一个文件的内容，排序，重新存入文件。
 * 可执行文件名 待排序文件名
 */
int main(int argc, char **argv)
{
	// 参数检查
	if (argc < 2 || strlen(argv[1]) < 1)
	{
		cout << "call error!" << endl << "Usage: ./" << argv[0] << " FileName" << endl;
		return -1;
	}

	Sort(argv[1]);

	return 0;
}
