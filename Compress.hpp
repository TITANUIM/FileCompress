

//使用Huffaman树编码对文件进行压缩和解压缩处理

#pragma once

#include "Huffman.h"

typedef long LongType;

struct CharInfo
{
	unsigned char _ch;//字符
	LongType _count;//出现次数
	string _code;//哈夫曼编码

	CharInfo(const LongType count = 0)
		:_count(count)
	{}

	CharInfo(const char ch)
		:_ch(ch)
	{}

	bool operator!=(const CharInfo& c)const
	{
		return _count != c._count;
	}

	CharInfo operator+(const CharInfo& c)const
	{
		return CharInfo(_count + c._count);
	}

	bool operator<(const CharInfo& c)const
	{
		return _count < c._count;
	}
};

class FileComparess
{
public:
	//文件压缩
	void Comparess(const char* filename)
	{
		FILE* fout = fopen(filename, "rb");
		if (fout == NULL)
		{
			cout << "打开待压缩文件失败" << endl;
			return;
		}
		for (int i = 0; i < 256; i++)
		{
			_info[i]._ch = i;
		}
		unsigned char ch = fgetc(fout); //不能使用有符号char，如果，压缩汉字时的字符出现范围是0~255
		while (!feof(fout)) //检测流上的文件结束符，判断是否到文件结尾
		{                   //如果文件结束，则返回非0值，否则返回0，文件结束符只能被clearerr()清除
			
			if (ch == '\r')
			{
				ch = fgetc(fout); //跳过
				if (ch != '\n')
				{
					fseek(fout, -1, SEEK_CUR);//遇到‘\r\n’时屏幕上打印换行  //文件内部位置指针
	//偏移起始位置：文件头0(SEEK_SET)，当前位置1(SEEK_CUR)，文件尾2(SEEK_END)）为基准，偏移offset（指针偏移量）个字节的位置
				}
			}
			_info[ch]._count++;       //统计出现的次数
			ch = fgetc(fout);
		}

		HuffmanTree<CharInfo> h(_info, 256, CharInfo());
		HuffmanNode<CharInfo>* root = h.GetRootNode();

		string str;
		GenerateHaffmanCode(root, str);
		//重新打开待压缩文件读
		fseek(fout, 0, SEEK_SET);
		ch = fgetc(fout);

		unsigned char data = 0;   //要写入压缩文件的数据
		int bitcount = 7;  //标记移位信息
		//打开文件写压缩后的编码

		string write(filename);
		write = write + ".comparess";

		FILE* fwrite = fopen(write.c_str(), "wb");

		while (!feof(fout))
		{
			if (ch == '\r')
			{
				ch = fgetc(fout);
				if (ch != '\n')
				{
					fseek(fout, -1, SEEK_CUR);
				}
			}

			const char* cur = _info[ch]._code.c_str();

			while (*cur)
			{
				if (bitcount >= 0)
				{
					data = data | ((*cur - '0') << bitcount);
					bitcount--;
				}

				if (bitcount < 0)
				{
					fputc(data, fwrite);
					bitcount = 7;
					data = 0;
				}
				cur++;
			}

			ch = fgetc(fout);
		}

		fputc(data, fwrite);
		//写配置文件
		WriteConfig(filename);
		fclose(fout);
		fclose(fwrite);
	}
	

	//文件解压缩
	void UnComparess(const char* filename)
	{
		CharInfo arry[256];
		//读配置文件
		ReadConfig(filename, arry);
		//重建Haffman树
		HuffmanTree<CharInfo> h(arry, 256, CharInfo());
		//遍历树，找叶子结点，写输出文件
		HuffmanNode<CharInfo>* root = h.GetRootNode();
		HuffmanNode<CharInfo>* cur = root;
		//打开压缩文件读
		string Curf(filename);
		Curf = Curf + ".comparess";

		FILE* fread = fopen(Curf.c_str(), "rb");
		unsigned char ch = fgetc(fread);

		FILE* fwrite = fopen("uncompress", "wb");

		int readcount = root->_weight._count;//根节点的_count值就是整棵树字符出现的次数//出现次数越多越靠近根节点，编码越短

		while (readcount)
		{
			int tmp = 1;
			int bit = 7;   //左移的位数

			while (bit >= 0)
			{
				if (ch & (tmp << bit))   //判断最低位是0还是1  从而决定是left还是right
				{
					cur = cur->_right;
					bit--;
				}
				else
				{
					cur = cur->_left;
					bit--;
				}
				//找到叶子结点
				if (cur->_left == NULL&&cur->_right == NULL)
				{
					fputc(cur->_weight._ch, fwrite);
					cur = root;
					readcount--;
					//最后一个字符的编码在最后两个字节当中的情况
					if (!readcount)  //
					{
						break;
					}
				}
			}

			ch = fgetc(fread);
		}

		fclose(fread);
		fclose(fwrite);
	}

protected:
	//get到Haffman编码
	void GenerateHaffmanCode(HuffmanNode<CharInfo>* root, string& code)
	{
		if (root == NULL)
			return;

		GenerateHaffmanCode(root->_left, code + '0');
		GenerateHaffmanCode(root->_right, code + '1');

		root->_weight._code = code;

		if (root->_left == NULL&&root->_right == NULL)
		{
			_info[root->_weight._ch]._code = code;
		}

	}

	void WriteConfig(const char* filename)
	{
		string conf(filename);
		conf = conf + "config";
		FILE* fcon = fopen(conf.c_str(), "wb");

		for (int i = 0; i < 256; ++i)
		{

			if (_info[i]._count)
			{
				fputc(_info[i]._ch, fcon);
				fputc(',', fcon);

				char count[100];
				_itoa(_info[i]._count, count, 10);

				fputs(count, fcon);
				fputc(',', fcon);
				fputs(_info[i]._code.c_str(), fcon);
				fputc('\n', fcon);
			}
		}

		fclose(fcon);
	}

	void ReadConfig(const char* filename, CharInfo* HNarry)
	{
		string conf(filename);
		conf = conf + "config";
		FILE* fread = fopen(conf.c_str(), "rb");

		if (fread == NULL)
		{
			cout << "打开待压缩文件失败" << endl;
			return;
		}

		char str[100];

		while (fgets(str, 100, fread))//得到配置文件的一行
		{
			char* ptr = str;
			unsigned char index = (unsigned char)*ptr;

			if (index == '\n')
			{
				HNarry[index]._ch = index;
				fgets(str, 100, fread);

				char* ptr = str;
				ptr++;

				LongType count = 0;//字符出现的次数

				while (*ptr != ',' && *ptr)//字符转换成数据
				{
					count *= 10;
					count += (*ptr - '0');
					ptr++;
				}

				HNarry[index]._count = count;
				ptr++;

				string code(ptr);
				HNarry[index]._code = code;
			}
			else
			{
				HNarry[index]._ch = index;
				ptr += 2;

				LongType count = 0;

				while (*ptr != ',' && *ptr)
				{
					count *= 10;
					count += (*ptr - '0');
					ptr++;
				}

				HNarry[index]._count = count;
				ptr++;

				string code(ptr);
				HNarry[index]._code = code;
			}
		}
	}

protected:
	CharInfo _info[256];
};
