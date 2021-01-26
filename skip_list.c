#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "skip_list.h"

void input();
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

skip_list* list;

int main(void) {
	initialize();
	input();
	reBalance();
	clear();
  	return 0;
}

void input() {
	FILE* fp = fopen("input.txt", "r");
	int temp;

	for(int i = 0; i < 10000; i++) { 
		fscanf(fp, "%d", &temp);
		add(temp);
	}

	fclose(fp);
}

int add(int element) {
	if(contains(element))
		return 0;
	
	skip_node* new_node = (skip_node*) malloc(sizeof(skip_node));
	skip_node* current = list->head;
	int level = random_level();

	new_node->value = element;

	while (level > list->maximum_level) {
		list->maximum_level++;
		list->head->nodes[level] = NULL;
	}

	int temp = list->maximum_level;
	
	if (list->head->level < level)
		list->head->level = level;

	new_node->level = level;

	while (temp > level) {
		current = search_same_level(element, current, temp);
		temp--;
	}

	do {
		current = search_same_level(element, current, level);
		new_node->nodes[level] = current->nodes[level];
		current->nodes[level] = new_node;
		level--;
	} while (level >= 0);

	list->size++;
	return 1;
}

int remove_node(int element) {
	if(!contains(element))
		return 0;

	skip_node* node = search(element, list->head, list->maximum_level);
	skip_node* temp = list->head;
	int level = node->level;
	int diff = list->maximum_level;

	while (diff > level) {
		temp = search_previous_same_level(element, temp, diff);
		diff--;
	}

	do {
		temp = search_previous_same_level(element, temp, level);
		temp->nodes[level] = node->nodes[level];
		node->nodes[level] = NULL;
		level--;
	} while (level >= 0);

	free(node);
	list->size--;
	return 1;
}

int size() {
	return list->size;
}

int isEmpty() {
	if (list->size == 0) 
		return 1;
	else return 0;
}

void clear() {
	skip_node* current = list->head;
	skip_node* next = current->nodes[0];

	while (next != NULL) {
		remove_node(current->value);
		current = next;
		next = current->nodes[0];
	}
}

int contains(int element) {
	skip_node* search_node = search(element, list->head, list->maximum_level);

	if(element == search_node->value)
		return 1;
	else
		return 0;
}

skip_node* search(int element, skip_node* current, int level) {
	do {
		current = search_same_level(element, current, level);
		level--;
	} while (level >= 0);

	return current;
}

skip_node* search_same_level(int element, skip_node* current, int level) {
	skip_node* temp = current->nodes[level];

	while (temp != NULL) {
		if (element < temp->value)
			return current;

		current = temp;
		temp = current->nodes[level];
	}

	return current;
}

skip_node* search_previous_same_level(int element, skip_node* current, int level) {
	skip_node* temp = current->nodes[level];	

	while (temp != NULL) {
		if (element <= temp->value)
			return current;

		current = temp;
		temp = current->nodes[level];
	}

	return current;
}

int random_level() {
	double probability = 0.5;
	int level = 0;
	int maximum_height = (int) (log(list->size) / log(2));
	srand(time(0));

	while (rand() <= probability && level < maximum_height)
		level++;

	return level;
}

void reBalance() {
	skip_node* current = list->head;
	skip_node* next = current->nodes[0];
	skip_node** previous = malloc(sizeof(skip_node) * list->maximum_level);

	for (int i = 0; i <= list->maximum_level; i++)
		previous[i] = current;

	int max = -1;
	int original_level;
	int level;

	while (next != NULL) {
		current = next;
		level = random_level();
		original_level = current->level;

		if (current->level < level) {
			current->level = level;

			for (int i = original_level; i < level; i++)
				current->nodes[i] = NULL;

			while (list->maximum_level < level) {
				list->maximum_level++;
				list->head->nodes[list->maximum_level] = NULL;
				previous[list->maximum_level] = search_same_level(current->value, list->head, list->maximum_level);
			}

			do {
				current->nodes[level] = previous[level]->nodes[level];
				previous[level]->nodes[level] = current;
				level--; 
			} while (level > original_level);
		}

		else if (current->level > level) {
			current->level = level;

			do {
				previous[original_level]->nodes[original_level] = current->nodes[original_level];
				current->nodes[original_level] = NULL;
				original_level--;
			} while (original_level > level);
		}

		next = current->nodes[0];

		if (max < level)
			max = level;

		for (int i = 0; i <= level; i++)
			previous[i] = current;
	}
	
	list->maximum_level = max;
	int head_level = list->head->level;

	while (head_level > list->maximum_level) {
		previous[head_level] = NULL;
		list->head->nodes[head_level] = NULL;
		head_level--;
	}

	free(previous);
}

void initialize() {
	list = (skip_list*) malloc(sizeof(skip_list));
	list->head = (skip_node*) malloc(sizeof(skip_node));

	list->head->nodes[0] = NULL;

	list->size = 0;
	list->maximum_level = 0;
}
