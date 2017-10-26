/*
 * tec_cache.h
 *
 *  Created on: 2017-9-16
 *      Author: root
 */

#ifndef TEC_CACHE_H_
#define TEC_CACHE_H_

#include <stdio.h>
#include <stdbool.h>

#ifndef LRUCacheImpl_h
#define LRUCacheImpl_h

/*定义LRU缓存的缓存单元*/
typedef struct cacheEntryS{
    char key;   /* 数据的key */
    char* data;  /* 数据的data   */

    struct cacheEntryS *hashListPrev;   /* 缓存哈希表指针， 指向哈希链表的前一个元素 */
    struct cacheEntryS *hashListNext;   /* 缓存哈希表指针， 指向哈希链表的后一个元素 */

    struct cacheEntryS *lruListPrev;    /* 缓存双向链表指针， 指向链表的前一个元素 */
    struct cacheEntryS *lruListNext;    /* 缓存双向链表指针， 指向链表后一个元素 */
}cacheEntryS;


/* 定义LRU缓存 */
typedef struct LRUCacheS{
    int cacheCapacity;  /*缓存的容量*/
    cacheEntryS **hashMap;  /*缓存的哈希表*/

    cacheEntryS *lruListHead;   /*缓存的双向链表表头*/
    cacheEntryS *lruListTail;   /*缓存的双向链表表尾*/
    int lruListSize;    /*缓存的双向链表节点个数*/
}LRUCacheS;

#endif /* LRUCacheImpl_h */

#endif /* TEC_CACHE_H_ */
