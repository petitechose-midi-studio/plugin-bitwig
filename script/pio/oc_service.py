# OC Bridge Service Manager for PlatformIO
#
# Stops OC Bridge service before upload, restarts after.
# Cross-platform: Windows (sc), Linux (systemctl), macOS (launchctl)
#
# pyright: reportUndefinedVariable=false, reportMissingImports=false
import subprocess
import platform
import os
import time

# Service names per platform
SERVICE_NAMES = {
    "Windows": "OpenControlBridge",
    "Linux": "open-control-bridge",
    "Darwin": "com.petitechose.open-control-bridge",
}

_system = platform.system()
_was_running = False
_env_cache = None


def _get_env() -> dict:
    """Get environment, with DBus fix for Linux. Cached for efficiency."""
    global _env_cache
    if _env_cache is not None:
        return _env_cache

    _env_cache = os.environ.copy()
    if _system == "Linux" and "DBUS_SESSION_BUS_ADDRESS" not in _env_cache:
        uid = os.getuid()
        _env_cache["DBUS_SESSION_BUS_ADDRESS"] = f"unix:path=/run/user/{uid}/bus"
    return _env_cache


def _run(cmd: list[str]) -> subprocess.CompletedProcess:
    """Run command with platform-appropriate environment."""
    return subprocess.run(cmd, capture_output=True, text=True, env=_get_env())


def _is_running() -> bool:
    """Check if OC Bridge service is running."""
    name = SERVICE_NAMES.get(_system)
    if not name:
        return False

    try:
        if _system == "Windows":
            r = _run(["sc", "query", name])
            return "RUNNING" in r.stdout

        elif _system == "Linux":
            r = _run(["systemctl", "--user", "is-active", name])
            return r.returncode == 0

        elif _system == "Darwin":
            # launchctl list <label> returns 0 if loaded, non-zero otherwise
            r = _run(["launchctl", "list", name])
            return r.returncode == 0

    except FileNotFoundError:
        # Command not found (sc/systemctl/launchctl)
        pass
    return False


def _stop_service() -> bool:
    """Stop the service. Returns True if command succeeded."""
    name = SERVICE_NAMES.get(_system)
    if not name:
        return False

    try:
        if _system == "Windows":
            r = _run(["sc", "stop", name])
            ok = r.returncode == 0 or "STOP_PENDING" in r.stdout
        elif _system == "Linux":
            r = _run(["systemctl", "--user", "stop", name])
            ok = r.returncode == 0
        elif _system == "Darwin":
            r = _run(["launchctl", "stop", name])
            ok = r.returncode == 0
        else:
            return False

        # Give the OS time to release the serial port
        if ok:
            time.sleep(0.5)
        return ok

    except FileNotFoundError:
        pass
    return False


def _start_service() -> bool:
    """Start the service. Returns True if command succeeded."""
    name = SERVICE_NAMES.get(_system)
    if not name:
        return False

    try:
        if _system == "Windows":
            r = _run(["sc", "start", name])
            return r.returncode == 0 or "START_PENDING" in r.stdout
        elif _system == "Linux":
            r = _run(["systemctl", "--user", "start", name])
            return r.returncode == 0
        elif _system == "Darwin":
            r = _run(["launchctl", "start", name])
            return r.returncode == 0

    except FileNotFoundError:
        pass
    return False


# =============================================================================
# PlatformIO hooks
# =============================================================================

def before_upload(source, target, env):
    """Stop service before upload to free serial port."""
    global _was_running
    _was_running = _is_running()
    if _was_running:
        name = SERVICE_NAMES.get(_system, "service")
        print(f"[OC] Stopping {name}...")
        if _stop_service():
            print("[OC] Service stopped")
        else:
            print("[OC] Warning: Could not stop service")


def after_upload(source, target, env):
    """Restart service after upload."""
    if _was_running:
        name = SERVICE_NAMES.get(_system, "service")
        print(f"[OC] Restarting {name}...")
        if _start_service():
            print("[OC] Service restarted")
        else:
            print("[OC] Warning: Could not restart service")


Import("env")
env.AddPreAction("upload", before_upload)
env.AddPostAction("upload", after_upload)
