//"Standard" C static library header file
#ifndef SEPERATE_CHAIN_HASH
#define SEPERATE_CHAIN_HASH

struct STRING_STRING_NODE {
    char* key;
    char* value;
    struct* STRING_STRING_NODE next;
}

struct HASH_TABLE {
    unsigned int setSize, capacity;
    int coeff1, coeff2; //2^31 - 1 == 2147483647 is a well-known merssenne prime
    struct STRING_STRING_NODE** array;
}

int rehash() { //RNG
    static long state = 0x6a09e667; //First 32 bits of sqrt(2); state should be random and NOT a multiple of 2147483647 == 0xFFFFFFFF.
    state *= 0xbb67ae85; //First 32 bits of sqrt(3)
    while(state >= 0xFFFFFFFF) { state += (state >> 32); }
    return state;
}

struct HASH_TABLE* newHashTable(unsigned int Initsize) {
    struct HASH_TABLE* table = {0, 8, rehash(), rehash(), {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};
    if (/*Initsize != NULL && */Initsize > 8) {
        table.size = Initsize;
        table->array = (struct STRING_STRING_NODE**)malloc(sizeof(struct STRING_STRING_NODE*) * table->capacity);
    }
    return table;
}

int gethash(struct HASH_TABLE* table, char* key) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) { //C strings are null terminated, so we should iterate unitl we hit '\0'.
        hash = hash * table.coeff1 + table.coeff2 * key[i];
        while(state >= 0xFFFFFFFF) { state += (state >> 32); }
    }
    return hash;
}

struct HASH_TABLE* rebuild(struct HASH_TABLE* oldtable) {
    struct HASH_TABLE* newtable = {0, oldtable.capacity, rehash(), rehash(), {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};
    unsigned int chainLens[newtable.capacity] = (unsigned int[newtable.capacity])malloc(sizeof(unsigned int) * newtable->capacity);
    table->array = (struct STRING_STRING_NODE**)malloc(sizeof(struct STRING_STRING_NODE*) * newtable->capacity);
    for (int curChain = 0; curChain < newtable.capacity; curChain++) { chainLens[curChain] = 0; }
    
    while (newtable.setSize < oldTable.setSize) {
        int bucketIndex = gethash(newtable, key);
        struct STRING_STRING_NODE* newNode = {key, value, NULL};
        if (newtable->array[bucketIndex] != NULL) { newNode->next = newtable->array[bucketIndex]; }
        newtable->array[bucketIndex] = newNode;

        //I'm not going to let the new table have a chain of size >= (table.capacity / 2).
        for (int curLen = 0; curLen < newtable.capacity; curLen++) {
            if (chainLen >= (newtable.capacity / 2)) {
                newtable = {0, oldtable.capacity, rehash(), rehash(), rehash(), {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};
                table->array = (struct STRING_STRING_NODE**)malloc(sizeof(struct STRING_STRING_NODE*) * newtable->capacity);
                for (int curChain = 0; curChain < newtable.capacity; curChain++) { chainLens[curChain] = 0; }
                break;
            }
        }
    }
    return newtable;
}

void insert(struct HASH_TABLE* table, struct STRING_STRING_NODE* node, char* key, char* value) {
    int bucketIndex = gethash(table, key);
    struct STRING_STRING_NODE* newNode = {key, value, NULL};
    if (table->array[bucketIndex] != NULL) {
        newNode->next = table->array[bucketIndex];
    }
    table->array[bucketIndex] = newNode;
    int chainLen = 0;
    struct STRING_STRING_NODE* currNode = table->array[bucketIndex];
    while (currNode != NULL && chainLen < (table.capacity / 2)) { chainLen++; }
    if (chainLen >= (table.capacity / 2)) { rebuild(struct HASH_TABLE* table); } //A hash table with a seperate chain of size >= (table.capacity / 2) is ridiculous and should be rebuilt.
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
    struct STRING_STRING_NODE* currNode = table->array[bucketIndex];
    while (currNode != NULL && curElem < (table.capacity / 2)) {
        if (compareStrings(key, currNode->key) == 1) { //1 is True, 0 is false
            if (currNode == table->array[bucketIndex]) {
                table->array[bucketIndex] = currNode->next;
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
