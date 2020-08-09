/*
 File       : fifo.h
 Created on : 3 June, 2013, 7:00 PM
 Description: This file contains all the API for operating with the FIFO.
 
-------   BSD 2-Clause License ----------------------------
Copyright (c) 2013, Arvind Devarajan <arvind dot devarajan at outlook dot com>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this 
  list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include<stdint.h>

#ifndef FIFO_H
#define	FIFO_H

#ifdef	__cplusplus
extern "C" {
#endif        
    /**
     * Some of the functions return a boolean value
     */
    enum fbool {
        FIFO_FALSE,
        FIFO_TRUE
    };
    
    typedef enum fbool fifo_bool;
    
    /**
     * Result of some of the fifo calls.
     */
    enum result {
        FIFO_FAIL = -1,        
        FIFO_SUCC = 0
    };
    
    typedef enum result fifo_result; 
    typedef struct fifo *hfifo;
    
    /**
     * This function is called automatically by the fifo_destroy() for every
     * fifo element that it destroys. Since the type of fifo data element is
     * user-defined, the user can release all resources held by the element
     * in this function.
     */
    typedef void (*fifo_element_destroyfn)(void *);    
        
    /**
     * Create a new fifo. This FIFO does does not impose any restriction
     * on the type of data that it holds in each element. Hence, during the
     * destruction of the FIFO, it needs a user-defined function to destroy
     * individual element data. The FIFO will progressively call this function
     * for every element that it destroys so that the user can do any frees
     * needed for each data element.
     * 
     * @param fn Function that destroys an element data.
     * @return FIFO handle.
     */
    hfifo fifo_create(fifo_element_destroyfn fn);
    
    /**
     * Puts a data at the end of the fifo.
     * 
     * @param h FIFO handle as returned by fifo_create()
     * @param pdata Pointer to the data to be held in the FIFO (cannot be NULL)
     * @return FIFO_SUCC: Success; FIFO_FAIL: Failed.
     */
    fifo_result fifo_put(hfifo h, void *pdata);
    
    /**
     * Get the data contained at the head of the FIFO. 
     * This function removes the head element too.
     * 
     * @param h FIFO handle as returned by fifo_create()
     * @return Data at the head of the FIFO or NULL if no more elements
     */
    void *fifo_get(hfifo h);
    
    /**
     * Gets the next element in the FIFO.
     * 
     * This function can be used for iterating through the FIFO to keep getting
     * all elements until we are at the end of the FIFO.
     * 
     * Usage:
     *   void *pstate=NULL; // Very important to initialise this with NULL!!!
     *   void *pdata;
     * 
     *   while ((pdata = fifo_get_next(h,&pstate)) != NULL) {
     *       // Next data element is in pdata
     *       // Do something...
     *   }
     * 
     * @param h      FIFO handle as returned by fifo_create()
     * @param ppstate  Internal state of the iteration (user should not change this,
     *               and should just pass this to the function as it is).
     * @return Data in the next element in the FIFO or NULL if no more elements.
     */
    void *fifo_get_next(hfifo h, void **ppstate);   
    
    /**
     * Moves a data element from the fifo1 to fifo2. 
     * 
     * The function searches for the data pdata, removes it from
     * fifo1, and then adds it to the fifo2.
     * 
     * @param h FIFO handle as returned by fifo_create
     * @param pdata Data to be moved.
     * @param fifo1 FIFO from which the element should be moved
     * @param fifo2 FIFO to which the element should be added
     * @return FIFO_SUCC: Success; FIFO_FAIL: Failed.
     */
    fifo_result fifo_move_element(void *pdata, hfifo fifo1, hfifo fifo2);        
    
    /**
     * Releases all resources of the fifo.
     * @param h FIFO handle as retuned by fifo_create
     * @param fn Pointer to a function that destroys an element of fifo.
     */
    void fifo_destroy(hfifo h, fifo_element_destroyfn fn);
    
#ifdef	__cplusplus
}
#endif

#endif	/* FIFO_H */

