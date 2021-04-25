/**
 * @brief hfsm event type.
 *
 */
typedef enum
{
  hfsm_ENTRY = 1,
  hfsm_EXIT = 2,
  hfsm_ERROR = 3,
  hfsm_TIMEOUT = 4,
  hfsm_EVENT_BASE = 10,
} hfsm_event_type;

#define hfsm_EVENT(id) ((int32_t)(hfsm_EVENT_BASE + id))

struct hfsm_event
{
  hfsm_event_type type;
  void* data;
};

typedef struct hfsm_event hfsm_event;

extern const hfsm_event hfsm_entry_event;
extern const hfsm_event hfsm_exit_event;
extern const hfsm_event hfsm_timeout_event;
