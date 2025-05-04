#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;

    new->lower_than = lower_than;
    return new;
}
/*
Implemente la función void insertTreeMap(TreeMap * tree, void* key, void * value). 
Esta función inserta un nuevo dato (key,value) en el árbol y hace que el current 
apunte al nuevo nodo.
Para insertar un dato, primero debe realizar una búsqueda para encontrar donde 
debería ubicarse. Luego crear el nuevo nodo y enlazarlo. Si la clave del dato ya 
existe retorne sin hacer nada (recuerde que el mapa no permite claves repetidas).
*/

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (searchTreeMap(tree, key) != NULL) return;   // Si hay un nodo con la misma clave

    TreeNode *new_node = createTreeNode(key, value);    // Para insertar
    TreeNode *aux_node = tree->root;    // Para iterar

    while (1){
        void *current_key = aux_node->pair->key;
        if (tree->lower_than(key, current_key)){
            if (aux_node->left == NULL){
                new_node->parent = aux_node;    // Enlazamos los nodos
                aux_node->left = new_node;
                tree->current = new_node;   // Current apunta al nuevo nodo
                return;
            }
            aux_node = aux_node->left;
        } else {
            if (aux_node->right == NULL){
                new_node->parent = aux_node;
                aux_node->right = new_node;
                tree->current = new_node;
                return;
            }
        }
        aux_node = aux_node->right;
    }
}

TreeNode * minimum(TreeNode * x){

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}
// key podria ser un caracter!!!
Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux_node = tree->root;
    while (aux_node != NULL){
        void* current_key = aux_node->pair->key;
        if (tree->lower_than(key, current_key) == 0 && tree->lower_than(current_key, key) == 0){    // Si son iguales
            tree->current = aux_node;
            return aux_node->pair;
        }
        else if (tree->lower_than(key, current_key)){   // Si key es menor a current
            aux_node = aux_node->left;
        } else {
            aux_node = aux_node->right;
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
