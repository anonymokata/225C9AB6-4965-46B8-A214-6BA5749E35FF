#include "errors.h"

/* global var to store error flags */
unsigned int ERROR_FLAGS = 0;

void resetErrors() 
{
   ERROR_FLAGS = 0;
}

/* set a bit representing an error */
void setError(errorflags_t errorflag) 
{
   ERROR_FLAGS |= (1<<errorflag);
}

/* get a bit representing an error */
int getError(errorflags_t errorflag) 
{
   return (ERROR_FLAGS & (1<<errorflag) ? 1 : 0);
}

/* return the error flags */
int getErrors(void) 
{
   return (ERROR_FLAGS);
}
