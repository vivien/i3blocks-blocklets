# cpu_usage

Show CPU usage.

![](cpu_usage.png)

# Dependencies

* mpstat - Usually a part of sysstat package

# Config

```
[cpu_usage]
command=$SCRIPT_DIR/cpu_usage
interval=10
LABEL=CPU 
#min_width=CPU: 100.00%
#T_WARN=50
#T_CRIT=80
#DECIMALS=2
#COLOR_NORMAL=#EBDBB2
#COLOR_WARN=#FFFC00
#COLOR_CRIT=#FF0000
```
