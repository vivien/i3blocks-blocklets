# memory3

Shows memory (ram or swap) usage.

This blocklet does what
[`memory`](https://github.com/vivien/i3blocks-contrib/tree/master/memory)
is intended to do, but implemented as a C program that stays in the
background, instead of a shell script that is restarted every time the value
is updated. It combines parts of
[`cpu_usage2`](https://github.com/vivien/i3blocks-contrib/tree/master/cpu_usage2),
[`procps`](https://gitlab.com/procps-ng/procps), and
[`htop`](https://github.com/htop-dev/htop).

- Ram usage is shown by default. Pass the `-s` flag or define the `SWAP`
  environment variable to show the swap usage.
- Usage percentage can be appended to the output by passing the `-p` flag
  or defining the `PERCENT` environment variable.
- The same computations as in `htop` are performed by default. Pass the `-d`
  flag or define the `PROCPS` environment to instead produce the same values
  as `free` from `procps` does.
- You can define the refresh time with `-t` or `REFRESH_TIME` (the former has
  priority).

## Overview
Without percentage:

![](memory3-1.png)

With percentage:

![](memory3-2.png)

## Config

```
[memory]
command=$SCRIPT_DIR/memory3
markup=pango
interval=persist
#min_width=MEM 2.2Gi/22Gi
#REFRESH_TIME=5
#LABEL=MEM 
#WARN_PERCENT=50
#CRIT_PERCENT=80

[memory]
command=$SCRIPT_DIR/memory3
markup=pango
interval=persist
#min_width=SWP 2.2Gi/8Gi
#REFRESH_TIME=15
#LABEL=SWP 
SWAP=
#WARN_PERCENT=50
#CRIT_PERCENT=80
```
