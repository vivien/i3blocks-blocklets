# Incoming

Print the incoming messages on HTTP server on host `localhost:9493`

## Configuration

```toml
[incoming]
interval=persist
markup=pango
#INCOMING_PORT=9493
#INCOMING_HOSTNAME=localhost
```

- **Dependencies**: `python3`

## Usage

- Display a message

```bash
curl -X POST -d 'Hello World' http://localhost:9493
```

- Display a message with color

```bash
curl -X POST -d '<span foreground="red">Hello World</span>' http://localhost:9493
```

```bash
curl -X POST -d '<span foreground="#d6af4e">Hello World</span>' http://localhost:9493
```

- Random color text

```python
import random
import time
import requests


def randhex256():
    return format(random.randint(0, 255), "02x")


while True:
    text = f'<span foreground="#{randhex256()}{randhex256()}{randhex256()}">Some Random Colors...</span>'
    requests.post("http://localhost:9493", data=text)
    time.sleep(0.05)
```
