#include "app.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

static void write_log_line(const char *path, const char *line) {
    FILE *f = fopen(path, "a");
    if (!f) {
        return;
    }
    fputs(line, f);
    fputc('\n', f);
    fclose(f);
}

static void format_timestamp(char *buffer, size_t buffer_len) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    strftime(buffer, buffer_len, "%Y-%m-%d %H:%M:%S", tm_now);
}

void log_info(const logging_config_t *logging, const char *fmt, ...) {
    char message[1024];
    char timestamp[64];

    format_timestamp(timestamp, sizeof(timestamp));

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    if (logging->console_enabled) {
        fprintf(stdout, "[INFO %s] %s\n", timestamp, message);
    }

    if (logging->file_enabled && logging->info_log_path[0]) {
        char line[1200];
        snprintf(line, sizeof(line), "[INFO %s] %s", timestamp, message);
        write_log_line(logging->info_log_path, line);
    }
}

void log_error(const logging_config_t *logging, const char *fmt, ...) {
    char message[1024];
    char timestamp[64];

    format_timestamp(timestamp, sizeof(timestamp));

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    if (logging->console_enabled) {
        fprintf(stderr, "[ERROR %s] %s\n", timestamp, message);
    }

    if (logging->file_enabled && logging->error_log_path[0]) {
        char line[1200];
        snprintf(line, sizeof(line), "[ERROR %s] %s", timestamp, message);
        write_log_line(logging->error_log_path, line);
    }
}

void log_translation(const logging_config_t *logging, const char *text) {
    if (!logging->file_enabled || !logging->translate_log_path[0]) {
        return;
    }
    char timestamp[64];
    format_timestamp(timestamp, sizeof(timestamp));

    char line[1024];
    snprintf(line, sizeof(line), "[%s] %s", timestamp, text);
    write_log_line(logging->translate_log_path, line);
}

bool verify_translation_api(const translation_config_t *translation) {
    // Stub that pretends to verify connectivity by checking both URL and key are set
    return translation->api_url[0] != '\0' && translation->api_key[0] != '\0';
}

char *perform_ocr(const rect_t *region) {
    // Stub implementation; in a real build this would capture the screen within the region
    // and run an OCR engine such as Tesseract.
    const char *placeholder = "stubbed OCR text";
    (void)region; // unused in placeholder

    char *result = malloc(strlen(placeholder) + 1);
    if (result) {
        strcpy(result, placeholder);
    }
    return result;
}

char *translate_text(const translation_config_t *translation, const char *text) {
    // Stub translation using a fixed prefix to demonstrate pluggable API calls.
    const char *prefix = "[translated] ";
    size_t total_len = strlen(prefix) + strlen(text) + 1;
    char *result = malloc(total_len);
    if (result) {
        snprintf(result, total_len, "%s%s", prefix, text);
    }
    (void)translation;
    return result;
}

void set_overlay_bounds(overlay_state_t *overlay, rect_t bounds) {
    overlay->bounds = bounds;
}

void set_overlay_locked(overlay_state_t *overlay, bool locked) {
    overlay->locked = locked;
}

void set_overlay_opacity(overlay_state_t *overlay, float opacity) {
    if (opacity < 0.0f) opacity = 0.0f;
    if (opacity > 1.0f) opacity = 1.0f;
    overlay->opacity = opacity;
}

static void simulate_overlay_render(const overlay_state_t *overlay, const char *text) {
    printf("[OVERLAY] pos=(%d,%d) size=%dx%d locked=%s opacity=%.2f text='%s'\n",
           overlay->bounds.x,
           overlay->bounds.y,
           overlay->bounds.width,
           overlay->bounds.height,
           overlay->locked ? "yes" : "no",
           overlay->opacity,
           text);
}

int run_app(const app_config_t *config) {
    log_info(&config->logging, "Starting OCR overlay");

    if (config->translation.verify_before_use && !verify_translation_api(&config->translation)) {
        log_error(&config->logging, "Translation API verification failed");
        return 1;
    }

    overlay_state_t overlay = {
        .bounds = { .x = 100, .y = 100, .width = 320, .height = 240 },
        .locked = config->start_locked,
        .opacity = 0.8f,
    };

    rect_t ocr_region = overlay.bounds;
    char *ocr_text = perform_ocr(&ocr_region);
    if (!ocr_text) {
        log_error(&config->logging, "OCR failed; no text captured");
        return 1;
    }

    char *translated = translate_text(&config->translation, ocr_text);
    if (!translated) {
        free(ocr_text);
        log_error(&config->logging, "Translation failed");
        return 1;
    }

    log_translation(&config->logging, translated);

    simulate_overlay_render(&overlay, translated);

    free(translated);
    free(ocr_text);

    log_info(&config->logging, "Exiting cleanly");
    return 0;
}

static void initialize_default_config(app_config_t *config) {
    memset(config, 0, sizeof(*config));
    strcpy(config->translation.api_url, "https://api.example.com/translate");
    strcpy(config->translation.api_key, "your-api-key");
    config->translation.verify_before_use = true;

    config->logging.console_enabled = true;
    config->logging.file_enabled = true;
    strcpy(config->logging.info_log_path, "./ocr_info.log");
    strcpy(config->logging.error_log_path, "./ocr_error.log");
    strcpy(config->logging.translate_log_path, "./ocr_translate.log");

    config->start_locked = false;
}

int main(void) {
    app_config_t config;
    initialize_default_config(&config);

    return run_app(&config);
}
