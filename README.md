Toto README obsahuje pouze nejdůležitějši informace, zbytek informací lze nalézt v dokumentaci k programu, která vznikne vygenerovaním pomocí make doxygen v adresáři ./doc/html

## Použití

 make - Překlad serveru a klienta
 make run - Spuštení klienta
 make runserver - Spuštení serveru
 make doxygen - Vygenerování dokumentace do ./doc/html/
 make pack - Vytvoření archivu s projektem
 make clean - Odstranění dočasných souborů a vygenerované dokumentace

## Login

 Uživatelské jméno: "testuser"
 Heslo: "password"

## Příklady

 ./examples/networks - Příklady sítí
 ./examples/configs - Konfigurační soubory klienta
 ./examples/history - Ukázka souboru s historií simulace
 ./examples/protocol - Ukázka zpráv komunikačního protokolu a samotný komunikační protokol

## Server

Server2012 implementuje TCP server a metody pro simulaci vysokoúrovňových petriho sítí.

## Implementace

Komunikace s klientem probíhá pomocí **XML** souboru. Krok simulace ma 3 hlavní fáze: ověření dostupnosti **tokenu**, kontrola podmínek a přesun tokenu. Každé místo v petriho siti obsahuje množinu tokenů, která je postupně filtrována. Pokud je po vyfiltrování množina prázdná, přechod nelze provést a klient tuto skutečnost oznamí přislušnou hláškou.

## Spuštění

Server se spouští bez parametrů příkazem **make runserver**. Standardně naslouchá na portu **44444**. Kdyz je port 44444 nedostupný, použije se náhodný volný port.

## Klient

Klient slouži k tvorbě, prohlížení a simulaci simulačních souborů, které je mozné nahrát z místního úložistě, či ze serveru, ke kterému se klient připojuje na základě zadané ip adresy a portu. Klient je následně autentizován pomocí zadaného jména a hesla. Sítě, které byly ulozeny na server je mozné procházet a filtrovat dle vyhledávaného řetězce v polích, které uživatel vybere pro prohledávání. Při procházení sítí je možné procházet také simulačni historii dané sítě. Klientská aplikace uživateli umožňuje vytvářet Petriho sítě volbou daného módu myši, který se přepíná v toolbaru **Mode**. Místa a přechody ve scéně lze kopírovat, vkládat a odstraňovat s tím, že se při vložení vždy vytvoří nový prvek s unikátním ID. Pro úpravu vlastností daného prvku je k dispozici widget, jehož zobrazení lze zapnout či vypnout pomocí tlačítka "Display item properties". Simulace je pak spouštěna, či krokována pomocí tlačítek v toolbaru. Simulaci je možné vrátit do předchozího stavu pomocí tlačítka se zpětnou šipkou. Aktuální nastavení je možné uložit do souboru config.xml. V adresáři ./examples/configs jsou připraveny soubory config1.xml a config2.xml, které je možné načíst a nastavit tak chování klienta.

## Další soubory

 ./src/Doxyfile - Nastavení pro vygenerovaní dokumentace
 pomocí programu doxygen
 ./src/icon.png - Ikona programu pro doxygen
