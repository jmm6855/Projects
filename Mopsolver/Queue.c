//@author Jacob Manelius
//queue implemention

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Queue.h"

//create a new node
Qnode* createNode(int row, int col, Qnode* parent)
{
    Qnode* node = (Qnode*)malloc(sizeof(Qnode));
    node->row = row;
    node->col = col;
    node->parent = parent;
    return node;
}

//create a new empty queue
Queue* createQueue()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->size = 0;
    queue->front = 0;
    queue->capacity = 0;
    queue->list = (Qnode**)malloc(sizeof(Qnode*));
    return queue;
}

//add a node to the queue
void enqueue(Queue* queue, Qnode* newNode)
{
    //increase the size of the queue
    queue->list = (Qnode**)realloc(queue->list, sizeof(Qnode*)*(queue->capacity+1));
    queue->capacity = queue->capacity + 1;
    queue->rear = queue->capacity - 1;
    queue->list[queue->rear] = newNode;
    queue->size = queue->size + 1;
}

//determine if the queue is empty
bool isEmpty(Queue* queue)
{
    if(queue->size == 0)
        return true;
    else
        return false;
}

//remove the first node from the queue
//decrement the size of the queue and move the front value
Qnode* dequeue(Queue* queue)
{
    Qnode* returnNode = queue->list[queue->front];
    queue->front = queue->front + 1;
    queue->size = queue->size - 1;
    return returnNode;
}

//get the front of the queue without removing it
Qnode* getFront(Queue* queue)
{
    return queue->list[queue->front];
}

//check if the queue contains a given node
bool contains(Queue* queue, Qnode* node)
{
    for(int i = 0; i < queue->capacity; i++)
    {
        if(queue->list[i]->row == node->row && queue->list[i]->col == node->col)
            return true;
    }
    return false;
}

//free resources for the queue
void freeQueue( Queue* queue )
{
    //free nodes
    for(int i = 0; i < queue->capacity; i++)
    {
        free(queue->list[i]);
    }
    //free list and queue
    free(queue->list);
    free(queue);
}
