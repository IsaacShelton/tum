
/*
    MIT License

    Copyright (c) 2019 Isaac Shelton

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

// ---------------------OPTIONS---------------------
#define TEST_ADDITIONS // Throws 100 thousand mostly unique additions at the system
//#define TEST_DUPLICATES // Throws 18 million duplicates at the system
#define PRINT_TABLE // Prints the table after complete
// ------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *strclone(const char *src){
    size_t size = strlen(src) + 1;
    char *clone = malloc(size);
    memcpy(clone, src, size);
    return clone;
}

typedef struct {
    char *name;
} tum_entry_t;

typedef struct {
    tum_entry_t *entries;
    size_t length;
    size_t capacity;
} tum_table_t;

tum_entry_t tum_entry(char *name){
    tum_entry_t entry;
    entry.name = name;
    return entry;
}

void tum_entry_free(tum_entry_t *entry){
    free(entry->name);
}

int tum_entry_compare(const void *one, const void *two){
    return strcmp(((tum_entry_t*) one)->name, ((tum_entry_t*) two)->name);
}

void tum_table_init(tum_table_t *table){
    table->entries = NULL;
    table->length = 0;
    table->capacity = 0;
}

void tum_table_free(tum_table_t *table){
    for(size_t i = 0; i != table->length; i++) tum_entry_free(&table->entries[i]);
    free(table->entries);
}

bool tum_table_find(tum_table_t *table, const char *name, int *out_position){
    int first = 0, middle = 0, last = table->length - 1, comparison = 0;

    while(first <= last){
        middle = (first + last) / 2;
        comparison = strcmp(table->entries[middle].name, name);

        if(comparison == 0){
            *out_position = middle;
            return true;
        }
        else if(comparison > 0) last = middle - 1;
        else first = middle + 1;
    }

    *out_position = comparison >= 0 ? middle : middle + 1;
    return false;
}

void tum_table_print(tum_table_t *table){
    printf("---------TABLE--------\n");
    for(int i = 0; i != table->length; i++){
        printf("-> %s\n", table->entries[i].name);
    }
}

void tum_table_add(tum_table_t *table, tum_entry_t weak_entry){
    int position;

    // Don't add if it already exists
    if(tum_table_find(table, weak_entry.name, &position)) return;

    // Expand entries list
    if(table->length == table->capacity){
        table->capacity += 64;
        
        tum_entry_t *new_entries = malloc(sizeof(tum_entry_t) * table->capacity);
        memcpy(new_entries, table->entries, sizeof(tum_entry_t) * table->length);
        free(table->entries);
        table->entries = new_entries;
    }

    memmove(&table->entries[position + 1], &table->entries[position], (table->length - position) * sizeof(tum_entry_t));
    table->length++;

    // Instantiate entry with it's own data
    table->entries[position].name = strclone(weak_entry.name);
}

int main(int argc, const char **argv){
    tum_table_t table;

    tum_table_init(&table);
    tum_table_add(&table, tum_entry("one"));
    tum_table_add(&table, tum_entry("zero"));
    tum_table_add(&table, tum_entry("six"));
    tum_table_add(&table, tum_entry("one"));
    tum_table_add(&table, tum_entry("fourty"));
    tum_table_add(&table, tum_entry("four"));
    tum_table_add(&table, tum_entry("fourty"));
    tum_table_add(&table, tum_entry("five"));
    tum_table_add(&table, tum_entry("two"));
    tum_table_add(&table, tum_entry("one"));
    tum_table_add(&table, tum_entry("seven"));
    tum_table_add(&table, tum_entry("one"));
    tum_table_add(&table, tum_entry("one"));
    tum_table_add(&table, tum_entry("three"));
    tum_table_add(&table, tum_entry("one"));
    tum_table_add(&table, tum_entry("two"));
    tum_table_add(&table, tum_entry("three"));
    tum_table_add(&table, tum_entry("one"));

    // Adding speed test - 100 thousand random entries
    #ifdef TEST_ADDITIONS
    srand(12345);
    for(size_t i = 0; i != 100000; i++){
        char name[5];
        name[0] = rand() % 90 + 33;
        name[1] = rand() % 90 + 33;
        name[2] = rand() % 90 + 33;
        name[3] = rand() % 90 + 33;
        name[4] = 0;
        tum_table_add(&table, tum_entry(name));
    }
    #endif

    // Lookup speed test - 18 million duplicates
    #ifdef TEST_DUPLICATES
    for(size_t i = 0; i != 1000000; i++){
        tum_table_add(&table, tum_entry("one"));
        tum_table_add(&table, tum_entry("zero"));
        tum_table_add(&table, tum_entry("six"));
        tum_table_add(&table, tum_entry("one"));
        tum_table_add(&table, tum_entry("fourty"));
        tum_table_add(&table, tum_entry("four"));
        tum_table_add(&table, tum_entry("fourty"));
        tum_table_add(&table, tum_entry("five"));
        tum_table_add(&table, tum_entry("two"));
        tum_table_add(&table, tum_entry("one"));
        tum_table_add(&table, tum_entry("seven"));
        tum_table_add(&table, tum_entry("one"));
        tum_table_add(&table, tum_entry("one"));
        tum_table_add(&table, tum_entry("three"));
        tum_table_add(&table, tum_entry("one"));
        tum_table_add(&table, tum_entry("two"));
        tum_table_add(&table, tum_entry("three"));
        tum_table_add(&table, tum_entry("one"));
    }
    #endif

    #ifdef PRINT_TABLE
    tum_table_print(&table);
    #endif

    tum_table_free(&table);
}
