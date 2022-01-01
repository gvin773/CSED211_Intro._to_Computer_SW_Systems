/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4 //Word size, size of header and footer
#define DSIZE 8 //Double word size
#define CHUNKSIZE (1<<12) //Unit size for heap expansion

#define MAX(x, y) ((x) > (y) ? (x) : (y)) //Max function

#define PACK(size, alloc) ((size) | (alloc)) //Pack block size and alloc bit to one word for header and footer

#define GET(p) (*(unsigned int *)(p)) //Read one word from address p
#define PUT(p, val) (*(unsigned int *)(p) = (val)) //Write val on address p

#define GET_SIZE(p) (GET(p) & ~0x7) //Get size of block that p points
#define GET_ALLOC(p) (GET(p) & 0x1) //Get alloc bit of block that p points

#define HDRP(bp) ((char *)(bp) - WSIZE) //Get header address of block that bp points
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) //Get footer address of block that bp points

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) //Get next block address of block that bp points
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //Get previous block address of block that bp points

#define NPTR(bp) (*(char **)(bp)) //Get block pointer of next linked block in doubly linked list
#define PPTR(bp) (*(char **)((char *)bp + 2*WSIZE)) //Get block pointer of previous linked block in doubly linked list

#define PUT_NPTR(bp, np) (NPTR(bp) = (np)) //Make link; bp->np
#define PUT_PPTR(bp, pp) (PPTR(bp) = (pp)) //Make link; pp<-bp

#define CNUM 17 //Num of free lists
#define CPTR(bp) *(char **)(bp) //Get head of linked list that bp points
#define PUT_CPTR(bp, hp) (*(char **)(bp) = (hp)) //insert hp to head of linked list that bp points

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static void *BPTR(int idx);
static int class_idx(unsigned int asize);
static void add_list(void *bp);
static void remove_list(void *bp);

static void *heap_listp;
static void *list;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    void *ptr;
    void *base;
    int i;

    //Init heap
	if ((list = mem_sbrk((CNUM+2) * DSIZE)) == (void *)-1) return -1;

    for(i = 0; i < CNUM; i++) //Make head of free list NULL
    {
        base = BPTR(i);
        PUT_CPTR(base, NULL);
    }

    heap_listp = list + CNUM*DSIZE;
    PUT(heap_listp, 0); //Alignment padding
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE,1)); //Set prologue header
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE,1)); //Set prologue footer
    PUT(heap_listp + (3*WSIZE), PACK(0,1)); //Set epliogue header
    heap_listp += (2*WSIZE); //heap_listp == prologue block footer

    //Extend heap
    if((ptr = extend_heap(CHUNKSIZE/WSIZE) == NULL)) return -1;
    base = BPTR(8); //(1<<12) / 4 * 4 = 2^12 = 4096 -> idx 8
    
    //Set initial free block
    PUT_CPTR(base, ptr);
    PUT_NPTR(base, NULL);
    PUT_PPTR(base, NULL);

    return 0;
}

static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; //for alignment
    if((long)(bp = mem_sbrk(size)) == -1) return NULL; //fail to expand the heap

    PUT(HDRP(bp), PACK(size, 0)); //make header; save size of block and make alloc bit free
    PUT(FTRP(bp), PACK(size, 0)); //make footer; save size of block and make alloc bit free
    PUT(FTRP(bp)+WSIZE, PACK(0, 1)); //new epilogue header

    return coalesce(bp); //Coalesce if the previous block was free
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;

    if(size == 0) return NULL; //malloc returns NULL when size is 0
    
    //Adjust block size to include overhead and alignment reqs
    if(size <= DSIZE) asize = 2*DSIZE;
    else asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    if((bp = find_fit(asize)) != NULL) //search free block
    {
        place(bp, asize); //allocate
        return bp;
    }

    extendsize = MAX(asize, CHUNKSIZE); //need more memory
    if((bp = extend_heap(extendsize/WSIZE)) == NULL) return NULL;
    
    add_list(bp); //add bp to appropriate list
    place(bp, asize); //allocate

    return bp;
}

static void *find_fit(size_t asize)
{
    void *bp;
    void *base;
    int i;

    for(i = class_idx(asize); i < CNUM; i++) //Check all possible list in ascending order
    {
        base = BPTR(i);
        for(bp = CPTR(base); bp != NULL; bp = NPTR(bp)) //Search free block
        {
            if(asize <= GET_SIZE(HDRP(bp))) return bp; //suitable free block exists
        }
    } //search free block from possible minimum size list

    return NULL;
}

static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    
    remove_list(bp);

    if((csize - asize) >= 3*DSIZE) //rest part is enough to use later
    {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));

        add_list(coalesce(bp));
    }
    else //rest part is too small to use later
    {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));

    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    PUT_NPTR(ptr, NULL);
    PUT_PPTR(ptr, NULL);

    add_list(coalesce(ptr));
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if(prev_alloc && next_alloc) return bp; //prev - alloc, next - alloc
    else if(prev_alloc && !next_alloc) //prev - alloc, next - free
    {
        remove_list(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if(!prev_alloc && next_alloc) //prev - free, next - alloc
    {
        remove_list(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    else //prev - free, next - free
    {
        remove_list(PREV_BLKP(bp));
        remove_list(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    return bp;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize, nextSize, asize;

    if(ptr == NULL) return mm_malloc(size);
    if(size == 0)
    {
        mm_free(oldptr);
        return NULL;
    }

    copySize = GET_SIZE(HDRP(oldptr));
    if(size == copySize) return ptr; //requested same size

    nextSize = GET_SIZE(HDRP(NEXT_BLKP(oldptr)));
    if(!GET_ALLOC(HDRP(NEXT_BLKP(oldptr))) && copySize + nextSize >= size) //possible to coalesce with next block
    {
        //Adjust block size to include overhead and alignment reqs
        if(size <= DSIZE) asize = 2*DSIZE;
        else asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

        //coalesce
        remove_list(NEXT_BLKP(oldptr));
        copySize += nextSize;
        PUT(HDRP(oldptr), PACK(copySize, 0));
        PUT(FTRP(oldptr), PACK(copySize, 0));

        place(oldptr, asize); //reallocate
        newptr = oldptr;
    }
    else
    {
        newptr = mm_malloc(size);
        if(size < copySize) copySize = size;
        memcpy(newptr, oldptr, copySize);
        mm_free(oldptr);
    }

    return newptr;
}

static void *BPTR(int idx)
{
    return ((char *)list + idx*DSIZE);
}

static int class_idx(unsigned int asize)
{
    int idx;

    if(asize == 24) idx = 0;
    else if(asize <= 32) idx = 1;
    else if(asize <= 64) idx = 2;
    else if(asize <= 128) idx = 3;
    else if(asize <= 256) idx = 4;
    else if(asize <= 512) idx = 5;
    else if(asize <= 1024) idx = 6;
    else if(asize <= 2048) idx = 7;
    else if(asize <= 4096) idx = 8;
    else if(asize <= 8192) idx = 9;
    else if(asize <= 16384) idx = 10;
    else if(asize <= 32768) idx = 11;
    else if(asize <= 65536) idx = 12;
    else if(asize <= 131072) idx = 13;
    else if(asize <= 262144) idx = 14;
    else if(asize <= 524288) idx = 15;
    else idx = 16;

    return idx;
}

static void add_list(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    int i = class_idx(size);
    void *base = BPTR(i);

    if(CPTR(base) == NULL) //list[i] is empty
    {
        PUT_NPTR(bp, NULL);
        PUT_PPTR(bp, NULL);
        PUT_CPTR(base, bp);
    }
    else //list[i] is not empty
    {
        PUT_PPTR(bp, NULL);
        PUT_NPTR(bp, CPTR(base));
        PUT_PPTR(CPTR(base), bp);
        PUT_CPTR(base, bp);
    }
}

static void remove_list(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    int i = class_idx(size);
    void *base = BPTR(i);

    if(NPTR(bp) == NULL && PPTR(bp) == NULL) PUT_CPTR(base, NULL); //list[i].size == 1
    else if(NPTR(bp) != NULL && PPTR(bp) == NULL) //first element of list[i]
    {
        PUT_PPTR(NPTR(bp), NULL);
        PUT_CPTR(base, NPTR(bp));
        PUT_NPTR(bp, NULL);
        PUT_PPTR(bp, NULL);
    }
    else if(NPTR(bp) == NULL && PPTR(bp) != NULL) //last element of list[i]
    {
        PUT_NPTR(PPTR(bp), NULL);
        PUT_NPTR(bp, NULL);
        PUT_PPTR(bp, NULL);
    }
    else //middle of list[i]
    {
        PUT_NPTR(PPTR(bp), NPTR(bp));
        PUT_PPTR(NPTR(bp), PPTR(bp));
        PUT_NPTR(bp, NULL);
        PUT_PPTR(bp, NULL);
    }
}
