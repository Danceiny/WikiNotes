```python
# coding: utf-8

from state_machine import State, Event, acts_as_state_machine, after, before, InvalidStateTransition

@acts_as_state_machine
class Process(object):
    created = State(initial=True)
    waiting = State()
    running = State()
    terminated = State()
    blocked = State()
    swapped_out_waiting = State()
    swapped_out_blocking = State()

    wait = Event(from_states=(created, running, blocked, swapped_out_waiting), to_state=waiting)

    run = Event(from_states=waiting, to_state=running)
```