#ifndef SKIP_LIST_H
#define SKIP_LIST_H

typedef struct skip_list_node {
	int value;
	int level;
	struct skip_list_node* nodes[1000];
} skip_node;

typedef struct skip_list_set {
	skip_node* head;
	int size;
	int maximum_level;
} skip_list;

int add(int element);
int remove_node(int element);
void clear();
skip_node* search(int element, skip_node* current, int level);
int contains(int element);
skip_node* search_same_level(int element, skip_node* current, int level);
skip_node* search_previous_same_level(int element, skip_node* current, int level);
int random_level();
void reBalance();
void initialize();

#endif