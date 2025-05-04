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
            aux_node = aux_node->right;
        }
    }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    // Caso sin hijos (hoja)
    if (node->left == NULL && node->right == NULL){
        // Subcaso: El nodo es la raiz
        if (node->parent == NULL){  
            tree->root = NULL; 
        } else if (node->parent->left == node){
            // Desenlazar el nodo del lado izquierdo de parent
            node->parent->left = NULL;
        } else {
            // Desenlazar el nodo del lado derecho de parent
            node->parent->right = NULL;
        }
        free(node); // Eliminar el nodo liberando memoria
        return;
    }
    // Caso con un hijo
    if (node->left == NULL || node->right == NULL){
        // En primer lugar hay que verificar de que lado el hijo de node no es null
        TreeNode *hijo = (node->left != NULL) ? node->left : node->right;

        // Subcaso: node es la raiz
        if (node->parent == NULL){
            tree->root = hijo; // Ahora el hijo de node es la raiz

        // En caso de que si tenga padre, hay que verificar de que lado esta node
        // En caso de que sea hijo izquierdo
        } else if (node->parent->left == node) {
            node->parent->left = hijo; // El puntero de parent apunta al hijo de node
        // En caso de que node sea hijo derecho
        } else {
            node->parent->right = hijo;
        }
        // El puntero del hijo de node apunta a parent de node
        hijo->parent = node->parent;
        free(node);
        return;
    }
    // Caso con dos hijos (el orden sera el menor del sub arbol derecho)
    TreeNode *success = minimum(node->right);
    // Copiar los datos del sucesor
    node->pair->key = success->pair->key;
    node->pair->value = success->pair->value;

    // Eliminar el sucesor
    removeNode(tree, success);
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
    // Caso en el cual si esta la clave
    if (searchTreeMap(tree, key) != NULL) return tree->current->pair;

    TreeNode *aux_node = tree->root;
    TreeNode *candidate = NULL;

    while (aux_node != NULL){
        // En caso de que key es menor a key de aux
        if (tree->lower_than(key, aux_node->pair->key)){
            candidate = aux_node;   // Es candidato
            aux_node = aux_node->left; // Y seguimos buscando si hay algun candidato mas pequeño
        } else {
            // Caso en el cual key es mayor o igual
            aux_node = aux_node->right;
        }
    }
    // Retornar el mejor candidato
    if (candidate != NULL){
        tree->current = candidate;
        return candidate->pair;
    }
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root == NULL) return NULL;
    TreeNode *first_node = minimum(tree->root);
    tree->current = first_node;
    return first_node->pair;
}
// NextTree busca el par siguiente a current en orden creciente segun key
Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux_node = tree->current;

    // Si tiene subarbol derecho, el par debe estar en el minimo de ese subarbol
    if (aux_node->right != NULL){
        aux_node = minimum(aux_node->right);
        tree->current = aux_node;
        return aux_node->pair;
    }
    // En otro caso, buscar el siguiente en parents
    TreeNode *parent = aux_node->parent;
    // "subir" hasta encontrar un nodo donde current es el hijo izquierdo
    while (parent != NULL && parent->right == aux_node){
        aux_node = parent;
        parent = parent->parent;
    }
    if (parent == NULL) return NULL;
    tree->current = parent;
    return parent->pair;
}
