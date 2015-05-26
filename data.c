#include "data.h"

// Data generation/free
struct Data* data_generate(uint16_t data_number)
{
    // Allocates memory for the struct
    struct Data* d = malloc(sizeof(struct Data));

    if (!d)
    {
        #ifdef DEBUG
            error("ERROR : data_generate : Dynamic allocation not possible for the data structure");
        #endif // DEBUG
        #ifdef __AVR__
            while(1){}
        #else
            system("PAUSE>NUL");
            exit(EXIT_FAILURE);
        #endif
    }

    // Allocates memory for the array
    uint16_t n = (data_number - 1) / 8 + 1; // Number of byte needed
    d->data_array = calloc(n,sizeof(uint8_t)); // Automatically sets the bits to zero
    if (d->data_array == NULL)
    {
        #ifdef DEBUG
            error("ERROR : data_generate : Dynamic allocation not possible for the array");
        #endif // DEBUG
        #ifdef __AVR__
            while(1){}
        #else
            system("PAUSE>NUL");
            exit(EXIT_FAILURE);
        #endif
    }

    // Sets the data_number field
    d->data_number = data_number;
    return d;
}

void data_free(struct Data* d)
{
    if(d != NULL)
    {
        free(d->data_array);
        free(d);
    }
}

// Data operations
uint8_t data_get(uint16_t n, struct Data* d)
{
    if(n < d->data_number)
        return (d->data_array[n/8] >> ( n - 8*(n/8) ) ) & 1;
    else
    {
        #ifdef DEBUG
            error("ERROR: Incorect data number. Function data_get");
        #endif // DEBUG
        #ifdef __AVR__
            while(1){}
        #else
            system("PAUSE>NUL");
            exit(EXIT_FAILURE);
        #endif
    }

    return -1;
}

void data_set(uint16_t n, uint8_t data, struct Data* d)
{
    if(n < d->data_number)
        if (data)
            d->data_array[n / 8] |=  (1 << ( n - 8*(n/8) ));
        else
            d->data_array[n / 8] &=  ~(1 << ( n - 8*(n/8) ));
    else
    {
        #ifdef DEBUG
            error("ERROR: Incorect data number. Function data_set.");
        #endif // DEBUG
        #ifdef __AVR__
            while(1){}
        #else
            system("PAUSE>NUL");
            exit(EXIT_FAILURE);
        #endif
    }
}

void data_delete(uint16_t n, struct Data* d)
{
    if (0 <= n && n < d->data_number)
    {
        // Move the data, starting from the data we want to delete.
        for(uint16_t i = n; i < d->data_number - 1; i++)
            data_set(i, data_get(i + 1, d), d);

        // Determines whether or not it is necessary to allocate a new block of memory.
        uint16_t old_byte_number_needed = (d->data_number - 1)/8 + 1;;
        uint16_t new_byte_number_needed = d->data_number/8 + 1;

        if(new_byte_number_needed < old_byte_number_needed)
            d->data_array = realloc(d->data_array, new_byte_number_needed);
        else
            data_set(d->data_number - 1, 0, d);

        d->data_number -= 1;
    }
    else
    {
        #ifdef DEBUG
            error("ERROR: Wrong block. Function data_delete.");
        #endif // DEBUG
        #ifdef __AVR__
            while(1){}
        #else
            system("PAUSE>NUL");
            exit(EXIT_FAILURE);
        #endif
    }
}


#ifdef DEBUG
    void data_show(struct Data* d)
    {
        #ifdef __AVR__
            if(d == NULL)
                uart_tx_str("data == NULL\r\n");
            else
            {
                for(uint16_t i = 0; i  < d->data_number; i++)
                {
                    char str[5];
                    sprintf(str, "%d", data_get(i, d));
                    uart_tx_str(str);
                }
                uart_newline();
            }
        #else
            if(d == NULL)
                printf("\ndata == NULL\n");
            else
            {
                printf("(");
                for(uint16_t i = 0; i  < d->data_number; i++)
                    printf("%d ", data_get(i, d));
                printf(")\n\n");
            }
        #endif // __AVR__
    }
#endif
