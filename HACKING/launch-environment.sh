#!/bin/bash
IMAGINE_DIR=/home/$USER/Documents/imagine2022-embedded
mkdir -p "$IMAGINE_DIR"
podman run --rm -it                             \
    --name=imaginerit-embedded-dev              \
    -v "$IMAGINE_DIR":/ImagineRIT2022           \
    --device=/dev/ttyUSB0:rwm                   \
    --group-add keep-groups                     \
    --annotation io.crun.keep_original_groups=1 \
    --annotation run.oci.keep_original_groups=1 \
    --security-opt label=disable \
    imaginerit-embedded-dev

# To open a shell, run: podman exec -it imaginerit-embedded-dev bash
# To add device, insert a device mapping:
#    --device=/dev/ttyUSB0    \
