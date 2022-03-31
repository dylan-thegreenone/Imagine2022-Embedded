#!/bin/bash
IMAGINE_DIR=/home/$USER/Code/ImagineRIT2022

mkdir -p "$IMAGINE_DIR"
podman run --rm -it                             \
    --name=imaginerit-embedded-dev              \
    -v "$IMAGINE_DIR":/ImagineRIT2022           \
    --device=/dev/ttyUSB0:rwm                   \
    --group-add keep-groups                     \
    --annotation io.crun.keep_original_groups=1 \
    imaginerit-embedded-dev

# To open a shell, run: podman exec -it imaginerit-embedded-dev bash

# To add device, insert a device mapping:
#    --device=/dev/ttyUSB0    \