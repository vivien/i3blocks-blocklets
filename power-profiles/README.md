# power-profile

Displays the current power profile set using power-profiles-daemon, and offers controls to cycle through available power profiles using right/left click .

![](power-profile-balanced.png)
![](power-profile-power-saver.png)

# Dependencies

power-profiles-daemon, fontawesome (for the icon)

# Usage

Left/right clicks cycles through power profiles.

If you have any other scripts or hotkeys to change power profiles, you can add `&& pkill -RTMIN+2 i3blocks` to the end of the command to have it update the block, or you can set an interval other that once. 

Where the number `2` in `-RTMIN+2` is the same as the signal you picked in your i3blocks config.

# Config

A unique singal is required
lable is optional and you can pick what you want

```INI
[power-profile]
command=$SCRIPT_DIR/power-profiles
interval=once
signal=2
label=:

```

