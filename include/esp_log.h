#ifndef LOG_H_
#define LOG_H_

#ifndef ESP_LOG_LEVEL
#define ESP_LOG_LEVEL   ESP_LOG_LEVEL_INFO
#endif

typedef enum {
    ESP_LOG_LEVEL_CRITICAL = 0,
    ESP_LOG_LEVEL_ERROR,
    ESP_LOG_LEVEL_WARNING,
    ESP_LOG_LEVEL_INFO,
    ESP_LOG_LEVEL_DEBUG,
} esp_log_level_t;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ESP_LOG_LEVEL_CRITICAL_COLOR_FMT   ANSI_COLOR_RED    "C: "
#define ESP_LOG_LEVEL_ERROR_COLOR_FMT      ANSI_COLOR_BLUE   "E: "
#define ESP_LOG_LEVEL_WARNING_COLOR_FMT    ANSI_COLOR_YELLOW "W: "
#define ESP_LOG_LEVEL_INFO_COLOR_FMT       ANSI_COLOR_GREEN  "I: "
#define ESP_LOG_LEVEL_DEBUG_COLOR_FMT      ANSI_COLOR_CYAN   "D: "

#define ESP_FORMATTED_LOG(tag, level, fmt, ...) do {                                            \
        if (level <= ESP_LOG_LEVEL) {                                                           \
            Serial.printf(level ## _COLOR_FMT "%s: " fmt ANSI_COLOR_RESET "\n\r", tag, ##__VA_ARGS__); \
        }} while(0)

#define ESP_CLOG(tag, fmt, ...)   ESP_FORMATTED_LOG(tag, ESP_LOG_LEVEL_CRITICAL, fmt, ##__VA_ARGS__)
#define ESP_WLOG(tag, fmt, ...)   ESP_FORMATTED_LOG(tag, ESP_LOG_LEVEL_WARNING,  fmt, ##__VA_ARGS__)
#define ESP_ILOG(tag, fmt, ...)   ESP_FORMATTED_LOG(tag, ESP_LOG_LEVEL_INFO,     fmt, ##__VA_ARGS__)
#define ESP_DLOG(tag, fmt, ...)   ESP_FORMATTED_LOG(tag, ESP_LOG_LEVEL_DEBUG,    fmt, ##__VA_ARGS__)

#endif
