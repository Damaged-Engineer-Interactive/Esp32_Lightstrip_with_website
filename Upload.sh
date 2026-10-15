#!/bin/sh
set -eu

cd "$(dirname "$0")"

environment=az-delivery-devkit-v4
device_url=${ESP32_URL:-http://192.168.54.153}
firmware=".pio/build/$environment/firmware.bin"

if command -v pio >/dev/null 2>&1; then
    platformio_cli=$(command -v pio)
elif [ -x "$HOME/.platformio/penv/bin/pio" ]; then
    platformio_cli="$HOME/.platformio/penv/bin/pio"
else
    printf '%s\n' "PlatformIO CLI (pio) is required." >&2
    exit 1
fi

if ! command -v curl >/dev/null 2>&1; then
    printf '%s\n' "curl is required." >&2
    exit 1
fi

"$platformio_cli" run -e "$environment"

if [ ! -f "$firmware" ]; then
    printf 'Build did not produce %s\n' "$firmware" >&2
    exit 1
fi

printf 'Uploading %s to %s/update...\n' "$firmware" "${device_url%/}"
response=$(curl --silent --show-error --fail \
    --form "update=@$firmware;type=application/octet-stream" \
    "${device_url%/}/update")

if [ "$response" != "OK" ]; then
    printf 'Device returned an unexpected response: %s\n' "$response" >&2
    exit 1
fi

printf '%s\n' "Upload successful; the ESP32 is rebooting."