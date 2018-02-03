#include "layout.h" 
#include <stddef.h>
#include <string.h>
#include <stdbool.h>


Position sum(Position pos1, Position pos2) {
    Position ret; 
    ret.x = pos1.x + pos2.x;
    ret.y = pos1.y + pos2.y;
    
    return ret;
}

bool not_zero(Position pos) {
    //return true if pos is not (0,0)
    return !(pos.x == 0 && pos.y == 0);
}

Position get_position(Node* root, SearchType search, void* ptr) {
    //easy to maintain if only one function that does the tree traversing 
    if (root == NULL) {
        return (Position){.x = 0, .y = 0};
    }

    switch (search) {
        case NODE:  
            if (root == (Node*)ptr) {
                return root->pos;
            }
            break;
        case ID: 
            if (root->id == *(int*)ptr) {
                return root->pos;
            }
            break;
        case NAME: 
            if (strcmp((char*)ptr, root->name) == 0) {
                return root->pos;
            }
            break;
    }


    Node* curr = root->childern_root;
    while(curr != NULL) {
        Position temp = get_position(curr, search, ptr);
        
        if (not_zero(temp)) {
            return sum(temp, root->pos);
        }
        else {
            curr = curr->sibling_next;
        }
    }
    
    //return (0,0) if not found 
    return (Position){.x = 0, .y = 0};
}


//! Initialize node with the given values
//! @param node the node to initialize
//! @param name A name for the node. You do not have to deep copy this value
//! @param id An id for the node
//! @param position A relative position for this node with respect to it's parent
void layout_node_init(Node* node, const char* name, int id, Position position) {
    node->id = id;
    node->name = name;
    node->pos = position;

    node->sibling_next = NULL;
    node->childern_next = NULL;
    node->childern_root = NULL;
}

//! Add an initialized node as the root of the tree
void layout_init(Layout* layout, Node* root) {
    layout->root = root;
    return;
}

//! Add an intialized node as the child of another node already in the Layout tree
void layout_add_child(Layout* layout, Node* parent, Node* child) {
    if (parent->childern_root == NULL) {
        //no childern
        parent->childern_root = child;
        parent->childern_next = child;
    }
    else {
        //with childern add to linkedlist
        parent->childern_next->sibling_next = child;
        parent->childern_next = child;
    }

    return;
}

//! Change the position of a node. This should work whether or not the node is already in a tree
void layout_node_update_position(Layout* layout, Node* node, Position position) {
    node->pos = position;
    return;
}

//! @return the absolute position for the node with the given memory address
struct Position layout_get_position_for_node(Layout* layout, Node* node) {
    SearchType search = NODE;
    Position ret = get_position(layout->root, search, node);
    return ret;
}

//! @return the absolute position for the node with the given name
struct Position layout_get_position_for_name(Layout* layout, const char* name) {
    SearchType search = NAME;
    Position ret = get_position(layout->root, search, name);
    return ret;
}
//! @return the absolute position for the node with the given id
struct Position layout_get_position_for_id(Layout* layout, int id) {
    SearchType search = ID;
    Position ret = get_position(layout->root, search, &id);
    return ret;
}

