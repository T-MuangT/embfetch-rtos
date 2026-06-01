#!/usr/bin/env python3
# package.py
# Usage: python package.py <platform> <destination_dir>

import sys
import os
import re
import shutil

def parse_manifest(manifest_path):
    entries = {}
    with open(manifest_path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = [p.strip() for p in line.split(':')]
            if len(parts) != 2:
                continue
            platform, filepath = parts
            entries.setdefault(platform, []).append(filepath)
    return entries

def validate(manifest, platform, embfetch_root):
    missing = []
    for filepath in manifest.get('COMMON', []) + manifest.get(platform, []):
        full = os.path.join(embfetch_root, filepath)
        if not os.path.isfile(full):
            missing.append(filepath)
    return missing

def copy_files(manifest, platform, embfetch_root, dest):
    copied = []
    for filepath in manifest.get('COMMON', []) + manifest.get(platform, []):
        src = os.path.join(embfetch_root, filepath)
        dst = os.path.join(dest, os.path.basename(filepath))
        shutil.copy2(src, dst)
        copied.append(os.path.basename(filepath))
    return copied

def enable_platform(dest, platform):
    config = os.path.join(dest, 'embfetch_config.h')
    define = f'EMBFETCH_{platform}'
    with open(config, 'r') as f:
        content = f.read()
    updated = re.sub(
        rf'//\s*#define\s+{define}',
        f'#define {define}',
        content
    )
    if updated == content:
        return False
    with open(config, 'w') as f:
        f.write(updated)
    return True

def main():
    if len(sys.argv) != 3:
        print(f"Usage: python package.py <platform> <destination_dir>")
        print(f"Platforms: FREERTOS ESPIDF ZEPHYR NRF_CONNECT NUTTX RTTHREAD THREADX KEIL_RTX4 KEIL_RTX5")
        sys.exit(1)

    platform = sys.argv[1].upper()
    dest     = sys.argv[2]

    embfetch_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    manifest_path = os.path.join(embfetch_root, 'scripts', 'manifest.db')

    # Validate
    if not os.path.isdir(dest):
        print(f"Error: Destination directory '{dest}' does not exist.")
        sys.exit(1)

    manifest = parse_manifest(manifest_path)

    if platform not in manifest and platform != 'COMMON':
        print(f"Error: Unknown platform '{platform}'.")
        print(f"Available: {', '.join(k for k in manifest if k != 'COMMON')}")
        sys.exit(1)

    print(f"Validating manifest for {platform}...")
    missing = validate(manifest, platform, embfetch_root)
    if missing:
        print("Error: The following source files are missing:")
        for f in missing:
            print(f"  {f}")
        sys.exit(1)
    print("  All source files found.")

    # Copy
    print(f"\nCopying embfetch files for {platform}...")
    copied = copy_files(manifest, platform, embfetch_root, dest)
    for f in copied:
        print(f"  Copied: {f}")

    # Enable platform
    print(f"\nEnabling EMBFETCH_{platform} in embfetch_config.h...")
    if enable_platform(dest, platform):
        print(f"  Enabled: #define EMBFETCH_{platform}")
    else:
        print(f"  Warning: EMBFETCH_{platform} not found in embfetch_config.h")

    # Next steps
    print("\nNext steps:")
    print("  1. Add the copied .c files to your build system.")
    print("  2. Include 'embfetch_port.h' in your shell command source file.")
    print("  3. Call the appropriate sysinfo_print_* function.")

if __name__ == "__main__":
    main()