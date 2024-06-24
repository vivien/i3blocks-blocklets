# rescuetime

Shows the number of productive hours as calculated by the
[Rescuetime](https://www.rescuetime.com/dashboard) dashboard.

A Rescuetime API key must exist in `~/.rescuetime_key`. You can create API keys
[here](https://www.rescuetime.com/anapi/manage).

![](example.png)

## Setup / Usage

Suggested i3blocks configuration:

```
[rescuetime]
command=$SCRIPT_DIR/rescuetime/rescuetime
interval=120
markup=pango
```

## Dependencies

- python3.4 and above
