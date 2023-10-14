#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #define STACK_OVERFLOW  -100
// #define STACK_UNDERFLOW -101
// #define OUT_OF_MEMORY   -102
// #define ERROR_ON_CALC   -103
#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#define INIT_SIZE 10

#define ADD_SIZE 5

typedef struct Stack_tag {
    char *data;
    size_t size;
    size_t top;
} Stack_t;


void deleteStack(Stack_t **stack);
Stack_t* createStack();
void resize(Stack_t *stack);
void push(Stack_t *stack, char value) ;
char pop(Stack_t *stack);
char peek(const Stack_t *stack);

typedef struct Stack_double {
    double *data;
    size_t size;
    size_t top;
} Stack_f;

void deleteFStack(Stack_f **stack);
Stack_f* createFStack();
void Fresize(Stack_f *stack);
void Fpush(Stack_f *stack, double value) ;
double Fpop(Stack_f *stack);
double Fpeek(const Stack_f *stack);