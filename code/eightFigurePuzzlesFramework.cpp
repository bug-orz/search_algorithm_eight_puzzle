#include <iostream>
#include <vector>
#include <queue>
#include<map>
#include <time.h>
#include "eightFigurePuzzles.h"
#include <windows.h>
#include<cmath>
using namespace std;

//https://www.itread01.com/content/1545899983.html

//���ڼ�¼��ǰ״̬�Ƿ񱻷��ʹ���
map<int, int> visited;

//�����������������������ȡ�
#define MAX_DEPTH 25

//openList��closeList����A*������
vector<PUZZLE_NODE> closeList;
vector<PUZZLE_NODE> openList;
void display_node(PUZZLE_NODE node) {
	for (int xPos = 0; xPos < 3; xPos++) {
		for (int yPos = 0; yPos < 3; yPos++) {
			int id = node.puzzle[xPos * 3 + yPos].puzzleId;
			cout << id << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//�����������
int* binaryFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
//result[1] ���� steps 
	int result[2] = { 0,0 };
	display_node(initialNode);
	cout << "��ʼ�ڵ�״̬��" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	/*
		���ڸ�λ����ɹ����������������
	*/
	PUZZLE_NODE puzzleNode = initialNode;
	queue<PUZZLE_NODE> puzzleNodeQueue;
	puzzleNode.depth = 0;
	int depth = 0;
	puzzleNodeQueue.push(puzzleNode);
	while (puzzleNodeQueue.size()) {
		PUZZLE_NODE currentPuzzleNode = puzzleNodeQueue.front();
		if (checkObject(currentPuzzleNode, objPuzzleNode)) {

			for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
				outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
			}
			cout << "�ҵ���ȷ���:" << endl;
			for (int i = 0; i < 3; i++) {
				cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
					"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
			}
			cout << endl;

			result[0] = 1;
			result[1] = currentPuzzleNode.depth;
			return result;
		}
		else {
			visited[visitedNum(currentPuzzleNode)] = 1;
			if (currentPuzzleNode.nextActionList.size() == 0) {
				currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
			}
			puzzleNodeQueue.pop();
			for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {
				PUZZLE_NODE nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
				if (!currentPuzzleNode.precedeActionList.empty()) {
					for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++)
					{
						nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);
					}
				}
				nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
				if (visited[visitedNum(nextPuzzleNode)] == 1) {
					continue;
				}
				nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
				puzzleNodeQueue.push(nextPuzzleNode);
			}
		}
	}
	return result;

}

//�����������
int* depthFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {

	//result[0] 1:correct;0:wrong
	//result[1] ���� steps 
	int result[2] = { 0,0 };
	//display_node(initialNode);
	/*
		���ڸ�λ�������������������������޶�Ϊ25��
	*/
	visited[visitedNum(initialNode)] = 1;

	if (initialNode.depth < 0) {
		initialNode.depth = 0;
	}
	result[1] = initialNode.depth;
	PUZZLE_NODE currentPuzzleNode = initialNode;
	//cout << currentPuzzleNode.depth << endl;
	if (currentPuzzleNode.nextActionList.size() == 0) {
		currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
	}
	if (checkObject(initialNode, objPuzzleNode)){
		for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
			outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
		}
		cout << "�ҵ���ȷ���:" << endl;
		for (int i = 0; i < 3; i++) {
			cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
				"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
		}
		cout << endl;
		result[0] = 1;
		result[1] = currentPuzzleNode.depth;		
		return result;
	}

	for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {
		PUZZLE_NODE nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
		if (visited[visitedNum(nextPuzzleNode)] == 1) {
			continue;
		}
		if (!currentPuzzleNode.precedeActionList.empty()) {

			for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++)
			{
				nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);
			}
		}
		nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
		nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
		if (nextPuzzleNode.depth > MAX_DEPTH) {
			continue;
		}
		int *result = (depthFirstSearch(nextPuzzleNode, objPuzzleNode));
		if (result[0] == 0) {
			continue;
		}
		else {
			return result;
		}
	}
	return result;
}

int h1_correct_num(PUZZLE_NODE node) {
	int ans = 0;
	for (int xPos = 0; xPos < 3; xPos++) {
		for (int yPos = 0; yPos < 3; yPos++) {
			if (node.puzzle[xPos * 3 + yPos].puzzleId== (xPos * 3 + yPos)) {
				ans++;
			}
		}
	}
	return ans;
}

int h2_man_dis(PUZZLE_NODE node) {
	int ans = 0;
	int x0 = 0;
	int y0 = 0;
	int id = 0;
	for (int xPos = 0; xPos < 3; xPos++) {
		for (int yPos = 0; yPos < 3; yPos++) {
			id = node.puzzle[xPos * 3 + yPos].puzzleId;
			x0 = id % 3;
			y0 = id - 3 * x0;
			ans += abs(x0 - xPos) + abs(y0 - yPos);
		}
	}
	return ans;
}

bool cmp1(PUZZLE_NODE node1, PUZZLE_NODE node2) {
	int a = -node1.depth + h1_correct_num(node1);
	int b = -node2.depth + h1_correct_num(node2);
	return a < b;
}

bool cmp2(PUZZLE_NODE node1,PUZZLE_NODE node2) {
	int a = -node1.depth + h2_man_dis(node1);
	int b = -node2.depth + h2_man_dis(node2);
	return a < b;
}

//����ʽ����1
int* heuristicSearchInformedByIncorrectNum(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
	//result[1] ���� steps 
	int result[2] = { 0,0 };
	/*
		���ڸ�λ���������ʽ����������ʽ����ʹ�ò���ȷλ�õ����������
	*/
	display_node(initialNode);
	vector<PUZZLE_NODE> vec;
	if (initialNode.depth < 0) {
		initialNode.depth = 0;
	}
	//vec.push_back()
	PUZZLE_NODE currentPuzzleNode = initialNode;
	while (!checkObject(currentPuzzleNode, objPuzzleNode)) {
		PUZZLE_NODE nextPuzzleNode;
		if (currentPuzzleNode.nextActionList.size() == 0) {
			currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
		}
		for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {
			nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
			if (visited[visitedNum(nextPuzzleNode)] == 1) {
				continue;
			}
			if (!currentPuzzleNode.precedeActionList.empty()) {
				for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++){
					nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);
				}
			}
			nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
			nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
			visited[visitedNum(nextPuzzleNode)] = 1;
			vec.push_back(nextPuzzleNode);
		}
		sort(vec.begin(), vec.end(), cmp1);
		currentPuzzleNode = vec[vec.size()-1];
		vec.pop_back();
	}
	for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
		outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
	}
	cout << "�ҵ���ȷ���:" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	result[0] = 1;
	result[1] = currentPuzzleNode.depth;
	return result;
}

//����ʽ����2
int* heuristicSearchInformedByManhattonDis(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
		//result[1] ���� steps 
	int result[2] = { 0,0 };
	/*
		���ڸ�λ���������ʽ����������ʽ����ʹ�ò���ȷλ�õ����������
	*/
	display_node(initialNode);
	vector<PUZZLE_NODE> vec;
	if (initialNode.depth < 0) {
		initialNode.depth = 0;
	}
	//vec.push_back()
	PUZZLE_NODE currentPuzzleNode = initialNode;
	while (!checkObject(currentPuzzleNode, objPuzzleNode)) {
		PUZZLE_NODE nextPuzzleNode;
		if (currentPuzzleNode.nextActionList.size() == 0) {
			currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
		}
		for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {
			nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
			if (visited[visitedNum(nextPuzzleNode)] == 1) {
				continue;
			}
			if (!currentPuzzleNode.precedeActionList.empty()) {
				for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++) {
					nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);
				}
			}
			nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
			nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
			visited[visitedNum(nextPuzzleNode)] = 1;
			vec.push_back(nextPuzzleNode);
		}
		sort(vec.begin(), vec.end(), cmp2);
		currentPuzzleNode = vec[vec.size() - 1];
		vec.pop_back();
	}
	for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
		outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
	}
	cout << "�ҵ���ȷ���:" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	result[0] = 1;
	result[1] = currentPuzzleNode.depth;
	return result;
}


//�����������
int* binaryFirstSearchDemo(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
	//result[1] ���� steps 
	int result[2] = { 0,0 };
	display_node(initialNode);
	cout << "��ʼ�ڵ�״̬��" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	/*
		���ڸ�λ����ɹ����������������
	*/
	PUZZLE_NODE puzzleNode = initialNode;
	queue<PUZZLE_NODE> puzzleNodeQueue;
	puzzleNode.depth = 0;
	int depth = 0;
	puzzleNodeQueue.push(puzzleNode);
	while (puzzleNodeQueue.size()) {
		PUZZLE_NODE currentPuzzleNode = puzzleNodeQueue.front();
		if (checkObject(currentPuzzleNode, objPuzzleNode)) {

			for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
				outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
			}
			cout << "�ҵ���ȷ���:" << endl;
			for (int i = 0; i < 3; i++) {
				cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
					"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
			}
			cout << endl;

			result[0] = 1;
			result[1] = currentPuzzleNode.depth;
			return result;
		}
		else {
			visited[visitedNum(currentPuzzleNode)] = 1;
			if (currentPuzzleNode.nextActionList.size() == 0) {
				currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
			}
			puzzleNodeQueue.pop();
			for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {
				PUZZLE_NODE nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
				if (!currentPuzzleNode.precedeActionList.empty()) {
					for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++)
					{
						nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);
					}
				}
				nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
				if (visited[visitedNum(nextPuzzleNode)] == 1) {
					continue;
				}
				nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
				puzzleNodeQueue.push(nextPuzzleNode);
			}
		}

	}
	return result;
}

int main() {
	PUZZLE_NODE objPuzzleNode;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			objPuzzleNode.puzzle[i * 3 + j].puzzleId = i * 3 + j;
			objPuzzleNode.puzzle[i * 3 + j].xPosition = i;
			objPuzzleNode.puzzle[i * 3 + j].yPosition = j;
		}
	}
	objPuzzleNode = updatePuzzleNodeActionList(objPuzzleNode);

	int setup = 0;
	while (setup != -1) {

		/*//////////////////*
		int ans = 0;
		for (int i = 0; i < 20; i++) {
			PUZZLE_NODE initialNode = initialPuzzleNode(20);
			int *result = binaryFirstSearch(initialNode, objPuzzleNode);
			ans += result[1];
		}
		cout<<"�������   "<<ans<<endl;
		*//////////////////
		visited.clear();
		cout << "-------------------------------------------------" << endl;
		cout << "�������������:" << endl;
		cout<< " -1:�˳�; " << endl;
		cout << "0:�����������ʾ��;" << endl;
		cout << "1:�����������; " << endl;
		cout << "2:�����������; " << endl;
		cout << "3:����ʽ����1; " << endl;
		cout<<  "4:����ʽ����2):" << endl; 
		
		cin >> setup;
		int backwardSteps;
		cout << "��������ڵ���5С�ڵ���20�Ļ��˲���" << endl;
		cin >> backwardSteps;
		while (backwardSteps < 5 || backwardSteps >20) {
			cout << "���������������ڵ���5С�ڵ���20�Ļ��˲���" << endl;
			cin >> backwardSteps;
		}


		PUZZLE_NODE initialNode = initialPuzzleNode(backwardSteps);
		cout << endl;

		
		int* result;
		if (setup == 1) {
			result = binaryFirstSearch(initialNode, objPuzzleNode);
		}
		else if (setup == 2) {
			result = depthFirstSearch(initialNode, objPuzzleNode);
		}
		else if (setup == 3) {
			result = heuristicSearchInformedByIncorrectNum(initialNode, objPuzzleNode);
		}
		else if (setup == 4) {
			result = heuristicSearchInformedByManhattonDis(initialNode, objPuzzleNode);
		}
		else if (setup == 0) {
			cout << "�����������ʾ��:" << endl;
			result = binaryFirstSearchDemo(initialNode, objPuzzleNode);
		}
		else {
			cout << "����������������������" << endl;
			cout << endl;
			return 0;
		}

		if (result[0] == 1) {
			cout << "���Ϊcorrect,����Ϊ" << result[1] << endl;
		}
		else {
			cout << "���Ϊwrong" << endl;
		}
		cout << "-------------------------------------------------" << endl;

		cout << endl;
	}
	return 0;

}
