function newHashTable(size) {
    this.elems = new Array(size);
    this.elemCnt = 0;
    this.capacity = size;
    const k = (Math.random() * (this.capacity - 1)) + 1;
    this.hash = function(key) {
        let hash = 0;
        for (let i = 0; i < key.length; i++) { hash = (hash * k + key.charCodeAt(i)) % this.capacity; }
        return hash;
    }
    for (let curSlot = 0; curSlot < size; curSlot++) { this.elems[curSlot] = []; }
}

newHashTable.prototype.rehash = function(key) {
    const k = (Math.random() * (this.capacity - 1)) + 1;
    this.hash = function(key) {
        let hash = 0;
        for (let i = 0; i < key.length; i++) { hash = (hash * k + key.charCodeAt(i)) % this.capacity; }
        return hash;
    };
}

newHashTable.prototype.rebuild = function(key, value) {
    let newTable = [], newElemCnt = 0;
    for (let curSlot = 0; curSlot < this.capacity; curSlot++) { newTable[curSlot] = []; }
    this.rehash();
    while (newElemCnt < this.elemCnt) {
        const index = this.hash(key);
        const bucket = this.elems[index];
        bucket.push([key, value]);
        newElemCnt++;
    }
    this.elems = newTable;
}

newHashTable.prototype.insert = function(key, value) {
    const index = this.hash(key);
    const bucket = this.elems[index];

    // Check if the key already exists in the bucket and update its value
    for (let i = 0; i < bucket.length; i++) {
        if (bucket[i][0] === key) {
            bucket[i][1] = value;
            return;
        }
    }
    // If key doesn't exist, add the new key-value pair to the bucket
    bucket.push([key, value]);
    this.elemCnt++;
}

newHashTable.prototype.lookup = function(key) {
    const index = this.hash(key);
    const bucket = this.elems[index];

    // Search for the key in the bucket's array
    for (let i = 0; i < bucket.length; i++) {
        if (bucket[i][0] === key) {
            return bucket[i][1];
        }
    }
    return undefined; // Key not found
}

newHashTable.prototype.delete = function(key) {
    const index = this.hash(key);
    const bucket = this.elems[index];

    // Find and remove the key-value pair from the bucket
    for (let i = 0; i < bucket.length; i++) {
        if (bucket[i][0] === key) {
            bucket.splice(i, 1);
            this.elemCnt--;
            return true; // Successfully removed
        }
    }
    return false; // Key not found
}
