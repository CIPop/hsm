#ifndef HSM_STACK_H
#define HSM_STACK_H

#include <stdint.h>
#include <assert.h>

/**
 * @brief HSM event type.
 *
 */
typedef enum
{
  HSM_ENTRY = 1,
  HSM_EXIT = 2,
  HSM_ERROR = 3,
  HSM_TIMEOUT = 4,
  HSM_EVENT_BASE = 10,
} hsm_event_type;

#define HSM_EVENT(id) ((int32_t)(HSM_EVENT_BASE + id))

typedef struct hsm_event hsm_event;
typedef struct hsm hsm;
typedef int (*state_handler)(hsm* me, hsm_event event, int(** super_state)());

struct hsm_event
{
  hsm_event_type type;
  void* data;
};

struct hsm
{
  state_handler current_state;
};

extern const hsm_event hsm_entry_event;
extern const hsm_event hsm_exit_event;
extern const hsm_event hsm_timeout_event;

int hsm_init(hsm* h, state_handler initial_state)
{
  assert(h != NULL);
  h->current_state = initial_state;
  return h->current_state(h, hsm_entry_event, NULL);
}

int _hsm_recursive_exit(
    hsm* h,
    state_handler source_state,
    state_handler destination_state)
{
  assert(h != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret = 0;
  // Super-state handler making a transition must exit all inner states:
  while (source_state != h->current_state)
  {
    // The current state cannot be null while walking the hierarchy chain from an inner state to the
    // super-state:
    assert(h->current_state != NULL);

    state_handler super_state;
    ret = h->current_state(h, hsm_exit_event, &super_state);
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
    - super state transitioning to another peer state (all inner states will exit).
*/
int hsm_transition(hsm* h, state_handler source_state, state_handler destination_state)
{
  assert(h != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret = 0;
  // Super-state handler making a transition must exit all inner states:
  ret = _hsm_recursive_exit(h, source_state, destination_state);

  if (source_state == h->current_state)
  {
    // Exit the source state.
    ret = h->current_state(h, hsm_exit_event, NULL);
    if (!ret)
    {
      // Enter the destination state:
      h->current_state = destination_state;
      ret = h->current_state(h, hsm_entry_event, NULL);
    }
  }

  return ret;
}

/*
    Supported transitions limited to the following:
    - peer state transitioning to first-level inner state.
    - super state transitioning to another first-level inner state.
*/
int hsm_transition_inner(
    hsm* h,
    state_handler source_state,
    state_handler destination_state)
{
  assert(h != NULL);
  assert(h->current_state != NULL);
  assert(source_state != NULL);
  assert(destination_state != NULL);

  int ret;
  // Super-state handler making a transition must exit all inner states:
  ret = _hsm_recursive_exit(h, source_state, destination_state);

  if (source_state == h->current_state)
  {
    // Transitions to inner states will not exit the super-state:
    h->current_state = destination_state;
    ret = h->current_state(h, hsm_entry_event, NULL);
  }

  return ret;
}

bool hsm_post_sync(hsm* h, hsm_event event)
{
  assert(h != NULL);
  return h->current_state(h, event, NULL);
}

#endif //!HSM_STACK_H
