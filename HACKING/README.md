# Contributing to this project

First of all, thanks! Second of all, this is pretty much just a stub right now, but here's how you use the Dockerfile.

## Dockerfile
 Run with:
 ```
podman build . --tag esp-idf
podman run -dit --device=/dev/ttyUSB0:/dev/ttyUSB0:rwm --group-add keep-groups --annotation io.crun.keep_original_groups=1  14f167d6ad5e
 ```

Once you're inside, use these commands to run the demo:
```
. $HOME/esp/esp-idf/export.sh
cd ~/esp/esp-idf/examples/bluetooth/bluedroid/classic_bt/bt_discovery
idf.py -p /dev/ttyUSB0 flash monitor
```

See [the documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) for more information.
