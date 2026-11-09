"""Tests Linux : vraies fenêtres Allegro, clics X11, détection d'erreurs mémoire."""

import os
from pathlib import Path
import subprocess
import sys
import time


def main():
    root = Path(__file__).resolve().parents[1]
    build = root / "build"
    executable = build / "regression"
    sources = sorted(str(p) for p in root.glob("*.c") if p.name != "main.c")
    subprocess.run([
        "cc", "-std=gnu11", "-g", "-fsanitize=address,undefined",
        "-fno-omit-frame-pointer", "-I", str(root),
        "-isystem", str(build / "allegro/include"),
        str(root / "tests/regression.c"), *sources,
        "-L", str(build / "allegro/lib"),
        f"-Wl,-rpath,{build / 'allegro/lib'}", "-lalleg", "-lX11", "-lXtst",
        "-pthread", "-o", str(executable),
    ], check=True)
    # Éviter les écrans du bureau : l'auto-détection Xvfb peut supprimer leur socket.
    display = next(n for n in range(98, 198)
                   if not Path(f"/tmp/.X{n}-lock").exists()
                   and not Path(f"/tmp/.X11-unix/X{n}").exists())
    with (build / "xvfb.log").open("w") as log:
        server = subprocess.Popen(
            ["Xvfb", f":{display}", "-screen", "0", "1000x800x24", "-nolisten", "tcp"],
            stdout=log, stderr=log,
        )
        try:
            for _ in range(50):
                if server.poll() is not None:
                    raise RuntimeError("Xvfb n'a pas démarré : voir build/xvfb.log")
                if Path(f"/tmp/.X11-unix/X{display}").exists():
                    break
                time.sleep(0.1)
            else:
                raise RuntimeError("Xvfb n'est pas prêt")
            env = dict(os.environ, DISPLAY=f":{display}",
                       ASAN_OPTIONS="detect_leaks=0:halt_on_error=1",
                       UBSAN_OPTIONS="halt_on_error=1:print_stacktrace=1")
            scenarios = sys.argv[1:] or ("quitter", "menu2", "menu4", "combat", "animations", "victoire", "rejouer", "timer")
            for scenario in scenarios:
                subprocess.run([str(executable), scenario], cwd=root / "cmake-build-debug",
                               env=env, check=True, timeout=40)
        finally:
            server.terminate()
            server.wait(timeout=5)


if __name__ == "__main__":
    main()
