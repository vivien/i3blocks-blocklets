# powerprofilesctl

Shows the current powerprofile and gives the ability to change it by clicking on it.

![powerprofilesctl-performance](./powerprofilesctl-performance.png)
![powerprofilesctl-balanced](./powerprofilesctl-balanced.png)
![powerprofilesctl-power-saver](./powerprofilesctl-power-saver.png)

# Dependencies
- [powerprofilesctl](https://manpages.debian.org/unstable/power-profiles-daemon/powerprofilesctl.1.en.html)

# Config
```
[powerprofilesctl]
command=$SCRIPT_DIR/powerprofilesctl
label=🔌
interval=30
```
