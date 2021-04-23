#include <stddef.h>
#include <hfsm.h>

const hfsm_event hfsm_entry_event = { hfsm_ENTRY, NULL };
const hfsm_event hfsm_exit_event = { hfsm_EXIT, NULL };
const hfsm_event hfsm_timeout_event = { hfsm_TIMEOUT, NULL };
