# key_layout2

Display the current keyboard layout using [xkblayout-state](https://github.com/nonpop/xkblayout-state).

![](key_layout2.png)

# Installation

Use the following in your i3blocks config:

```ini
[key_layout2]
interval=30
signal=10
command=$SCRIPT_DIR/key_layout2
```

To update the blocklet on layout change, you can define the keybinding in your i3 config, for example, to toggle keyboard layout on `Alt + Left Shift`:

```ini
bindcode Mod1+50 exec --no-startup-id xkblayout-state set +1 && pkill -RTMIN+10 i3blocks
```

Make sure `grp:alt_shift_toggle` option is not set somewhere in your X configuration.

For Arch Linux users, the xkblayout-state script is available in [AUR](https://aur.archlinux.org/packages/xkblayout-state/).
