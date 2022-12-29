#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct set_node
{
    const char *key;
    size_t key_len;
    struct set_node *next;
};

struct set_table
{
    struct set_node **nodes;
    size_t hashmap_size;
};

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

struct set_table *set_table_new(const size_t hashmap_size)
{
    struct set_table *table = malloc(sizeof(struct set_table));
    if (!table)
    {
        return NULL;
    }
    table->hashmap_size = hashmap_size;
    table->nodes = calloc(table->hashmap_size, sizeof(struct set_node *));
    if (!table->nodes)
    {
        free(table);
        return NULL;
    }
    return table;
}

struct set_node *set_insert(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];

    if (!head)
    {
        table->nodes[index] = malloc(sizeof(struct set_node));
        if (!table->nodes[index])
        {
            return NULL;
        }
        table->nodes[index]->key = key;
        table->nodes[index]->key_len = key_len;
        table->nodes[index]->next = NULL;

        return table->nodes[index];
    }

    struct set_node *new_item = malloc(sizeof(struct set_node));

    if (!new_item)
    {
        return NULL;
    }

    new_item->key = key;
    new_item->key_len = key_len;
    new_item->next = NULL;
    struct set_node *tail = head;

    while (head)
    {
        if (head->key_len == key_len && strcmp(head->key, key) == 0)
        {
            head->key = key;
            return head;
        }
        tail = head;
        head = head->next;
    }
    tail->next = new_item;

    return tail;
}

struct set_node *search(struct set_table *table, const char *key, size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    while (head)
    {
        if (head->key_len == key_len && strcmp(head->key, key) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void delete_element(struct set_table *table, const char *key, size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    if (head && head->key_len == key_len && strncmp(head->key, key, key_len) == 0)
    {
        table->nodes[index] = head->next;
        free(head);
        return;
    }
    while (head)
    {
        if (head->next && head->next->key_len == key_len && strncmp(head->key, key, key_len) == 0)
        {
            struct set_node *to_delete = head->next;
            head->next = to_delete->next;
            free(to_delete);
            return;
        }
        head = head->next;
    }
}

void print_set(struct set_table *table)
{
    for (int i = 0; i < table->hashmap_size; ++i)
    {
        printf("----------------");
        printf("index %d\n", i);
        struct set_node *head = table->nodes[i];

        while (head)
        {
            printf("valore %s\n", head->key);
            head = head->next;
        }
        printf("----------------\n");
    }
}

int main()
{
    struct set_table *my_set = set_table_new(12);
    
    set_insert(my_set, "red", 3);
    set_insert(my_set, "red", 3);
    set_insert(my_set, "blue", 4);
    set_insert(my_set, "white", 5);
    set_insert(my_set, "black", 5);
    set_insert(my_set, "yellow", 6);
    set_insert(my_set, "blue", 4);
    set_insert(my_set, "orange", 6);

    delete_element(my_set,"red",3);

    struct set_node *found = search(my_set, "red", 3);

    if (found)
    {
        printf("Node with key %s found\n", found->key);
    }
    else
    {
        printf("Node with key not found\n");
    }

    print_set(my_set);

    return 0;
}
