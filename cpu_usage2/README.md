# cpu_usage2

Show CPU usage.
This is a C version of the cpu_usage blocklet.

![](cpu_usage2.png)

# Usage
Compile the `cpu_usage2` executable from `*.c` file.
```
cd /path/to/i3blocks/scripts/cpu_usage2
make
```

# Config

```
[cpu_usage2]
command=$SCRIPT_DIR/cpu_usage2
markup=pango
interval=persist
#min_width=CPU 100.00%
#REFRESH_TIME=1
#LABEL=CPU 
#WARN_PERCENT=50
#CRIT_PERCENT=80
#DECIMALS=2
```
