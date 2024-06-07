# battery2

## Show the current status of your battery

### Plugged and full
![](images/full.png)

### Charging
![](images/charging.png)

### Discharging
![](images/unplugged_full.png)

![](images/unplugged_75.png)

![](images/unplugged_50.png)

![](images/unplugged_25.png)

![](images/unplugged_empty.png)

### Unknown status
![](images/unknown.png)

### No battery is present
![](images/nobattery.png)

## Show the notification when below threshold

![](images/warning_threshold.png)

# Dependencies (Debian like)

- `fonts-font-awesome`,
- `libnotify-bin`,
- `acpi`,
- `python3`

# Installation

To use with `i3blocks`, copy the blocklet configuration in the given
`i3blocks.conf` into your i3blocks configuration file, the recommended
config is:

```INI
[battery2]
command=$SCRIPT_DIR/battery2
markup=pango
interval=30
```
