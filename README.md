# GemSight

Dota 2 companion (draft intel, STRATZ, GSI). See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

## Build (Linux dev)

```bash
export QT_INSTALL_DIR="$HOME/Qt/6.11.2/gcc_64"
export CMAKE_PREFIX_PATH="$QT_INSTALL_DIR"
export PATH="$QT_INSTALL_DIR/bin:$PATH"
./run.sh
```

Windows: use Qt 6.11+ MSVC kit, same CMake options, launch `gemsight_app`.

## GSI

Copy `resources/gsi/gamestate_integration_gemsight.cfg` into Dota’s `game/dota/cfg/gamestate_integration/` and add Steam launch option `-gamestateintegration`.

## i18n

Default UI language: **Russian** (`settings` → `ui/language`, `ru`). English catalog: `translations/gemsight_en.ts` (run `qt6-lupdate` / `qt6-lrelease` via build).
