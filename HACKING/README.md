# Contributing to this project

First of all, thanks! Second of all, this is pretty much just a stub right now, but here's how you use the Dockerfile.

## Dockerfile
If you need to install podman, check the [podman documentation](https://podman.io/getting-started/installation.html).

First, build the container.
`
podman build . --tag esp-idf
`

Then, start the container. You might find it convenient to clone this repo outside of the container and mount it inside. Do so using the '-v' flag. For example...
`
podman run -dit -v /home/wilnil/imagine2022-esp-idf:/mnt/imagine2022-esp-idf --device=/dev/ttyUSB0:/dev/ttyUSB0:rwm --group-add keep-groups --annotation io.crun.keep_original_groups=1  esp-idf
`

Then you can attach a shell to the pod
`
podman exec -it <pod_id> bash
`

Once you're inside, use these commands to run the demo:
```
cd /mnt/imagine2022-esp-idf && idf.py -p /dev/ttyUSB0 flash monitor
```

See [the esp-idf documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) for more information.
