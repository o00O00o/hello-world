#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "layout.h" 

#define FOUND 1
#define NOT_FOUND 0

Position sum(Position pos1, Position pos2) {
    Position ret; 
    ret.x = pos1.x + pos2.x;
    ret.y = pos1.y + pos2.y;
    
    return ret;     
}

int DFS(Node* root, SearchType search, void* ptr, Position* ret) {
    //easy to maintain if only one function that does the tree traversing 
    if (root == NULL) {
        return NOT_FOUND;
    }

    switch (search) {
        case NODE:  
            if (root == (Node*)ptr) {
                return FOUND;
            }
            break;
        case ID: 
            if (root->id == *(int*)ptr) {
                return FOUND;
            }
            break;
        case NAME: 
            if (strcmp((char*)ptr, root->name) == 0) {
                return FOUND;
            }
            break;
    }

    Node* curr = root->children_root;
    while(curr != NULL) {
        int result = DFS(curr, search, ptr, ret);
        
        if (result == FOUND) {
            //calculater the position as you return from the DFS
            *ret = sum(curr->pos, *ret);
            return FOUND;
        }
        else {
            curr = curr->sibling_next;
        }
    }

    return NOT_FOUND;
}

Position get_position(Node* root, SearchType search, void* ptr) {
    Position ret;
    ret.x = 0;
    ret.y = 0;

    if (DFS(root, search, ptr, &ret) == FOUND) {
        ret = sum(root->pos, ret);
    }

    return ret;
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
    node->children_next = NULL;
    node->children_root = NULL;
}

//! Add an initialized node as the root of the tree
void layout_init(Layout* layout, Node* root) {
    layout->root = root;
    return;
}

//! Add an intialized node as the child of another node already in the Layout tree
void layout_add_child(Layout* layout, Node* parent, Node* child) {
    if (parent->children_root == NULL) {
        //no children
        parent->children_root = child;
        parent->children_next = child;
    }
    else {
        //with children add to linkedlist
        parent->children_next->sibling_next = child;
        parent->children_next = child;
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

