#include <iostream>
#include <fstream>
#include <string>
#include <cstring> // for string tokenizer and c-style string processing
#include <algorithm> // max function

using namespace std;

class BTNode {

private:
	int nodeid;
	int data;
	int levelNum;
	BTNode* leftChildPtr;
	BTNode* rightChildPtr;

public:

	BTNode() {}

	void setNodeId(int id) {
		nodeid = id;
	}

	int getNodeId() {
		return nodeid;
	}

	void setData(int d) {
		data = d;
	}

	int getData() {
		return data;
	}

	void setLevelNum(int level) {
		levelNum = level;
	}

	int getLevelNum() {
		return levelNum;
	}

	void setLeftChildPtr(BTNode* ptr) {
		leftChildPtr = ptr;
	}

	void setRightChildPtr(BTNode* ptr) {
		rightChildPtr = ptr;
	}

	BTNode* getLeftChildPtr() {
		return leftChildPtr;
	}

	BTNode* getRightChildPtr() {
		return rightChildPtr;
	}

	int getLeftChildID() {
		if (leftChildPtr == 0)
			return -1;

		return leftChildPtr->getNodeId();
	}

	int getRightChildID() {
		if (rightChildPtr == 0)
			return -1;

		return rightChildPtr->getNodeId();
	}
};



class Node {

private:
	int data;
	Node* nextNodePtr;
	Node* prevNodePtr;

public:
	Node() {}

	void setData(int d) {
		data = d;
	}

	int getData() {
		return data;
	}

	void setNextNodePtr(Node* nodePtr) {
		nextNodePtr = nodePtr;
	}

	Node* getNextNodePtr() {
		return nextNodePtr;
	}

	void setPrevNodePtr(Node* nodePtr) {
		prevNodePtr = nodePtr;
	}

	Node* getPrevNodePtr() {
		return prevNodePtr;
	}

};

class Queue {

private:
	Node* headPtr;
	Node* tailPtr;


public:
	Queue() {
		headPtr = new Node();
		tailPtr = new Node();
		headPtr->setNextNodePtr(0);
		tailPtr->setPrevNodePtr(0);
	}

	Node* getHeadPtr() {
		return headPtr;
	}

	Node* getTailPtr() {
		return tailPtr;
	}

	bool isEmpty() {

		if (headPtr->getNextNodePtr() == 0)
			return true;

		return false;
	}


	void enqueue(int data) {

		Node* newNodePtr = new Node();
		newNodePtr->setData(data);
		newNodePtr->setNextNodePtr(0);

		Node* lastNodePtr = tailPtr->getPrevNodePtr();

		if (lastNodePtr == 0) {

			headPtr->setNextNodePtr(newNodePtr);
			newNodePtr->setPrevNodePtr(0);

		}
		else {

			lastNodePtr->setNextNodePtr(newNodePtr);
			newNodePtr->setPrevNodePtr(lastNodePtr);

		}

		tailPtr->setPrevNodePtr(newNodePtr);

	}


	int dequeue() {

		Node* firstNodePtr = headPtr->getNextNodePtr();
		Node* nextNodePtr = 0;

		int poppedData = -100000; //empty queue

		if (firstNodePtr != 0) {
			nextNodePtr = firstNodePtr->getNextNodePtr();
			poppedData = firstNodePtr->getData();
		}
		else
			return poppedData;

		if (nextNodePtr != 0) {
			nextNodePtr->setPrevNodePtr(0);
			headPtr->setNextNodePtr(nextNodePtr);
		}
		else {
			headPtr->setNextNodePtr(0);
			tailPtr->setPrevNodePtr(0);
		}

		return poppedData;

	}


	int peek() {

		Node* firstNodePtr = headPtr->getNextNodePtr();

		if (firstNodePtr != 0)
			return firstNodePtr->getData();
		else
			return -100000; //empty queue

	}


};





class BinaryTree {

private:
	int numNodes;
	BTNode* arrayOfBTNodes;

public:

	BinaryTree(int n) {
		numNodes = n;
		arrayOfBTNodes = new BTNode[numNodes];

		for (int id = 0; id < numNodes; id++) {
			arrayOfBTNodes[id].setNodeId(id);
			arrayOfBTNodes[id].setLevelNum(-1);
			arrayOfBTNodes[id].setLeftChildPtr(0);
			arrayOfBTNodes[id].setRightChildPtr(0);
		}
	}

	void setLeftLink(int upstreamNodeID, int downstreamNodeID) {
		arrayOfBTNodes[upstreamNodeID].setLeftChildPtr(&arrayOfBTNodes[downstreamNodeID]);
	}

	void setRightLink(int upstreamNodeID, int downstreamNodeID) {
		arrayOfBTNodes[upstreamNodeID].setRightChildPtr(&arrayOfBTNodes[downstreamNodeID]);
	}

	void printLeafNodes() {

		for (int id = 0; id < numNodes; id++) {

			if (arrayOfBTNodes[id].getLeftChildPtr() == 0 && arrayOfBTNodes[id].getRightChildPtr() == 0)
				cout << id << " ";
		}

		cout << endl;
	}


	bool isLeafNode(int nodeid) {

		if (arrayOfBTNodes[nodeid].getLeftChildPtr() == 0 && arrayOfBTNodes[nodeid].getRightChildPtr() == 0)
			return true;

		return false;
	}

	int getNodeHeight(int nodeid) {

		if (nodeid == -1 || isLeafNode(nodeid))
			return 0;

		int leftChildID = arrayOfBTNodes[nodeid].getLeftChildID(); // -1 if not exist
		int rightChildID = arrayOfBTNodes[nodeid].getRightChildID(); // -1 if not exist

		return max(getNodeHeight(leftChildID), getNodeHeight(rightChildID)) + 1;

	}


	int getTreeHeight() {
		return getNodeHeight(0);
	}



	void assignLevelNumbers() {

		Queue queue;
		queue.enqueue(0);
		arrayOfBTNodes[0].setLevelNum(0);

		while (!queue.isEmpty()) {

			int firstNodeInQueue = queue.dequeue();

			int leftChildID = arrayOfBTNodes[firstNodeInQueue].getLeftChildID();
			if (leftChildID != -1) {
				queue.enqueue(leftChildID);
				arrayOfBTNodes[leftChildID].setLevelNum(arrayOfBTNodes[firstNodeInQueue].getLevelNum() + 1);
			}

			int rightChildID = arrayOfBTNodes[firstNodeInQueue].getRightChildID();
			if (rightChildID != -1) {
				queue.enqueue(rightChildID);
				arrayOfBTNodes[rightChildID].setLevelNum(arrayOfBTNodes[firstNodeInQueue].getLevelNum() + 1);
			}

		}


	}

	int getDepth(int nodeid) {
		return arrayOfBTNodes[nodeid].getLevelNum();
	}


	bool checkCompleteBinaryTree() {
		// Need to do three things
		// (1) Get the height of the tree
		// (2) Assign the level numbers for each node
		// (3) If a node is not a leaf node (i.e., an internal node), check if its has two child nodes
		//     If a node is a leaf node, check if its level number equals the height of the tree
		int heightOfTree = getTreeHeight();
		assignLevelNumbers();
		for (int id = 0; id < numNodes; id++) {
			if (!isLeafNode(id)) {
				if (arrayOfBTNodes[id].getLeftChildID() == -1 || arrayOfBTNodes[id].getRightChildID() == -1)// -1 if not exist
					return false;
			}
			else
				if (!(getDepth(id) == heightOfTree))
					return false;
		}
	}

};



int main() {

	string filename;
	cout << "Enter a file name: ";
	cin >> filename;

	int numNodes;
	cout << "Enter number of nodes: ";
	cin >> numNodes;

	BinaryTree binaryTree(numNodes);

	ifstream fileReader(filename);

	if (!fileReader) {
		cout << "File cannot be opened!! ";
		return 0;
	}

	int numCharsPerLine = 10;

	char *line = new char[numCharsPerLine];
	// '10' is the maximum number of characters per line

	fileReader.getline(line, numCharsPerLine, '\n');
	// '\n' is the delimiting character to stop reading the line

	while (fileReader) {
		char* cptr = strtok(line, ",: ");
		string upstreamNodeToken(cptr);
		int upstreamNodeID = stoi(upstreamNodeToken);
		cptr = strtok(NULL, ",: ");
		int childIndex = 0; // 0 for left child; 1 for right child
		while (cptr != 0) {
			string downstreamNodeToken(cptr);
			int downstreamNodeID = stoi(downstreamNodeToken);
			if (childIndex == 0 && downstreamNodeID != -1)
				binaryTree.setLeftLink(upstreamNodeID, downstreamNodeID);
			if (childIndex == 1 && downstreamNodeID != -1)
				binaryTree.setRightLink(upstreamNodeID, downstreamNodeID);
			cptr = strtok(NULL, ",: ");
			childIndex++;
		}
		fileReader.getline(line, numCharsPerLine, '\n');
	}



	if (binaryTree.checkCompleteBinaryTree())
		cout << "The binary tree is a complete binary tree " << endl;
	else
		cout << "The binary tree is not a complete binary tree " << endl;


	return 0;
}
