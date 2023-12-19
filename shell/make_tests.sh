#!/bin/bash

# Массив команд
commands=("pwd" "echo Hello, World1231!" "yes | head" "df -h" "uname -a")

# Создание временной директории
#temp_dir=$(mktemp -d /tmp/command_output_XXXXX)

# Переменная для индексации
index=0

# Перебор команд в массиве
for command in "${commands[@]}"; do
  # Имя файла для записи вывода
  output_file="/tmp/test${index}_expected_output.txt"
  # Имя файла для записи команды
  command_file="/tmp/test${index}_input.txt"

  # Запись команды в файл
  echo "${command} ; exit" > "$command_file"

  # Запуск команды, запись вывода в файл
  eval $command > "$output_file"
  #echo "\$art\$ " > "$output_file"
 	#eval $command | echo
  #echo "\$art\$ $p" > "$output_file"
  
  # Увеличение индекса
  ((index++))
done
