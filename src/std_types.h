#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <Arduino.h> 

/*
*   Incomplete. May not be necessary. 
*/ 

// template<typename Data>
// class Vector{
//     public: 

//         Vector(): d_size(0), d_capacity(0), data(0) {}; 
//         Vector(size_t _capacity): d_size(0), d_capacity(_capacity), data(0) {
//             data = (Data *) malloc(d_capacity * sizeof(Data) ); 
//         };

//         void push_back(Data _data); 
//         Data const &operator[](size_t idx) const { return d_data[idx] };
//         Data &operator[](size_t idx) { return d_data[idx] }; 
//         Vector &operator=(Vector const &other); 
//         Vector &operator<<(Data _data) { push_back(_data); } 

//     private: 
//         size_t d_size; 
//         size_t d_capacity;
//         Data* data;     
// };

/*
*
*/

namespace newstd{ 
    
    /*
    *   Nodes for LinkedList
    */

    template<class Data> struct Node{
        Data data; 
        Node* next; 
        Node(Data _data) : data(_data), next(NULL) {}
    }; 

    template<class Data> class LinkedList{ 
    
    private: 
        Node* head; 
        Node* curr;
        Node* tail; 
        size_t idx; 
        size_t size; 

    public: 
        LinkedList(Data _data); 
        LinkedList(); 

        void append(Data _new);
        // void remove(void); 
        // void remove_after(size_t remove_after_idx); 

        Node* get_current(void){ return curr }; // Return current
        Node* traverse(void); // Return current node and increment. 
        Node* reset(void){ curr = head; return curr; }; // Reset to head. 

    }; 
}; 

#endif