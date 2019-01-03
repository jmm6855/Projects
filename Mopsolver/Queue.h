//@author Jacob Manelius
//header file for queue structure

#ifndef QUEUE_H
#define QUEUE_H

//node structure for use in the queue
typedef struct Qnode
{
    int row;
    int col;
    bool visited;
    struct Qnode* parent;
}Qnode;

//queue structure to hold and organzie nodes
typedef struct Queue
{
    int front;
    int rear;
    int size;
    int capacity;
    struct Qnode** list;
}Queue;

//crate a new node
Qnode* createNode(int row, int col, Qnode* parent);

//create a new, empty, queue
Queue* createQueue();

//add a new item to the end of the queue
void enqueue(Queue* queue, Qnode* newNode);

//check if the queue is empty
bool isEmpty(Queue* queue);

//remove the first item from the queue
Qnode* dequeue(Queue* queue);

//get the front item of the queue without removing it
Qnode* getFront(Queue* queue);

//check the queue for an equivalent item
bool contains(Queue* queue, Qnode* node);

//free the queue
void freeQueue(Queue* queue);

#endif
