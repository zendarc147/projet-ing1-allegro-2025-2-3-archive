"""Compile le jeu original et le lance depuis son dossier de ressources."""

import argparse
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-only", action="store_true", help="Compiler sans lancer")
    args = parser.parse_args()
    root = Path(__file__).resolve().parent
    build = root / "build"
    subprocess.run(
        ["cmake", "-S", str(root), "-B", str(build), "-G", "Ninja"], check=True
    )
    subprocess.run(["cmake", "--build", str(build), "--parallel", "4"], check=True)
    if not args.build_only:
        executable = build / ("allegro_sprite.exe" if sys.platform == "win32" else "allegro_sprite")
        return subprocess.run([str(executable)], cwd=root / "cmake-build-debug").returncode
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
