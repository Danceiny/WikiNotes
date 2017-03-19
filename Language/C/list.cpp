/*****************************************************************************
 Copyright (c) 2008, Hangzhou H3C Technologies Co., Ltd. All rights reserved.
------------------------------------------------------------------------------
                            list.h
  Project Code: Comware Leopard
   Module Name:
  Date Created: 2008-3-28
        Author: c02254
   Description: This file defines four types of data structures: singly-linked
                lists, singly-linked tail queues, doubly-linked lists and
                doubly-linked tail queues.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------
2009-8-5      Y01604       Add RCU supporting
*****************************************************************************/
#ifndef _SYS_LIST_H_
#define _SYS_LIST_H_

#ifdef  __cplusplus
extern "C"{
#endif

/*
 * Singly-linked List
 *
 * A singly-linked list is headed by a single forward pointer. The elements
 * are singly linked for minimum space and pointer manipulation overhead at
 * the expense of O(n) removal for arbitrary elements. New elements can be
 * added to the list after an existing element or at the head of the list.
 * Elements being removed from the head of the list should use the explicit
 * macro for this purpose for optimum efficiency. A singly-linked list may
 * only be traversed in the forward direction.  Singly-linked lists are ideal
 * for applications with large datasets and few or no removals or for
 * implementing a LIFO queue.
                     +--------------+      +--------------+
                     |user structure|      |user structure|
                     +--------------+      +--------------+
                     |   ......     |      |   ......     |
                     +--------------+      +--------------+
 +------------+  +-->|+------------+|  +-->|+------------+|
 | SL_HEAD_S  |  |   || SL_NODE_S  ||  |   || SL_NODE_S  ||
 +------------+  |   |+------------+|  |   |+------------+|
 | pstFirst   |--+   || pstNext    ||--+   || pstNext    ||----+
 +------------+      |+------------+|      |+------------+|   -+-
                     +--------------+      +--------------+
                     |   ......     |      |   ......     |
                     +--------------+      +--------------+
 */
typedef struct tagSL_NODE
{
    struct tagSL_NODE* pstNext;  /* the next element */
} SL_NODE_S;

#define SL_ENTRY(ptr, type, member) (container_of(ptr, type, member))

typedef struct tagSL_HEAD
{
    SL_NODE_S* pstFirst;
} SL_HEAD_S;

static inline VOID SL_Init(IN SL_HEAD_S* pstList);
static inline VOID SL_NodeInit(IN SL_NODE_S* pstNode);
static inline BOOL_T SL_IsEmpty(IN SL_HEAD_S* pstList);
static inline SL_NODE_S* SL_First(IN SL_HEAD_S* pstList);
static inline SL_NODE_S* SL_Next(IN SL_NODE_S* pstNode);
static inline VOID SL_AddHead(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode);
static inline SL_NODE_S* SL_DelHead(IN SL_HEAD_S* pstList);
static inline VOID SL_AddAfter(IN SL_HEAD_S* pstList,
                               IN SL_NODE_S* pstPrev,
                               IN SL_NODE_S* pstInst);
static inline SL_NODE_S* SL_DelAfter(IN SL_HEAD_S* pstList,
                                     IN SL_NODE_S* pstPrev);
static inline VOID SL_Del(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode);
static inline VOID SL_Append(IN SL_HEAD_S* pstDstList, IN SL_HEAD_S* pstSrcList);
static inline VOID SL_FreeAll(IN SL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));

/*****************************************************************************
    Func Name: SL_Init
 Date Created: 2008/3/28
       Author: c02254
  Description: Initialize the singly-linked list head
        Input: SL_HEAD_S* pstList  the pointer to the list head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID SL_Init(IN SL_HEAD_S* pstList)
{
    pstList->pstFirst = (SL_NODE_S *)NULL;
    return;
}

/*****************************************************************************
    Func Name: SL_NodeInit
 Date Created: 2008/8/6
       Author: c02254
  Description: Initialize the singly-linked list node
        Input: IN SL_NODE_S* pstNode  List node pointer
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID SL_NodeInit(IN SL_NODE_S* pstNode)
{
    pstNode->pstNext = (SL_NODE_S *)NULL;
}

/*****************************************************************************
    Func Name: SL_IsEmpty
 Date Created: 2008/3/28
       Author: c02254
  Description: Whether the singly-linked list is empty
        Input: SL_HEAD_S* pstList  the pointer to the list head
       Output: No output
       Return: BOOL_TRUE or BOOL_FLASE
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T SL_IsEmpty(IN SL_HEAD_S* pstList)
{
    return (pstList->pstFirst == NULL);
}

/*****************************************************************************
    Func Name: SL_First
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the first element of a singly-linked list
        Input: SL_HEAD_S* pstList  the pointer to the list head
       Output: No output
       Return: the pointer to first element of the singly-linked list
               NULL if the list is empty
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline SL_NODE_S* SL_First(IN SL_HEAD_S* pstList)
{
    return (pstList->pstFirst);
}

/*****************************************************************************
    Func Name: SL_Next
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the next element of current one
        Input: SL_NODE_S* pstNode  the pointer to current element
       Output: No output
       Return: the pointer to next element of current element
               NULL if the current element is the last one
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline SL_NODE_S* SL_Next(IN SL_NODE_S* pstNode)
{
    return (pstNode->pstNext);
}

/*****************************************************************************
    Func Name: SL_AddHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element to the head of the list
        Input: SL_HEAD_S* pstList  the pointer to the list head
               SL_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID SL_AddHead(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode)
{
    pstNode->pstNext  = pstList->pstFirst;
    pstList->pstFirst = pstNode;
    return;
}

/*****************************************************************************
    Func Name: SL_DelHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete the first element from the list
        Input: SL_HEAD_S* pstList  the pointer to the list head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline SL_NODE_S* SL_DelHead(IN SL_HEAD_S* pstList)
{
    SL_NODE_S* pstNode = pstList->pstFirst;

    if (NULL != pstNode)
    {
        pstList->pstFirst = pstNode->pstNext;
    }

    return pstNode;
}

/*****************************************************************************
    Func Name: SL_AddAfter
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element after another one
        Input: SL_HEAD_S* pstList  the pointer to the list head
               SL_NODE_S* pstPrev  pointer to the previous element
               SL_NODE_S* pstInst  pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID SL_AddAfter(IN SL_HEAD_S* pstList,
                               IN SL_NODE_S* pstPrev,
                               IN SL_NODE_S* pstInst)
{
    if (NULL == pstPrev)
    {
        SL_AddHead (pstList, pstInst);
    }
    else
    {
        pstInst->pstNext = pstPrev->pstNext;
        pstPrev->pstNext = pstInst;
    }
    return;
}

/*****************************************************************************
    Func Name: SL_DelAfter
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete the element  after the specified one
        Input: SL_HEAD_S* pstList  the pointer to the list head
               SL_NODE_S* pstPrev  pointer to the element before the one
                                   to be deleted
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline SL_NODE_S* SL_DelAfter(IN SL_HEAD_S* pstList,
                                     IN SL_NODE_S* pstPrev)
{
    SL_NODE_S* pstNode;

    if (NULL == pstPrev)
    {
        pstNode = SL_DelHead (pstList);
    }
    else
    {
        pstNode = pstPrev->pstNext;
        if (NULL != pstNode)
        {
            pstPrev->pstNext = pstNode->pstNext;
        }
    }

    return pstNode;
}

/* macro for walk the singly-linked list */
#define SL_FOREACH(pstList, pstNode) \
    for ((pstNode) = SL_First(pstList); \
         NULL != (pstNode); \
         (pstNode) = SL_Next(pstNode))

#define SL_FOREACH_SAFE(pstList, pstNode, pstNext) \
    for ((pstNode) = SL_First((pstList)); \
         (NULL != (pstNode)) && ({(pstNext) = SL_Next(pstNode); BOOL_TRUE;}); \
         (pstNode) = (pstNext))

#define SL_FOREACH_PREVPTR(pstList, pstNode, pstPrev) \
    for ((pstNode) = SL_First(pstList), (pstPrev) = (SL_NODE_S *)NULL; \
         NULL != (pstNode); \
         (VOID)({(pstPrev) = (pstNode); (pstNode) = SL_Next(pstNode);}))

#define SL_ENTRY_FIRST(pstList, type, member) \
    (SL_IsEmpty(pstList) ? NULL : SL_ENTRY(SL_First(pstList), type, member))

#define SL_ENTRY_NEXT(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
        (NULL == SL_Next(&((pstEntry)->member))) ? NULL : \
           SL_ENTRY(SL_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member))

#define SL_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
          NULL != (pstEntry); \
         (pstEntry) = SL_ENTRY_NEXT(pstEntry, member))

#define SL_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = SL_ENTRY_NEXT(pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define SL_FOREACH_ENTRY_PREVPTR(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), \
            (pstPrevEntry) = NULL; \
          NULL != (pstEntry); \
          (VOID)({(pstPrevEntry) = (pstEntry); \
                   (pstEntry) = SL_ENTRY_NEXT(pstEntry, member);}))

/*****************************************************************************
    Func Name: SL_Del
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete an element from the list
        Input: SL_HEAD_S* pstList  the pointer to the list head
               SL_NODE_S* pstNode  the pointer to the element to be deleted
       Output: No output
       Return: No return
      Caution: It takes the expense of O(n). If you have got a pointer to the
               previous element, you'd better use SL_AddAfter whitch takes
               the expense of O(1).
               This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID SL_Del(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode)
{
    SL_NODE_S *pstCur, *pstPrev;

    SL_FOREACH_PREVPTR (pstList, pstCur, pstPrev)
    {
        if (pstCur == pstNode)
        {
            (VOID) SL_DelAfter(pstList, pstPrev);
            break;
        }
    }
    return;
}

/*****************************************************************************
    Func Name: SL_Append
 Date Created: 2008/3/28
       Author: c02254
  Description: Append pstSrcList to pstDstList
        Input: SL_HEAD_S* pstDstList  the list to be appended to
               SL_HEAD_S* pstSrcList  the list to be append
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID SL_Append(IN SL_HEAD_S* pstDstList, INOUT SL_HEAD_S* pstSrcList)
{
    SL_NODE_S *pstNode, *pstPrev;

    if (BOOL_TRUE != SL_IsEmpty(pstSrcList))
    {
        SL_FOREACH_PREVPTR(pstDstList, pstNode, pstPrev)
        {
            ; /* do nothing */
        }
        
        if (NULL == pstPrev)
        {
            pstDstList->pstFirst = SL_First(pstSrcList);
        }
        else
        {
            pstPrev->pstNext = SL_First(pstSrcList);
        }

        SL_Init(pstSrcList);
    }
    
    return;
}

/*****************************************************************************
    Func Name: SL_FreeAll
 Date Created: 2009/10/27 
       Author: Yang Yiquan
  Description: Remove and free all nodes from SL list
        Input: IN SL_HEAD_S *pstList      
                IN VOID (*pfFree)(VOID *)  
       Output: 
       Return: STATIC
      Caution: 
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
static inline VOID SL_FreeAll(IN SL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    SL_NODE_S *pstCurNode;
    SL_NODE_S *pstNextNode;

    /* Free all node from list */
    SL_FOREACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    SL_Init(pstList);
    return;
}

/*
 * Singly-linked Tail queue
 *
 * A singly-linked tail queue is headed by a pair of pointers, one to the
 * head of the list and the other to the tail of the list. The elements are
 * singly linked for minimum space and pointer manipulation overhead at the
 * expense of O(n) removal for arbitrary elements. New elements can be added
 * to the list after an existing element, at the head of the list, or at the
 * end of the list. Elements being removed from the head of the tail queue
 * should use the explicit macro for this purpose for optimum efficiency.
 * A singly-linked tail queue may only be traversed in the forward direction.
 * Singly-linked tail queues are ideal for applications with large datasets
 * and few or no removals or for implementing a FIFO queue.
                      +---------------+      +---------------+
                      |user structure |      |user structure |
                      +---------------+      +---------------+
                      |   ......      |      |   ......      |
                      +---------------+      +---------------+
 +-------------+  +-->|+-------------+|  +-->|+-------------+|<-------+
 | STQ_HEAD_S  |  |   || STQ_NODE_S  ||  |   || STQ_NODE_S  ||        |
 +-------------+  |   |+-------------+|  |   |+-------------+|        |
 | pstFirst    |--+   || pstNext     ||--+   || pstNext     ||----+   |
 +-------------+      |+-------------+|      |+-------------+|   -+-  |
 | pstLast     |--+   +---------------+      +---------------+        |
 +-------------+  |   |   ......      |      |   ......      |        |
                  |   +---------------+      +---------------+        |
                  +---------------------------------------------------+
 */
typedef struct tagSTQ_NODE
{
    struct tagSTQ_NODE* pstNext; /* the next element */
} STQ_NODE_S;

#define STQ_ENTRY(ptr, type, member)    (container_of(ptr, type, member))

typedef struct tagSTQ_HEAD
{
    STQ_NODE_S *pstFirst;  /* the first element */
    STQ_NODE_S *pstLast;   /* the last element  */
} STQ_HEAD_S;

static inline VOID STQ_Init(IN STQ_HEAD_S* pstList);
static inline VOID STQ_NodeInit(IN STQ_NODE_S* pstNode);
static inline BOOL_T STQ_IsEmpty(IN STQ_HEAD_S* pstList);
static inline STQ_NODE_S* STQ_First(IN STQ_HEAD_S* pstList);
static inline STQ_NODE_S* STQ_Last(IN STQ_HEAD_S* pstList);
static inline STQ_NODE_S* STQ_Next(IN STQ_NODE_S* pstNode);
static inline VOID STQ_AddHead(IN STQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode);
static inline STQ_NODE_S* STQ_DelHead(STQ_HEAD_S* pstList);
static inline VOID STQ_AddTail(STQ_HEAD_S* pstList, STQ_NODE_S* pstNode);
static inline VOID STQ_AddAfter(IN STQ_HEAD_S* pstList,
                                IN STQ_NODE_S* pstPrev,
                                IN STQ_NODE_S* pstInst);
static inline STQ_NODE_S* STQ_DelAfter(IN STQ_HEAD_S* pstList,
                                       IN STQ_NODE_S* pstPrev);
static inline VOID STQ_Del(IN STQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode);
static inline VOID STQ_Append(IN STQ_HEAD_S* pstDstList,
                              IN STQ_HEAD_S* pstSrcList);
static inline VOID STQ_FreeAll(IN STQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));

/*****************************************************************************
    Func Name: STQ_Init
 Date Created: 2008/3/28
       Author: c02254
  Description: Initial a singly-linked tail queue head
        Input: STQ_HEAD_S* pstList  the singly-linked tail queue head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID STQ_Init(IN STQ_HEAD_S* pstList)
{
    pstList->pstFirst = (STQ_NODE_S *)NULL;
    pstList->pstLast  = (STQ_NODE_S *)NULL;
    return;
}

/*****************************************************************************
    Func Name: STQ_NodeInit
 Date Created: 2008/8/6
       Author: c02254
  Description: Initial a singly-linked tail queue node
        Input: IN STQ_NODE_S* pstNode  list node pointer
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID STQ_NodeInit(IN STQ_NODE_S* pstNode)
{
    pstNode->pstNext = (STQ_NODE_S *)NULL;
}

/*****************************************************************************
    Func Name: STQ_IsEmpty
 Date Created: 2008/3/28
       Author: c02254
  Description: Whether the singly-linked tail queue is empty
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: BOOL_TRUE or BOOL_FLASE
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T STQ_IsEmpty(IN STQ_HEAD_S* pstList)
{
    return (pstList->pstFirst == NULL);
}

/*****************************************************************************
    Func Name: STQ_First
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the first element of a singly-linked tail queue
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to first element of the singly-linked tail queue
               NULL if the list is empty
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline STQ_NODE_S* STQ_First(IN STQ_HEAD_S* pstList)
{
    return pstList->pstFirst;
}

/*****************************************************************************
    Func Name: STQ_Last
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the last element of a singly-linked tail queue
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to last element of the singly-linked tail queue
               NULL if the list is empty
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline STQ_NODE_S* STQ_Last(IN STQ_HEAD_S* pstList)
{
    return pstList->pstLast;
}

/*****************************************************************************
    Func Name: STQ_Next
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the next element of current one
        Input: STQ_NODE_S* pstNode  the pointer to current element
       Output: No output
       Return: the pointer to next element of current element
               NULL if the current element is the last one
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline STQ_NODE_S* STQ_Next(IN STQ_NODE_S* pstNode)
{
    return pstNode->pstNext;
}

/*****************************************************************************
    Func Name: STQ_AddHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element to the head of the queue
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
               STQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID STQ_AddHead(IN STQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    pstNode->pstNext = pstList->pstFirst;
    pstList->pstFirst = pstNode;
    if (NULL == pstList->pstLast)
    {
        pstList->pstLast = pstNode;
    }
    return;
}

/*****************************************************************************
    Func Name: STQ_DelHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete the first element from the queue
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline STQ_NODE_S* STQ_DelHead(STQ_HEAD_S* pstList)
{
    STQ_NODE_S* pstNode = pstList->pstFirst;

    if (NULL != pstNode)
    {
        pstList->pstFirst = pstNode->pstNext;
    }
    if (NULL == pstList->pstFirst)
    {
        pstList->pstLast = (STQ_NODE_S *)NULL;
    }

    return pstNode;
}

/*****************************************************************************
    Func Name: STQ_AddTail
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element to the tail of the queue
        Input: STQ_HEAD_S* pstList  the pointer to the queue tail
               STQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID STQ_AddTail(STQ_HEAD_S* pstList, STQ_NODE_S* pstNode)
{
    pstNode->pstNext = (STQ_NODE_S *)NULL;
    if (NULL != pstList->pstLast)
    {
        pstList->pstLast->pstNext = pstNode;
        pstList->pstLast = pstNode;
    }
    else
    {
        pstList->pstLast  = pstNode;
        pstList->pstFirst = pstNode;
    }
    return;
}

/*****************************************************************************
    Func Name: STQ_AddAfter
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element after another one
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
               STQ_NODE_S* pstPrev  pointer to the previous element
               STQ_NODE_S* pstInst  pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID STQ_AddAfter(IN STQ_HEAD_S* pstList,
                                IN STQ_NODE_S* pstPrev,
                                IN STQ_NODE_S* pstInst)
{
    if (NULL == pstPrev)
    {
        STQ_AddHead (pstList, pstInst);
    }
    else
    {
        pstInst->pstNext = pstPrev->pstNext;
        pstPrev->pstNext = pstInst;
        if (pstList->pstLast == pstPrev)
        {
            pstList->pstLast = pstInst;
        }
    }
    return;
}

/*****************************************************************************
    Func Name: STQ_DelAfter
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete the element after the specified one
        Input: STQ_NODE_S* pstPrev  pointer to the element before the one
                                    to be deleted
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline STQ_NODE_S* STQ_DelAfter(IN STQ_HEAD_S* pstList,
                                       IN STQ_NODE_S* pstPrev)
{
    STQ_NODE_S* pstNode;

    if (NULL == pstPrev)
    {
        pstNode = STQ_DelHead (pstList);
    }
    else
    {
        pstNode = pstPrev->pstNext;
        if (NULL != pstNode)
        {
            pstPrev->pstNext = pstNode->pstNext;
        }
        if (pstList->pstLast == pstNode)
        {
            pstList->pstLast = pstPrev;
        }
    }

    return pstNode;
}

/* macro for walk the singly-linked tail queue */
#define STQ_FOREACH(pstList, pstNode) \
    for((pstNode) = STQ_First(pstList); \
        NULL != (pstNode); \
        (pstNode) = STQ_Next(pstNode))

#define STQ_FOREACH_SAFE(pstList, pstNode, pstNext) \
    for ((pstNode) = STQ_First(pstList); \
         (NULL != (pstNode)) && ({(pstNext) = STQ_Next(pstNode); BOOL_TRUE;}); \
         (pstNode) = (pstNext))

#define STQ_FOREACH_PREVPTR(pstList, pstNode, pstPrev) \
    for ((pstNode) = STQ_First(pstList), (pstPrev) = (STQ_NODE_S *)NULL; \
         NULL != (pstNode); \
         (VOID)({(pstPrev) = (pstNode); (pstNode) = STQ_Next(pstNode);}))

#define STQ_ENTRY_FIRST(pstList, type, member) \
    (STQ_IsEmpty(pstList) ? NULL : STQ_ENTRY(STQ_First(pstList), type, member))

#define STQ_ENTRY_LAST(pstList, type, member) \
    (STQ_IsEmpty(pstList) ? NULL : STQ_ENTRY(STQ_Last(pstList), type, member))

#define STQ_ENTRY_NEXT(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
       (NULL == STQ_Next(&((pstEntry)->member)) ? NULL : \
          STQ_ENTRY(STQ_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define STQ_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         NULL != (pstEntry); \
         (pstEntry) = STQ_ENTRY_NEXT(pstEntry, member))

#define STQ_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = STQ_ENTRY_NEXT(pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define STQ_FOREACH_ENTRY_PREVPTR(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), \
            (pstPrevEntry) = NULL; \
          NULL != (pstEntry); \
          (VOID)({(pstPrevEntry) = (pstEntry); \
                   (pstEntry) = STQ_ENTRY_NEXT(pstEntry, member);}))

/*****************************************************************************
    Func Name: STQ_Del
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete an element from the tail queue
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
               STQ_NODE_S* pstNode  the pointer to the element to be deleted
       Output: No output
       Return: No return
      Caution: It takes the expense of O(n). If you have got a pointer to the
               previous element, you'd better use STQ_DelAfter whitch takes
               the expense of O(1).
               This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID STQ_Del(IN STQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    STQ_NODE_S *pstPrev, *pstCur;

    STQ_FOREACH_PREVPTR (pstList, pstCur, pstPrev)
    {
        if (pstCur == pstNode)
        {
            (VOID)STQ_DelAfter (pstList, pstPrev);
            break;
        }
    }
    return;
}

/*****************************************************************************
    Func Name: STQ_Append
 Date Created: 2008/3/28
       Author: c02254
  Description: Append pstSrcList to pstDstList
        Input: STQ_HEAD_S* pstDstList  the list to be appended to
               STQ_HEAD_S* pstSrcList  the list to be append
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID STQ_Append(IN STQ_HEAD_S* pstDstList,
                              INOUT STQ_HEAD_S* pstSrcList)
{
    if (BOOL_TRUE != STQ_IsEmpty(pstSrcList))
    {
        if (NULL != pstDstList->pstLast)
        {
            pstDstList->pstLast->pstNext = STQ_First(pstSrcList);
        }
        else
        {
            pstDstList->pstFirst = STQ_First(pstSrcList);
        }
        
        pstDstList->pstLast = STQ_Last(pstSrcList);
        STQ_Init(pstSrcList);
    }
    
    return;
}

/*****************************************************************************
    Func Name: STQ_FreeAll
 Date Created: 2009/10/27 
       Author: Yang Yiquan
  Description: Remove and free all nodes from STQ list
        Input: IN STQ_HEAD_S *pstList      
                IN VOID (*pfFree)(VOID *)  
       Output: 
       Return: STATIC
      Caution: 
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
static inline VOID STQ_FreeAll(IN STQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    STQ_NODE_S *pstCurNode;
    STQ_NODE_S *pstNextNode;

    /* Free all node from list */
    STQ_FOREACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    STQ_Init(pstList);
    return;
}

/*
 * Doubly-linked List
 *
 * A doubly-linked list is headed by a single forward pointer (or an array of
 * forward pointers for a hash table header). The elements are doubly linked
 * so that an arbitrary element can be removed without a need to traverse the
 * list. New elements can be added to the list before or after an existing
 * element or at the head of the list. A doubly-linked list may only be
 * traversed in the forward direction.
                         +--------------+      +--------------+
                         |user structure|      |user structure|
                         +--------------+      +--------------+
                         |   ......     |      |   ......     |
                         +--------------+      +--------------+
    +------------+  +--->|+------------+|  +-->|+------------+|
    | DL_HEAD_S  |  |    || DL_NODE_S  ||  |   || DL_NODE_S  ||
    +------------+  | +->|+------------+|  |   |+------------+|
 +->| pstFirst   |--+ |  || pstNext    ||--+   || pstNext    ||----+
 |  +------------+    |  |+------------+|      |+------------+|   -+-
 +--------------------C--|| ppstPre    ||  +---|| ppstPre    ||
                      |  |+------------+|  |   |+------------+|
                      |  +--------------+  |   +--------------+
                      |  |   ......     |  |   |   ......     |
                      |  +--------------+  |   +--------------+
                      +--------------------+
 */
typedef struct tagDL_NODE
{
    struct tagDL_NODE*  pstNext;  /* the next element */
    struct tagDL_NODE** ppstPre;  /* the address of previous element's pstNext */
} DL_NODE_S;

#define DL_ENTRY(ptr, type, member) (container_of(ptr, type, member))

#define DL_NODE_FROM_PPRE(ppstPre)  (container_of(ppstPre, DL_NODE_S, pstNext))

#define DL_ENTRY_FROM_PPRE(ppstPre, type, member) \
    DL_ENTRY(DL_NODE_FROM_PPRE(ppstPre), type, member)

typedef struct tagDL_HEAD
{
    DL_NODE_S* pstFirst; /* the first element */
} DL_HEAD_S;

static inline VOID DL_Init(IN DL_HEAD_S* pstList);
static inline VOID DL_NodeInit(IN DL_NODE_S* pstNode);
static inline BOOL_T DL_IsEmpty(IN DL_HEAD_S* pstList);
static inline DL_NODE_S* DL_First(IN DL_HEAD_S* pstList);
static inline DL_NODE_S* DL_Next(IN DL_NODE_S* pstNode);
static inline DL_NODE_S* DL_Prev(IN DL_NODE_S* pstNode);
static inline VOID DL_Del(IN DL_NODE_S* pstNode);
static inline VOID DL_AddHead(IN DL_HEAD_S* pstList, IN DL_NODE_S* pstNode);
static inline DL_NODE_S* DL_DelHead(IN DL_HEAD_S* pstList);
static inline VOID DL_AddAfter(IN DL_NODE_S* pstPrev, IN DL_NODE_S* pstInst);
static inline VOID DL_AddAfterPtr (IN DL_NODE_S **ppstPre, IN DL_NODE_S  *pstInst);
static inline VOID DL_AddBefore(IN DL_NODE_S* pstNext, IN DL_NODE_S* pstInst);
static inline VOID DL_Append(IN DL_HEAD_S* pstDstList, IN DL_HEAD_S* pstSrcList);
static inline VOID DL_FreeAll(IN DL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));

/*****************************************************************************
    Func Name: DL_Init
 Date Created: 2008/3/28
       Author: c02254
  Description: Initial a doubly-linked list head
        Input: DL_HEAD_S* pstList  the doubly-linked list head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_Init(IN DL_HEAD_S* pstList)
{
    pstList->pstFirst = (DL_NODE_S*)NULL;
    return;
}

/*****************************************************************************
    Func Name: DL_NodeInit
 Date Created: 2008/8/6
       Author: c02254
  Description: Initialize a doubly-linked list node
        Input: IN DL_NODE_S* pstNode  list node pointer
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_NodeInit(IN DL_NODE_S* pstNode)
{
    pstNode->pstNext = (DL_NODE_S*)NULL;
    pstNode->ppstPre = (DL_NODE_S**)NULL;
}

/*****************************************************************************
    Func Name: DL_IsEmpty
 Date Created: 2008/3/28
       Author: c02254
  Description: Whether the singly-linked list is empty
        Input: DL_HEAD_S* pstList  the pointer to the list head
       Output: No output
       Return: BOOL_TRUE or BOOL_FLASE
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T DL_IsEmpty(IN DL_HEAD_S* pstList)
{
    return (pstList->pstFirst == NULL);
}

/*****************************************************************************
    Func Name: DL_First
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the first element of a doubly-linked list
        Input: DL_HEAD_S* pstList  the pointer to the list head
       Output: No output
       Return: the pointer to first element of the doubly-linked list
               NULL if the list is empty
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DL_NODE_S* DL_First(IN DL_HEAD_S* pstList)
{
    return (pstList->pstFirst);
}

/*****************************************************************************
    Func Name: DL_Next
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the next element of current one
        Input: DL_NODE_S* pstNode  the pointer to current element
       Output: No output
       Return: the pointer to next element of current element
               NULL if the current element is the last one
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DL_NODE_S* DL_Next(IN DL_NODE_S* pstNode)
{
    return pstNode->pstNext;
}

/*****************************************************************************
    Func Name: DL_Prev
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the previous element of current one
        Input: DL_NODE_S* pstNode  the pointer to current element
       Output: No output
       Return: the pointer to previous element of current element
               NULL if the current element is the first one
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DL_NODE_S* DL_Prev(IN DL_NODE_S* pstNode)
{
    return DL_NODE_FROM_PPRE(pstNode->ppstPre);
}

/*****************************************************************************
    Func Name: DL_Del
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete an element from the list
        Input: DL_NODE_S* pstNode  the pointer to the element to be deleted
       Output: No output
       Return: No return
      Caution: It takes the expense of O(1).
               This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_Del(IN DL_NODE_S* pstNode)
{
    if (NULL != pstNode->ppstPre)
    {
        *pstNode->ppstPre = pstNode->pstNext;
    }
    if (NULL != pstNode->pstNext)
    {
        pstNode->pstNext->ppstPre = pstNode->ppstPre;
    }

    return;
}

/*****************************************************************************
    Func Name: DL_AddHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element to the head of the list
        Input: DL_HEAD_S* pstList  the pointer to the list head
               DL_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_AddHead(IN DL_HEAD_S* pstList, IN DL_NODE_S* pstNode)
{
    pstNode->ppstPre = &pstList->pstFirst;
    pstNode->pstNext = pstList->pstFirst;
    if (NULL != pstNode->pstNext)
    {
        pstNode->pstNext->ppstPre = &pstNode->pstNext;
    }
    pstList->pstFirst = pstNode;
    return;
}

/*****************************************************************************
    Func Name: DL_DelHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete the first element from the list
        Input: DL_HEAD_S* pstList  the pointer to the list head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DL_NODE_S* DL_DelHead(IN DL_HEAD_S* pstList)
{
    DL_NODE_S* pstNode = DL_First(pstList);
    if (NULL != pstNode)
    {
        DL_Del (pstNode);
    }

    return pstNode;
}

/*****************************************************************************
    Func Name: DL_AddAfter
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element after another one
        Input: DL_NODE_S* pstPrev  pointer to the previous element
               DL_NODE_S* pstInst  pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_AddAfter(IN DL_NODE_S* pstPrev, IN DL_NODE_S* pstInst)
{
    pstInst->ppstPre = &pstPrev->pstNext;
    pstInst->pstNext = pstPrev->pstNext;
    pstPrev->pstNext = pstInst;
    if (NULL != pstInst->pstNext)
    {
        pstInst->pstNext->ppstPre = &pstInst->pstNext;
    }

    return;
}

/*****************************************************************************
    Func Name: DL_AddAfterPtr
 Date Created: 2008/8/6
       Author: c02254
  Description: Add an element after another one
        Input: DL_NODE_S** ppstPre  address of the previous element's pstNext
               DL_NODE_S*  pstInst  pointer to the element to be added
       Output: No output
       Return: No return
      Caution: ppstPre is not the address of a pointer to previous element's
               but is the address of previous element's pstNext.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_AddAfterPtr (IN DL_NODE_S **ppstPre, IN DL_NODE_S  *pstInst)
{
    pstInst->ppstPre = ppstPre;
    pstInst->pstNext = *ppstPre;
    *ppstPre = pstInst;
    if (NULL != pstInst->pstNext)
    {
        pstInst->pstNext->ppstPre = &pstInst->pstNext;
    }
    return;
}

/*****************************************************************************
    Func Name: DL_AddBefore
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element after another one
        Input: DL_HEAD_S* pstList  the pointer to the list head
               DL_NODE_S* pstPrev  pointer to the previous element
               DL_NODE_S* pstInst  pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_AddBefore(IN DL_NODE_S* pstNext, IN DL_NODE_S* pstInst)
{
    pstInst->ppstPre = pstNext->ppstPre;
    pstInst->pstNext = pstNext;
    if (NULL != pstInst->ppstPre)
    {
        *pstInst->ppstPre = pstInst;
    }
    pstInst->pstNext->ppstPre = &pstInst->pstNext;

    return;
}

/* macro for walk the doubly-linked list */
#define DL_FOREACH(pstList, pstNode) \
    for ((pstNode) = DL_First((pstList)); \
         NULL != (pstNode); \
         (pstNode) = DL_Next(pstNode))

#define DL_FOREACH_SAFE(pstList, pstNode, pstNext) \
    for ((pstNode) = DL_First((pstList)); \
        (NULL != (pstNode)) && ({(pstNext) = DL_Next(pstNode); BOOL_TRUE;}); \
        (pstNode) = (pstNext))

#define DL_FOREACH_PREVPTR(pstList, pstNode, ppstPre) \
    for ((pstNode) = DL_First(pstList), (ppstPre) = &((pstList)->pstFirst); \
         NULL != (pstNode); \
         (VOID)({(ppstPre) = &((pstNode)->pstNext); \
                  (pstNode) = DL_Next(pstNode);}))

#define DL_ENTRY_FIRST(pstList, type, member) \
    (DL_IsEmpty(pstList) ? NULL : DL_ENTRY(DL_First(pstList), type, member))

#define DL_ENTRY_NEXT(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
       (NULL == DL_Next(&((pstEntry)->member)) ? NULL : \
          DL_ENTRY(DL_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define DL_ENTRY_PREV(pstEntry, member) \
    (NULL == (pstEntry) ? NULL : \
       (NULL == DL_Prev(&((pstEntry)->member)) ? NULL : \
           DL_ENTRY(DL_Prev(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define DL_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         NULL != (pstEntry); \
         (pstEntry) = DL_ENTRY_NEXT(pstEntry, member))

#define DL_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = DL_ENTRY_NEXT(pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define DL_FOREACH_ENTRY_PREVPTR(pstList, pstEntry, ppstPre, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), \
          (ppstPre) = &((pstList)->pstFirst); \
          NULL != (pstEntry); \
          (VOID)({(ppstPre) = &((pstEntry)->member.pstNext); \
                   (pstEntry) = DL_ENTRY_NEXT(pstEntry, member);}))

/*****************************************************************************
    Func Name: DL_Append
 Date Created: 2008/3/28
       Author: c02254
  Description: Append pstSrcList to pstDstList
        Input: DL_HEAD_S* pstDstList  the list to be appended to
               DL_HEAD_S* pstSrcList  the list to be append
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DL_Append(IN DL_HEAD_S* pstDstList, INOUT DL_HEAD_S* pstSrcList)
{
    DL_NODE_S *pstNode, **ppstPre;

    if (BOOL_TRUE != DL_IsEmpty (pstSrcList))
    {
        /* seek pstPrev to the tail of pstDstList */
        DL_FOREACH_PREVPTR (pstDstList, pstNode, ppstPre)
        {
            ; /* do nothing */
        }
        
        *ppstPre = pstSrcList->pstFirst;
        pstSrcList->pstFirst->ppstPre = ppstPre;
        DL_Init(pstSrcList);
    }
    return;
}

/*****************************************************************************
    Func Name: DL_FreeAll
 Date Created: 2009/10/27 
       Author: Yang Yiquan
  Description: Remove and free all nodes from DL list
        Input: IN DL_HEAD_S *pstList      
                IN VOID (*pfFree)(VOID *)  
       Output: 
       Return: STATIC
      Caution: 
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
static inline VOID DL_FreeAll(IN DL_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    DL_NODE_S *pstCurNode;
    DL_NODE_S *pstNextNode;

    /* Free all node from list */
    DL_FOREACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    DL_Init(pstList);
    return;
}

/*
 * Doubly-linked Tail queue
 *
 * A doubly-linked tail queue is headed by a pair of pointers, one to the head
 * of the list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the list.
 * A doubly-linked tail queue may be traversed in either direction.
 +-------------------------------------------------------------------------+
 |                             +---------------+        +---------------+  |
 |                             |user structure |        |user structure |  |
 |    +---------------+        +---------------+        +---------------+  |
 |    |  DTQ_HEAD_S   |        |   ......      |        |   ......      |  |
 |    +---------------+        +---------------+        +---------------+  |
 | +->|+-------------+|<-+  +->|+-------------+|<-+  +->|+-------------+|<-+
 | |  || DTQ_NODE_S  ||  |  |  || DTQ_NODE_S  ||  |  |  || DTQ_NODE_S  ||
 | |  |+-------------+|  |  |  |+-------------+|  |  |  |+-------------+|
 +-C--|| pstPrev     ||  +--C--|| pstPrev     ||  +--C--|| pstPrev     ||
   |  |+-------------+|     |  |+-------------+|     |  |+-------------+|
   |  || pstNext     ||-----+  || pstNext     ||-----+  || pstNext     ||--+
   |  |+-------------+|        |+-------------+|        |+-------------+|  |
   |  +---------------+        +---------------+        +---------------+  |
   |                           |   ......      |        |   ......      |  |
   |                           +---------------+        +---------------+  |
   |-----------------------------------------------------------------------+
 */
typedef struct tagDTQ_NODE
{
    struct tagDTQ_NODE* pstPrev; /* the previous element */
    struct tagDTQ_NODE* pstNext; /* the next element */
} DTQ_NODE_S;
#define DTQ_ENTRY(ptr, type, member)    (container_of(ptr, type, member))

typedef struct tagDTQ_HEAD
{
    DTQ_NODE_S stHead;   /* stHead.pstNext is the head of list
                            stHead.pstPrev is the tail of list */
} DTQ_HEAD_S;

static inline VOID DTQ_Init(IN DTQ_HEAD_S* pstList);
static inline VOID DTQ_NodeInit(IN DTQ_NODE_S* pstNode);
static inline BOOL_T DTQ_IsEmpty(IN DTQ_HEAD_S* pstList);
static inline DTQ_NODE_S* DTQ_First(IN DTQ_HEAD_S* pstList);
static inline DTQ_NODE_S* DTQ_Last(IN DTQ_HEAD_S* pstList);
static inline BOOL_T DTQ_IsEndOfQ(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode);
static inline DTQ_NODE_S* DTQ_Prev(IN DTQ_NODE_S* pstNode);
static inline DTQ_NODE_S* DTQ_Next(IN DTQ_NODE_S* pstNode);
static inline VOID DTQ_AddAfter(IN DTQ_NODE_S* pstPrev, IN DTQ_NODE_S* pstInst);
static inline VOID DTQ_AddBefore(IN DTQ_NODE_S* pstNext, IN DTQ_NODE_S* pstInst);
static inline VOID DTQ_Del(IN DTQ_NODE_S* pstNode);
static inline VOID DTQ_AddHead(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode);
static inline DTQ_NODE_S* DTQ_DelHead(IN DTQ_HEAD_S* pstList);
static inline VOID DTQ_AddTail(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode);
static inline DTQ_NODE_S* DTQ_DelTail(IN DTQ_HEAD_S* pstList);
static inline VOID DTQ_Append(IN DTQ_HEAD_S* pstDstList, IN DTQ_HEAD_S* pstSrcList);
static inline VOID DTQ_FreeAll(IN DTQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));

/*****************************************************************************
    Func Name: DTQ_Init
 Date Created: 2008/3/28
       Author: c02254
  Description: Initial a doubly-linked tail queue head
        Input: STQ_HEAD_S* pstList  the doubly-linked tail queue head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_Init(IN DTQ_HEAD_S* pstList)
{
    pstList->stHead.pstNext = &pstList->stHead;
    pstList->stHead.pstPrev = &pstList->stHead;
    return;
}

/*****************************************************************************
    Func Name: DTQ_NodeInit
 Date Created: 2008/8/6
       Author: c02254
  Description: Initialize a doubly-linked tail queue node
        Input: IN DTQ_NODE_S* pstNode  pointer to node
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_NodeInit(IN DTQ_NODE_S* pstNode)
{
    pstNode->pstNext = (DTQ_NODE_S*)NULL;
    pstNode->pstPrev = (DTQ_NODE_S*)NULL;
    return;
}

/*****************************************************************************
    Func Name: DTQ_IsEmpty
 Date Created: 2008/3/28
       Author: c02254
  Description: Whether the doubly-linked tail queue is empty
        Input: STQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: BOOL_TRUE or BOOL_FLASE
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T DTQ_IsEmpty(IN DTQ_HEAD_S* pstList)
{
    return (pstList->stHead.pstNext == &pstList->stHead);
}

/*****************************************************************************
    Func Name: DTQ_First
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the first element of a doubly-linked tail queue
        Input: DTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to first element of the doubly-linked tail queue
               NULL if the list is empty
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* DTQ_First(IN DTQ_HEAD_S* pstList)
{
    if (DTQ_IsEmpty(pstList))
    {
        return (DTQ_NODE_S*)NULL;
    }
    return (pstList->stHead.pstNext);
}

/*****************************************************************************
    Func Name: DTQ_Last
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the last element of a doubly-linked tail queue
        Input: DTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to last element of the doubly-linked tail queue
               NULL if the list is empty
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* DTQ_Last(IN DTQ_HEAD_S* pstList)
{
    if (DTQ_IsEmpty(pstList))
    {
        return (DTQ_NODE_S*)NULL;
    }
    return (pstList->stHead.pstPrev);
}

/*****************************************************************************
    Func Name: DTQ_IsEndOfQ
 Date Created: 2008/8/6
       Author: c02254
  Description: Wether the node is the end of the queue
        Input: DTQ_HEAD_S* pstList  the pointer to the queue head
               DTQ_NODE_S* pstNode  the pointer to the node
       Output: No output
       Return: BOOL_TURE if is end of the queue, BOOL_FALSE is not
      Caution: DTQ is a circle doubly link list, we can not use NULL to
               decide wether reaching the end of list
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T DTQ_IsEndOfQ(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    if (DTQ_IsEmpty(pstList))
    {
        return BOOL_TRUE;
    }
    if (NULL == pstNode)
    {
        return BOOL_TRUE;
    }
    return (pstNode == &pstList->stHead);
}

/*****************************************************************************
    Func Name: DTQ_Prev
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the previous element of current one
        Input: DTQ_NODE_S* pstNode  the pointer to current element
       Output: No output
       Return: the pointer to previous element of current element
               NULL if the current element is the last one
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* DTQ_Prev(IN DTQ_NODE_S* pstNode)
{
    return (pstNode->pstPrev);
}

/*****************************************************************************
    Func Name: DTQ_Next
 Date Created: 2008/3/28
       Author: c02254
  Description: Get the next element of current one
        Input: DTQ_NODE_S* pstNode  the pointer to current element
       Output: No output
       Return: the pointer to next element of current element
               NULL if the current element is the last one
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* DTQ_Next(IN DTQ_NODE_S* pstNode)
{
    return (pstNode->pstNext);
}

/*****************************************************************************
    Func Name: DTQ_AddAfter
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element after another one
        Input: DTQ_NODE_S* pstPrev  pointer to the previous element
               DTQ_NODE_S* pstInst  pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_AddAfter(IN DTQ_NODE_S* pstPrev, IN DTQ_NODE_S* pstInst)
{
    pstInst->pstPrev = pstPrev;
    pstInst->pstNext = pstPrev->pstNext;
    pstPrev->pstNext = pstInst;
    pstInst->pstNext->pstPrev = pstInst;

    return;
}

/*****************************************************************************
    Func Name: DTQ_AddBefore
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element before another one
        Input: DTQ_NODE_S* pstNext  pointer to the next element
               DTQ_NODE_S* pstInst  pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_AddBefore(IN DTQ_NODE_S* pstNext, IN DTQ_NODE_S* pstInst)
{
    pstInst->pstPrev = pstNext->pstPrev;
    pstInst->pstNext = pstNext;
    pstInst->pstPrev->pstNext = pstInst;
    pstInst->pstNext->pstPrev = pstInst;

    return;
}

/*****************************************************************************
    Func Name: DTQ_Del
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete an element from the tail queue
        Input: DTQ_NODE_S* pstNode  the pointer to the element to be deleted
       Output: No output
       Return: No return
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_Del(IN DTQ_NODE_S* pstNode)
{
    pstNode->pstPrev->pstNext = pstNode->pstNext;
    pstNode->pstNext->pstPrev = pstNode->pstPrev;

    return;
}

/*****************************************************************************
    Func Name: DTQ_AddHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element to the head of the queue
        Input: DTQ_HEAD_S* pstList  the pointer to the queue head
               DTQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_AddHead(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    DTQ_AddAfter (&pstList->stHead, pstNode);

    return;
}

/*****************************************************************************
    Func Name: DTQ_DelHead
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete the first element from the queue
        Input: DTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* DTQ_DelHead(IN DTQ_HEAD_S* pstList)
{
    DTQ_NODE_S* pstNode = DTQ_First(pstList);

    if (DTQ_IsEndOfQ(pstList, pstNode))
    {
        pstNode = (DTQ_NODE_S*)NULL;
    }
    else
    {
        DTQ_Del (pstNode);
    }

    return pstNode;
}

/*****************************************************************************
    Func Name: DTQ_AddTail
 Date Created: 2008/3/28
       Author: c02254
  Description: Add an element to the tail of the queue
        Input: DTQ_HEAD_S* pstList  the pointer to the queue head
               DTQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_AddTail(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    DTQ_AddBefore (&pstList->stHead, pstNode);
    return;
}

/*****************************************************************************
    Func Name: DTQ_DelTail
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete the last element from the queue
        Input: DTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* DTQ_DelTail(IN DTQ_HEAD_S* pstList)
{
    DTQ_NODE_S* pstNode = DTQ_Last (pstList);

    if (DTQ_IsEndOfQ(pstList, pstNode))
    {
        pstNode = (DTQ_NODE_S*)NULL;
    }
    else
    {
        DTQ_Del (pstNode);
    }

    return pstNode;
}

/* macro for walk the doubly-linked tail queue */
#define DTQ_FOREACH(pstList, pstNode) \
    for ((pstNode) = DTQ_First(pstList); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)); \
         (pstNode) = DTQ_Next(pstNode))

#define DTQ_FOREACH_SAFE(pstList, pstNode, pstNext) \
    for ((pstNode) = DTQ_First((pstList)); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)) && \
         ({(pstNext) = DTQ_Next(pstNode); BOOL_TRUE;}); \
         (pstNode) = (pstNext))

#define DTQ_FOREACH_REVERSE(pstList, pstNode) \
    for ((pstNode) = DTQ_Last(pstList); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)); \
         (pstNode) = DTQ_Prev(pstNode))

#define DTQ_FOREACH_REVERSE_SAFE(pstList, pstNode, pstPrev) \
    for ((pstNode) = DTQ_Last(pstList); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)) && \
         ({(pstPrev) = DTQ_Prev(pstNode); BOOL_TRUE;}); \
         (pstNode) = (pstPrev))

#define DTQ_ENTRY_FIRST(pstList, type, member) \
    (DTQ_IsEmpty(pstList) ? NULL : DTQ_ENTRY(DTQ_First(pstList), type, member))

#define DTQ_ENTRY_LAST(pstList, type, member) \
    (DTQ_IsEmpty(pstList) ? NULL : DTQ_ENTRY(DTQ_Last(pstList), type, member))

#define DTQ_ENTRY_NEXT(pstList, pstEntry, member) \
    (DTQ_IsEndOfQ(pstList, (NULL == (pstEntry) ? NULL : DTQ_Next(&((pstEntry)->member)))) ? \
        NULL : \
        DTQ_ENTRY(DTQ_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member))

#define DTQ_ENTRY_PREV(pstList, pstEntry, member) \
    (DTQ_IsEndOfQ(pstList, (NULL == (pstEntry) ? NULL : DTQ_Prev(&((pstEntry)->member)))) ? \
        NULL : \
        DTQ_ENTRY(DTQ_Prev(&((pstEntry)->member)), typeof(*(pstEntry)), member))

#define DTQ_FOREACH_ENTRY(pstList, pstEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         NULL != (pstEntry); \
         (pstEntry) = DTQ_ENTRY_NEXT(pstList, pstEntry, member))

#define DTQ_FOREACH_ENTRY_SAFE(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = DTQ_ENTRY_NEXT(pstList, pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define DTQ_FOREACH_ENTRY_REVERSE(pstList, pstEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_LAST(pstList, typeof(*(pstEntry)), member); \
         NULL != (pstEntry); \
         (pstEntry) = DTQ_ENTRY_PREV(pstList, pstEntry, member))

#define DTQ_FOREACH_ENTRY_REVERSE_SAFE(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_LAST(pstList, typeof(*(pstEntry)), member); \
         (NULL != (pstEntry)) && \
         ({(pstPrevEntry) = DTQ_ENTRY_PREV(pstList, pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = (pstPrevEntry))

/*****************************************************************************
    Func Name: DTQ_Append
 Date Created: 2008/3/28
       Author: c02254
  Description: Append pstSrcList to pstDstList
        Input: DTQ_HEAD_S* pstDstList  the list to be appended to
               DTQ_HEAD_S* pstSrcList  the list to be append
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID DTQ_Append(IN DTQ_HEAD_S* pstDstList, INOUT DTQ_HEAD_S* pstSrcList)
{
    if (BOOL_TRUE != DTQ_IsEmpty (pstSrcList))
    {
        pstSrcList->stHead.pstNext->pstPrev = pstDstList->stHead.pstPrev;
        pstSrcList->stHead.pstPrev->pstNext = pstDstList->stHead.pstPrev->pstNext;
        pstDstList->stHead.pstPrev->pstNext = pstSrcList->stHead.pstNext;
        pstDstList->stHead.pstPrev = pstSrcList->stHead.pstPrev;
        DTQ_Init(pstSrcList);
    }
    return;
}

/*****************************************************************************
    Func Name: DTQ_FreeAll
 Date Created: 2009/10/27 
       Author: Yang Yiquan
  Description: Remove and free all nodes from DTQ list
        Input: IN DTQ_HEAD_S *pstList      
                IN VOID (*pfFree)(VOID *)  
       Output: 
       Return: STATIC
      Caution: 
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
static inline VOID DTQ_FreeAll(IN DTQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    DTQ_NODE_S *pstCurNode;
    DTQ_NODE_S *pstNextNode;

    /* Free all node from list */
    DTQ_FOREACH_SAFE(pstList, pstCurNode, pstNextNode)
    {
        pfFree(pstCurNode);
    }

    DTQ_Init(pstList);
    return;
}

/* Rcu supporting, used in kernel only */
#ifdef __KERNEL__
#include <sys/rcu.h>

static inline VOID SL_AddHead_Rcu(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode)
{
    pstNode->pstNext  = pstList->pstFirst;
    smp_wmb();
    pstList->pstFirst = pstNode;
    return;
}

static inline VOID SL_AddAfter_Rcu(IN SL_HEAD_S* pstList,
                               IN SL_NODE_S* pstPrev,
                               IN SL_NODE_S* pstInst)
{
    if (NULL == pstPrev)
    {
        SL_AddHead (pstList, pstInst);
    }
    else
    {
        pstInst->pstNext = pstPrev->pstNext;
        smp_wmb();
        pstPrev->pstNext = pstInst;
    }
    return;
}

#define SL_FOREACH_RCU(pstList, pstNode) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst); \
         (NULL != (pstNode)) && ({Prefetch((pstNode)->pstNext); BOOL_TRUE;}); \
         (pstNode) = RCU_DeRef((pstNode)->pstNext))

#define SL_FOREACH_SAFE_RCU(pstList, pstNode, pstNextNode) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst); \
         (NULL != (pstNode)) && ({(pstNextNode) = RCU_DeRef((pstNode)->pstNext); BOOL_TRUE;}); \
         (pstNode) = (pstNextNode))

#define SL_FOREACH_PREVPTR_RCU(pstList, pstNode, pstPrev) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst), (pstPrev) = (SL_NODE_S *)NULL; \
         (NULL != (pstNode)) && ({Prefetch((pstNode)->pstNext); BOOL_TRUE;}); \
         (VOID)({(pstPrev) = (pstNode); (pstNode) = RCU_DeRef((pstNode)->pstNext);}))

#define SL_ENTRY_NEXT_Prefetch(pstEntry, member) \
    ((NULL == SL_Next(&((pstEntry)->member))) ? NULL : \
           Prefetch(SL_ENTRY(SL_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define SL_FOREACH_ENTRY_RCU(pstList, pstEntry, member) \
        for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
              (NULL != (pstEntry)) && ({SL_ENTRY_NEXT_Prefetch(pstEntry, member); BOOL_TRUE;}); \
             (pstEntry) = SL_ENTRY_NEXT(pstEntry, member), (pstEntry) = RCU_DeRef(pstEntry))
    
#define SL_FOREACH_ENTRY_SAFE_RCU(pstList, pstEntry, pstNextEntry, member) \
        for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
             (NULL != (pstEntry)) && \
             ({(pstNextEntry) = SL_ENTRY_NEXT(pstEntry, member), (pstNextEntry) = RCU_DeRef(pstNextEntry); BOOL_TRUE;}); \
             (pstEntry) = (pstNextEntry))
    
#define SL_FOREACH_ENTRY_PREVPTR_RCU(pstList, pstEntry, pstPrevEntry, member) \
        for ((pstEntry) = SL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), \
                (pstEntry) = RCU_DeRef(pstEntry), (pstPrevEntry) = NULL; \
              (NULL != (pstEntry)) && ({SL_ENTRY_NEXT_Prefetch(pstEntry, member); BOOL_TRUE;}); \
              (VOID)({(pstPrevEntry) = (pstEntry); \
                       (pstEntry) = SL_ENTRY_NEXT(pstEntry, member), (pstEntry) = RCU_DeRef(pstEntry);}))

static inline VOID SL_Del_Rcu(IN SL_HEAD_S* pstList, IN SL_NODE_S* pstNode)
{
    SL_NODE_S *pstCur, *pstPrev;

    SL_FOREACH_PREVPTR_RCU(pstList, pstCur, pstPrev)
    {
        if (pstCur == pstNode)
        {
            (VOID) SL_DelAfter(pstList, pstPrev);
            break;
        }
    }
    return;
}

static inline VOID SL_Append_Rcu(IN SL_HEAD_S* pstDstList, INOUT SL_HEAD_S* pstSrcList)
{
    SL_NODE_S *pstNode, *pstPrev;

    if (BOOL_TRUE != SL_IsEmpty (pstSrcList))
    {
        SL_FOREACH_PREVPTR_RCU(pstDstList, pstNode, pstPrev)
        {
            ; /* do nothing */
        }
        
        if (NULL == pstPrev)
        {
            pstDstList->pstFirst = SL_First(pstSrcList);
        }
        else
        {
            pstPrev->pstNext = SL_First(pstSrcList);
        }

        SL_Init(pstSrcList);
    }
    
    return;
}

static inline VOID STQ_AddHead_Rcu(IN STQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    pstNode->pstNext = pstList->pstFirst;
    smp_wmb();
    pstList->pstFirst = pstNode;
    if (NULL == pstList->pstLast)
    {
        pstList->pstLast = pstNode;
    }
    return;
}

static inline VOID STQ_AddTail_Rcu(STQ_HEAD_S* pstList, STQ_NODE_S* pstNode)
{
    pstNode->pstNext = (STQ_NODE_S *)NULL;
    if (NULL != pstList->pstLast)
    {
        pstList->pstLast->pstNext = pstNode;
        smp_wmb();
        pstList->pstLast = pstNode;
    }
    else
    {
        pstList->pstLast  = pstNode;
        pstList->pstFirst = pstNode;
    }
    return;
}

static inline VOID STQ_AddAfter_Rcu(IN STQ_HEAD_S* pstList,
                                IN STQ_NODE_S* pstPrev,
                                IN STQ_NODE_S* pstInst)
{
    if (NULL == pstPrev)
    {
        STQ_AddHead_Rcu(pstList, pstInst);
    }
    else
    {
        pstInst->pstNext = pstPrev->pstNext;
        smp_wmb();
        pstPrev->pstNext = pstInst;
        if (pstList->pstLast == pstPrev)
        {
            pstList->pstLast = pstInst;
        }
    }
    return;
}

#define STQ_FOREACH_RCU(pstList, pstNode) \
    for((pstNode) = RCU_DeRef((pstList)->pstFirst); \
        (NULL != (pstNode)) && ({Prefetch((pstNode)->pstNext); BOOL_TRUE;}); \
        (pstNode) = RCU_DeRef((pstNode)->pstNext))

#define STQ_FOREACH_SAFE_RCU(pstList, pstNode, pstNextNode) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst); \
         (NULL != (pstNode)) && ({(pstNextNode) = RCU_DeRef((pstNode)->pstNext); BOOL_TRUE;}); \
         (pstNode) = (pstNextNode))

#define STQ_FOREACH_PREVPTR_RCU(pstList, pstNode, pstPrev) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst), (pstPrev) = NULL; \
         (NULL != (pstNode)) && ({Prefetch((pstNode)->pstNext); BOOL_TRUE;}); \
         (VOID)({(pstPrev) = (pstNode); (pstNode) = RCU_DeRef((pstNode)->pstNext);}))

#define STQ_ENTRY_NEXT_Prefetch(pstEntry, member) \
           (NULL == STQ_Next(&((pstEntry)->member)) ? NULL : \
              Prefetch(STQ_ENTRY(STQ_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define STQ_FOREACH_ENTRY_RCU(pstList, pstEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), pstEntry = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && ({STQ_ENTRY_NEXT_Prefetch(pstEntry, member), BOOL_TRUE;}); \
         (pstEntry) = STQ_ENTRY_NEXT(pstEntry, member), pstEntry = RCU_DeRef(pstEntry))

#define STQ_FOREACH_ENTRY_SAFE_RCU(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), pstEntry = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = STQ_ENTRY_NEXT(pstEntry, member), pstNextEntry = RCU_DeRef(pstNextEntry); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define STQ_FOREACH_ENTRY_PREVPTR_RCU(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = STQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member),     \
            pstEntry = RCU_DeRef(pstEntry), (pstPrevEntry) = NULL; \
          (NULL != (pstEntry)) && ({STQ_ENTRY_NEXT_Prefetch(pstEntry, member); BOOL_TRUE;}); \
          (VOID)({(pstPrevEntry) = (pstEntry); \
                   (pstEntry) = STQ_ENTRY_NEXT(pstEntry, member), pstEntry = RCU_DeRef(pstEntry);}))

static inline VOID STQ_Del_Rcu(IN STQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    STQ_NODE_S *pstPrev, *pstCur;

    STQ_FOREACH_PREVPTR_RCU(pstList, pstCur, pstPrev)
    {
        if (pstCur == pstNode)
        {
            (VOID)STQ_DelAfter (pstList, pstPrev);
            break;
        }
    }
    return;
}

static inline VOID DL_AddHead_Rcu(IN DL_HEAD_S* pstList, IN DL_NODE_S* pstNode)
{
    pstNode->ppstPre = &pstList->pstFirst;
    pstNode->pstNext = pstList->pstFirst;
    smp_wmb();
    if (NULL != pstNode->pstNext)
    {
        pstNode->pstNext->ppstPre = &pstNode->pstNext;
    }
    pstList->pstFirst = pstNode;
    return;
}

static inline VOID DL_AddAfter_Rcu(IN DL_NODE_S* pstPrev, IN DL_NODE_S* pstInst)
{
    pstInst->ppstPre = &pstPrev->pstNext;
    pstInst->pstNext = pstPrev->pstNext;
    smp_wmb();
    pstPrev->pstNext = pstInst;
    if (NULL != pstInst->pstNext)
    {
        pstInst->pstNext->ppstPre = &pstInst->pstNext;
    }

    return;
}

static inline VOID DL_AddAfterPtr_Rcu(IN DL_NODE_S **ppstPre, IN DL_NODE_S  *pstInst)
{
    pstInst->ppstPre = ppstPre;
    pstInst->pstNext = *ppstPre;
    smp_wmb();
    *ppstPre = pstInst;
    if (NULL != pstInst->pstNext)
    {
        pstInst->pstNext->ppstPre = &pstInst->pstNext;
    }
    return;
}

static inline VOID DL_AddBefore_Rcu(IN DL_NODE_S* pstNext, IN DL_NODE_S* pstInst)
{
    pstInst->ppstPre = pstNext->ppstPre;
    pstInst->pstNext = pstNext;
    smp_wmb();
    pstNext->ppstPre = &pstInst->pstNext;
    if (NULL != pstInst->ppstPre)
    {
        *pstInst->ppstPre = pstInst;
    }    

    return;
}


#define DL_FOREACH_RCU(pstList, pstNode) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst); \
         (NULL != (pstNode)) && ({Prefetch(pstNode->pstNext); BOOL_TRUE;}); \
         (pstNode) = RCU_DeRef(pstNode->pstNext))

#define DL_FOREACH_SAFE_RCU(pstList, pstNode, pstNextNode) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst); \
        (NULL != (pstNode)) && ({(pstNextNode) = RCU_DeRef((pstNode)->pstNext); BOOL_TRUE;}); \
        (pstNode) = (pstNextNode))

#define DL_FOREACH_PREVPTR_RCU(pstList, pstNode, ppstPre) \
    for ((pstNode) = RCU_DeRef((pstList)->pstFirst), (ppstPre) = &((pstList)->pstFirst); \
         (NULL != (pstNode)) && ({Prefetch((pstNode)->pstNext); BOOL_TRUE;}); \
         (VOID)({(ppstPre) = &((pstNode)->pstNext); \
                  (pstNode) = RCU_DeRef((pstNode)->pstNext);}))

#define DL_ENTRY_NEXT_Prefetch(pstEntry, member) \
        (NULL == DL_Next(&((pstEntry)->member)) ? NULL : \
              Prefetch(DL_ENTRY(DL_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define DL_FOREACH_ENTRY_RCU(pstList, pstEntry, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && ({DL_ENTRY_NEXT_Prefetch(pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = DL_ENTRY_NEXT(pstEntry, member), (pstEntry) = RCU_DeRef(pstEntry))

#define DL_FOREACH_ENTRY_SAFE_RCU(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = DL_ENTRY_NEXT(pstEntry, member), (pstNextEntry) = RCU_DeRef(pstNextEntry); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define DL_FOREACH_ENTRY_PREVPTR_RCU(pstList, pstEntry, ppstPre, member) \
    for ((pstEntry) = DL_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry), \
          (ppstPre) = &((pstList)->pstFirst); \
          (NULL != (pstEntry)) && ({DL_ENTRY_NEXT_Prefetch(pstEntry, member); BOOL_TRUE;}); \
          (VOID)({(ppstPre) = &((pstEntry)->member.pstNext); \
                   (pstEntry) = DL_ENTRY_NEXT(pstEntry, member), (pstEntry) = RCU_DeRef(pstEntry);}))

static inline VOID DL_Append_Rcu(IN DL_HEAD_S* pstDstList, INOUT DL_HEAD_S* pstSrcList)
{
    DL_NODE_S *pstNode, **ppstPre;

    if (BOOL_TRUE != DL_IsEmpty (pstSrcList))
    {
        /* seek pstPrev to the tail of pstDstList */
        DL_FOREACH_PREVPTR_RCU(pstDstList, pstNode, ppstPre)
        {
            ; /* do nothing */
        }
        
        *ppstPre = pstSrcList->pstFirst;
        pstSrcList->pstFirst->ppstPre = ppstPre;
        DL_Init(pstSrcList);
    }
    
    return;
}

static inline VOID DTQ_AddAfter_Rcu(IN DTQ_NODE_S* pstPrev, IN DTQ_NODE_S* pstInst)
{
    pstInst->pstPrev = pstPrev;
    pstInst->pstNext = pstPrev->pstNext;
    smp_wmb();
    pstPrev->pstNext = pstInst;
    pstInst->pstNext->pstPrev = pstInst;

    return;
}

static inline VOID DTQ_AddBefore_Rcu(IN DTQ_NODE_S* pstNext, IN DTQ_NODE_S* pstInst)
{
    pstInst->pstPrev = pstNext->pstPrev;
    pstInst->pstNext = pstNext;
    smp_wmb();
    pstInst->pstPrev->pstNext = pstInst;
    pstInst->pstNext->pstPrev = pstInst;

    return;
}

static inline VOID DTQ_AddHead_Rcu(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    DTQ_AddAfter_Rcu(&pstList->stHead, pstNode);

    return;
}

static inline VOID DTQ_AddTail_Rcu(IN DTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    DTQ_AddBefore_Rcu(&pstList->stHead, pstNode);
    return;
}

#define DTQ_FOREACH_RCU(pstList, pstNode) \
    for ((pstNode) = RCU_DeRef((pstList)->stHead.pstNext); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)) && ({Prefetch((pstNode)->pstNext);BOOL_TRUE;}); \
         (pstNode) = RCU_DeRef((pstNode)->pstNext))

#define DTQ_FOREACH_SAFE_RCU(pstList, pstNode, pstNextNode) \
    for ((pstNode) = RCU_DeRef((pstList)->stHead.pstNext); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)) && \
         ({(pstNextNode) = RCU_DeRef((pstNode)->pstNext); BOOL_TRUE;}); \
         (pstNode) = (pstNextNode))

#define DTQ_FOREACH_REVERSE_RCU(pstList, pstNode) \
    for ((pstNode) = RCU_DeRef((pstList)->stHead.pstPrev); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)) && ({Prefetch((pstNode)->pstPrev);BOOL_TRUE;}); \
         (pstNode) = RCU_DeRef((pstNode)->pstPrev))

#define DTQ_FOREACH_REVERSE_SAFE_RCU(pstList, pstNode, pstPrev) \
    for ((pstNode) = RCU_DeRef((pstList)->stHead.pstPrev); \
         (BOOL_TRUE != DTQ_IsEndOfQ(pstList, pstNode)) && \
         ({(pstPrev) = RCU_DeRef((pstNode)->pstPrev); BOOL_TRUE;}); \
         (pstNode) = (pstPrev))

#define DTQ_ENTRY_NEXT_Prefetch(pstList, pstEntry, member) \
        (DTQ_IsEndOfQ(pstList, DTQ_Next(&((pstEntry)->member))) ? \
            NULL : Prefetch(DTQ_ENTRY(DTQ_Next(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define DTQ_FOREACH_ENTRY_RCU(pstList, pstEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && ({DTQ_ENTRY_NEXT_Prefetch(pstList, pstEntry, member); BOOL_TRUE;}); \
         (pstEntry) = DTQ_ENTRY_NEXT(pstList, pstEntry, member), (pstEntry) = RCU_DeRef(pstEntry))

#define DTQ_FOREACH_ENTRY_SAFE_RCU(pstList, pstEntry, pstNextEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_FIRST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && \
         ({(pstNextEntry) = DTQ_ENTRY_NEXT(pstList, pstEntry, member),  \
            (pstNextEntry) = RCU_DeRef(pstNextEntry); BOOL_TRUE;}); \
         (pstEntry) = (pstNextEntry))

#define DTQ_ENTRY_PREV_Prefetch(pstList, pstEntry, member) \
        (DTQ_IsEndOfQ(pstList, DTQ_Prev(&((pstEntry)->member))) ? \
            NULL : Prefetch(DTQ_ENTRY(DTQ_Prev(&((pstEntry)->member)), typeof(*(pstEntry)), member)))

#define DTQ_FOREACH_ENTRY_REVERSE_RCU(pstList, pstEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_LAST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && ({DTQ_ENTRY_PREV_Prefetch(pstList, pstEntry, member); BOOL_TRUE;})\
         (pstEntry) = DTQ_ENTRY_PREV(pstList, pstEntry, member), (pstEntry) = RCU_DeRef(pstEntry))

#define DTQ_FOREACH_ENTRY_REVERSE_SAFE_RCU(pstList, pstEntry, pstPrevEntry, member) \
    for ((pstEntry) = DTQ_ENTRY_LAST(pstList, typeof(*(pstEntry)), member), (pstEntry) = RCU_DeRef(pstEntry); \
         (NULL != (pstEntry)) && \
         ({(pstPrevEntry) = DTQ_ENTRY_PREV(pstList, pstEntry, member),  \
            (pstPrevEntry) = RCU_DeRef(pstPrevEntry); BOOL_TRUE;}); \
         (pstEntry) = (pstPrevEntry))

static inline VOID DTQ_Append_Rcu(IN DTQ_HEAD_S* pstDstList, INOUT DTQ_HEAD_S* pstSrcList)
{
    if (BOOL_TRUE != DTQ_IsEmpty(pstSrcList))
    {
        pstSrcList->stHead.pstNext->pstPrev = pstDstList->stHead.pstPrev;
        pstSrcList->stHead.pstPrev->pstNext = pstDstList->stHead.pstPrev->pstNext;
        smp_wmb();
        pstDstList->stHead.pstPrev->pstNext = pstSrcList->stHead.pstNext;
        pstDstList->stHead.pstPrev = pstSrcList->stHead.pstPrev;

        DTQ_Init(pstSrcList);
    }
    
    return;
}
#else /* USER SPACE */
#include <pthread.h>
    
typedef struct tagRWSTQ_HEAD
{
    STQ_HEAD_S stHead;
    pthread_rwlock_t stRwLock;
} RWSTQ_HEAD_S;

static inline VOID RWSTQ_ReadLock (IN RWSTQ_HEAD_S* pstNode);
static inline BOOL_T RWSTQ_ReadTryLock (IN RWSTQ_HEAD_S* pstNode);
static inline VOID RWSTQ_ReadUnlock (IN RWSTQ_HEAD_S* pstNode);
static inline VOID RWSTQ_WriteLock (IN RWSTQ_HEAD_S* pstNode);
static inline BOOL_T RWSTQ_WriteTryLock (IN RWSTQ_HEAD_S* pstNode);
static inline VOID RWSTQ_WriteUnlock (IN RWSTQ_HEAD_S* pstNode);
static inline VOID RWSTQ_DeInit (IN RWSTQ_HEAD_S* pstNode);
static inline VOID RWSTQ_Init(IN RWSTQ_HEAD_S* pstList);
static inline BOOL_T RWSTQ_IsEmpty(IN RWSTQ_HEAD_S* pstList);
static inline VOID RWSTQ_AddHead(IN RWSTQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode);
static inline STQ_NODE_S* RWSTQ_DelHead(IN RWSTQ_HEAD_S* pstList);
static inline VOID RWSTQ_AddTail(IN RWSTQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode);
static inline VOID RWSTQ_Del(IN RWSTQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode);
static inline VOID RWSTQ_FreeAll(IN RWSTQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));
/*****************************************************************************
    Func Name: RWSTQ_ReadLock
 Date Created: 2010/12/03
       Author: s07542
  Description: lock RWSTQ for reading
        Input: RWSTQ_HEAD_S* pstNode  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_ReadLock (IN RWSTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_rdlock(&pstNode->stRwLock); 
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_ReadTryLock
 Date Created: 2010/12/03
       Author: s07542
  Description: try lock RWSTQ for reading
        Input: RWSTQ_HEAD_S* pstNode  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T RWSTQ_ReadTryLock (IN RWSTQ_HEAD_S* pstNode)
{
    return (0 == pthread_rwlock_tryrdlock(&pstNode->stRwLock));
}

/*****************************************************************************
    Func Name: RWSTQ_ReadUnlock
 Date Created: 2010/12/03
       Author: s07542
  Description: unlock RWSTQ 
        Input: RWSTQ_HEAD_S* pstNode  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_ReadUnlock (IN RWSTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_unlock(&pstNode->stRwLock);
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_WriteLock
 Date Created: 2010/12/03
       Author: s07542
  Description: lock RWSTQ for write
        Input: RWSTQ_HEAD_S* pstNode  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_WriteLock (IN RWSTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_wrlock(&pstNode->stRwLock); 
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_WriteTryLock
 Date Created: 2010/12/03
       Author: s07542
  Description: try lock RWSTQ for write
        Input: RWSTQ_HEAD_S* pstNode  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T RWSTQ_WriteTryLock (IN RWSTQ_HEAD_S* pstNode)
{
    return (0 == pthread_rwlock_trywrlock(&pstNode->stRwLock));
}

/*****************************************************************************
    Func Name: RWSTQ_WriteUnlock
 Date Created: 2010/12/03
       Author: s07542
  Description: unlock RWSTQ for write
        Input: RWSTQ_HEAD_S* pstNode  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_WriteUnlock (IN RWSTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_unlock(&pstNode->stRwLock);
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_DeInit
 Date Created: 2010/12/03
       Author: s07542
  Description: Destroy RWSTQ  lock
        Input: RWSTQ_HEAD_S* pstNode  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_DeInit (IN RWSTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_destroy(&pstNode->stRwLock);
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_Init
 Date Created: 2010/12/03
       Author: s07542
  Description: RWSTQ init
        Input: RWSTQ_HEAD_S* pstList  RWSTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_Init(IN RWSTQ_HEAD_S* pstList)
{
    STQ_Init(&pstList->stHead);
    (VOID)pthread_rwlock_init(&pstList->stRwLock,NULL);
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_IsEmpty
 Date Created: 2010/12/03
       Author: s07542
  Description: Whether the singly-linked tail queue is empty
        Input: RWSTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: BOOL_TRUE or BOOL_FLASE
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T RWSTQ_IsEmpty(IN RWSTQ_HEAD_S* pstList)
{
    BOOL_T bIsemp;
    RWSTQ_ReadLock(pstList);
    bIsemp = STQ_IsEmpty(&pstList->stHead);
    RWSTQ_ReadUnlock(pstList);
    return bIsemp;
}

/*****************************************************************************
    Func Name: RWSTQ_AddHead
 Date Created: 2010/12/03
       Author: s07542
  Description: Add an element to the head of the queue
        Input: RWSTQ_HEAD_S* pstList  the pointer to the queue head
               STQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_AddHead(IN RWSTQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    RWSTQ_WriteLock(pstList);
    STQ_AddHead(&pstList->stHead, pstNode);
    RWSTQ_WriteUnlock(pstList);
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_DelHead
 Date Created: 2010/12/03
       Author: s07542
  Description: Delete the first element from the queue
        Input: RWSTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline STQ_NODE_S* RWSTQ_DelHead(IN RWSTQ_HEAD_S* pstList)
{
    STQ_NODE_S *pstNode = NULL;
    RWSTQ_WriteLock(pstList);
    pstNode = STQ_DelHead(&pstList->stHead);
    RWSTQ_WriteUnlock(pstList);
    return pstNode;
}

/*****************************************************************************
    Func Name: RWSTQ_AddTail
 Date Created: 2010/12/03
       Author: s07542
  Description: Add an element to the tail of the queue
        Input: RWSTQ_HEAD_S* pstList  the pointer to the queue tail
               STQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_AddTail(IN RWSTQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    RWSTQ_WriteLock(pstList);
    STQ_AddTail(&pstList->stHead, pstNode);
    RWSTQ_WriteUnlock(pstList);
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_Del
 Date Created: 2010/12/03
       Author: s07542
  Description: Delete an element from the tail queue
        Input: RWSTQ_HEAD_S* pstList  the pointer to the queue head
               STQ_NODE_S* pstNode  the pointer to the element to be deleted
       Output: No output
       Return: No return
      Caution: It takes the expense of O(n). If you have got a pointer to the
               previous element, you'd better use STQ_DelAfter whitch takes
               the expense of O(1).
               This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWSTQ_Del(IN RWSTQ_HEAD_S* pstList, IN STQ_NODE_S* pstNode)
{
    RWSTQ_WriteLock(pstList);
    STQ_Del(&pstList->stHead, pstNode);
    RWSTQ_WriteUnlock(pstList);
    return;
}

/*****************************************************************************
    Func Name: RWSTQ_FreeAll
 Date Created: 2010/12/03
       Author: s07542
  Description: Remove and free all nodes from STQ list
        Input: IN STQ_HEAD_S *pstList      
               IN VOID (*pfFree)(VOID *)  
       Output: 
       Return: STATIC
      Caution: 
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
static inline VOID RWSTQ_FreeAll(IN RWSTQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    RWSTQ_WriteLock(pstList);
    STQ_FreeAll(&pstList->stHead, pfFree);
    RWSTQ_WriteUnlock(pstList);
    return;
}

typedef struct tagRWDTQ_HEAD
{
    DTQ_HEAD_S stHead;   
    pthread_rwlock_t stRwLock;
} RWDTQ_HEAD_S;

static inline VOID RWDTQ_ReadLock (IN RWDTQ_HEAD_S* pstNode);
static inline BOOL_T RWDTQ_ReadTryLock (IN RWDTQ_HEAD_S* pstNode);
static inline VOID RWDTQ_ReadUnlock (IN RWDTQ_HEAD_S* pstNode);
static inline VOID RWDTQ_WriteLock (IN RWDTQ_HEAD_S* pstNode);
static inline BOOL_T RWDTQ_WriteTryLock (IN RWDTQ_HEAD_S* pstNode);
static inline VOID RWDTQ_WriteUnlock (IN RWDTQ_HEAD_S* pstNode);
static inline VOID RWDTQ_DeInit (IN RWDTQ_HEAD_S* pstNode);
static inline VOID RWDTQ_Init(IN RWDTQ_HEAD_S* pstList);
static inline BOOL_T RWDTQ_IsEmpty(IN RWDTQ_HEAD_S* pstList);
static inline BOOL_T RWDTQ_IsEndOfQ(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode);
static inline VOID RWDTQ_AddHead(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode);
static inline DTQ_NODE_S* RWDTQ_DelHead(IN RWDTQ_HEAD_S* pstList);
static inline VOID RWDTQ_AddTail(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode);
static inline DTQ_NODE_S* RWDTQ_DelTail(IN RWDTQ_HEAD_S* pstList);
static inline VOID RWDTQ_FreeAll(IN RWDTQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *));
static inline VOID RWDTQ_Del(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode);
/*****************************************************************************
    Func Name: RWDTQ_ReadLock
 Date Created: 2010/12/03
       Author: s07542
  Description: lock RWDTQ for reading
        Input: RWDTQ_HEAD_S* pstNode  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_ReadLock (IN RWDTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_rdlock(&pstNode->stRwLock); 
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_ReadTryLock
 Date Created: 2010/12/03
       Author: s07542
  Description: try lock RWDTQ for reading
        Input: RWDTQ_HEAD_S* pstNode  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T RWDTQ_ReadTryLock (IN RWDTQ_HEAD_S* pstNode)
{
    return (0 == pthread_rwlock_tryrdlock(&pstNode->stRwLock));
}

/*****************************************************************************
    Func Name: RWDTQ_ReadUnlock
 Date Created: 2010/12/03
       Author: s07542
  Description: unlock RWDTQ 
        Input: RWDTQ_HEAD_S* pstNode  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_ReadUnlock (IN RWDTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_unlock(&pstNode->stRwLock);
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_WriteLock
 Date Created: 2010/12/03
       Author: s07542
  Description: lock RWDTQ for write
        Input: RWDTQ_HEAD_S* pstNode  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_WriteLock (IN RWDTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_wrlock(&pstNode->stRwLock); 
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_WriteTryLock
 Date Created: 2010/12/03
       Author: s07542
  Description: try lock RWDTQ for write
        Input: RWDTQ_HEAD_S* pstNode  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T RWDTQ_WriteTryLock (IN RWDTQ_HEAD_S* pstNode)
{
    return (0 == pthread_rwlock_trywrlock(&pstNode->stRwLock));
}

/*****************************************************************************
    Func Name: RWDTQ_WriteUnlock
 Date Created: 2010/12/03
       Author: s07542
  Description: unlock RWDTQ for write
        Input: RWDTQ_HEAD_S* pstNode  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_WriteUnlock (IN RWDTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_unlock(&pstNode->stRwLock);
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_DeInit
 Date Created: 2010/12/03
       Author: s07542
  Description: Destroy RWDTQ  lock
        Input: RWDTQ_HEAD_S* pstNode  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_DeInit (IN RWDTQ_HEAD_S* pstNode)
{
    (VOID)pthread_rwlock_destroy(&pstNode->stRwLock);
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_Init
 Date Created: 2010/12/03
       Author: s07542
  Description: RWDTQ init
        Input: RWDTQ_HEAD_S* pstList  RWDTQ head
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_Init(IN RWDTQ_HEAD_S* pstList)
{
    DTQ_Init(&pstList->stHead);
    (VOID)pthread_rwlock_init(&pstList->stRwLock,NULL);
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_IsEmpty
 Date Created: 2010/12/03
       Author: s07542
  Description: Whether the DTQ tail queue is empty
        Input: RWDTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: BOOL_TRUE or BOOL_FLASE
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T RWDTQ_IsEmpty(IN RWDTQ_HEAD_S* pstList)
{
    BOOL_T bIsemp;
    RWDTQ_ReadLock(pstList);
    bIsemp = DTQ_IsEmpty(&pstList->stHead);
    RWDTQ_ReadUnlock(pstList);
    return bIsemp;
}

/*****************************************************************************
    Func Name: RWDTQ_IsEndOfQ
 Date Created: 2010/12/03
       Author: s07542
  Description: Wether the node is the end of the queue
        Input: RWDTQ_HEAD_S* pstList  the pointer to the queue head
               DTQ_NODE_S* pstNode  the pointer to the node
       Output: No output
       Return: BOOL_TURE if is end of the queue, BOOL_FALSE is not
      Caution: RWDTQ is a circle doubly link list, we can not use NULL to
               decide wether reaching the end of list
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline BOOL_T RWDTQ_IsEndOfQ(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    BOOL_T bIsemp;
    RWDTQ_ReadLock(pstList);
    bIsemp = DTQ_IsEndOfQ(&pstList->stHead, pstNode);
    RWDTQ_ReadUnlock(pstList);
    return bIsemp;
}

/*****************************************************************************
    Func Name: RWDTQ_AddHead
 Date Created: 2010/12/03
       Author: s07542
  Description: Add an element to the head of the queue
        Input: RWDTQ_HEAD_S* pstList  the pointer to the queue head
               DTQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_AddHead(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    RWDTQ_WriteLock(pstList);
    DTQ_AddHead(&pstList->stHead, pstNode);
    RWDTQ_WriteUnlock(pstList);
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_DelHead
 Date Created: 2010/12/03
       Author: s07542
  Description: Delete the first element from the queue
        Input: RWDTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* RWDTQ_DelHead(IN RWDTQ_HEAD_S* pstList)
{
    DTQ_NODE_S *pstNode = NULL;
    RWDTQ_WriteLock(pstList);
    pstNode = DTQ_DelHead(&pstList->stHead);
    RWDTQ_WriteUnlock(pstList);
    return pstNode;
}

/*****************************************************************************
    Func Name: RWDTQ_Del
 Date Created: 2008/3/28
       Author: c02254
  Description: Delete an element from the tail queue
        Input: DTQ_NODE_S* pstNode  the pointer to the element to be deleted
       Output: No output
       Return: No return
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_Del(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    RWDTQ_WriteLock(pstList);
    pstNode->pstPrev->pstNext = pstNode->pstNext;
    pstNode->pstNext->pstPrev = pstNode->pstPrev;
    RWDTQ_WriteUnlock(pstList);

    return;
}

/*****************************************************************************
    Func Name: RWDTQ_AddTail
 Date Created: 2010/12/03
       Author: s07542
  Description: Add an element to the tail of the queue
        Input: RWDTQ_HEAD_S* pstList  the pointer to the queue head
               DTQ_NODE_S* pstNode  the pointer to the element to be added
       Output: No output
       Return: No return
      Caution:
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline VOID RWDTQ_AddTail(IN RWDTQ_HEAD_S* pstList, IN DTQ_NODE_S* pstNode)
{
    RWDTQ_WriteLock(pstList);
    DTQ_AddTail(&pstList->stHead, pstNode);
    RWDTQ_WriteUnlock(pstList);
    return;
}

/*****************************************************************************
    Func Name: RWDTQ_DelTail
 Date Created: 2010/12/03
       Author: s07542
  Description: Delete the last element from the queue
        Input: RWDTQ_HEAD_S* pstList  the pointer to the queue head
       Output: No output
       Return: the pointer to the deleted element, NULL if no element is deleted
      Caution: This function only delete the element from the list, and does
               NOT free the memory of the element.
------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
  --------------------------------------------------------------------------

*****************************************************************************/
static inline DTQ_NODE_S* RWDTQ_DelTail(IN RWDTQ_HEAD_S* pstList)
{
    DTQ_NODE_S *pstNode = NULL;
    RWDTQ_WriteLock(pstList);
    pstNode = DTQ_DelTail(&pstList->stHead);
    RWDTQ_WriteUnlock(pstList);
    return pstNode;
    
}

/*****************************************************************************
    Func Name: RWDTQ_FreeAll
 Date Created: 2010/12/03
       Author: s07542
  Description: Remove and free all nodes from DTQ list
        Input: IN RWDTQ_HEAD_S *pstList      
                IN VOID (*pfFree)(VOID *)  
       Output: 
       Return: STATIC
      Caution: 
------------------------------------------------------------------------------
  Modification History                                                      
  DATE        NAME             DESCRIPTION                                  
  --------------------------------------------------------------------------
                                                                            
*****************************************************************************/
static inline VOID RWDTQ_FreeAll(IN RWDTQ_HEAD_S *pstList, IN VOID (*pfFree)(VOID *))
{
    RWDTQ_WriteLock(pstList);
    DTQ_FreeAll(&pstList->stHead, pfFree);
    RWDTQ_WriteUnlock(pstList);
    return;
}

#endif

#ifdef  __cplusplus
}
#endif

#endif /* _SYS_LIST_H_ */

