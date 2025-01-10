#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <functional>

template <typename T>
class LocklessQueue
{
private:
    struct Node
    {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    LocklessQueue()
    {
        Node* dummy = new Node(T());
        head.store(dummy, std::memory_order_relaxed);
        tail.store(dummy, std::memory_order_relaxed);
    }

    ~LocklessQueue()
    {
        while (Node* node = head.load(std::memory_order_relaxed))
        {
            head.store(node->next, std::memory_order_relaxed);
            delete node;
        }
    }
    // Adjust tail pointer atomically
    void enqueue(const T& value)
    {
        Node* newNode = new Node(value);
        Node* oldTail = tail.exchange(newNode, std::memory_order_acq_rel);
        oldTail->next = newNode;
    }

    bool dequeue(T& result)
    {
        Node* oldHead = head.load(std::memory_order_acquire);
        Node* next = oldHead->next;

        if (next == nullptr)
        {
            return false; // Queue is empty
        }

        result = next->data;
        head.store(next, std::memory_order_release);
        delete oldHead;
        return true;
    }
};