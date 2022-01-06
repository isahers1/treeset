#ifndef TREESET_HPP_INCLUDED

#define TREESET_HPP_INCLUDED

#include <cs70/randuint32.hpp>
#include <cstddef>
#include <queue>
#include <iostream>
#include <utility>
#include <string>

using namespace std;

enum treetype { LEAF, ROOT, RANDOMIZED };

template <typename T>
class TreeSet {
 private:
    // Forward declaration of private class.
    class Iterator;

 public:
    TreeSet();
    ~TreeSet();
    TreeSet(treetype t);
    TreeSet(treetype t, size_t s);
    TreeSet(const TreeSet& orig) = delete;
    TreeSet& operator=(const TreeSet& rhs) = delete;

    // member functions
    /**
    * \brief Calculate size of Tree
    * \param None
    * \returns size of Tree
    **/
    size_t size() const;

    /**
    * \brief Insert element into current Tree
    * \param T to insert
    * \returns void
    **/
    void insert(const T &t);

    /**
    * \brief Check whether T exists in Tree
    * \param T to check
    * \returns boolean whether element exists or not
    **/
    bool exists(const T &t) const;

    /**
    * \brief Calculate height of Tree
    * \param None
    * \returns height of Tree
    **/
    int height() const;

    /**
    * \brief Calculate average depth of Tree
    * \param None
    * \returns Average depth of Tree
    **/
    double averageDepth() const;

    /**
    * \brief Print Tree using CS70 rules
    * \param os stream to print into
    * \returns ostream&
    **/
    ostream& print(ostream& os) const;

    /**
    * \brief Print Tree sizes using CS70 rules
    * \param os stream to print into
    * \returns ostream&
    **/
    ostream& printSizes(ostream& os) const;

    /**
    * \brief Calculate and print out statistics for a Tree
    * \param os stream to print to
    * \returns ostream with printed statistics
    **/
    ostream& showStatistics(ostream& os) const;

    // allow users to iterate through Tree
    using iterator = Iterator;
    // An iterator that refers to the first node
    iterator begin() const;
    // An iterator that refers to node just after last node
    iterator end() const;

    // operators
    bool operator==(const TreeSet& rhs) const;
    bool operator!=(const TreeSet& rhs) const;

    // friend std::ostream& operator<<(std::ostream& os, const TreeSet<T>& c);

 private:
    struct Node {
        T value_;  // T value at node
        Node* leftChild_;  // left Tree of node
        Node* rightChild_;  // right Tree of node
        size_t size_;  // size of subtree with Node as root

        Node(T t);
        Node() = delete;
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        ~Node() = default;
    };

    Node* root_;  // root node of Tree
    treetype type_;
    RandUInt32 rand_;

    /**
    * \brief Rotate tree right at root
    * \param root to rotate on
    * \returns void
    **/
    void rotateRight(Node*& root);

    /**
    * \brief Rotate tree left at root
    * \param root to rotate on
    * \returns void
    **/
    void rotateLeft(Node*& root);

    /**
    * \brief Reset size of node
    * \param Node to set size of
    * \returns size of Tree
    **/
    void setNodeSize(Node*& cur);


    /**
    * \brief Insert element at leaf of given Tree
    * \param Tree to push into, T to add
    * \returns void
    **/
    void insertAtLeaf(Node*& Tree, const T &t);

    /**
    * \brief Insert element at random in given tree
    * \param Tree to push into, T to add
    * \returns void
    **/
    void insertAtRandom(Node*& tree, const T& t);

    /**
    * \brief Insert element at root of given Tree
    * \param Tree to push into, T to add
    * \returns void
    **/
    void insertAtRoot(Node*& Tree, const T &t);

    /**
    * \brief Check whether value exists in current Tree
    * \param Tree and T to check
    * \returns whether T is in Tree
    **/
    bool existsHelper(const Node* Tree, const T& str) const;

    /**
    * \brief Delete Tree
    * \param Tree to delete
    * \returns void
    **/
    void deleteHelper(Node*& Tree);

    /**
    * \brief Print Tree using CS70 rules
    * \param Tree to print, os stream to print into
    * \returns ostream&
    **/
    ostream& printerHelper(const Node* Tree, ostream& os) const;

    /**
    * \brief Print Tree sizes helper using recursion
    * \param os stream to print into
    * \returns ostream&
    **/
    ostream& printSizesHelper(ostream& os, Node*& cur) const;

    /**
    * \brief Calculate average depth of Tree
    * \param Node position, current height
    * \returns Average depth of Tree
    **/
    double depthHelper(const Node* cur, double& h) const;

    /**
    * \brief Calculate height of Tree
    * \param current node
    * \returns height of Tree
    **/
    int heightHelper(const Node* current) const;

    bool consistent() const;

    class Iterator {
     public:
        using value_type = T;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::forward_iterator_tag;

        Iterator() = default;
        Iterator(const Iterator&) = default;
        Iterator& operator=(const Iterator&) = default;
        ~Iterator() = default;

        Iterator& operator++();
        reference operator*() const;
        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;
        pointer operator->() const;

     private:
        friend class TreeSet;
        explicit Iterator(Node* current);
        // Friends create non-default iterators
        Node* current_;  // The current list node
        std::queue<Node*> pending_;
    };
};

template <typename T>
typename std::ostream& operator<<(std::ostream& os, const TreeSet<T>& c);

#endif  // TreeSET_HPP_INCLUDED

#include "treeset-private.hpp"
