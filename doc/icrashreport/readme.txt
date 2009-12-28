Способ, которым APuma.dll оповещает пользователя о случившемся падении, настраивается через реестр.
Например, таким файлом:

--------------------------------------------------------------------------------

Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SOFTWARE\Cognitive Technologies Ltd.\CuneiForm\PumaCrashRpt]
@=""

[HKEY_LOCAL_MACHINE\SOFTWARE\Cognitive Technologies Ltd.\CuneiForm\PumaCrashRpt\Batch]
"Action"="QuietStore"
"DumpType"="Full"
"StoreFolder"="D:\\Cuneiform\\bug\\Reports"

[HKEY_LOCAL_MACHINE\SOFTWARE\Cognitive Technologies Ltd.\CuneiForm\PumaCrashRpt\Default]
"Action"="GUI"
"StoreFolder"=""
"DumpType"="Referenced"

--------------------------------------------------------------------------------

По умолчанию, если информация в реестре отсутствует, ошибки выводятся в виде 
диалогового окна с предложением отправить отчет об ошибке.
В реестре ищется ветка [HKEY_LOCAL_MACHINE\SOFTWARE\Cognitive Technologies Ltd.\CuneiForm\PumaCrashRpt]. 
В ней - ключ с именем приложения, вызвавшего Apuma.dll, в примере выше - Batch. Имя приложения определяется 
при помощи GetModuleFileName. Если такого ключа нет - берется ключ Default. 

В ключе могут быть определены 3 строковых значения: Action, StoreFolder и DumpType

Action - допустимые варианты:
GUI - вывод диалогового окна (значение по умолчанию)
QuietStore - сохранять файлы отчета в указанную в StoreFolder папку
NoAction - отчеты не сохраняются, пользовтелю ничего не выводится

StoreFolder используется только если Action=QuietStore. Допустимые варианты:
Temp folder - сохранять файлы отчета во временную папку
C:\Any Folder\Any Subfolder - сохранять файлы отчета в указанную папку (в данном случае - C:\Any Folder\Any Subfolder)

DumpType - допустимые варианты:
Mini - записывается стек и глобальные переменные. Размер дампа ~ 1Мб, в сжатом виде ~ 200Кб.
Referenced - записывается память, на которую ссылаются указатели в стеке и глобальные переменные. 
             Размер дампа ~ 7Мб, в сжатом виде ~ 1Мб (значение по умолчанию).
Full - записывается вся выделенная память приложения. Размер дампа ~ 80Мб, в сжатом виде ~ 30Мб.