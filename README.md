# ModernStaggerLock - AE 1.7.104 Unofficial Patch

Unofficial recompiled DLL for ModernStaggerLock, compatible with Skyrim AE 1.7.104.

This patch fixes the "must be recompiled for new address library" error when using
ModernStaggerLock on Skyrim AE 1.7.104. It is based on the original ModernStaggerLock
source code by max-su-2019, forked from SkyHorizon3's maintained fork.

Changes made in this patch:

- Migrated hardcoded SE `REL::ID` offsets to `RELOCATION_ID(SE_ID, AE_ID)`.
- Bumped the NPC trampoline allocation from 16 bytes to 512 bytes.
- Rebuilt against alandtse's CommonLibSSE-NG (`ng` branch).
- Hook IDs and offsets cross-checked against DeadOnKeyboard1's AE 1.7.104 fork.

No gameplay values, animations, or configuration files were modified.

## Source Code

This repository is a fork of SkyHorizon3/ModernStaggerLock, updated for Skyrim AE 1.7.104.

Original ModernStaggerLock source: https://github.com/max-su-2019/ModernStaggerLock

## Requirements

- Skyrim SE / AE 1.7.104
- SKSE64 2.3.1 or newer
- Address Library for SKSE Plugins (latest)
- CommonLibSSE-NG (ng branch), DKUtil, spdlog
- CMake 3.21+, Visual Studio 2022 or newer, vcpkg

## Build

```powershell
cmake --preset AE
cmake --build --preset AE --config Release
```

The build output is `build/AE/Release/ModernStaggerLock.dll`.

## License

This project is licensed under the GNU General Public License v3.0. See the
`LICENSE` file for the full license text.

Copyright (C) 2026 CharleneHoo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.

## Credits

- max-su-2019 — original author of ModernStaggerLock
- SkyHorizon3 — maintained fork (v1.07a) that this repository is based on
- DeadOnKeyboard1 — AE 1.7.104 reference fork
- alandtse — CommonLibSSE-NG maintainer
- Doodlum — DKUtil
- Everyone in the SKSE plugin dev community
