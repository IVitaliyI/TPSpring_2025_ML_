#!/bin/bash

if [ ! -f .clang-format ]; then
    echo "Файл .clang-format не найден в текущей директории."
    exit 1
fi

EXTENSIONS=("*.h" "*.hpp" "*.hh" "*.c" "*.cpp" "*.cc" "*.cxx")

FILES=()
for EXT in "${EXTENSIONS[@]}"; do
    while IFS= read -r -d $'\0' file; do
        FILES+=("$file")
    done < <(find Task1/include Task1/src -type f -name "$EXT" -print0)
done

if [ ${#FILES[@]} -eq 0 ]; then
  echo "Нет файлов для форматирования."
  exit 0
fi

echo "Форматирование файлов:"
for file in "${FILES[@]}"; do
  echo "  $file"
  clang-format -i "$file"
done

echo "Форматирование завершено."