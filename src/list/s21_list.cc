#ifndef S21_LIST_TPP_
#define S21_LIST_TPP_
#include "s21_list.h"

namespace s21::list_details {
  void list_node_base::unlink() {
    next->prev = prev;
    prev->next = next;
    init();
  }

  void list_node_base::link_before(list_node_base *node) {
    this->prev = node->prev;
    this->next = node;
    node->prev->next = this;
    node->prev = this;
  }

  void list_node_base::link_group_before(
    list_node_base *first, list_node_base *last) {
    this->prev->next = first;
    first->prev = this->prev;
    this->prev = last;
    last->next = this;
  }

  void list_node_base::unlink_group(
    list_node_base *first, list_node_base *last) noexcept {
    first->prev->next = last->next;
    last->next->prev = first->prev;
    last->next = first;
    first->prev = last;
  }

  void list_node_base::swap(list_node_base *lhs, list_node_base *rhs) noexcept {
    std::swap(lhs->prev, rhs->prev);
    std::swap(lhs->next, rhs->next);
  }
} // namespace s21::list_details

#endif