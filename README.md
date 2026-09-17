# GemSight

Dota 2 companion (draft intel, STRATZ, GSI). See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## Build (Linux dev)

```bash
export QT_INSTALL_DIR="$HOME/Qt/6.11.2/gcc_64"
export CMAKE_PREFIX_PATH="$QT_INSTALL_DIR"
export PATH="$QT_INSTALL_DIR/bin:$PATH"
./run.sh
```

## Build (Windows)

Qt **6.11+** (MinGW or MSVC kit from Qt Online Installer). From repo root in PowerShell:

```powershell
# Optional: point at your kit if auto-detect fails
$env:CMAKE_PREFIX_PATH = "C:\Qt\6.11.3\mingw_64"
.\run.ps1
```

| Flag | Action |
|------|--------|
| `.\run.ps1` | configure (if needed) + build + run |
| `.\run.ps1 --rebuild` | wipe `build-win`, then build + run |
| `.\run.ps1 --run` | run only (exe must exist) |
| `.\run.ps1 --package` | Release build + `GemSight-win64-Release.zip` |

Env: `BUILD_TYPE`, `QT_INSTALL_DIR`, `GEMSIGHT_QT_KIT`, `FETCHCONTENT_BASE_DIR` (see `.\run.ps1 --help`).

### STRATZ (M3)

In-app **Settings** (gear on the rail): paste your [STRATZ](https://stratz.com/api) GraphQL token. Enemy profiles load via STRATZ when the token is set; otherwise OpenDota avatar fallback. Pick advisor recalculates from SQLite matchup matrix on live draft updates (demo Turbo still uses fixed tier examples).

## GSI

Copy `resources/gsi/gamestate_integration_gemsight.cfg` into Dota’s `game/dota/cfg/gamestate_integration/` and add Steam launch option `-gamestateintegration`.

## i18n

Default UI language: **Russian** (`settings` → `ui/language`, `ru`). English catalog: `translations/gemsight_en.ts` (run `qt6-lupdate` / `qt6-lrelease` via build).
