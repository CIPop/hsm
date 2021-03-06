#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <hfsm_stack.h>

int hfsm_init(hfsm* h, state_handler initial_state)
{
  assert(h != NULL);
  h->current_state = initial_state;
  return h->current_state(h, hfsm_entry_event, NULL);
}

int _hfsm_recursive_exit(
    hfsm* h,
    state_handler source_state)
{
  assert(h != NULL);
  assert(source_state != NULL);

  int ret = 0;
  // Super-state handler making a transition must exit all substates:
  while (source_state != h->current_state)
  {
    // The current state cannot be null while walking the hierarchy chain from an sub-state to the
    // super-state:
    assert(h->current_state != NULL);

    state_handler super_state;
    ret = h->current_state(h, hfsm_exit_event, &super_state);
    if (ret)
    {
      break;
    }

    h->current_state = super_state;
  }

  return ret;
}

/*
    Supported transitions limited to the following:
    - peer states (within the same top-level state).
    - super state transitioning to another peer state (all sub-states will exit).
*/
int hfsm_transition_peer(hfsm* h, state_handler source_state, state_handler destination_state)
{
  assert(h != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret = 0;
  // Super-state handler making a transition must exit all inner states:
  ret = _hfsm_recursive_exit(h, source_state);

  if (source_state == h->current_state)
  {
    // Exit the source state.
    ret = h->current_state(h, hfsm_exit_event, NULL);
    if (!ret)
    {
      // Enter the destination state:
      h->current_state = destination_state;
      ret = h->current_state(h, hfsm_entry_event, NULL);
    }
  }

  return ret;
}

/*
    Supported transitions limited to the following:
    - peer state transitioning to first-level inner state.
    - super state transitioning to another first-level inner state.
*/
int hfsm_transition_substate(
    hfsm* h,
    state_handler source_state,
    state_handler destination_state)
{
  assert(h != NULL);
  assert(h->current_state != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret;
  // Super-state handler making a transition must exit all inner states:
  ret = _hfsm_recursive_exit(h, source_state);

  if (source_state == h->current_state)
  {
    // Transitions to sub-states will not exit the super-state:
    h->current_state = destination_state;
    ret = h->current_state(h, hfsm_entry_event, NULL);
  }

  return ret;
}

/*
    Supported transitions limited to the following:
    - peer state transitioning to first-level inner state.
    - super state transitioning to another first-level inner state.
*/
int hfsm_transition_superstate(
    hfsm* h,
    state_handler source_state,
    state_handler destination_state)
{
  assert(h != NULL);
  assert(h->current_state != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret;
  // Super-state handler making a transition must exit all inner states:
  ret = _hfsm_recursive_exit(h, source_state);

  if (source_state == h->current_state)
  {
    // Transitions to super states will exit the substate but not enter the superstate again:
    ret = h->current_state(h, hfsm_exit_event, NULL);
    h->current_state = destination_state;
  }

  return ret;
}

int hfsm_post_event(hfsm* h, hfsm_event event)
{
  assert(h != NULL);
  return h->current_state(h, event, NULL);
}
