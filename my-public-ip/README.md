# `my-public-ip`

Display the public IP address returned by an public IP provider,
such as [ip.yunohost.org](http://ip.yunohost.org) or
[ipv4.icanhazeip.com](http://ipv4.icanhazeip.com).

Offers clipboarding with middle mouse button click on the block, even
in short display.

![](my-public-ip-long.png)
![](my-public-ip-short.png)

# Dependencies

- `wget` or `curl` for HTTP request,
- `xclip` for clipboarding.

# Config
## Output with pango markup available
```INI
[my-public-ip]
command=$SCRIPT_DIR/my-public-ip
markup=pango
interval=60
color=#aaffaa
# public_ip_provider=ip.yunohost.org
```

## Classic output
```INI
[my-public-ip]
command=$SCRIPT_DIR/my-public-ip
interval=60
color=#aaffaa
# public_ip_provider=ipv4.icanhazeip.com
```
