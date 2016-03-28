//
//  queue.h
//  osh
//
//  Created by Bitex Kuang on 3/27/15.
//  Copyright (c) 2015 Bitex. All rights reserved.
//

#ifndef osh_queue_h
#define osh_queue_h

#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 80

typedef char* string;

typedef struct Node {
    string elem;
    struct Node * prev, * next;
} Node, *Queue;

Queue CreateQueue();
Node * CreateNode();
void Enqueue(Queue queue, string elem);
void Dequeue(Queue queue);
Node * GetQueueElement(Queue queue, int index);
Node * QueueHead(Queue queue);
Node * QueueTail(Queue queue);
int QueueLength(Queue queue);
int QueueEmpty(Queue queue);

#endif
