#ifndef OCR_APP_H
#define OCR_APP_H

#include <stdbool.h>
#include <stddef.h>

// Translation provider configuration
typedef struct {
    char api_url[256];
    char api_key[128];
    bool verify_before_use;
} translation_config_t;

// Log targets
typedef struct {
    bool console_enabled;
    bool file_enabled;
    char info_log_path[260];
    char error_log_path[260];
    char translate_log_path[260];
} logging_config_t;

// App level configuration
typedef struct {
    translation_config_t translation;
    logging_config_t logging;
    bool start_locked;
} app_config_t;

// Region of interest for OCR or overlay
typedef struct {
    int x;
    int y;
    int width;
    int height;
} rect_t;

// Describes live overlay state
typedef struct {
    rect_t bounds;
    bool locked;
    float opacity; // 0.0 transparent, 1.0 opaque
} overlay_state_t;

// Utility API
void log_info(const logging_config_t *logging, const char *fmt, ...);
void log_error(const logging_config_t *logging, const char *fmt, ...);
void log_translation(const logging_config_t *logging, const char *text);
bool verify_translation_api(const translation_config_t *translation);

// OCR and translation pipeline (stubbed for demo)
char *perform_ocr(const rect_t *region);
char *translate_text(const translation_config_t *translation, const char *text);

// Overlay controls
void set_overlay_bounds(overlay_state_t *overlay, rect_t bounds);
void set_overlay_locked(overlay_state_t *overlay, bool locked);
void set_overlay_opacity(overlay_state_t *overlay, float opacity);

// Application loop entry
int run_app(const app_config_t *config);

#endif // OCR_APP_H
