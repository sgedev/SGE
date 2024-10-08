/*
 *
 */
#ifndef SGE_LIST_H
#define SGE_LIST_H

#include <sge/common.h>

SGE_BEGIN_DECLS

typedef struct sge_list_node {
    struct sge_list_node* prev;
    struct sge_list_node* next;
} sge_list_node_t;

SGE_FORCE_INLINE void sge_list_node_reset(sge_list_node_t* node) {
    SGE_ASSERT(node != NULL);
    node->prev = node;
    node->next = node;
}

SGE_FORCE_INLINE bool sge_list_node_is_linked(const sge_list_node_t* node) {
    SGE_ASSERT(node != NULL);
    return (node->next != node);
}

SGE_FORCE_INLINE void sge_list_node_link(sge_list_node_t* node, sge_list_node_t* prev, sge_list_node_t* next) {
    SGE_ASSERT(node != NULL);
    SGE_ASSERT(prev != NULL);
    SGE_ASSERT(next != NULL);
    node->prev = prev;
    node->next = next;
    prev->next = node;
    next->prev = node;
}

SGE_FORCE_INLINE void sge_list_node_unlink(sge_list_node_t* node) {
    SGE_ASSERT(node != NULL);
    SGE_ASSERT(node->prev != NULL);
    SGE_ASSERT(node->next != NULL);
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node;
    node->next = node;
}

SGE_FORCE_INLINE sge_list_node_t* sge_list_node_prev(sge_list_node_t* node) {
    SGE_ASSERT(node != NULL);
    return node->prev;
}

SGE_FORCE_INLINE sge_list_node_t* sge_list_node_next(sge_list_node_t* node) {
    SGE_ASSERT(node != NULL);
    return node->next;
}

typedef struct {
    sge_list_node_t knot;
} sge_list_t;

size_t sge_list_length(const sge_list_t* list);
bool sge_list_is_contains(const sge_list_t* list, const sge_list_node_t* node);

SGE_FORCE_INLINE void sge_list_reset(sge_list_t* list) {
    SGE_ASSERT(list != NULL);
    sge_list_node_reset(&list->knot);
}

SGE_FORCE_INLINE bool sge_list_is_empty(const sge_list_t* list) {
    SGE_ASSERT(list != NULL);
    return !sge_list_node_is_linked(&list->knot);
}

SGE_FORCE_INLINE sge_list_node_t* sge_list_knot(sge_list_t* list) {
    SGE_ASSERT(list != NULL);
    return &list->knot;
}

SGE_FORCE_INLINE sge_list_node_t* sge_list_first(sge_list_t* list) {
    SGE_ASSERT(list != NULL);
    return list->knot.next;
}

SGE_FORCE_INLINE sge_list_node_t* sge_list_last(sge_list_t* list) {
    SGE_ASSERT(list != NULL);
    return list->knot.prev;
}

SGE_FORCE_INLINE void sge_list_prepend(sge_list_t* list, sge_list_node_t* node) {
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    sge_list_node_link(node, sge_list_knot(list), sge_list_first(list));
}

SGE_FORCE_INLINE void sge_list_append(sge_list_t* list, sge_list_node_t* node) {
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    sge_list_node_link(node, sge_list_last(list), sge_list_knot(list));
}

SGE_FORCE_INLINE void sge_list_insert_before(sge_list_node_t* curr, sge_list_node_t* node) {
    SGE_ASSERT(curr != NULL);
    SGE_ASSERT(node != NULL);
    sge_list_node_link(node, sge_list_node_prev(curr), curr);
}

SGE_FORCE_INLINE void sge_list_insert_after(sge_list_node_t* curr, sge_list_node_t* node) {
    SGE_ASSERT(curr != NULL);
    SGE_ASSERT(node != NULL);
    sge_list_node_link(node, curr, sge_list_node_next(curr));
}

SGE_FORCE_INLINE sge_list_node_t* sge_list_remove_first(sge_list_t* list) {
    sge_list_node_t* p;
    SGE_ASSERT(list != NULL);
    p = sge_list_first(list);
    sge_list_node_unlink(p);
    return p;
}

SGE_FORCE_INLINE sge_list_node_t* sge_list_remove_last(sge_list_t* list) {
    sge_list_node_t* p;
    SGE_ASSERT(list != NULL);
    p = sge_list_last(list);
    sge_list_node_unlink(p);
    return p;
}

SGE_FORCE_INLINE void sge_list_remove_node(sge_list_t* list, sge_list_node_t* node) {
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    SGE_ASSERT(sge_list_is_contains(list, node));
    sge_list_node_unlink(node);
}

SGE_END_DECLS

#define SGE_LIST_FOREACH(pnode, plist) \
    for ((pnode)  = sge_list_first(plist); \
         (pnode) != sge_list_knot(plist); \
         (pnode)  = sge_list_node_next(pnode))

#define SGE_LIST_FOREACH_R(pnode, plist) \
    for ((pnode)  = sge_list_last(plist); \
         (pnode) != sge_list_knot(plist); \
         (pnode)  = sge_list_node_prev(pnode))


#endif /* SGE_LIST_H */
