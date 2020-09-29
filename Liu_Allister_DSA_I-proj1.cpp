//
//  DSA_I-1.cpp
//  
//
//  Created by Allister Liu on 11/20/19.
/*
    Name: Allister Liu
    Course: DSA I
    Instructor: Professor Sable
    
    Programming Assignment #1 - Stack/Queue Implementation & Manipulation
    
    Program Description: This program processes the user's input file & performs the requested stack
    and queue manipulations. During this procedure, the program writes to an output file, specified
    by the user, describing the processing being done or any errors that occur.
 */
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

using namespace std;

template <typename T>
class SimpleList {
public:
    // pure virtual function providing template for derived class to override;
    explicit SimpleList (const string &name);
    //set the list name and constructor to initialize everything;
    virtual void push(T d)=0;
    virtual T pop()=0;
    virtual int getSize() = 0;
    string listName;

protected:
    // set the three function as required and also a int size to trace the size;
    void insert_rear(T d);
    void insert_front(T d);
    T remove_front();
    int size;

private:
    //set up a new class node as required, one field to data and another to pointer point to nex node;
    class Node {
    public:
        Node(T d, Node * ptr) {
            this->data = d;
            this->next = ptr;
        }
        T data;
        Node * next;
    };
    // declare pointer point to front and rear of the linked list;
    Node * front, * rear;
};




//STAAAAAACKKKKKKK

// Derived classes, override push and pop for each stack and queue while set public getsize function allowed for later use;
template <typename T>
class Stack : public SimpleList<T> {
public:
    Stack(const string &name);
    void push (T d);
    T pop ();
    int getSize();
};

//function to insert from front which is the push function for stacks;
template <typename T>
void SimpleList<T>::insert_front(T d) {
    //create a new node and make front point to it
    Node* temp = new Node(d, this->front);
    this->front = temp;
}

//stack constructor;
template <typename T>
Stack<T>::Stack(const string &name) : SimpleList<T>(name) {}

//stack push: insert from the front and increment the size;
template <typename T>
void Stack<T>::push(T d) {
    this->insert_front(d);
    this->size++;
}

//stack pop:remove from the front and decrement the size;
template <typename T>
T Stack<T>::pop() {
    this->size--;
    return this->remove_front();
}

//return the size(protected) for getsize();
template <typename T>
int Stack<T>::getSize () {
    return this->size;
}








//QQQQQQQQQQQQQQQQ

template <typename T>
class Queue : public SimpleList<T> {
public:
    Queue(const string &name);
    void push (T d);
    T pop ();
    int getSize();
};

//insert from rear , basically push function for queue;
template <typename T>
void SimpleList<T>::insert_rear (T d) {
    //create a new node point to the null;
    Node* temp = new Node(d, nullptr);
    //if its an empty list set the front and rear to the new node, otherwise just set rear to the new temp;
    if (this->size == 0) {
        this->rear = temp;
        this->front = temp;
    } else {
        this->rear->next = temp;
        this->rear = temp;
    }
}

//queue constructor;
template <typename T>
Queue<T>::Queue(const string &name) : SimpleList<T>(name) {}

//queue push : insert from the end and increment the size;
template <typename T>
void Queue<T>::push(T d) {
    this->insert_rear(d);
    this->size++;
}

//queue pop: remove from the front and decrement the size;
template <typename T>
T Queue<T>::pop() {
    this->size--;
    return this->remove_front();
}

//return the size(protected) for getsize;
template <typename T>
int Queue<T>::getSize () {
    return this->size;
}








//function to remove from front which is the pop function to both stack and queue;
template <typename T>
T   SimpleList<T>::remove_front () {
    //remove the front data(pop) by making the front point to the next node
    Node * temp = this->front;
    this->front = temp->next;
    T data = temp->data;
    // free the unused memory and return the data removed
    delete temp;
    return data;
}
//use template to realize the search, which run with an iterator
template <typename T>
SimpleList<T> * search_list (const string &name, list<SimpleList<T> *> * l) {
    // use a for loop to see if the target list can be found: if found return the pointer, otherwise return nullptr;
    for (typename list<SimpleList<T> *>::const_iterator it = l->begin(); it != l->end(); ++it) {
        if ((*it)->listName == name) return *it;
    }
    return nullptr;
}

//function to create stack or queue and check if list already exists
template <typename T>
void create_list(list<SimpleList<T> *> *l, const string &listName, const string &listType, ofstream &outFile){
    if(search_list(listName, l) != nullptr){
        outFile << "ERROR: This name already exists!" << endl;
    } else {
        SimpleList<T> * tempList;
        if(listType == "stack"){
            tempList = new Stack<T>(listName);
            l->push_back(tempList);
        } else {
            tempList = new Queue<T>(listName);
            l->push_back(tempList);
        }
    }
}

//function to push values onto the stack or queue
template <typename T>
void push_list(list<SimpleList<T> *> *l, const string &listName, T data, ofstream &outFile){
    SimpleList<T> *tempList = search_list(listName, l);
    if (tempList != nullptr){
        tempList->push(data);
    } else {
        outFile << "ERROR: This name does not exist!" << endl;
    }
}

//function to pop off the stack or queue and return the value popped
template <typename T>
void pop_list(list<SimpleList<T> *> *l, const string &listName, ofstream &outFile){
    SimpleList<T> *tempList = search_list(listName, l);
    if(tempList != nullptr){
        if(tempList->getSize() != 0){
            outFile << "Value popped: " << tempList->pop() << endl;
        } else {
            outFile << "ERROR: This list is empty!" << endl;
        }
    } else {
        outFile << "ERROR: This name does not exist!" << endl;
    }
}










/*simpleList constructor to initialize everything, size to 0 and list's name with front and rear as nullprt since it is an empty list;*/
template <typename T>
SimpleList<T>::SimpleList (const string &name) {
    this->size = 0;
    this->listName = name;
    this->front = nullptr;
    this->rear = nullptr;
}





//function to process all the commands
void translate(const string &input, const string &output){

    //declare three big lists storing pointers to different types of stacks and queues;
    list<SimpleList<int> *> listSLi;
    list<SimpleList<double> *> listSLd;
    list<SimpleList<string> *> listSLs;

    //open the read and write files
    ifstream inFile(input);
    ofstream outFile(output);

    //check if the file is opened, otherwise invalid input file name
    if (inFile.is_open()){
        string action, listName, data, commands;
        char type;

        while(getline(inFile, commands)){
            //use istringstream to parse the line into strings;
            istringstream iss(commands);
            // read the first two word of the line;
            iss >> action >> listName;
            // read the first letter of the name that indicates type
            type = listName.at(0);

            // output processing command as required;
            outFile << "PROCESSING COMMAND: " << commands << endl;

            //if command is create, use the create_list function
            if(action == "create"){
                iss >> data;
                if(type == 'i'){
                    create_list<int> (&listSLi, listName, data, outFile);
                }
                else if(type == 'd'){
                    create_list<double> (&listSLd, listName, data, outFile);
                } else {
                    create_list<string> (&listSLs, listName, data, outFile);
                }
            }
            //if commands is push, use the push_list function
            else if(action == "push"){
                iss >> data;
                if(type == 'i'){
                    int tmp = stoi(data);
                    push_list<int> (&listSLi, listName, tmp, outFile);
                }
                else if(type == 'd'){
                    double tmp = stod(data);
                    push_list<double> (&listSLd, listName, tmp, outFile);
                } else {
                    push_list<string> (&listSLs, listName, data, outFile);
                }
            }
            //if the command is pop, use the pop_list function
            else{
                if(type == 'i'){
                    pop_list<int> (&listSLi, listName, outFile);
                }
                else if(type == 'd'){
                    pop_list<double> (&listSLd, listName, outFile);
                } else {
                    pop_list<string> (&listSLs, listName, outFile);
                }
            }
        }
        // close both files to ensure it can be reused again;
        inFile.close();
        outFile.close();
    } else {
        outFile << "ERROR: The input file does not exist!";
    }
}

int main() {
    string input_file, output_file;
    //ask for the input and output name;
    cout << "Enter name of input file: ";
    cin >> input_file;
    cout << "Enter name of output file: ";
    cin >> output_file;
    //use the process function;
    translate (input_file, output_file);

    return 0;
}
