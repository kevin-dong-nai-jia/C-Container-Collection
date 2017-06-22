#ifndef OPENGC3_LIST_CCDLL_H
#define OPENGC3_LIST_CCDLL_H

#include "base.h"
#include "../base/pool.h"
#include "../base/misc.h"
#include "../base/snym.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/* synonym */

#define PRV lnk[0]
#define NXT lnk[1]

#define CCDLL       ADDID(CCDLL)
#define CCDLL_CONT  ADDID(CCDLL_CONT)
#define CCDLL_NODE  ADDID(CCDLL_NODE)
#define CCDLL_BLCK  ADDID(CCDLL_BLCK)
#define CCDLL_ITER  ADDID(CCDLL_ITER)
#define CCDLL_CURR  ADDID(CCDLL_CURR)


/* ccdll create */


#define ccdll(elem_t)                                                          \
                                                                               \
        ccdll_extd(elem_t, 1, NORMAL)

#define ccdll_pckd(elem_t)                                                     \
                                                                               \
        ccdll_extd(elem_t, 1, PACKED)

#define ccdll_extd(elem_t, _n_iter, _ALIGN_)                                   \
                                                                               \
        typedef ccdll_struct_extd(elem_t, _n_iter, _ALIGN_) *CCDLL;  CCDLL

#define ccdll_type(elem_t)                                                     \
                                                                               \
        typedef ccdll_struct_extd(elem_t, 1, NORMAL) *


#define ccdll_struct(elem_t)                                                   \
                                                                               \
        ccdll_struct_extd(elem_t, 1, NORMAL)

#define ccdll_struct_pckd(elem_t)                                              \
                                                                               \
        ccdll_struct_extd(elem_t, 1, PACKED)

#define ccdll_struct_extd(elem_t, _n_iter, _ALIGN_)                            \
                                                                               \
    struct CCDLL_CONT                                                          \
    {                                                                          \
        int size,  last,  vcnt;                   /* size and node record */   \
        int start, ratio, thrsh;                  /* block increment info */   \
                                                                               \
        struct CCDLL_NODE                                                      \
        {   struct CCDLL_NODE *lnk[2];                                         \
            elem_t val;                           /* val with prv and nxt */   \
        }   *avsp, *pnode, head, tail, swap;      /* available space list */   \
                                                                               \
        struct CCDLL_BLCK                                                      \
        {   struct CCDLL_BLCK *bprv, *bnxt;       /* points to prev block */   \
            unsigned int ncnt;                    /* the item of the node */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            struct CCDLL_NODE nodes[];            /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct CCDLL_ITER                                                      \
        {   struct CCDLL_CURR                                                  \
            {   struct CCDLL_NODE *node;                                       \
            }   curr;                             /* points to curr  node */   \
            struct CCDLL_CONT *ccdll;             /* points to ccdll body */   \
        }   (*itarr)[_n_iter], *_iter, **_it;                                  \
                                                                               \
        struct CCDLL_CONT **_co;                  /* internal use _it _co */   \
                                                                               \
        unsigned char _it_base, _it_limit;                                     \
        unsigned char _co_base, _co_limit;                                     \
    }



/* ccdll initialize */


#define ccdll_init(_ccdll)                                                     \
                                                                               \
        ccdll_init_extd(_ccdll,     16,      2,  65536)

#define ccdll_init_extd(_ccdll, _start, _ratio, _thrsh)                        \
                                                                               \
        cc_ll_init_extd(_ccdll, _start, _ratio, _thrsh, ccdll)


#define _ccdll_init(_ccdll_dst, _ccdll_src, _alloc)                            \
                                                                               \
        _ccdll_init_extd(_ccdll_dst, -1,     -1,     -1, _alloc)

#define _ccdll_init_extd(_ccdll, _start, _ratio, _thrsh, _alloc)               \
                                                                               \
        _cc_ll_init_extd(_ccdll, _start, _ratio, _thrsh, _alloc, ccdll)


#define _ccdll_init_core(_ccdll)                                               \
                                                                               \
        _cc_ll_init_core(_ccdll, ccdll)


#define _ccdll_init_seed(_ccdll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccdll)->size = 0,                                                        \
    (_ccdll)->last = (_ccdll)->vcnt = 0,                                       \
                                                                               \
    (_ccdll)->head.PRV = NULL,                                                 \
    (_ccdll)->head.NXT = &((_ccdll)->tail),                                    \
    (_ccdll)->tail.PRV = &((_ccdll)->head),                                    \
    (_ccdll)->tail.NXT = NULL                                                  \
)


#define _ccdll_init_info(_ccdll, _start, _ratio, _thrsh)                       \
                                                                               \
        _cc_ll_init_info(_ccdll, _start, _ratio, _thrsh)


#define ccdll_iter_init(_iter, _ccdll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = NULL,                                                 \
    (_iter)->ccdll = (_ccdll)                                                  \
)


#define _ccdll_iter_init(_iter, _ccdll, _alloc)                                \
                                                                               \
        _cc_ll_iter_init(_iter, _ccdll, _alloc, ccdll)



/* ccdll destroy */


#define ccdll_free(_ccdll)  cc_ll_free(_ccdll)



/* ccdll access */


#define ccdll_front(_ccdll)  ((_ccdll)->head.NXT->val)

#define ccdll_back(_ccdll)   ((_ccdll)->tail.PRV->val)



/* ccdll capacity */


#define ccdll_size(_ccdll)   ((_ccdll)->size)

#define ccdll_empty(_ccdll)  ((_ccdll)->head.NXT == &((_ccdll)->tail) &&       \
                              (_ccdll)->tail.PRV == &((_ccdll)->head))



/* ccdll modifiers */


#define ccdll_push_front(_ccdll, _val) _ccdll_push(_ccdll, _val, head, NXT, PRV)

#define ccdll_push_back(_ccdll, _val)  _ccdll_push(_ccdll, _val, tail, PRV, NXT)

#define _ccdll_push(_ccdll, _val, _hdtl_, _pn_1_, _pn_2_)                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _ccdll_push_alloc(_ccdll, _hdtl_, _pn_1_, _pn_2_);                         \
                                                                               \
    (_ccdll)->_hdtl_._pn_1_->val = (_val);                                     \
)


#define ccdll_push_front_alloc(_ccdll) _ccdll_push_alloc(_ccdll, head, NXT, PRV)

#define ccdll_push_back_alloc(_ccdll)  _ccdll_push_alloc(_ccdll, tail, PRV, NXT)

#define _ccdll_push_alloc(_ccdll, _hdtl_, _pn_1_, _pn_2_)                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_ccdll)->pnode, (_ccdll));                                    \
                                                                               \
    (_ccdll)->pnode->_pn_1_ =   (_ccdll)->_hdtl_._pn_1_;                       \
    (_ccdll)->pnode->_pn_2_ = &((_ccdll)->_hdtl_);                             \
                                                                               \
    (_ccdll)->pnode->PRV->NXT = (_ccdll)->pnode;                               \
    (_ccdll)->pnode->NXT->PRV = (_ccdll)->pnode;                               \
                                                                               \
    (_ccdll)->size++;                                                          \
)


#define  ccdll_pop_front(_ccdll)  _ccdll_pop(_ccdll, head, NXT)

#define  ccdll_pop_back(_ccdll)   _ccdll_pop(_ccdll, tail, PRV)

#define _ccdll_pop(_ccdll, _hdtl_, _np_)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccdll_empty((_ccdll)))  break;                                         \
                                                                               \
    (_ccdll)->pnode = (_ccdll)->_hdtl_._np_;                                   \
                                                                               \
    (_ccdll)->pnode->PRV->NXT = (_ccdll)->pnode->NXT;                          \
    (_ccdll)->pnode->NXT->PRV = (_ccdll)->pnode->PRV;                          \
                                                                               \
    _node_clear((_ccdll)->pnode, (_ccdll));                                    \
                                                                               \
    (_ccdll)->size--;                                                          \
)


#define ccdll_insert(_iter, _val)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccdll_iter_at_head((_iter)))  break;                                   \
                                                                               \
    _node_alloc((_iter)->ccdll->pnode, (_iter)->ccdll);                        \
                                                                               \
    (_iter)->ccdll->pnode->val = (_val);                                       \
                                                                               \
    (_iter)->ccdll->pnode->PRV = (_iter)->curr.node->PRV;                      \
    (_iter)->ccdll->pnode->NXT = (_iter)->curr.node;                           \
                                                                               \
    (_iter)->curr.node->PRV->NXT = (_iter)->ccdll->pnode;                      \
    (_iter)->curr.node->PRV      = (_iter)->ccdll->pnode;                      \
    (_iter)->curr.node           = (_iter)->ccdll->pnode;                      \
                                                                               \
    (_iter)->ccdll->size++;                                                    \
)


#define ccdll_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccdll_iter_at_head((_iter)) || ccdll_iter_at_tail((_iter)))  break;    \
                                                                               \
    (_iter)->curr.node->PRV->NXT = (_iter)->curr.node->NXT;                    \
    (_iter)->curr.node->NXT->PRV = (_iter)->curr.node->PRV;                    \
                                                                               \
    _node_clear((_iter)->curr.node, (_iter)->ccdll);                           \
                                                                               \
    (_iter)->curr.node = (_iter)->curr.node->NXT;                              \
                                                                               \
    (_iter)->ccdll->size--;                                                    \
)


#define ccdll_swap(_ccdll_a, _ccdll_b)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    void *_bup = (_ccdll_a);                                                   \
                                                                               \
    (_ccdll_a) = (_ccdll_b);                                                   \
    (_ccdll_b) = _bup;                                                         \
)


#define ccdll_resize(_ccdll, _items, _val)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _size = ccdll_size((_ccdll)) - (_items);                               \
                                                                               \
         if (_size > 0)  while(_size--)  ccdll_pop_back ((_ccdll));            \
    else if (_size < 0)  while(_size++)  ccdll_push_back((_ccdll), (_val));    \
)


#define ccdll_clear(_ccdll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(ccdll_empty((_ccdll))))     ccdll_pop_back((_ccdll));             \
)



/* ccdll operations */


#define ccdll_move(_iter_p, _iter_i)                                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_i)->curr.node == (_iter_p)->curr.node->PRV))  break;  \
                                                                               \
    if (_unlikely(ccdll_iter_at_head((_iter_p)) ||                             \
                  ccdll_iter_at_head((_iter_i)) ||                             \
                  ccdll_iter_at_tail((_iter_i))))  break;                      \
                                                                               \
    (_iter_i)->curr.node->PRV->NXT = (_iter_i)->curr.node->NXT;                \
    (_iter_i)->curr.node->NXT->PRV = (_iter_i)->curr.node->PRV;                \
                                                                               \
    (_iter_i)->curr.node->PRV      = (_iter_p)->curr.node->PRV;                \
    (_iter_i)->curr.node->NXT      = (_iter_p)->curr.node;                     \
                                                                               \
    (_iter_p)->curr.node->PRV      = (_iter_i)->curr.node;                     \
    (_iter_i)->curr.node->PRV->NXT = (_iter_i)->curr.node;                     \
                                                                               \
    (_iter_i)->ccdll->size--;                                                  \
    (_iter_i)->ccdll = (_iter_p)->ccdll;                                       \
    (_iter_i)->ccdll->size++;                                                  \
)


#define ccdll_move_range(_iter_p, _iter_l, _iter_r)                            \
                                                                               \
        ccdll_move_range_extd(_iter_p, _iter_l, _iter_r,    -1)

#define ccdll_move_range_extd(_iter_p, _iter_l, _iter_r, _dist)                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->ccdll != (_iter_r)->ccdll))  break;               \
                                                                               \
    if (_unlikely((_iter_p)->ccdll != (_iter_l)->ccdll))                       \
    {                                                                          \
        int _dist_m = (_dist);                                                 \
                                                                               \
        if (_dist_m < 0)                                                       \
            ccdll_iter_distance((_iter_l), (_iter_r), &_dist_m);               \
                                                                               \
        (_iter_p)->ccdll->size += _dist_m;                                     \
        (_iter_l)->ccdll->size -= _dist_m;                                     \
        (_iter_l)->ccdll = (_iter_p)->ccdll;                                   \
    }                                                                          \
                                                                               \
    if (_unlikely((_iter_l)->curr.node == (_iter_r)->curr.node))  break;       \
                                                                               \
    void *_bup = (_iter_r)->curr.node;                                         \
                                                                               \
    (_iter_r)->curr.node = (_iter_r)->curr.node->PRV;                          \
                                                                               \
    (_iter_l)->curr.node->PRV->NXT = (_iter_r)->curr.node->NXT;                \
    (_iter_r)->curr.node->NXT->PRV = (_iter_l)->curr.node->PRV;                \
                                                                               \
    (_iter_l)->curr.node->PRV      = (_iter_p)->curr.node->PRV;                \
    (_iter_p)->curr.node->PRV->NXT = (_iter_l)->curr.node;                     \
                                                                               \
    (_iter_r)->curr.node->NXT      = (_iter_p)->curr.node;                     \
    (_iter_p)->curr.node->PRV      = (_iter_r)->curr.node;                     \
                                                                               \
    (_iter_r)->curr.node = _bup;                                               \
)


#define  ccdll_merge(_ccdll_d, _ccdll_s)                                       \
                                                                               \
         ccdll_merge_extd(_ccdll_d, _ccdll_s, DLEQ)

#define  ccdll_merge_extd(_ccdll_d, _ccdll_s, _leq)                            \
                                                                               \
         cc_ll_merge_extd(_ccdll_d, _ccdll_s, _leq, ccdll)

#define _ccdll_merge_extd(_ccdll_d, _iter_l, _iter_m, _iter_r, _leq)           \
                                                                               \
        _cc_ll_merge_extd(_ccdll_d, _iter_l, _iter_m, _iter_r, _leq, ccdll)


#define  ccdll_merge_range(_iter_l, _iter_m, _iter_r)                          \
                                                                               \
         ccdll_merge_range_extd(_iter_l, _iter_m, _iter_r, DLEQ)

#define  ccdll_merge_range_extd(_iter_l, _iter_m, _iter_r, _leq)               \
                                                                               \
         cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _leq, ccdll)

#define _ccdll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq)      \
                                                                               \
        _cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq, ccdll)


#define  ccdll_sort(_ccdll)                                                    \
                                                                               \
         ccdll_sort_extd(_ccdll, DLEQ)

#define  ccdll_sort_extd(_ccdll, _leq)                                         \
                                                                               \
         cc_ll_sort_extd(_ccdll, _leq, ccdll)

#define _ccdll_sort_extd(_ccdll, _carry, _pbuck, _iter_a, _iter_b, _leq)       \
                                                                               \
        _cc_ll_sort_extd(_ccdll, _carry, _pbuck, _iter_a, _iter_b, _leq, ccdll)



/* ccdll comparator */


#define ccdll_comp_leq(_iter_a, _iter_b)  (DREF((_iter_a)) <= DREF((_iter_b)))

#define ccdll_comp_geq(_iter_a, _iter_b)  (DREF((_iter_a)) >= DREF((_iter_b)))



/* ccdll iterators */


#define ccdll_iter_copy(_iter_dst, _iter_src)                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    *(_iter_dst) = *(_iter_src)                                                \
)


#define ccdll_iter_head(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = &((_iter)->ccdll->head)                               \
)


#define ccdll_iter_tail(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = &((_iter)->ccdll->tail)                               \
)


#define ccdll_iter_begin(_iter)                                                \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = ((_iter)->ccdll->head.NXT)                            \
)


#define ccdll_iter_end(_iter)                                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = ((_iter)->ccdll->tail.PRV)                            \
)


#define ccdll_iter_at_head(_iter)   ( (_iter)->curr.node->PRV == NULL)

#define ccdll_iter_at_tail(_iter)   ( (_iter)->curr.node->NXT == NULL)

#define ccdll_iter_at_begin(_iter)  ( (_iter)->curr.node->PRV ==               \
                                    &((_iter)->ccdll->head))

#define ccdll_iter_at_end(_iter)    ( (_iter)->curr.node->NXT ==               \
                                    &((_iter)->ccdll->tail))


#define ccdll_iter_incr(_iter)                                                 \
(                                                                              \
    (ccdll_iter_at_tail(_iter)) ? (NULL) :                                     \
    ((_iter)->curr.node = (_iter)->curr.node->NXT)->NXT                        \
)


#define ccdll_iter_decr(_iter)                                                 \
(                                                                              \
    (ccdll_iter_at_head(_iter)) ? (NULL) :                                     \
    ((_iter)->curr.node = (_iter)->curr.node->PRV)->PRV                        \
)


#define ccdll_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _len = (_diff);                                                        \
                                                                               \
         if (_len > 0)  while (ccdll_iter_incr((_iter)) && --_len);            \
    else if (_len < 0)  while (ccdll_iter_decr((_iter)) && ++_len);            \
)


#define ccdll_iter_distance(_iter_a, _iter_b, _pdist)                          \
                                                                               \
        cc_ll_iter_distance(_iter_a, _iter_b, _pdist, ccdll)



/* ccdll traversor */


#define CCDLL_INCR(_iter)  CC_LL_INCR(_iter, ccdll)

#ifndef CC_STRICT

#define CCDLL_INCR_AUTO(_pval, _ccdll)                                         \
                                                                               \
        CCDLL_INCR_EXTD(_pval, _ccdll, (void)0)

#define CCDLL_INCR_EXTD(_pval, _ccdll, ...)                                    \
                                                                               \
        CC_LL_INCR_EXTD(_pval, _ccdll, ccdll, __VA_ARGS__)

#endif // CC_STRICT


#define CCDLL_DECR(_iter)  CC_LL_DECR(_iter, ccdll)

#ifndef CC_STRICT

#define CCDLL_DECR_AUTO(_pval, _ccdll)                                         \
                                                                               \
        CCDLL_DECR_EXTD(_pval, _ccdll, (void)0)

#define CCDLL_DECR_EXTD(_pval, _ccdll, ...)                                    \
                                                                               \
        CC_LL_DECR_EXTD(_pval, _ccdll, ccdll, __VA_ARGS__)

#endif // CC_STRICT



#endif