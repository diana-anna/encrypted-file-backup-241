/**
 * Mini Valgrind Lab
 * CS 241 - Spring 2019
 */
 
#include "mini_valgrind.h"
#include <stdio.h>
#include <string.h>

size_t total_memory_freed;
size_t total_memory_requested;
size_t invalid_addresses;
meta_data *head;

void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
    // your code here
    if (request_size == 0) return NULL;

    total_memory_requested += request_size;
    meta_data* md = malloc(sizeof(meta_data) + request_size);

    // failed to allocate
    if (!md) return NULL;

    md->request_size = request_size;
    md->filename = filename;
    md->instruction = instruction;

    // insert at head
    md->next = head;
    head = md;
    return md + 1;
}

void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
    // your code here
    if (num_elements == 0 || element_size == 0) return NULL;

    total_memory_requested += (element_size * num_elements);
    meta_data* md = calloc(num_elements, element_size);

    // failed to allocate
    if (!md) return NULL;

    md->request_size = element_size * num_elements;
    md->filename = filename;
    md->instruction = instruction;

    // insert at head
    md->next = head;
    head = md;
    return md + 1;
}

void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
    // your code here
    if (!payload && !request_size) return NULL;
    if (!payload) {
      return mini_malloc(request_size, filename, instruction);
    }
    if (!request_size) {
      mini_free(payload);
      return NULL;
    }

    meta_data* pyld = (meta_data*) payload;
    pyld--;

    size_t old_size = pyld->request_size;
    if (old_size > request_size) {
      total_memory_freed += (old_size - request_size);
      pyld->request_size =  old_size - request_size;
    } else {
      total_memory_requested += (request_size - old_size);
      pyld->request_size = request_size - old_size;
    }

    if (pyld == head) {
        head = head->next;
        free(pyld);
    } else {
      meta_data* ptr2 = head->next;
      meta_data* ptr1 = head;
      
      while (ptr2) {
        if (ptr2 == pyld) {
          ptr1->next = ptr2->next;
          free(pyld);
          break;
        }
        ptr1 = ptr2;
        ptr2 = ptr2->next;
      }
    }

    meta_data* payoff = malloc(sizeof(meta_data) + request_size);
    payoff->instruction = instruction;
    payoff->request_size = request_size;
    payoff->filename = filename;
    payoff->next = head;
    head = payoff;

    return payoff + 1;
}

void mini_free(void *payload) {
    // your code here
    if (payload) {
      if (!head) {
        invalid_addresses++;
        return;
      }

      meta_data* pyld = (meta_data*) payload;
      pyld--;

      // passed head
      if (pyld == head) {
        total_memory_freed += pyld->request_size;
        head = head->next;
        free(pyld);
        return;
      }

      meta_data* temp = head;
      while(temp) {
        if (temp == pyld) {
          meta_data* temp2 = temp;
          temp = temp2->next;

          total_memory_freed += pyld->request_size;
          free (pyld);
          return;
        }
        temp = temp->next;
      }

      invalid_addresses++;
    }
}
