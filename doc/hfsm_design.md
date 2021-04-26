## 1. hfsm_stack : CPU stack based implementation

This implementation requires n + 2 stack frames, where n is the depth of the hierarchical state machine.

## 2. hfsm_hardcoded: Hardcoded hierarchy definition

Stack usage improvements:

1. Superstate event handling (`default:` switch case) can be postponed after the stack-frame exits, within the `hfsm_post_event()` function by using return codes. The deep CPU stack can be converted into a `for` loop.
