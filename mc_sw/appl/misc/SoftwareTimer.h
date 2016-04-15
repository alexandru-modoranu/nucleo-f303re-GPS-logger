#if !defined(SOFTWARE_TIMER_H)
#define SOFTWARE_TIMER_H

#define swTimer_set(tim, val)		((tim) = (val))
#define swTimer_stop(tim)			((tim) = 0)
#define swTimer_tick(tim)			if ((tim) > 0){((tim)--);}
#define swTimer_isElapsed(tim)		((tim) == 0)

#endif /* SOFTWARE_TIMER_H */
