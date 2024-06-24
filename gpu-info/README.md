# gpu-info
Displays more detailed information on NVIDIA GPUs by querying `nvidia-smi`.

![Screenshot](gpu-info.png)

```
[gpu-info]
label=GPU 
interval=5
# Uses FontAwesome Icons when True
USE_ICON=True
# Shows free memory when True
SHOW_FREE_MEMORY=True
```
Turning off `SHOW_FREE_MEMORY` produces:

![No Free Memory](gpu-info_nomem.png)

Turning off `USE_ICON` produces:

![No Icons](gpu-info_noicons.png)
