//
//  queue.c
//  osh
//
//  Created by Bitex Kuang on 3/27/15.
//  Copyright (c) 2015 Bitex. All rights reserved.
//

#include <stdio.h>
#include "queue.h"

Queue CreateQueue() {
    Queue queue = (Queue) malloc(sizeof (Node));
    queue->prev = queue->next = NULL;
    return queue;
}

Node * CreateNode() {
    Node * node = (Node *) malloc(sizeof (Node));
    node->elem = (char *) malloc(MAX_STRING_LENGTH * sizeof (char));
    node->prev = node->next = NULL;
    return node;
}

void Enqueue(Queue queue, string elem) {
    Node * node = queue;
    while (node->next != NULL) {
        node = node->next;
    }
    Node * newNode = CreateNode();
    strcpy(newNode->elem, elem);
    node->next = newNode;
    newNode->prev = node;
}

void Dequeue(Queue queue) {
    if (queue->next != NULL) {
        Node * firstNode = queue->next;
        queue->next = firstNode->next;
        firstNode->next->prev = queue;
        free(firstNode);
    }
}

Node * GetQueueElement(Queue queue, int index) {
    int i;
    Node * node = queue;
    for (i = 0; i <= index; i ++) {
        node = node->next;
        if (node == NULL) {
            return NULL;
        }
    }
    return node;
}

Node * QueueHead(Queue queue) {
    return queue->next;
}

Node * QueueTail(Queue queue) {
    if (queue->next == NULL) {
        return NULL;
    } else {
        Node * node = queue->next;
        while (node->next != NULL) {
            node = node->next;
        }
        return node;
    }
}

int QueueLength(Queue queue) {
    int n = 0;
    Node * node = queue->next;
    while (node != NULL) {
        n ++;
        node = node->next;
    }
    return n;
}

int QueueEmpty(Queue queue) {
    return (queue->next == 0);
}
