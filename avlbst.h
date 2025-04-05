#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight(AVLNode<Key, Value>* n);
    void rotateLeft(AVLNode<Key, Value>* n);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int diff);
};

// Rotation helper functions
template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* n)
{
    // Important nodes in relation to node
    AVLNode<Key,Value>* parent = n->getParent();
    AVLNode<Key,Value>* left = n->getLeft();
    // Return if child doesn't exist (prevents rotating w/ nullptr)
    if (left == nullptr) return;
    AVLNode<Key,Value>* rightOfLeft = left->getRight();
    // Algorithm
    if (parent != nullptr) {
        if (parent->getLeft() == n) parent->setLeft(left);
        else parent->setRight(left);
    }
    else this->root_ = left;
    if (rightOfLeft != nullptr) rightOfLeft->setParent(n);
    left->setParent(parent);
    left->setRight(n);
    n->setLeft(rightOfLeft);
    n->setParent(left);         
}


template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* n)
{
    // Important nodes in relation to node
    AVLNode<Key,Value>* parent = n->getParent();
    AVLNode<Key,Value>* right = n->getRight();
    // Return if child doesn't exist (prevents rotating w/ nullptr)
    if (right == nullptr) return;
    AVLNode<Key,Value>* leftOfRight = right->getLeft();
    // Algorithm
    if (parent != nullptr) {
        if (parent->getRight() == n) parent->setRight(right);
        else parent->setLeft(right);
    }
    else this->root_ = right;
    if (leftOfRight != nullptr) leftOfRight->setParent(n);
    right->setParent(parent);
    right->setLeft(n);
    n->setRight(leftOfRight);
    n->setParent(right);   
}

// Function to balance after insert
template <class Key, class Value>
void  AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    AVLNode<Key, Value>* g = p->getParent();
    if (p == nullptr || g == nullptr) return;
    // If p is left child of g
    if (g->getLeft() == p) {
        g->updateBalance(-1);
        if (g->getBalance() == 0) return;
        if (g->getBalance() == -1) insertFix(g, p);
        // Zig-Zig Case
        else if (p->getLeft() == n) {
            rotateRight(g);
            p->setBalance(0);
            g->setBalance(0);
        }
        // Zig-Zag Case
        else {
            rotateLeft(p);
            rotateRight(g);
            if (n->getBalance() == -1) {
                p->setBalance(0);
                g->setBalance(1);
                n->setBalance(0);
            }
            else if (n->getBalance() == 0) {
                p->setBalance(0);
                g->setBalance(0);
                n->setBalance(0);
            }
            else {
                p->setBalance(-1);
                g->setBalance(0);
                n->setBalance(0);
            }
        }
    }
    // If p is right child of g
    else {
        g->updateBalance(1);
        if (g->getBalance() == 0) return;
        if (g->getBalance() == 1) insertFix(g, p);
        // Zig-Zig Case
        else if (p->getRight() == n) {
            rotateLeft(g);
            p->setBalance(0);
            g->setBalance(0);
        }
        // Zig-Zag Case
        else {
            rotateRight(p);
            rotateLeft(g);
            if (n->getBalance() == 1) {
                p->setBalance(0);
                g->setBalance(-1);
                n->setBalance(0);
            }
            else if (n->getBalance() == 0) {
                p->setBalance(0);
                g->setBalance(0);
                n->setBalance(0);
            }
            else {
                p->setBalance(1);
                g->setBalance(0);
                n->setBalance(0);
            }
        }
    }
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    // If tree is empty then make root node
    if (static_cast<AVLNode<Key, Value>*>(this->root_) == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    // Find correct parent for new node or overwrite existing key
    AVLNode<Key, Value>* parent = nullptr;
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (temp != nullptr) {
        parent = temp;
        if (new_item.first < parent->getKey()) {temp = temp->getLeft();}
        else if (new_item.first > parent->getKey()) {temp = temp->getRight();}
        else {
            temp->setValue(new_item.second);
            return;
        }
    }
    // Add node to tree
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (newNode->getKey() < parent->getKey()) parent->setLeft(newNode);
    else parent->setRight(newNode);
    // NOW balance if needed
    if (parent->getBalance() == -1 || parent->getBalance() == 1) parent->setBalance(0);
    else if (parent->getLeft() == newNode) {
        parent->updateBalance(-1);
        insertFix(parent, newNode);
    }
    else {
        parent->updateBalance(1);
        insertFix(parent, newNode);
    }
}

// Function to balance after remove
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
    if (n == nullptr) return;
    // Calculate arguments for recursive call
    AVLNode<Key, Value>* p = n->getParent();
    int ndiff;
    if (p != nullptr) {
        if (p->getLeft() == n) ndiff = 1;
        else ndiff = -1;
    }
    // If diff = -1
    if (diff == -1) {
        // Case 1: b(n) + diff == -2
        if (n->getBalance() + diff == -2) {
            AVLNode<Key, Value>* c = n->getLeft();
            // Zig-Zig Case
            if (c->getBalance() == -1) {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Zig-Zig Case
            else if (c->getBalance() == 0) {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
            }
            // Zig-Zag Case
            else if (c->getBalance() == 1) {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);
                if (g->getBalance() == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        // Case 2: b(n) + diff == -1
        else if (n->getBalance() + diff == -1) n->setBalance(-1);
        // Case 3: b(n) + diff == 0
        else if (n->getBalance() + diff == 0) {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
    // If diff = 1, mirror case
    else {
        // Case 1: b(n) + diff == 2
        if (n->getBalance() + diff == 2) {
            AVLNode<Key, Value>* c = n->getRight();
            // Zig-Zig Case
            if (c->getBalance() == 1) {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
            }
            // Zig-Zig Case
            else if (c->getBalance() == 0) {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
            }
            // Zig-Zag Case
            else if (c->getBalance() == -1) {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);
                if (g->getBalance() == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                removeFix(p, ndiff);
            }
        }
        // Case 2: b(n) + diff == 1
        else if (n->getBalance() + diff == 1) n->setBalance(1);
        // Case 3: b(n) + diff == 0
        else if (n->getBalance() + diff == 0) {
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    // If node is found, then do appropriate swaps / removal
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (temp != nullptr) {
        AVLNode<Key, Value>* child = nullptr;
        AVLNode<Key, Value>* parent = temp->getParent();
        // If no children, update parent and delete
        if (temp->getLeft() == nullptr && temp->getRight() == nullptr) {}
        // If 1 child, promote child
        else if (temp->getLeft() == nullptr) {
            child = temp->getRight();
            child->setParent(temp->getParent());
        }
        else if (temp->getRight() == nullptr) {
            child = temp->getLeft();
            child->setParent(temp->getParent());
        }
        // If 2 children, swap with predecessor, then check if there is still a left child
        else {
            AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
            nodeSwap(temp, pred);
            parent = temp->getParent();
            if (temp->getLeft() != nullptr) {
                child = temp->getLeft();
                child->setParent(temp->getParent());
            }
        }
        // Update parent and delete node and calculate diff, if deleting root then replace w/ child
        int diff = 0;
        if (parent == nullptr) this->root_ = child;
        else if (parent->getLeft() == temp) {
            parent->setLeft(child);
            diff = 1;
        }
        else if (parent->getRight() == temp) {
            parent->setRight(child);
            diff = -1;
        }
        delete temp;
        // Patch tree
        removeFix(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
