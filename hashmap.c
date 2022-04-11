#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
	Pair **buckets;
	long size;	 // cantidad de datos/pairs en la tabla
	long capacity; // capacidad de la tabla
	long current;  // index del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
	Pair *new = (Pair *)malloc(sizeof(Pair));
	new->key = key;
	new->value = value;
	return new;
}

long hash(char *key, long capacity) {
	unsigned long hash = 0;
	char *ptr;
	for (ptr = key; *ptr != '\0'; ptr++) {
		hash += hash * 32 + tolower(*ptr);
	}
	return hash % capacity;
}

int is_equal(void *key1, void *key2) {
	if (key1 == NULL || key2 == NULL)
		return 0;
	if (strcmp((char *)key1, (char *)key2) == 0)
		return 1;
	return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
	long index = hash(key, map->capacity);
	while (map->buckets[index] != NULL && map->buckets[index]->key != NULL) {
		if (is_equal(map->buckets[index]->key, key) == 1)
			return;
		index = (index + 1) % map->capacity;
	}

	if (map->buckets[index] != NULL) {
		map->buckets[index]->key = key;
		map->buckets[index]->value = value;
	} else {
		map->buckets[index] = createPair(key, value);
	}
	map->size++;
	if ((double)map->size / (double)map->capacity > 0.75) {
		enlarge(map);
	}
}

void enlarge(HashMap *map) {
	enlarge_called = 1; // no borrar (testing purposes)
  Pair **auxBuckets;
  
  map->capacity *= 2;
  
  map->buckets = calloc(map->capacity, sizeof(Pair));
  map->size = 0;
  
  for (int i = 0; i < map->capacity; i++){
    insertMap(map, auxBuckets[i]->key, auxBuckets[i]->value);
  }

}

HashMap *createMap(long capacity) {
	HashMap *map = (HashMap *)malloc(sizeof(HashMap));
	map->buckets = calloc(capacity, sizeof(Pair));
	map->buckets[0] = NULL;
	map->size = 0;
	map->capacity = capacity;
	map->current = -1;
	return map;
}

void eraseMap(HashMap *map, char *key) {
	long indice = hash(key, map->capacity);
	if (searchMap(map, key) != NULL) {
		while (is_equal(map->buckets[indice]->key, key) == 0) {
			indice++;
		}
		map->buckets[indice]->key = NULL;
		map->size--;
	}
}

Pair *searchMap(HashMap *map, char *key) {
	long search_key = hash(key, map->capacity);
	// long capacidad = hash(key, map->capacity);
	// Todo dudoso
	while (map->buckets[search_key] != NULL) {
		if (map->buckets[search_key] == NULL)
			return NULL;

		if (is_equal(map->buckets[search_key]->key, key) == 1) {
			map->current = search_key;
			return map->buckets[search_key];
		} else {
			search_key += 1;
		}
	}

	return map->buckets[search_key];
}

Pair *firstMap(HashMap *map) {
	
  int aux;
	for (aux = 0; aux < map->capacity; aux++) {
		if (map->buckets[aux] != NULL) {
			if (map->buckets[aux]->key != NULL) {
				map->current = aux;
        return map->buckets[aux];
			}
		}
	}
  
	return NULL;
}

Pair *nextMap(HashMap *map) {
  
  int aux;
	for (aux = map->current; aux < map->capacity; aux++) {
		if (map->buckets[aux] != NULL && aux != map->current) {
			if (map->buckets[aux]->key != NULL) {
				map->current = aux;
        return map->buckets[aux];
			}
		}
	}
  
	return NULL;
}
