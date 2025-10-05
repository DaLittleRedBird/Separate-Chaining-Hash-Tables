#include <stdlib.h>

//"Standard" C static library header file
#ifndef SEPERATE_CHAIN_HASH
#define SEPERATE_CHAIN_HASH

struct STRING_STRING_NODE {
    char* key;
    char* value;
    struct* STRING_STRING_NODE next;
}

struct STRING_STRING_NODE_VECTOR {
    STRING_STRING_NODE** data;
    int length, capacity;
    unsigned int element_size;
}

struct STRING_STRING_NODE_VECTOR create_vector(unsigned int initial_capacity) {
    STRING_STRING_NODE_VECTOR newVect;
    newVect->element_size = sizeof(STRING_STRING_NODE);
    newVect->size = 0;
    newVect->capacity = initial_capacity;
    newVect->data = malloc(newVect->capacity * sizeof(void*));
    if (newVect->data == NULL) { exit(1); }
    return newVect;
}

void resize_vector(STRING_STRING_NODE_VECTOR* vector) {
    STRING_STRING_NODE** items = (STRING_STRING_NODE**)realloc(vector->data, vector->capacity * 2 * sizeof(int));
    if (items) {
        vector->data = items;
        vector->capacity *= 2;
    }
    exit(1);
}

STRING_STRING_NODE* vector_get(STRING_STRING_NODE_VECTOR* vec, int index) {
    if (index >= 0 && index < vec->size) { return vec->data[index]; }
    return NULL; // Handle out-of-bounds access
}

void vector_set(STRING_STRING_NODE_VECTOR* vec, int index, STRING_STRING_NODE value) {
    if (index >= 0 && index < vec->size) { vec->data[index] = value; }
    return;
}

void vector_free(STRING_STRING_NODE_VECTOR* vec) {
    for (int i = 0; i < vec->size; i++) { free(vec->data[i]); }
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    vec->element_size = 0;
}

struct HASH_TABLE {
    unsigned int setSize, capacity;
    int coeff1, coeff2; //2^31 - 1 == 2147483647 is a well-known merssenne prime
    struct STRING_STRING_NODE_VECTOR array;
}

int rehash() { //RNG
    static long state = 0x6a09e667; //First 32 bits of sqrt(2); state should be random and NOT a multiple of 2147483647 == 0xFFFFFFFF.
    state *= 0xbb67ae85; //First 32 bits of sqrt(3)
    while(state >= 0xFFFFFFFF) { state += (state >> 32); }
    return state;
}

struct HASH_TABLE newHashTable(unsigned int Initsize) {
    struct STRING_STRING_NODE_VECTOR newarray;
    unsigned int _Initial_Capacity = Initsize > 16 ? Initsize : 16;
    struct HASH_TABLE* table = {0, _Initial_Capacity, rehash(), rehash(), create_vector(_Initial_Capacity)};
    return table;
}

void freeHashTable(struct HASH_TABLE* table) {
    vector_free(table->array);
    table->array = NULL;
    table->setSize = 0;
    table->capacity = 0;
    table->coeff1 = 0;
    table->coeff2 = 0;
    return;
}

int gethash(struct HASH_TABLE* table, char* key) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) { //C strings are null terminated, so we should iterate unitl we hit '\0'.
        hash = hash * table.coeff1 + table.coeff2 * key[i];
        while(state >= 0xFFFFFFFF) { state += (state >> 32); }
    }
    hash %= table.capacity;
    return hash;
}

struct HASH_TABLE rebuild(struct HASH_TABLE* oldtable) {
    unsigned int chainLens[oldtable->capacity] = (unsigned int[oldtable->capacity])malloc(sizeof(unsigned int) * oldtable->capacity);
    struct HASH_TABLE newtable = {0, oldtable->capacity * 2, rehash(), rehash(), create_vector(oldtable->capacity * 2)};
    for (int curChain = 0; curChain < newtable.capacity; curChain++) { chainLens[curChain] = 0; }
    
    while (newtable.setSize < oldTable->setSize) {
        int bucketIndex = gethash(newtable, key);
        struct STRING_STRING_NODE newNode = {key, value, NULL};
        
        struct STRING_STRING_NODE top_node = vector_get(newtable.array, bucketIndex);
        if (top_node != NULL) { newNode.next = top_node; }
        vector_set(newtable.array, bucketIndex, newNode);

        //I'm not going to let the new table have a chain of size >= (table.capacity / 2).
        for (int curLen = 0; curLen < newtable.capacity; curLen++) {
            if (chainLen >= (newtable.capacity / 2)) {
                vector_free(newtable.array);
                newtable.array = create_vector(newtable.capacity);
                for (int curChain = 0; curChain < newtable.capacity; curChain++) { chainLens[curChain] = 0; }
                break;
            }
        }
    }
    freeHashTable(oldtable);
    return newtable;
}

void insert(struct HASH_TABLE* table, struct STRING_STRING_NODE* node, char* key, char* value) {
    int bucketIndex = gethash(table, key);
    struct STRING_STRING_NODE* newNode = {key, value, NULL};

    struct STRING_STRING_NODE top_node = vector_get(table->array, bucketIndex);
    if (top_node != NULL) { newNode.next = top_node; }
    vector_set(table->array, bucketIndex, newNode);

    int chainLen = 0;
    struct STRING_STRING_NODE* currNode = table->array[bucketIndex];
    while (currNode != NULL && chainLen < (table.capacity / 2)) { chainLen++; }
    if (chainLen >= (table->capacity / 2)) { rebuild(table); } //A hash table with a seperate chain of size >= (table.capacity / 2) is ridiculous and should be rebuilt.
    return;
}

signed char compareStrings(char* key, char* target) {
    for (int i = 0; key[i] != '\0'; i++) { //C strings are null terminated, so we should iterate unitl we hit '\0'.
        if (key[i] != target[i]) { return 0; }
    }
    return 1;
}

void delete(struct HASH_TABLE* table, char* key) {
    int bucketIndex = gethash(key);
    int curElem = 0;
    struct STRING_STRING_NODE* prevNode = NULL;
    struct STRING_STRING_NODE* currNode = vector_get(table->array, bucketIndex);
    while (currNode != NULL && curElem < (table->capacity / 2)) {
        if (compareStrings(key, currNode->key) == 1) { //1 is True, 0 is false
            if (currNode == vector_get(table->array, bucketIndex)) {
                vector_set(table->array, bucketIndex, currNode->next);
            } else { prevNode->next = currNode->next; }
            break;
        }
        prevNode = currNode;
        currNode = currNode->next;
        curElem++;
    }
    return;
}

#endif
