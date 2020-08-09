/*
 File       : fifo.c
 Created on : 3 June, 2013, 8:43 PM
 Description:

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
#include "fifo.h"
#include <stdio.h>
#include <malloc.h>

/* Private data structures */

/**
 * FIFO element.     
 */
struct fifo_ele {
    void *pdata; //! FIFO data element
    struct fifo_ele *pnext; //! Next in the linked list
    struct fifo_ele *pprev; //! Previous in the linked list
};

/**
 * Type of fifo element.
 */
typedef struct fifo_ele fifo_element;

/**
 * Main FIFO structure.
 *      
 * FIFO elements are connected in the FIFO like this:
 * 
 *    Tail      Prev   Next    Prev   Next   Head
 *        | E |<-----------|E|<-----------|E|
 */
struct fifo {
    fifo_element_destroyfn destroyfn; //! Element destroy function
    fifo_element *phead;
    fifo_element *ptail;
};

/* STATIC FUNCTIONS USED ONLY WITHIN THIS FILE */

/**
 * Searches for a data in the fifo.
 * 
 * @param h FIFO handle as returned by fifo_create
 * @param pdata Data to be searched.
 * @return Pointer to the fifo element containing the data if the found; 
 * @return NULL otherwise.
 */
static fifo_element *fifo_search(hfifo h, void *pdata) {
    if (!h || !pdata)
        return NULL;

    // Start from the head element, and then keep going round until the data element
    // to be searched is found.
    fifo_element *pele = h->phead;

    while (pele) {
        if (pele->pdata == pdata)
            return pele;
        else
            pele = pele->pnext;
    }
    return NULL;
}

/**
 * Removes a FIFO element containing the given data.
 * 
 * @param h Handle as returned by fifo_create
 * @param pdata Data to be searched and removed
 * @return FIFO element in which the data was found.
 */
static fifo_element *fifo_remove_element(hfifo h, void *pdata) {
    if (!h || !pdata)
        return NULL;

    // First search for the data element
    fifo_element *pele = fifo_search(h, pdata);
    if (!pele)
        return NULL;

    // If this element is the head element of the fifo, just get this
    // element ("get") also removes the head, and readjusts pointers).
    if (pele == h->phead) {
        (void) fifo_get(h); // voiding return value as we dont need it.
        return pele;
    }

    // Now, remove this element from the FIFO by adjusting the prev and
    // the next pointers of the fifo elements.
    fifo_element *pnext = pele->pnext; // The "next" element may not exist if
    // this element happens to be the tail
    // element of the fifo.
    fifo_element *pprev = pele->pprev;
    pprev->pnext = pnext; // THis could be NULL if the element we are removing is tail

    // Adjust next pointer only if pnext exists (i.e., we are not removing the tail)
    if (pnext)
        pnext->pprev = pprev;
    else {
        // Adjust the tail of the fifo
        h->ptail = pprev; // The previous element now becomes the tail too.
    }

    return pele;
}

/*---------------- Public functions, exposed via fifo.h ------------------- */

hfifo fifo_create(fifo_element_destroyfn fn) {
    hfifo pfifo;

    if (!fn) {
        return NULL;
    }

    pfifo = calloc(1, sizeof (struct fifo));
    if (!pfifo)
        return NULL;

    pfifo->destroyfn = fn;
    pfifo->phead = NULL;
    pfifo->ptail = NULL;
    return pfifo;
}

fifo_result fifo_put(hfifo h, void *pdata) {
    if (!h || !pdata)
        return FIFO_FAIL;

    fifo_element *pele = calloc(1, sizeof (fifo_element));
    if (!pele)
        return FIFO_FAIL;

    pele->pdata = pdata;

    if (!h->phead) {
        // THis is the first element in the FIFO. Hence,
        // this is both the head and the tail element of the FIFO.
        h->phead = pele;
        h->ptail = pele;

        // This new element does not have a previous and the next elements
        // as it is the only element in the fifo.
        pele->pprev = pele->pnext = NULL;
    } else {
        // We already have some elements in the FIFO.
        // Add this new element at the tail.
        pele->pnext = NULL; // Since this is the tail element, there is no
        // "next" element in the FIFO.

        // To add this as the tail element, we first make the "previous" 
        // element of this element to be the "current" tail element of this fifo.
        pele->pprev = h->ptail;

        // Finally, append this element to the end of the FIFO
        h->ptail = pele;
    }

    return FIFO_SUCC;
}

void *fifo_get(hfifo h) {
    if (!h)
        return NULL;

    // First, store the head element to be returned.
    fifo_element *pele = h->phead; // This is the "current" head element
    void *pdata = pele->pdata;

    // Now, remove the head element from the fifo and 
    // re-adjust all pointers.

    // The next element of the current "head" element becomes
    // the head element of the fifo.
    h->phead = pele->pnext;

    // The previous element of the new head element is now NULL, as there
    // is no more element "before" the head element.
    h->phead->pprev = NULL; // (the next element of the head element remains as before)

    // Release this element
    free(pele);

    // Return the element data    
    return pdata;
}

void *fifo_get_next(hfifo h, void **ppstate) {
    if (!h || !ppstate)
        return NULL;

    fifo_element *pele = *ppstate;

    // If pele is NULL, this is the first run of this iteration. Just
    // return the data in the head element.
    if (pele == NULL) {
        *ppstate = h->phead;
    } else {
        // This is not the first iteration. Move the state to the next element
        // and return data from the next element.
        if (pele->pnext != NULL) {
            *ppstate = pele->pnext;
        } else {
            return NULL;
        }
    }
    pele = *ppstate;
    return pele->pdata;
}

fifo_result fifo_move_element(void *pdata, hfifo fifo1, hfifo fifo2) {
    if (!pdata || !fifo1 || !fifo2)
        return FIFO_FAIL;

    // Remove this element from fifo1
    fifo_element *pele = fifo_remove_element(fifo1, pdata);
    if (!pele)
        return FIFO_FAIL;

    // Add this element in FIFO2
    fifo_result ret = fifo_put(fifo2, pdata);

    return ret;
}

void fifo_destroy(hfifo h, fifo_element_destroyfn fn) {
    fifo_element *pcur = h->phead;
    fifo_element *pnext;
    
    // Destroy all the fifo elements
    while (pcur) {
        pnext=pcur->pnext;
        h->destroyfn(pcur->pdata); // Destroy fifo data using user-supplied destroy function
        free(pcur);
        pcur=pnext;
    }
    
    // Finally, release the fifo
    free(h);    
}


