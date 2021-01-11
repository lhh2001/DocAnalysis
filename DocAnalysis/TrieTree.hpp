#pragma once
#include <map>
#include <queue>
#include <string>
#include <vector>

namespace //TrieTreeNode结构体对外隐藏
{
	template<typename T>
	struct TrieTreeNode
	{
		T c; //当前节点所存字符
		bool isEnd; //是否是单词的结尾
		std::map<T, TrieTreeNode<T>*> next; //next[c]表示下个字符的地址
		int freq; //单词的频数
		TrieTreeNode(T c_, bool isEnd_) : c(c_), isEnd(isEnd_), freq(0) {};
	};
}

template<typename T>
class TrieTree
{
public:
	//qtype为频数表的类型(优先队列)
	typedef std::priority_queue<std::pair<int, std::vector<T>>> qtype;

private:
	TrieTreeNode<T>* root;
	std::vector<T> currWord; //用于getFreqTable函数, 存储当前词

	void clearFreq(TrieTreeNode<T>* currNode)
	{
		currNode->freq = 0;
		for (auto iter = currNode->next.begin(); iter != currNode->next.end(); iter++)
		{
			clearFreq(iter->second);
		}
	}

	void destroyTrieTreeNode(TrieTreeNode<T>* currNode)
	{
		for (auto iter = currNode->next.begin(); iter != currNode->next.end(); iter++)
		{
			destroyTrieTreeNode(iter->second);
		}
		delete currNode;
	}

	void getFreqTable(qtype& q, TrieTreeNode<T>* currNode)
	{ 
		if (currNode != root)
		{
			currWord.push_back(currNode->c);
		}

		if (currNode->isEnd && currNode->freq) //freq不是0的单词才会加入到q中
		{
			q.push(std::make_pair(currNode->freq, currWord));
		}
		for (auto iter = currNode->next.begin(); iter != currNode->next.end(); iter++)
		{
			getFreqTable(q, iter->second);
		}

		if (currNode != root)
		{
			currWord.pop_back();
		}
	}

public:
	TrieTree()
	{
		root = new TrieTreeNode<T>(0, false);
	}

	~TrieTree()
	{
		destroyTrieTreeNode(root);
	}

	void insert(const T* s, int len)
	{
		if (len == 0)
		{
			return;
		}
		TrieTreeNode<T>* currNode = root;
		for (int i = 0; i < len; i++)
		{
			bool isEnd = i == len - 1 ? true : false;
			auto iter = currNode->next.find(s[i]);
			if (iter == currNode->next.end())
			{
				TrieTreeNode<T>* tmpNode = new TrieTreeNode<T>(s[i], isEnd);
				currNode->next[s[i]] = tmpNode;
				currNode = tmpNode;
			}
			else
			{
				currNode = iter->second;
			}
		}
	}

	bool find(std::vector<T> vecs)
	{
		int size = vecs.size();
		T* s = new T[size];
		for (int i = 0; i < size; i++)
		{
			s[i] = vecs[i];
		}
		bool re = find(s, size);
		delete[] s;
		return re;
	}

	bool find(const T* s, int len)
	{
		if (len == 0)
		{
			return false;
		}
		TrieTreeNode<T>* currNode = root;
		for (int i = 0; i < len; i++)
		{
			auto iter = currNode->next.find(s[i]);
			if (iter == currNode->next.end())
			{
				return false;
			}
			else
			{
				currNode = iter->second;
			}
		}
		if (currNode->isEnd)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void match(const T* s, int len) //用s中字符匹配TrieTree中的单词
	{
		for (int i = 0; i < len; i++)
		{
			TrieTreeNode<T>* currNode = root;
			for (int j = i; j < len; j++)
			{
				auto iter = currNode->next.find(s[j]);
				if (iter == currNode->next.end())
				{
					break;
				}
				else
				{
					currNode = iter->second;
					if (currNode->isEnd)
					{
						currNode->freq++; //单词频数 + 1
					}
				}
			}
		}
	}

	void getFreqTable(qtype& q)
	{
		getFreqTable(q, root);
	}

	void clearFreq()
	{
		clearFreq(root);
	}
};
