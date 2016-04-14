#if !defined(SOFTWARE_TIMER_H)
#define SOFTWARE_TIMER_H

#define swTimer_set(tim, val)		((tim) = (val))
#define swTimer_stop(tim)			((tim) = 0u)
#define swTimer_tick(tim)			if ((tim) > 0u){((tim)--);}
#define swTimer_isElapsed(tim)		((tim) == 0u)

#endif /* SOFTWARE_TIMER_H */
