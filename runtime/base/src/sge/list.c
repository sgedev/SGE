/*
 *
 */
#include <sge/list.h>

size_t sge_list_length(const sge_list_t* list) {
    size_t length = 0;
    const sge_list_node_t* p;
    SGE_ASSERT(list != NULL);
    for (p = list->knot.next; p != &list->knot; p = p->next) {
        length += 1;
    }
    return length;
}

bool sge_list_is_contains(const sge_list_t* list, const sge_list_node_t* node) {
    const sge_list_node_t* p;
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    for (p = list->knot.next; p != &list->knot; p = p->next) {
       if (p == node) {
        return true;
       }
    }
    return false;
}