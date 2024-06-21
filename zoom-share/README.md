# Zoom share mode

Pausing dunst service while zoom screen sharing.

If the zoom sharing window could be found, the dunst service will be paused and you will be notified over dunst that “Sharing mode” is activated. Upcoming dunst messages will be not removed so that by stop sharing your screen, all the messages will be shown.

# Dependencies

- [dunst](https://dunst-project.org/)
- [xwininfo](https://linux.die.net/man/1/xwininfo)

# Config

```INI
[zoom-share]
command=$SCRIPT_DIR/zoom-share
interval=5
markup=pango
#DUNST_TIMEOUT=3    # duration in seconds
#DUNST_URGENCY=low  # low, normal or critical
```
