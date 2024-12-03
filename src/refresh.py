import os
import glob

def clean_directory(directory):
    # Удаляем все файлы с расширением .tmp
    for tmp_file in glob.glob(os.path.join(directory, '*.tmp')):
        try:
            os.remove(tmp_file)
            print(f'Файл {tmp_file} удалён')
        except Exception as e:
            print(f'Ошибка при удалении {tmp_file}: {e}')

    # Пересоздаём все файлы с расширением .txt как пустые
    for txt_file in glob.glob(os.path.join(directory, '*.txt')):
        try:
            with open(txt_file, 'w') as f:
                pass  # Открываем и сразу закрываем файл, что очищает его
            print(f'Файл {txt_file} очищен')
        except Exception as e:
            print(f'Ошибка при очищении {txt_file}: {e}')


directory_path = 'result'  
clean_directory(directory_path)
