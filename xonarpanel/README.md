# Description

Toggles between the front and the back panel on click.
Tested with Xonar DGX sound card. 
Displays a speaker or headphone depending on panel status.
Can be used independently from i3blocks.
Check Script for more info.

# i3blocks.conf

```
[xonarpanel]
command=$SCRIPT_DIR/xonarpanel -s
interval=once
signal=5
```

- signal can be whatever

# using a keyboard shortcut with a signal

```
bindsym XF86Tools exec --no-startup-id xonarpanel -t && pkill -RTMIN+5 i3blocks
```
