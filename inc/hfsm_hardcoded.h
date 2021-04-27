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

#define RET_HANDLE_BY_SUPERSTATE -1

int hfsm_init(
    hfsm* h,
    state_handler initial_state,
    get_parent get_parent_func);

/*
    Supported transitions limited to the following:
    - peer states (within the same top-level state).
    - super state transitioning to another peer state (all sub-states will exit).
*/
int hfsm_transition_peer(
    hfsm* h,
    state_handler source_state,
    state_handler destination_state);

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

#endif //!HFSM_HARDCODED_H
