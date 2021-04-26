#include <stddef.h>
#include <hfsm.h>

const hfsm_event hfsm_entry_event = { HFSM_ENTRY, NULL };
const hfsm_event hfsm_exit_event = { HFSM_EXIT, NULL };
const hfsm_event hfsm_timeout_event = { HFSM_TIMEOUT, NULL };
