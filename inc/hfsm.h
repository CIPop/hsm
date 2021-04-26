/**
 * @brief hfsm event type.
 *
 */
typedef enum
{
  HFSM_ENTRY = 1,
  HFSM_EXIT = 2,
  HFSM_ERROR = 3,
  HFSM_TIMEOUT = 4,
  HFSM_EVENT_BASE = 10,
} hfsm_event_type;

#define HFSM_EVENT(id) ((int32_t)(HFSM_EVENT_BASE + id))

struct hfsm_event
{
  hfsm_event_type type;
  void* data;
};

typedef struct hfsm_event hfsm_event;

extern const hfsm_event hfsm_entry_event;
extern const hfsm_event hfsm_exit_event;
extern const hfsm_event hfsm_timeout_event;
