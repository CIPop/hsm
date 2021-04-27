#ifndef HFSM_STACK_H
#define HFSM_STACK_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <hfsm.h>

typedef struct hfsm hfsm;
typedef int (*state_handler)(hfsm* me, hfsm_event event, int(** super_state)());

struct hfsm
{
  state_handler current_state;
};

int hfsm_init(hfsm* h, state_handler initial_state);

/*
    Supported transitions limited to the following:
    - peer states (within the same top-level state).
    - super state transitioning to another peer state (all sub-states will exit).
*/
int hfsm_transition_peer(hfsm* h, state_handler source_state, state_handler destination_state);

/*
    Supported transitions limited to the following:
    - peer state transitioning to first-level inner state.
    - super state transitioning to another first-level inner state.
*/
int hfsm_transition_substate(
    hfsm* h,
    state_handler source_state,
    state_handler destination_state);

/*
    Supported transitions limited to the following:
    - peer state transitioning to first-level inner state.
    - super state transitioning to another first-level inner state.
*/
int hfsm_transition_superstate(
    hfsm* h,
    state_handler source_state,
    state_handler destination_state);

int hfsm_post_event(hfsm* h, hfsm_event event);

#endif //!HFSM_STACK_H
