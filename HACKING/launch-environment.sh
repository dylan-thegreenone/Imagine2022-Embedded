#!/bin/bash
IMAGINE_DIR=/home/$USER/Code/ImagineRIT2022
DEVICE0=/dev/ttyUSB0
# DEVICE1=/dev/ttyUSB1 # --device="$DEVICE1":"$DEVICE1":rwm          \
# DEVICE2=/dev/ttyUSB2 # --device="$DEVICE2":"$DEVICE2":rwm          \

mkdir -p "$IMAGINE_DIR"
podman run --rm -it                             \
    --name=imaginerit-embedded-dev              \
    --device="$DEVICE0":"$DEVICE0":rwm          \
    -v "$IMAGINE_DIR":/ImagineRIT2022           \
    --group-add keep-groups                     \
    --annotation io.crun.keep_original_groups=1 \
    imaginerit-embedded-dev
