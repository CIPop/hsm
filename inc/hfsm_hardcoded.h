#ifndef HFSM_HARDCODED_H
#define HFSM_HARDCODED_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <hfsm.h>

typedef struct hfsm hfsm;
typedef int (*state_handler)(hfsm* me, hfsm_event event);
typedef int (*transition_handler)(hfsm* me, state_handler source_state, state_handler destination_state);

struct hfsm
{
  state_handler current_state;
  transition_handler transition_func;
};

int hfsm_init(hfsm* h, state_handler initial_state, transition_handler transition_func)
{
  assert(h != NULL);
  h->current_state = initial_state;
  h->transition_func = transition_func;
  return h->current_state(h, hfsm_entry_event, NULL);
}

/*
    Supported transitions limited to the following:
    - peer states (within the same top-level state).
    - super state transitioning to another peer state (all sub-states will exit).
*/
int hfsm_transition(hfsm* h, state_handler source_state, state_handler destination_state)
{
  assert(h != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret = 0;
  // Super-state handler making a transition must exit all inner states:
  ret = h->transition_func(h, source_state, destination_state);

  return ret;
}

bool hfsm_post_event(hfsm* h, hfsm_event event)
{
  assert(h != NULL);
  return h->current_state(h, event, NULL);
}

#endif //!HFSM_HARDCODED_H
