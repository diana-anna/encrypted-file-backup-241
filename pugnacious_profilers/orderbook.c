/**
* pugnacious_profilers Lab
* CS 241 - Spring 2019
*/

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "orderbook.h"

#define STDOUT 1

/****Main Implementation****/

int main(int argc, char *argv[]) {
    InputType in_type = ASCII;
    StructType struct_type = LL;
    FILE *input = stdin;
    int outputFD = STDOUT;

    // Process the commandline
    proc_cmdline(argc, argv, &in_type, &struct_type, &input, &outputFD);

    if (struct_type == LL) { // linked list
        OrderNode *order_book = NULL;
        // process input
        proc_mesgs_LL(in_type, &order_book, input);
        // print the order_book
        print_orderbook_LL(order_book, outputFD);
        close(outputFD);
        // free data
        fclose(input);

        free_LL(&order_book);
    } else {
        // your code here
    }

    return 0;
}

/****Start Function implementations****/

/*
 * proc_scmdline - Processes the command line
 * argc - number of arguments
 * argv - arguments passed to the program command line
 * i_t - capture the input type
 * s_t - structure type: linked list
 * in - input file
 * out - output file
 */

void proc_cmdline(int argc, char *argv[], InputType *i_t, StructType *s_t,
                  FILE **in, int *out) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h")) {
            // your code here
        } else if (!strcmp(argv[i], "-i")) {
            i++;
            if ((*in = fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, "Unabled to open input file named %s\n",
                        argv[i]);
                exit(0);
            }
        } else if (!strcmp(argv[i], "-o")) {
            i++;

            if ((*out = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0777)) <
                0) {
                fprintf(stderr, "Unable to open output file named %s\n",
                        argv[i]);
                exit(0);
            }
        } else {
            fprintf(stderr, "Unexpected input flag %s\n", argv[i]);
        }
    }
}

/*
 * proc_mesgs_LL - Process a file into a linked list
 * in_type - Type of input (ASCII)
 * head - head of the linked list
 * in - inputfile
 */
void proc_mesgs_LL(InputType in_type, OrderNode **head, FILE *in) {
    char msg_type; // char to hold the operation type
    char side;     // sell Vs Buy
    OrderNode in_data;

    if (in_type == ASCII) {
        while (fscanf(in, "%c", &msg_type) != EOF) { // While not EOF
            // process data
            switch (msg_type) {
            case 'A':
                fscanf(in, "%ld %c %s %ld %lf\n", &(in_data.id), &side,
                       in_data.symbol, &(in_data.quantity), &(in_data.price));
                (side == 'B') ? (in_data.side = BUY) : (in_data.side = SELL);
                add_order_LL(head, &in_data);
                break;
            case 'X':
                fscanf(in, "%ld %s\n", &(in_data.id), in_data.symbol);
                cancel_order_LL(head, &in_data);
                break;
            case 'T': // Same as Case 'T'
            case 'C':
                fscanf(in, "%ld %s %ld\n", &(in_data.id), in_data.symbol,
                       &(in_data.quantity));
                reduce_quantity_LL(*head, &in_data);
                break;
            case 'R':
                fscanf(in, "%ld %s %ld %lf\n", &(in_data.id), in_data.symbol,
                       &(in_data.quantity), &(in_data.price));
                change_order_LL(*head, &in_data);
                break;
            default: // If we reach here, then we got wrong input
                // fprintf(stderr, "Unknown message type, %c, ignoring
                // message\n", msg_type);
                break;
            }
        }
    }
}

/****Start Linked List implementation****/

/*
 * add_order_LL - add order to linked list
 * head - head of the linked list
 * data - node data
 */
void add_order_LL(OrderNode **head, OrderNode *data) {
    if (*head == NULL) { // Empty list
        *head = (OrderNode *)malloc(sizeof(OrderNode));
        (*head)->id = data->id;
        (*head)->side = data->side;
        (*head)->quantity = data->quantity;
        (*head)->price = data->price;
        (*head)->next = NULL;
        (*head)->prev = NULL;
    } else { // Non Empty list
        OrderNode *tmp;
        tmp = (OrderNode *)malloc(sizeof(OrderNode));
        tmp->id = data->id;
        tmp->side = data->side;
        tmp->quantity = data->quantity;
        tmp->price = data->price;
        tmp->next = *head;
        tmp->prev = NULL;
        (*head)->prev = tmp;
        *head = tmp;
    }
}

/*
 * cancel_order_LL - cancel order from linked list
 * head - head of the linked list
 * data - node data
 */
void cancel_order_LL(OrderNode **head, OrderNode *data) {
    OrderNode *cur = *head;
    // find the location of the node to cancel
    while (cur != NULL && (cur->id != data->id))
        cur = cur->next;

    // delete node after finding its location
    if (cur != NULL) {
        if (cur == *head) { // curr is head, we should assign a new head
            *head = cur->next;
            (*head)->prev = NULL;
        } else { // cur isn't head node
            cur->prev->next = cur->next;
            if (cur->next)
                cur->next->prev = cur->prev;
        }
        // free the memory location
        free(cur);
    }
}

/*
 * reduce_quantity_LL - reduce quantity in a linked list
 * head - head of the linked list
 * data - node data
 */
void reduce_quantity_LL(OrderNode *head, OrderNode *data) {
    OrderNode *cur = head;

    // find the location of the node to be updated
    while (cur != NULL && (cur->id != data->id))
        cur = cur->next;

    // reduce quantity
    if (cur != NULL)
        cur->quantity = cur->quantity - data->quantity;
}

/*
 * change_order_LL - change the date of an order
 * head - head of the linked list
 * data - node data
 */
void change_order_LL(OrderNode *head, OrderNode *data) {
    OrderNode *cur = head;
    // find the location of the node to cancel
    while (cur != NULL && (cur->id != data->id))
        cur = cur->next;

    // change order
    if (cur != NULL) {
        cur->quantity = data->quantity;
        cur->price = data->price;
    }
}

/*
 * print_orderbook_LL - Print the order book
 * head - head of the linked list
 * out - outputfile
 */
void print_orderbook_LL(OrderNode *head, int out) {
    OrderNode *cur = head;
    char line[50];
    while (cur != NULL) { // loop on the list starting from head
        sprintf(line, "%ld %c %ld %lf\n", cur->id,
                (cur->side == BUY) ? 'B' : 'S', cur->quantity, cur->price);
        write(out, line, strlen(line));
        cur = cur->next;
    }
}

/*
 * free_LL - free the entire list
 * head - head of the linked list
 */
void free_LL(OrderNode **head) {
    OrderNode *last, *cur = *head;
    while (cur != NULL) { // loop on the list starting from head
        last = cur;
        cur = cur->next;
        // free the node
        free(last);
    }
}

// End Linked List Implementation
