/*
 * =====================================================================================
 *
 *       Filename:  GatherFilename.cc
 *
 *    Description:  把一个目录下的文件名提取出来，一个文件名占一行，放到一个文件中。
 *    参考：http://blog.csdn.net/code_robot/article/details/6060156
 * 	  增强功能：写入文件前，扫描文件，看某条目是否本来就存在，若存在，则不添加；若不存在，则添加。
 *
 *    与目录相关的两个结构体：
 *    #include <dirent.h>
 *
 *    struct __dirstream
 *    {
 *    	void *__fd;					// 文件描述符
 *    	char *__data;				// 目录块
 *    	int __entry_data;			// 条目号
 *    	char *__ptr;				// 指向当前块的指针
 *    	int __entry_ptr;			// 当前条目号的指针
 *    	size_t __allocation;		// 为块分配的空间
 *    	size_t __size;				// 块中总的有效数据
 *    	__libc_lock_define(,__lock)	// 该结构的mutex锁
 *    };
 *    typedef struct __dirstream DIR;
 *
 *    目录下条目信息
 *    struc dirent
 *    {
 *    	long d_inode;	// 索引节点号（该文件的节点数目）
 *    	off_t d_off;	// 文件在目录中的偏移
 *    	unsigned short d_reclen;	// 文件名长
 *    	unsigned char d_type; 		// 文件类型
 *    	char d_name[NAME_MAX+1];	// 文件名，最长255个字符
 *    };
 *
 *    关于文件类型d_type:
 *    enum
 *    {
 *    	DT_UNKNOWN = 0,
 *    	#define DT_UNKNOWN DT_UNKNOWN
 *    	DT_FIFO = 1,
 *    	#define DT_FIFO DT_FIFO
 *    	DT_CHR = 2,
 *    	#define DT_CHR DT_CHR
 *    	DT_DIR = 4,
 *    	#define DT_DIR DT_DIR
 *    	DT_BLK = 6,
 *    	#define DT_BLK DT_BLK
 *    	DT_REG = 8,
 *    	#define DT_REG DT_REG
 *    	DT_LNK = 10,
 *    	#define DT_LNK DT_LNK
 *    	DT_SOCK = 12,
 *    	#define DT_SOCK DT_SOCK
 *    	DT_WHT = 14,
 *    	#define DT_WHT DT_WHT
 *    };
 *
 *        Version:  1.0
 *        Created:  08/31/2012 12:20:00 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Rachel
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <cstring>	// strlen
#include <vector>	// vector
#include <sys/types.h>	// opendir
#include <dirent.h>	// opendir; DIR; dirent; readdir
#include <fstream>
#include <cstdlib>	// malloc; free

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::ios;

int GatherFilename(string Dir, string Extend, string Target);

/*
 * 到指定目录，递归子目录，获取指定扩展名的文件，将文件信息存储在一个表中，供后续工作使用。
 * 每读取到一个后缀符合条件的文件名，如果它在目标文件中不存在，就把它写入目标文件中。
 * @Dir:目录名
 * @Extend:文件扩展名
 * @Target:目标文件名
 * return：
 * 		>= 0:获取到的文件信息的数目
 * 		-1:传入的目录名或扩展名参数错误
 * 		-2:目录访问失败
 */
int GatherFilename(string Dir, string Extend, string Target)
{
	int count = 0; // 记录提取的文件名数目
	DIR *pDir; // 目录指针
	struct dirent *pDir_ent; // 目录下条目指针
	string fileinfo; // 存放提取的文件名
	int dot_pos;
	vector<string> DirList;	// 目录列表
	string CurDir;	// 当前目录
	string SubDir;	// 子目录

	ofstream output; // 写
	ifstream input; // 读
	string content; // 存放从文件中读取的行内容

	output.open(Target.c_str(), ios::app);
	input.open(Target.c_str(), ios::in);

	if (Extend[0] != '.') // 给后缀添加.
		Extend = '.' + Extend;
	if (Dir[Dir.size()-1] != '/') // 给目录添加/
		Dir += '/';

	DirList.push_back(Dir);	// 把传入的参数加到该vector中

	// 目录不为空时循环
	while (!DirList.empty())
	{
		pDir_ent = NULL;
		pDir = NULL;
		CurDir = DirList.back(); // 访问vector中最后一个元素
		DirList.pop_back(); // 删除vector中最后一个元素
		
		// opendir:Unix system call
		if ((pDir = opendir(CurDir.c_str())) == NULL)
		{
			cout << "打开目录" << CurDir.c_str() << "失败！" << endl;
			return -2;
		}
		
		cout << "=====================现在处理的是 " << CurDir << " 目录==========================" << endl;
		// 循环处理该目录下的每一条目
		while ((pDir_ent = readdir(pDir)) != NULL)
		{
			/*
			 * 忽略当前目录和父目录
			 */
			// if (strcmp(content.c_str(), ".") == 0 || strcmp(content.c_str(), "..") == 0)
			if (strcmp(pDir_ent->d_name, ".") == 0 || strcmp(pDir_ent->d_name, "..") == 0)
				continue;

			/*
			 * 处理子目录
			 */
			if (pDir_ent->d_type == DT_DIR)
			{
				SubDir = CurDir + pDir_ent->d_name + "/";
#ifndef NDEBUG
				cout << "pDir_ent->d_name = " << pDir_ent->d_name << endl;
				cout << "SubDir = " << SubDir << endl;
#endif
				DirList.push_back(SubDir);	// 把子目录加入vector中等待处理
				continue;
			}

			/*
			 * 处理文件
			 */
			content.clear();
			content = pDir_ent->d_name; // 获取文件名

			// 从字符串尾开始寻找，看是否有.，即判断是否有后缀
			if ((dot_pos = content.find_last_of(".")) == string::npos)
			{
				cout << pDir_ent->d_name << "  --->  does not have an extend" << endl;
				continue;
			}

			// 比较后缀，即判断后缀是否与输入相同
			if (content.substr(dot_pos, content.length() - dot_pos).compare(Extend) != 0)
			{
				cout << pDir_ent->d_name << "  --->  the extend does not satisfy the condition" << endl;
				continue;
			}

			// 去掉后缀
			fileinfo = content.substr(0, dot_pos);

			cout << "File--> file = " << fileinfo << endl;
			cout << pDir_ent->d_name << "  ---> OK!" << endl;
			cout << "*******************************************************" << endl;

			// 写入文件
			input.seekg(0, ios::beg); // 定位到文件开头
			while (!input.eof())
			{
				getline(input, content);
#ifndef NDEBUG
				cout << "Content = " << content << endl;
#endif
				if (content == fileinfo) // 该条目在文件中存在
					break;
			}

			if (input.eof())	// 该条目在文件中不存在
			{
				output << fileinfo << endl;
				count++;
				//output.write(fileinfo.c_str(), fileinfo.length());
				//output.write("\n", 1);	// 回车只占一位！！！
			}
		}

		closedir(pDir);
	}

	input.close();
	output.close();

	return count;
}

/*
 * 可执行文件名 目录名 文件名后缀 保存结果的文件
 */
int main(int argc, char **argv)
{
	// 参数检查
	if (argc < 4 || strlen(argv[1]) < 1 || strlen(argv[2]) < 1 || strlen(argv[3]) < 1)
	{
		cout << "call error!" << endl << "Usage: ./" << argv[0] << " Dirpath FileExtend FileName" << endl;
		return -1;
	}
	
	GatherFilename(argv[1], argv[2], argv[3]);
	
	return 0;
}
