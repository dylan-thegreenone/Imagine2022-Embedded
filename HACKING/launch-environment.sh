#!/bin/bash
IMAGINE_DIR=/home/$USER/Code/ImagineRIT2022
mkdir -p "$IMAGINE_DIR"
podman run --rm -it                             \
    --name=imaginerit-embedded-dev              \
    --device=/dev/ttyUSB0:/dev/ttyUSB0:rwm      \
    -v "$IMAGINE_DIR":/mnt/ImagineRIT2022       \
    --group-add keep-groups                     \
    --annotation io.crun.keep_original_groups=1 \
    imaginerit-embedded-dev:v0.0.1
#podman exec -it imaginerit-embedded-dev bash
