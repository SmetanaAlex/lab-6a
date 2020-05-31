#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <stdio.h>

using namespace std;

const int maxN = 20;
char ifchar[5] = "->  ";
char elsechar[5] = "    ";
typedef enum { WHITE, GREY, BLACK }Color;

typedef struct Node
{
	Node* pnext = nullptr;
	int Vertex = 0;
}*StrAdj;

struct Graph
{
	int numOfVertexes = 0;
	bool matrix[maxN][maxN] = { };

	StrAdj str_adj[maxN] = {};

	void addEdge(int i, int j)
	{
		if (i > numOfVertexes || j > numOfVertexes || i <= 0 || j <= 0 || i == j)
			return;
		i--; j--;
		matrix[i][j] = true;
		matrix[j][i] = true;
	}

	Graph()
	{
		for (int i = 0; i < maxN; i++)
			for (int j = 0; j < maxN; j++)
				matrix[i][j] = 0;
	}

	bool isInitialised()
	{
		return numOfVertexes;
	}

	void save(char* filename)
	{
		FILE* file;
		fopen_s(&file, filename, "wb+");

		if (file)
			fwrite(this, sizeof(Graph), 1, file);

		if (file)
			fclose(file);
	}

	void load(char* filename)
	{
		FILE* file;
		fopen_s(&file, filename, "rb+");

		if (file)
			fread(this, sizeof(Graph), 1, file);

		for (int i = 0; i < numOfVertexes; i++)
			str_adj[i] = nullptr;

		this->fromMatrToStrAdj();

		if (file)
			fclose(file);

	}

	void initGraph()
	{
		while (numOfVertexes <= 0 || numOfVertexes > maxN)
		{
			cout << "Num of verticles = ";
			cin >> numOfVertexes;
		}

		int answer = 1, firstVertex = 0, secondVertex = 0;
		while (answer == 1)
		{
			cout << "First vertex = ";
			cin >> firstVertex;

			cout << "Second vertex = ";
			cin >> secondVertex;

			addEdge(firstVertex, secondVertex);

			cout << "Do you want to insert more vertexes? (1 - Yes, 2 - No)\n";
			cin >> answer;

			system("cls");
		}

		fromMatrToStrAdj();

		system("cls");
	}

	bool isLinked(int v1, int v2)
	{
		return matrix[v1][v2];
	}

	int getNumOfV()
	{
		return numOfVertexes;
	}

	void printMatrix()
	{
		if (!numOfVertexes)
			return;

		cout << "MATRIX\n";

		for (int i = 0; i < 4; i++)
			cout << ' ';

		for (int i = 0; i < numOfVertexes; i++)
		{
			cout << '[' << i + 1 << ']';
		}
		cout << endl;

		for (int i = 0; i < numOfVertexes; i++)
		{
			cout << '[' << i + 1 << ']';
			cout << ' ';

			for (int j = 0; j < numOfVertexes; j++)
			{
				cout << ' ';
				if (matrix[i][j])
					cout << '1';
				else
					cout << '0';
				cout << ' ';
			}
			cout << endl;
		}
	}

	void printStrAdj()
	{
		if (!numOfVertexes)
			return;

		cout << "STRUCTURE OF ADJACENCY\n";

		for (int j = 0; j < numOfVertexes; j++)
		{
			cout << '[' << j + 1 << ']' << ' ';

			Node* current = str_adj[j];
			while (current)
			{
				cout << current->Vertex << ' ';
				current = current->pnext;
			}
			cout << endl;
		}
	}

	bool pullVertexes(int v1, int v2)
	{
		if (v1 <= 0 || v1 > numOfVertexes || v2 <= 0 || v2 > numOfVertexes) {
			cout << "Wrong vertexes!\nTry again.\n";
			system("pause");
			return 0;
		}
		if (!this->isInitialised()) {
			cout << "There is no graph!\nCreate one or load from the file!\n";
			system("pause");
			return 0;
		}
		if (v1 > v2) { int temp = v1; v1 = v2; v2 = temp; }
		v1--; v2--;
		for (int i = 0; i < maxN; ++i) {
			if (matrix[v2][i] && !matrix[v1][i] && i != v1) {
				matrix[v1][i] = 1;
				matrix[i][v1] = 1;
			}
		}
		for (int i = v2; i < maxN - 1; ++i) {
			for (int j = v2; j < maxN - 1; ++j) {
				matrix[i][j] = matrix[i + 1][j + 1];
			}
		}
		(this->numOfVertexes)--;
		for (int i = 0; i < numOfVertexes + 1; i++)	str_adj[i] = nullptr;
		fromMatrToStrAdj();
		printMatrix();
		cout << endl;
		printStrAdj();
	}

	void fromMatrToStrAdj()
	{
		if (!numOfVertexes)
			return;

		for (int j = 0; j < numOfVertexes; j++)
		{
			for (int i = 0; i < numOfVertexes; i++)
			{
				if (matrix[i][j])
				{
					Node* current = str_adj[j];

					if (!current)
					{
						str_adj[j] = new Node();
						str_adj[j]->Vertex = i + 1;
						current = str_adj[j];
						continue;
					}

					bool isRepeated = false;

					while (current->pnext)
					{
						if (current->Vertex == i + 1)
							isRepeated = true;
						current = current->pnext;
					}

					if (isRepeated)
						continue;

					current->pnext = new Node();
					(current->pnext)->Vertex = i + 1;
				}
			}
		}
	}
};


int menu(const char** param, const int numb);

int main()
{
	const int numb = 4;
	const char** param = new const char* [numb];
	for (int i = 0; i < numb; ++i)param[i] = new char[100];
	param[0] = "Create new graph and save it into file.\n";
	param[1] = "Load graph from the file and print it.\n";
	param[2] = "Pull the vertexes.\n";
	param[3] = "Exit.\n";
	Graph* newGraph = new Graph;
	char* filename = new char[100];
	strcpy(filename, "Graph.bin");
	bool isRunning = 1;
	int answer = 0;
	while (isRunning) {
		answer = menu(param, numb);
		switch (answer) {
		case 0: {
			newGraph->initGraph();
			newGraph->save(filename);
			break;
		}
		case 1: {
			newGraph->load(filename);
			newGraph->printMatrix();
			cout << endl;
			newGraph->printStrAdj();
			system("pause");
			break;
		}
		case 2: {
			if (!newGraph->isInitialised()) {
				cout << "Initialise graph first!\n";
				system("pause");
				break;
			}
			int v1 = 0;
			int v2 = 0;
			newGraph->printMatrix();
			cout << endl;
			do {
				cout << "Enter numbers of vertexes to pull:\nFirst vertex: ";
				cin >> v1;
				cout << "Second vertex: ";
				cin >> v2;
				cout << endl;
				newGraph->pullVertexes(v1, v2);
			} while (v1 <= 0 || v2 <= 0 || v1 > maxN || v2 > maxN);
			system("pause");
			break;
		}
		case 3: isRunning = 0; break;
		}
	}
	delete filename, newGraph;
	return 0;
}

int menu(const char** param, const int numb) {
	int key = 0;
	int code;
	do {
		system("cls");
		key = (key + numb) % numb;
		for (int i = 0; i < numb; ++i) {
			if (key == i)cout << ifchar << param[i];
			else cout << elsechar << param[i];
		}
		code = _getch();
		if (code == 224)
		{
			code = _getch();
			if (code == 80) key++;
			if (code == 72) key--;
		}
	} while (code != 13);
	system("cls");
	return key;
}