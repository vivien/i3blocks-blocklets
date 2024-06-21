# kbl_switcher: Keyboard Layout Switcher

Switch and display the current keyboard layout using setxkbmap.

# Dependencies
setxkbmap, sed, bc, tr, fontawesome (keyboard symbol)

# Usage
Switch between your list of keyboard layouts using left and right mouse click.

# Installation

Use the following in your i3blocks config:

```ini
[kbl_switcher]
command=$SCRIPT_DIR/kbl_switcher
label= 
LAYOUTS=pt,gb,de,us
interval=30
```

