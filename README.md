# OCR Translation Overlay (C Demo)

This repository contains a small C skeleton that demonstrates the flow for a GUI-based OCR + translation overlay application. The sample focuses on the core logic (OCR region capture, translation API verification, overlay state, and logging) and is designed to be extended with real GUI and OCR libraries.

## Features
- **Configurable OCR region** represented by a `rect_t` structure and wired into the pipeline.
- **Overlay controls** for sizing, moving, opacity, and locking to prevent accidental edits.
- **Translation API hook** with preflight verification and a pluggable translation call.
- **Logging options** for console output plus separate info, error, and translation history files.
- **Extensible main loop** (`run_app`) that demonstrates OCR → translate → render overlay.

## Building
This project is a reference stub. It uses only the C standard library, so it can compile with a simple command like:

```bash
cc -Iinclude src/main.c -o ocr_overlay
```

Replace `cc` with your preferred compiler. You can later link GUI/OCR dependencies (e.g., SDL2, Tesseract) as needed.

## Running
The default configuration writes logs to the current directory and prints overlay state to stdout:

```bash
./ocr_overlay
```

Sample output:

```
[INFO 2024-01-01 12:00:00] Starting OCR overlay
[OVERLAY] pos=(100,100) size=320x240 locked=no opacity=0.80 text='[translated] stubbed OCR text'
[INFO 2024-01-01 12:00:00] Exiting cleanly
```

## Extending
- Replace `perform_ocr` with a real screen capture and OCR backend.
- Implement GUI overlay rendering in `simulate_overlay_render` using your toolkit of choice (e.g., Win32, X11, SDL2, GLFW + OpenGL).
- Wire translation calls in `translate_text` to your API and reuse `verify_translation_api` for live key/URL checks.
- Add keyboard/mouse event handling to update `overlay_state_t` for resizing, dragging, opacity changes, and lock toggles.
- Update logging paths or toggle console/file logging through `logging_config_t`.
