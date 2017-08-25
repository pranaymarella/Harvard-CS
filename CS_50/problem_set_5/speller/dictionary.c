/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "dictionary.h"

// root is the beginning of our try
node *root;

// counter to keep track of words in dictionary
int n_dict_words = 0;

// takes a character as input, and outputs that character in lowercase
char make_lower(char c)
{
    // if character given and it is an uppercase letter, convert to lowercase
    if (c >= 'A' && c <= 'Z')
        return (c + 32);
    // otherwise, return back the character
    else
        return (c);
}

// need to initiaze nodes as we create new ones
node *initialize_node(void)
{
    // allocate space in memory for the node
    node *new_node = malloc(sizeof(node));
    // initialize is_word to false by default
    new_node->is_word = false;
    
    // return node
    return new_node;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // keep temp node for traversing root
    node *temp = root;
    
    for (int i = 0, position = 0; i < strlen(word); i++)
    {
        // check for apostrophe, then for letters
        if (word[i] == 39)
            position = LETTERS - 1;
        else if (make_lower(word[i]) >= 'a' && make_lower(word[i]) <= 'z')
            position = make_lower(word[i]) - 'a';
        
        // after finding position of str's letter, check with root
        if (temp->children[position] == NULL)
            return false;
        else
            temp = temp->children[position];
    }
    
    if (temp->is_word == true)
        return true;
    else
        return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Try to open dictionary, if it exists. Otherwise, return false.
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        unload();
        return false;
    }
    
    // Now that dictionary is read, we can initialize our root for TRIE
    root = initialize_node();
    
    // iterate over each character, till End Of File
    for (int c = fgetc(dict), position = 0; c != EOF; c = fgetc(dict))
    {
        //  create temp access to root, to iterate over TRIE
        node *temp = root;
        
        // iterate over all the characters in line
        while (c != '\n')
        {
            // check for apostrophe, then for letters
            if (c == 39)
                position = LETTERS - 1;
            else if (make_lower(c) >= 'a' && make_lower(c) <= 'z')
                position = make_lower(c) - 'a';
            
            // if position in root is NULL, then create a node
            if (temp->children[position] == NULL)
                temp->children[position] = initialize_node();
            
            // move onto the next position    
            temp = temp->children[position];
            // keep while loop moving
            c = fgetc(dict);
        }
        // after reading a whole line/word, get to the bottom of the trie and make is_word true
        temp->is_word = true;
        // keep track of words in dictionary
        n_dict_words++;
    }
    // done with file, close
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Returns number of words in the dictionary
    if (n_dict_words > 0)
        return n_dict_words;
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // To unload a TRIE, we must travel to lowest possible node and then start delinking. Recursion is useful.
    return unload_trie(root);
}

bool unload_trie(node *trie)
{
    // for each position in the root of the trie
    for (int i = 0; i < LETTERS; i++)
    {
        // if we can go further
        if (trie->children[i] != NULL)
            // keep going further down
            unload_trie(trie->children[i]);
    }
    
    // once we reach the end, free then return
    free(trie);
    return true;
}
