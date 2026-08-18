#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(ARRAY_LIST_TAG) || !defined(ARRAY_LIST_ITEM_TYPE)
#error "Missing type or tag definition"
#endif

// Optional macro to free items. Evaluates to no-op for primitives
#ifndef ARRAY_LIST_FREE_ITEM
  #define ARRAY_LIST_FREE_ITEM(item) ((void)(item))
#endif

#define AL_CONCAT(TAG, METHOD) TAG##_##METHOD
#define AL_CONCAT_EXP(TAG, METHOD) AL_CONCAT(TAG, METHOD)
#define AL_FN(METHOD) AL_CONCAT_EXP(ARRAY_LIST_TAG, METHOD)

// Definition of array list struct

    typedef struct ARRAY_LIST_TAG{             
        int size;                
        int capacity;            
        ARRAY_LIST_ITEM_TYPE* array;             
    } ARRAY_LIST_TAG;                      
                                 
/*                                                                            
Constructor                                                     
Allocate memory for the array buffer and set the initial size   
@return Pointer to a new array list struct                  
*/                                                          
 static inline ARRAY_LIST_TAG* AL_FN(initialize)() {                   
    ARRAY_LIST_TAG* list = malloc(sizeof(ARRAY_LIST_TAG));

    if(list == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for Array List!");
        exit(EXIT_FAILURE);
    }
                      
    list->size = 0;                                         
    list->capacity = 10;                                    
    list->array = malloc(list->capacity * sizeof(ARRAY_LIST_ITEM_TYPE));

    if(list->array == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for Array List buffer!");
        exit(EXIT_FAILURE);
    }

    return list;                                            
}                                                           
                                                            
/*
Private function to check if the array list is at full capacity  
@param list Pointer to an array list struct                 
@return true if the array list is full and false otherwise  
*/                                                          
static inline int AL_FN(is_full)(ARRAY_LIST_TAG* list) {              
    if(list->size >= list->capacity)                        
        return 1;                                           
    return 0;                                               
}                                                           
                                                            
/*                                                          
Private function used to double the capacity of the array   
@param list Pointer to an array list struct                 
*/                                                         
static inline void AL_FN(increase_capacity)(ARRAY_LIST_TAG* list) {          
    size_t new_capacity = list->capacity * 2;                      
    ARRAY_LIST_ITEM_TYPE* new_array = realloc(list->array, new_capacity * sizeof(ARRAY_LIST_ITEM_TYPE));    
    if (new_array == NULL) {                                                
        fprintf(stderr, "FATAL: Could not increase capacity of array list");                
        exit(EXIT_FAILURE);                                                             
    }                                                                       
    list->array = new_array;                                                
    list->capacity = new_capacity;                                          
}                                                                           
                                                                    
/*                                                                  
Check if the array list is empty                                    
@param list Pointer to an array list struct                          
@return true if the array list is empty and false otherwise         
*/                                                                  
static inline int AL_FN(is_empty)(ARRAY_LIST_TAG* list) {          
    return list->size == 0;                                         
}                                                                   

/*
Get the number of elements in the array list
@param list Pointer to an array list struct
@return size of the array list
*/
static inline int AL_FN(get_size)(ARRAY_LIST_TAG* list) {
    return list->size;
}

/*                                                                  
Adds an element after a specific index                              
@param list Pointer to array list struct                            
@param index Index in the array list to insert the element          
@param element Element to store in the array list                   
*/                                                                  
static inline void AL_FN(add_at_index)(ARRAY_LIST_TAG* list, int index, ARRAY_LIST_ITEM_TYPE element) {  
    if(index < 0 || index > list->size) {                                                 
        fprintf(stderr, "ERROR: Array List index out of bounds!");                  
        return;                                                   
    }                                                                                                       
                                                                    
    if(AL_FN(is_full)(list))                                        
        AL_FN(increase_capacity)(list);                             
                                                                    
    ARRAY_LIST_ITEM_TYPE* arr = list->array;                                        
    for(int i = list->size; i > index; i--)                         
        arr[i] = arr[i-1];                                          
                                                                    
    arr[index] = element;                                           
    list->size++;                                                   
}                                                                   
                                                                    
/*                                                                  
Add an element at the end of the list                               
@param list Pointer to an array list struct                         
@param element Element to store in the array list                   
*/                                                                  
static inline void AL_FN(add)(ARRAY_LIST_TAG* list, ARRAY_LIST_ITEM_TYPE element) {       
    AL_FN(add_at_index)(list, list->size, element);         
}                                                               
                                                                
/*                                                              
Add an element at the begining of the list                      
@param list Pointer to an array list struct                     
@param element Element to store in the array list               
*/                                                              
static inline void AL_FN(add_first)(ARRAY_LIST_TAG* list, ARRAY_LIST_ITEM_TYPE element) { 
    AL_FN(add_at_index)(list, 0, element);                      
}                                                               
                                                                
/*                                                              
Set the value at any specific index to another value            
@param list Pointer to an array list struct                     
@param index Index of the element to change                     
@element Element to be placed in the array list                 
*/                                                              
static inline void AL_FN(set)(ARRAY_LIST_TAG* list, int index, ARRAY_LIST_ITEM_TYPE element) {    
    if(index < 0 || index >= list->size) {                              
        fprintf(stderr, "ERROR: Invalid index Array List!");                            
        return;                                                         
    }
    // Attempt to free the previous item before replacing
    ARRAY_LIST_FREE_ITEM((list->array)[index]);
    (list->array)[index] = element;                                     
}                                                                       
                                                                        
/*                                                  
Get the element at a specific index                 
@param list Pointer to an array list struct         
@param index Index of the element to get            
@return Pointer to the element in the array list    
*/                                                  
static inline ARRAY_LIST_ITEM_TYPE AL_FN(get)(ARRAY_LIST_TAG* list, int index) {  
    if(index < 0 || index >= list->size) {              
        fprintf(stderr, "ERROR: Invalid index Array List!");            
        ARRAY_LIST_ITEM_TYPE dummy_element;
        memset(&dummy_element, 0, sizeof(ARRAY_LIST_ITEM_TYPE));
        return dummy_element;                                        
    }
    
    return (list->array)[index];                        
}                                                       
                                                        
/*                                              
Remove an element from the array list           
@param list Pointer to an array list struct     
@param index Index of the element to remove     
@return Pointer to the removed element          
*/                                              
static inline ARRAY_LIST_ITEM_TYPE AL_FN(remove)(ARRAY_LIST_TAG* list, int index) {   
    if(index < 0 || index >= list->size) {                  
        fprintf(stderr, "ERROR: Invalid index Array List!");                
        ARRAY_LIST_ITEM_TYPE dummy_element;
        memset(&dummy_element, 0, sizeof(ARRAY_LIST_ITEM_TYPE));
        return dummy_element;                                            
    }                                                       
    ARRAY_LIST_ITEM_TYPE* arr = list->array;                                
    ARRAY_LIST_ITEM_TYPE result = arr[index];                     
    for(int i = index; i < list->size - 1; i++) 
        arr[i] = arr[i+1];                                  
    list->size--;                                           
    return result;                                          
}                                                           
                                                            
/*                                                          
Destructor                                                  
Free the memory allocated for the array list buffer         
@param list Pointer to a pointer to an array list struct     
*/                                                          
static inline void AL_FN(destroy)(ARRAY_LIST_TAG** list) {            
    if(list == NULL || (*list) == NULL)                     
        return;                                             
    fprintf(stderr, "INFO: The array list %p has been destroyed!\n", (void*)(*list));  

    if((*list)->array != NULL) {  
        // Free each item from the array
        ARRAY_LIST_ITEM_TYPE* array = (*list)->array;
        for(int i = 0; i < (*list)->size; i++) 
            ARRAY_LIST_FREE_ITEM(array[i]);
                                                  
        free((*list)->array);                                           
        (*list)->array = NULL;                                          
    }                                                                   
    free((*list));                                                      
    *list = NULL;                                                       
}
#undef ARRAY_LIST_TAG
#undef ARRAY_LIST_ITEM_TYPE
#undef ARRAY_LIST_FREE_ITEM
#undef AL_CONCAT
#undef AL_CONCAT_EXP
#undef AL_FN
