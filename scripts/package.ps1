#!/usr/bin/env pwsh
# package.ps1
# Validates, copies, and configures embfetch-rtos for a target platform.
# Usage: .\package.ps1 <platform> <destination_dir>
# Example: .\package.ps1 FREERTOS C:\path\to\project\src

param (
    [Parameter(Mandatory=$true, Position=0)]
    [string]$Platform,

    [Parameter(Mandatory=$true, Position=1)]
    [string]$DestDir
)

$ErrorActionPreference = "Stop"

$ValidPlatforms = @(
    'FREERTOS', 'ESPIDF', 'ZEPHYR', 'NRF_CONNECT',
    'NUTTX', 'RTTHREAD', 'THREADX', 'KEIL_RTX4', 'KEIL_RTX5'
)

$Platform      = $Platform.ToUpper()
$EmbfetchRoot  = Resolve-Path "$PSScriptRoot\.."
$ManifestPath  = "$EmbfetchRoot\scripts\manifest.db"

# --- Validate platform ---
if ($Platform -notin $ValidPlatforms) {
    Write-Error "Unknown platform '$Platform'. Available: $($ValidPlatforms -join ', ')"
    exit 1
}

# --- Validate destination ---
if (-not (Test-Path $DestDir -PathType Container)) {
    Write-Error "Destination directory '$DestDir' does not exist."
    exit 1
}

# --- Validate manifest exists ---
if (-not (Test-Path $ManifestPath -PathType Leaf)) {
    Write-Error "manifest.db not found at '$ManifestPath'."
    exit 1
}

# --- Parse manifest ---
$Entries = @{}
Get-Content $ManifestPath | ForEach-Object {
    $line = $_.Trim()
    if ($line -match '^#' -or $line -eq '') { return }
    $parts = $line -split '\s*:\s*', 2
    if ($parts.Count -ne 2) { return }
    $type     = $parts[0].Trim()
    $filepath = $parts[1].Trim()
    if (-not $Entries.ContainsKey($type)) { $Entries[$type] = @() }
    $Entries[$type] += $filepath
}

# --- Pre-copy validation --- check all source files exist before copying ---
Write-Host "Validating manifest for $Platform..."

$missing = @()
foreach ($type in @('COMMON', $Platform)) {
    if ($Entries.ContainsKey($type)) {
        foreach ($file in $Entries[$type]) {
            $srcPath = Join-Path $EmbfetchRoot $file
            if (-not (Test-Path $srcPath -PathType Leaf)) {
                $missing += $file
            }
        }
    }
}

if ($missing.Count -gt 0) {
    Write-Host "Error: The following source files are missing from the repository:"
    $missing | ForEach-Object { Write-Host "  $_" }
    Write-Error "Aborting — no files were copied."
    exit 1
}
Write-Host "  All source files found."

# --- Copy files ---
Write-Host "`nCopying embfetch files for $Platform..."

foreach ($type in @('COMMON', $Platform)) {
    if ($Entries.ContainsKey($type)) {
        foreach ($file in $Entries[$type]) {
            $srcPath = Join-Path $EmbfetchRoot $file
            $dstPath = Join-Path $DestDir (Split-Path $file -Leaf)
            Copy-Item $srcPath $dstPath
            Write-Host "  Copied: $(Split-Path $file -Leaf)"
        }
    }
}

# --- Enable platform define in embfetch_config.h ---
$ConfigPath = Join-Path $DestDir "embfetch_config.h"
$Define     = "EMBFETCH_$Platform"

Write-Host "`nEnabling $Define in embfetch_config.h..."

if (-not (Test-Path $ConfigPath -PathType Leaf)) {
    Write-Warning "embfetch_config.h not found in '$DestDir'."
} else {
    $Content = Get-Content $ConfigPath -Raw
    $Pattern = "//\s*#define\s+$Define"
    $Replace = "#define $Define"

    if ($Content -match $Pattern) {
        $Content = $Content -replace $Pattern, $Replace
        Set-Content $ConfigPath $Content -NoNewline
        Write-Host "  Enabled: #define $Define"
    } else {
        Write-Warning "$Define not found in embfetch_config.h — check manually."
    }
}

# --- Next steps ---
Write-Host "`nNext steps:"
Write-Host "  1. Add the copied .c files to your build system."
Write-Host "  2. Include 'embfetch_port.h' in your shell command source file."
Write-Host "  3. Call the appropriate sysinfo_print_* function."
Write-Host "  4. Build your project."