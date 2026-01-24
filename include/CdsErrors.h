#ifndef CDS_ERRORS_H
#define CDS_ERRORS_H

typedef enum {

    CONTAINER_SUCCESS = 0,
    /* Returned when a `NULL` pointer is passed for the required container */
    CONTAINER_ERR_NULL_PTR = -1,
    /* Returned when memory allocation for internal structures fails */
    CONTAINER_ERROR_OUT_OF_MEMORY = -2,
    /* Returned when NULL data pointer is passed for storage */
    CONTAINER_ERROR_NULL_DATA = -3,
    /* Returned when attempting removal from an empty container */
    CONTAINER_ERROR_EMPTY = -4,
    /* Returned when an invalid or non-belonging node is used */
    CONTAINER_ERROR_INVALID_NODE = -5,
    /* Returned when no required callback function is set or provided */
    CONTAINER_ERROR_NO_CALLBACK = -6,
    /* Returned when searched data is not present in the container */
    CONTAINER_ERROR_NOT_FOUND = -7,
    /* Returned when `*_init` called on an initialized container again */
    CONTAINER_ERROR_ALREADY_INIT = -8,
    /* Returned when output pointer (e.g., data**) is NULL */
    CONTAINER_ERROR_NULL_OUTPUT = -9,
    /**/
    CONTAINER_ERROR_UNINIT = -10,

} ContainerError;

#endif /* CDS_ERRORS_H */