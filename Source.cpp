#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>


using namespace std;

class List
{
	public:
		string name;
		int id, headid;
		string value;
		vector<List*> childs;
		List* p;
		List()
		{
			name = "";
			id = 0;
			headid = 0;
			value = "";
		}
		~List()
		{

		}
};

void Show(List *Node)
{
	if (Node == NULL)
		return;
	for (int i = 0; i < Node->childs.size(); i++)
	{
		Show(Node->childs[i]);
	}
	cout << "ID узла: " << Node->id;
	cout << ", Имя узла: " << Node->name;
	cout << ", ID узла родителя: " << Node->headid;
	if (Node->childs.size() > 0)
	{
		cout << ", Узел-список, ID потомков:";
		for (int i = 0; i < Node->childs.size(); i++)
		{
			cout << " " << Node->childs[i]->id << ",";
		}
		cout << endl;
	}
	else
		cout << ", Значение узла: " << Node->value << endl;
}

int Parse(List *&Node,string s, int id)
{
	if (Node == NULL)
	{
		Node = new List();
		Node->headid = 0;
	}
	if (id == -1)
	{
		Node = NULL;
		return -1;
	}
	Node->headid = id;
	id = Node->id = rand();
	static int k = 0;
	bool IsWordEnded = false;
	while (k < s.size())
	{
		if (s[k] == ' ' || s[k] == '\n' || s[k] == '\t' || s[k] == '\r')
		{
			k++;
			if(s[k] != ' ')
				IsWordEnded = true;
			continue;
		}
		if (s[k] == '=')
		{
			k++;
			while (true)
			{
				if (s[k] == ' ' || s[k] == '\n' || s[k] == '\t' || s[k] == '\r')
				{
					k++;
					continue;
				} 
				else if (s[k] == '{')
				{
					k++;
					while (true)
					{
						
						if (s[k] == ' ' || s[k] == '\n' || s[k] == '\t' || s[k] == '\r')
						{
							k++;
							continue;
						}
						else if (s[k] == '}')
						{
							if (Node->headid == 0 && k + 2 < s.size())
							{
								Node = NULL;
								return -1;
							}
							k++;
							return id;
						}
						else
						{
							List *Child = NULL;
							if (Parse(Child, s, id) == -1)
							{
								Node = NULL;
								return -1;
							}
							else
							{
								Node->childs.push_back(Child);
								Child = NULL;
							}
						}

					}
				}
				else if (s[k] == '"')
				{
					k++;
					while (true)
					{
						if (s[k] == '\n')
						{
							Node = NULL;
							return -1;
						}
						Node->value.push_back(s[k]);
						k++;
						if (s[k] == '"')
						{
							k++;
							return id;
						}
					}
				}
				else
				{
					Node = NULL;
					return -1;
				}
			}
		}
		else if (IsWordEnded == false && s[k] == '_' && isalpha(s[k + 1]))
		{
			Node->name.push_back(s[k]);
			k++;
		}
		else if(IsWordEnded == false && isalnum(s[k]))
		{
			Node->name.push_back(s[k]);
			k++;
		}
		else
		{
			Node = NULL;
			return -1;
		}
	}
}

string TextToString(string filename)
{
	string buf;
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Такого файла не существует" << endl;
		return "0";
	}
	else
	{
		while (!fin.eof())
		{
			char a = fin.get();
			buf.push_back(a);
		}
	}
	fin.close();
	return buf;
}

void Save(ofstream &filename, List *Node)
{
	if (!filename.is_open())
		cout << "Ошибка создания файла" << endl;
	else
	{
		if (Node == NULL)
			return;
		for (int i = 0; i < Node->childs.size(); i++)
		{
			Save(filename, Node->childs[i]);
		}
		filename << "ID узла: " << Node->id;
		filename << ", Имя узла: " << Node->name;
		filename << ", ID узла родителя: " << Node->headid;
		if (Node->childs.size() > 0)
		{
			filename << ", Узел-список, ID потомков:";
			for (int i = 0; i < Node->childs.size(); i++)
			{
				filename << " " << Node->childs[i]->id << ",";
			}
			filename << endl;
		}
		else
			filename << ", Значение узла: " << Node->value << endl;
	}
}
int main()
{
	setlocale(LC_ALL, "Russian");
	
	srand(time(0));
	string file;
	cout << "Введите имя и расположение файла, который хотите пропарсить:";
	cin >> file;
	List *Node = NULL;
	if (TextToString(file) != "0")
	{
		if (Parse(Node, TextToString(file), 0) == -1)
			cout << "Неверный формат файла" << endl;
		else
		{
			string result;
			cout << "Введите название файла, в который хотите записать данные: ";
			cin >> result;
			ofstream fout(result);
			Save(fout, Node);
			fout.close();
		}
	}
	system("pause");
	return 0;
}