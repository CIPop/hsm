#ifndef HFSM_HARDCODED_H
#define HFSM_HARDCODED_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <hfsm.h>

typedef struct hfsm hfsm;
typedef int (*state_handler)(hfsm* me, hfsm_event event);
typedef state_handler (*get_parent)(state_handler child_state);

struct hfsm
{
  state_handler current_state;
  get_parent get_parent_func;
};

int hfsm_init(hfsm* h, state_handler initial_state, get_parent get_parent_func)
{
  assert(h != NULL);
  h->current_state = initial_state;
  h->get_parent_func = get_parent_func;
  return h->current_state(h, hfsm_entry_event);
}

int hfsm_transition_superstate(hfsm* h, state_handler source_state, state_handler destination_state)
{
  assert(h != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret = 0;

  return ret;
}

int hfsm_transition_substate(hfsm* h, state_handler source_state, state_handler destination_state)
{
  assert(h != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret = 0;

  return ret;
}

int hfsm_transition_peer(hfsm* h, state_handler source_state, state_handler destination_state)
{
  assert(h != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret = 0;

  return ret;
}


bool hfsm_post_event(hfsm* h, hfsm_event event)
{
  assert(h != NULL);
  return h->current_state(h, event);
}

#endif //!HFSM_HARDCODED_H
