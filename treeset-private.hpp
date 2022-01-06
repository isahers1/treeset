#ifndef TREESET_PRIVATE_HPP_INCLUDED

#define TREESET_PRIVATE_HPP_INCLUDED

#include <utility>
// Include the testing-logger library from
// the CS70 system directory in Docker.
#include <cs70/testinglogger.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>
#include <cs70/randuint32.hpp>

using namespace std;

template <typename T>
TreeSet<T>::TreeSet() : root_(nullptr),
        type_(treetype::LEAF), rand_{0} { }

template <typename T>
TreeSet<T>::TreeSet(treetype t) : root_(nullptr), type_(t), rand_{0} { }

template <typename T>
TreeSet<T>::TreeSet(treetype t, size_t s) : root_(nullptr),
     type_(t), rand_{s} { }

template <typename T>
TreeSet<T>::~TreeSet() {
    // only delete if tree actually exists
    if (root_ != nullptr) {
        deleteHelper(root_);
    }
}

template <typename T>
bool TreeSet<T>::consistent() const {
    return (((root_ == nullptr) && (root_->size_ == 0)) ||
            ((root_ != nullptr) && (root_->size_ > 0)));
}

template <typename T>
void TreeSet<T>::deleteHelper(Node*& tree) {
    // if we are past a leaf
    if (tree != nullptr) {
        // recursively delete left child if it exists
        deleteHelper(tree->leftChild_);
        // recursively delete right child if it exists
        deleteHelper(tree->rightChild_);
        // only after right and left child have been deleted
        // can we remove current node from the heap
        delete tree;
    }
}

template <typename T>
size_t TreeSet<T>::size() const {
    if (root_ == nullptr) {
        return 0;
    }
    return root_->size_;
}

template <typename T>
void TreeSet<T>::insertAtLeaf(Node*& tree, const T& val) {
    // if we are inserting into empty tree, tree become node containing valing
    if (tree == nullptr) {
        tree = new Node{val};  // Assumes a constructor for Node
    } else if (val < tree->value_) {  // insert in left tree if it's less
        insertAtLeaf(tree->leftChild_, val);
        ++tree->size_;
    } else {  // insert in right tree if valing is greater than current value
        insertAtLeaf(tree->rightChild_, val);
        ++tree->size_;
    }
}

template <typename T>
void TreeSet<T>::rotateRight(Node*& root) {
    // set newRightChild and newRoot as well as new rightchild's new left
    Node* newRightChild = root;
    Node* newRoot = root->leftChild_;
    Node* newRCLeftChild = newRoot->rightChild_;

    // Change newRightChilds left child to be newRCLeftChild
    newRightChild->leftChild_ = newRCLeftChild;

    // change size of the new right child
    setNodeSize(newRightChild);

    // asign new right child and change size of new root
    newRoot->rightChild_ = newRightChild;

    setNodeSize(newRoot);
    // reassign the root of the subtree
    root = newRoot;
}

template <typename T>
void TreeSet<T>::setNodeSize(Node*& cur) {
    cur->size_ = 1;
    if (cur->leftChild_ != nullptr) {
        cur->size_ += cur->leftChild_->size_;
    }
    if (cur->rightChild_ != nullptr) {
        cur->size_ += cur->rightChild_->size_;
    }
}

template <typename T>
void TreeSet<T>::rotateLeft(Node*& root) {
    // set newLeftChild and newRoot as well as new leftchild's new right
    Node* newLeftChild = root;
    Node* newRoot = root->rightChild_;
    Node* newLCRightChild = newRoot->leftChild_;

    // Change newLeftChild left child to be newLCRightChild
    newLeftChild->rightChild_ = newLCRightChild;

    // change size of the new left child
    setNodeSize(newLeftChild);

    newRoot->leftChild_ = newLeftChild;

    // change size of the new root
    setNodeSize(newRoot);

    // reassign the root of the subtree
    root = newRoot;
}

template <typename T>
void TreeSet<T>::insertAtRoot(Node*& tree, const T& val) {
    // if we are inserting into empty tree, tree become node containing valing
    if (tree == nullptr) {
        tree = new Node{val};  // Assumes a constructor for Node
    } else if (val < tree->value_) {  // insert in left tree if it's less
        insertAtRoot(tree->leftChild_, val);
        rotateRight(tree);
    } else {  // insert in right tree if valing is greater than current value
        insertAtRoot(tree->rightChild_, val);
        rotateLeft(tree);
    }
}

template <typename T>
void TreeSet<T>::insertAtRandom(Node*& tree, const T& val) {
    int randomInt;
    if (tree == nullptr) {
        // empty we always insert at root
        randomInt = rand_.get(1);
    } else {
        // Get a random number from 0 to node->size
        randomInt = rand_.get(tree->size_ + 1);
    }
    // Check if the number is 0 (1/size probability)
    bool doRootInsert = (randomInt == 0);

    // If the random number is 0, the new node becomes the root
    if (doRootInsert) {
        insertAtRoot(tree, val);
    } else {
    // Either add to the left subtree or right subtree based on the key_
        if (val < tree->value_) {
            insertAtRandom(tree->leftChild_, val);
        } else {
            insertAtRandom(tree->rightChild_, val);
        }
        ++tree->size_;
    }
}

template <typename T>
void TreeSet<T>::insert(const T& val) {
    // only insert if it does not exist in tree already
    if (!exists(val)) {
        if (type_ == treetype::LEAF) {
            insertAtLeaf(root_, val);
        } else if (type_ == treetype::ROOT) {
            insertAtRoot(root_, val);
        } else {
            insertAtRandom(root_, val);
        }
    }
}

template <typename T>
bool TreeSet<T>::existsHelper(const Node* tree, const T& val) const {
    // if tree is empty no elements can exist
    if (tree == nullptr) {
        return false;
    } else if (val == tree->value_) {  // return true if we find the value
        return true;
    } else if (val < tree->value_) {  // check left child if val is less
        return existsHelper(tree->leftChild_, val);
    } else {  // check right child if val is greater than current value
        return existsHelper(tree->rightChild_, val);
    }
}

template <typename T>
bool TreeSet<T>::exists(const T& val) const {
    return existsHelper(root_, val);
}

template <typename T>
bool TreeSet<T>::operator==(const TreeSet<T>& rhs) const {
    // check that sizes are equal
    if (size() != rhs.size()) {
        return false;
    }
    // if sizes are equal, iterate through one tree to check that all values
    // present are present in the other tree as well
    for (TreeSet<T>::iterator iter = begin(); iter != end(); ++iter) {
        if (!(rhs.exists(*iter))) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool TreeSet<T>::operator!=(const TreeSet<T>& rhs) const {
    return !operator==(rhs);
}

template <typename T>
ostream& operator<<(ostream& os, const TreeSet<T>& t) {
    return t.print(os);
}

template <typename T>
ostream& TreeSet<T>::printerHelper(const Node* tree, ostream& os) const {
    // print nothing if tree is empty
    if (tree == nullptr) {
        os << "-";
    } else {  // print using CS70 conventions otherwise
        os << "(";
        printerHelper(tree->leftChild_, os);
        os << ", " << tree->value_ << ", ";
        printerHelper(tree->rightChild_, os);
        os << ")";
    }
    return os;
}

template <typename T>
ostream& TreeSet<T>::print(ostream& os) const {
    return printerHelper(root_, os);
}

template <typename T>
typename TreeSet<T>::iterator TreeSet<T>::begin() const {
    return TreeSet<T>::Iterator(root_);
}

template <typename T>
typename TreeSet<T>::iterator TreeSet<T>::end() const {
    return TreeSet<T>::Iterator(nullptr);
}

template <typename T>
TreeSet<T>::Iterator::Iterator(Node* current) : current_(current) {
    // Nothing else to do.
}

template <typename T>
typename TreeSet<T>::Iterator& TreeSet<T>::Iterator::operator++() {
    if (current_->leftChild_ != nullptr) {  // add left child to node queue
        pending_.push(current_->leftChild_);
    }
    if (current_->rightChild_ != nullptr) {  // add right child to node queue
        pending_.push(current_->rightChild_);
    }
    if (!pending_.empty()) {  // visit next node
        current_ = pending_.front();
        pending_.pop();
    } else {  // if at end, set current to null pointer
        current_ = nullptr;
    }
    return *this;
}

template <typename T>
T& TreeSet<T>::Iterator::operator*() const {
    return current_ -> value_;
}

template <typename T>
T* TreeSet<T>::Iterator::operator->() const {
  return &(**this);
}

template <typename T>
bool TreeSet<T>::Iterator::operator==(const Iterator& rhs) const {
    return (current_ == rhs.current_) & (pending_ == rhs.pending_);
}

template <typename T>
bool TreeSet<T>::Iterator::operator!=(const Iterator& rhs) const {
    // Idiomatic code: leverage == to implement !=
    return !(*this == rhs);
}

template <typename T>
TreeSet<T>::Node::Node(T val)
    : value_(val), leftChild_(nullptr), rightChild_(nullptr), size_(1) {
        // nothing else to do
}

template <typename T>
double TreeSet<T>::depthHelper(const Node* cur, double& h) const {
    // if doesn't exist, it has no depth
    if (cur == nullptr) {
        return 0;
        // if neither child exists, we are at leaf so return height
    } else if (cur->leftChild_ == nullptr && cur->rightChild_ == nullptr) {
        return h;
    }
    // add one to height and recursively find total depth
    double newHeight = h + 1;
    return h + depthHelper(cur->leftChild_, newHeight)
                    + depthHelper(cur->rightChild_, newHeight);
}

template <typename T>
double TreeSet<T>::averageDepth() const {
    if (root_ == nullptr) {
        return 0.0;
    } else {
        double total = 0;
        return depthHelper(root_, total)/static_cast<double>(root_->size_);
    }
}

template <typename T>
int TreeSet<T>::heightHelper(const Node* current) const {
    // if we are at null it has no height
    if (current == nullptr) {
        return -1;
    } else {  // else return 1 plus max of height of right tree, left tree
        return 1 + max(heightHelper(current->leftChild_),
                        heightHelper(current->rightChild_));
    }
}

template <typename T>
int TreeSet<T>::height() const {
    return heightHelper(root_);
}

template <typename T>
ostream& TreeSet<T>::showStatistics(ostream& os) const {
    os << size() << " nodes, height " << height() << ", average depth "
        << averageDepth() << endl;
    return os;
}

template <typename T>
ostream& TreeSet<T>::printSizesHelper(ostream& os, Node*& cur) const {
    if (cur == nullptr) {
        os << "0";
    } else {
        os << "(" << printSizesHelper(os, cur->leftChild_) << ", " <<
        to_string(cur->size_) << ", " << printSizesHelper(os, cur->rightChild_)
        << ")";  // follow cs 70 idioms for printing out sizes
    }
    return os;
}

template <typename T>
ostream& TreeSet<T>::printSizes(ostream& os) const {
    // if tree is empty print "0"
    if (size() == 0) {
        os << "0";
        return os;
    } else {
        return printSizesHelper(os, root_);
    }
}

#endif
