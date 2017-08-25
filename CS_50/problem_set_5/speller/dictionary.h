/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define LETTERS 27

// definiton for TRIE
typedef struct node
{
    // each node wills store a word and a pointer to the next node
    bool is_word;
    struct node *children[LETTERS];
}
node;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Helper function for unload, uses recursion to unload memory of Trie from bottom to up.
 */
bool unload_trie(node *trie);

#endif // DICTIONARY_H
