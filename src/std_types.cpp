#include <Arduino.h>
#include "std_types.h"

// template<typename Data>
// class Vector{
//     size_t size; // Current no. of data. 
//     size_t capacity; // Current max. capacity. 
//     Data *data; // Data list. 

//     public: 
//         Vector(): size(0), capacity(0)
//         Vector(size_t _capacity): size(0), capacity(_capacity) {d_data = (Data *) malloc( capacity * sizeof(Data) );};
//         ~Vector() { free(data) }; 
//     private: 
//         void resize() { };     
// }

//         void append(Node _new); 
//        Node* get_current(void); 
//        Node* traverse(void); 
//        Node* reset(void); 


template<class Data>
newstd::LinkedList(Data _data){
    Node* _new_node_ptr = (Node *) malloc ( sizeof(Node<Data>) ); 
    _new_node_ptr->data = _data; 
    _new_node_ptr->next = NULL; 
}

template<class Data>
newstd::LinkedList(){

}

template<class Data> 
void newstd::LinkedList::append(Data _new){
    
    // Create a new node. 
    // Node _node = new newstd::Node(_new); 
    Node* _new_node_ptr = (Node *) malloc( sizeof(Node<Data>) ); 

    _new_node_ptr->data = _new; 
    _new_node_ptr->next = NULL; 

    // Let the tail point to new node. 
    this->tail->next = &_node; 
    this->tail = _new_node_ptr; 
    // this->tail->next = _new_node_ptr; 
}

template<class Data>


// template<class Data>
// void newstd::LinkedList::remove(void){

// }