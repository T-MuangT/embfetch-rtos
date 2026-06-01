#!/bin/sh
# package.sh
# Validates, copies, and configures embfetch-rtos for a target platform.
# Usage: ./package.sh <platform> <destination_dir>
# Example: ./package.sh FREERTOS /path/to/project/src

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
MANIFEST="$EMBFETCH_ROOT/scripts/manifest.db"
PLATFORMS="FREERTOS ESPIDF ZEPHYR NRF_CONNECT NUTTX RTTHREAD THREADX KEIL_RTX4 KEIL_RTX5"

# --- Argument handling ---
if [ -z "$1" ] || [ -z "$2" ]; then
    printf "Usage: %s <platform> <destination_dir>\n" "$0"
    printf "Platforms: %s\n" "$PLATFORMS"
    exit 1
fi

PLATFORM="$1"
DEST="$2"

# --- Validate platform ---
valid=0
for p in $PLATFORMS; do
    [ "$p" = "$PLATFORM" ] && valid=1 && break
done
if [ "$valid" = "0" ]; then
    printf "Error: Unknown platform '%s'.\n" "$PLATFORM"
    printf "Available: %s\n" "$PLATFORMS"
    exit 1
fi

# --- Validate destination ---
if [ ! -d "$DEST" ]; then
    printf "Error: Destination directory '%s' does not exist.\n" "$DEST"
    exit 1
fi

# --- Validate manifest exists ---
if [ ! -f "$MANIFEST" ]; then
    printf "Error: manifest.db not found at '%s'.\n" "$MANIFEST"
    exit 1
fi

# --- Pre-copy validation — check all source files exist before copying ---
printf "Validating manifest for %s...\n" "$PLATFORM"

missing=0
while IFS=' : ' read -r type file; do
    case "$type" in
        '#'*|'') continue ;;
    esac
    if [ "$type" = "COMMON" ] || [ "$type" = "$PLATFORM" ]; then
        src_path="$EMBFETCH_ROOT/$file"
        if [ ! -f "$src_path" ]; then
            printf "  Missing: %s\n" "$file"
            missing=1
        fi
    fi
done < "$MANIFEST"

if [ "$missing" = "1" ]; then
    printf "Error: Source files missing from repository. Aborting — no files were copied.\n"
    exit 1
fi
printf "  All source files found.\n"

# --- Copy files ---
printf "\nCopying embfetch files for %s...\n" "$PLATFORM"

while IFS=' : ' read -r type file; do
    case "$type" in
        '#'*|'') continue ;;
    esac
    if [ "$type" = "COMMON" ] || [ "$type" = "$PLATFORM" ]; then
        src_path="$EMBFETCH_ROOT/$file"
        dst_path="$DEST/$(basename "$file")"
        cp "$src_path" "$dst_path"
        printf "  Copied: %s\n" "$(basename "$file")"
    fi
done < "$MANIFEST"

# --- Enable platform define in embfetch_config.h ---
CONFIG="$DEST/embfetch_config.h"
DEFINE="EMBFETCH_$PLATFORM"

printf "\nEnabling %s in embfetch_config.h...\n" "$DEFINE"

if [ ! -f "$CONFIG" ]; then
    printf "  Warning: embfetch_config.h not found in '%s'.\n" "$DEST"
else
    if grep -q "//[[:space:]]*#define $DEFINE" "$CONFIG"; then
        sed "s|//[[:space:]]*#define $DEFINE|#define $DEFINE|" \
            "$CONFIG" > "${CONFIG}.tmp" && mv "${CONFIG}.tmp" "$CONFIG"
        printf "  Enabled: #define %s\n" "$DEFINE"
    else
        printf "  Warning: %s not found in embfetch_config.h — check manually.\n" "$DEFINE"
    fi
fi

# --- Next steps ---
printf "\nNext steps:\n"
printf "  1. Add the copied .c files to your build system.\n"
printf "  2. Include 'embfetch_port.h' in your shell command source file.\n"
printf "  3. Call the appropriate sysinfo_print_* function.\n"
printf "  4. Build your project.\n"