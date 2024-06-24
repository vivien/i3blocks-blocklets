# timer_and_stopwatch

A minimal timer and stopwatch blocklet. Right click switches between the two
functionalities, left click starts/stops the counter, wheel sets the timer
or the initialial stopwatch time.

![](running_timer.png)

![](paused_stopwatch.png)

* Copy the script into your directory of choice, e.g. ~/.i3blocks/blocklets
* Give it execution permission (`chmod +x timer_and_stopwatch`)
* Add the following blocklet to your i3blocks.conf:

```ini
[timer_and_stopwatch]
# the DEFAULT_MODE variable must be either timer or stopwatch
format=json
interval=1
#TIMER_LABEL=TM
#STOPWATCH_LABEL=SW
#DEFAULT_MODE=timer
#DEFAULT_STOPWATCH=0
#DEFAULT_TIMER=60
#PLAY_LABEL=(playing)
#PAUSE_LABEL=(paused)
#TIMER_LOOP=true
#UNIX_TIME=true
#NEUTRAL_COLOR=#000000
```
