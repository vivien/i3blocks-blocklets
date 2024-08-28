# `ping`

Display a green or red pill according to the ability to reach the
configured `ping_target` target host, or the `8.8.8.8` default target.

![](ping-ok.png)
![](ping-ko.png)
![](ping-error.png)

# Dependencies

- `python3`
- `ping`
- `fonts-noto-color-emoji`

# Config
## Output with pango markup available
```INI
[my-external-ip]
command=$SCRIPT_DIR/my-external-ip
markup=pango
interval=60
color=#aaffaa
# external_ip_provider=ip.yunohost.org
```

## Classic output
```INI
[my-external-ip]
command=$SCRIPT_DIR/my-external-ip
interval=60
color=#aaffaa
# external_ip_provider=ipv4.icanhazeip.com
```
