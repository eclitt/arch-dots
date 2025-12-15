#!/bin/bash

# Конфигурация
CACHE_DIR="${HOME}/.cache/weather"
CACHE_FILE="${CACHE_DIR}/weather.txt"
MAX_AGE_MINUTES=10

# Создаем директорию для кэша если не существует
mkdir -p "$CACHE_DIR"

# Проверка возраста файла
is_cache_valid() {
    [[ -f "$CACHE_FILE" ]] && \
    [[ $(find "$CACHE_FILE" -mmin +"$MAX_AGE_MINUTES" 2>/dev/null | wc -l) -eq 0 ]]
}

# Получение данных о погоде
get_weather() {
    curl -s --connect-timeout 5 "wttr.in/?format=1"
}

# Основная логика
if is_cache_valid; then
    cat "$CACHE_FILE"
else
    weather_data=$(get_weather)
    
    if [[ $? -eq 0 && -n "$weather_data" ]]; then
        echo "$weather_data" > "$CACHE_FILE"
        echo "$weather_data"
    else
        # Если запрос не удался, пробуем использовать старые данные если они есть
        if [[ -f "$CACHE_FILE" ]]; then
            echo "[Кэш] $(cat "$CACHE_FILE")"
        else
            echo "Не удалось получить данные о погоде"
            exit 1
        fi
    fi
fi