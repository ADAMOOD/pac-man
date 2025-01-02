# Pacman v C s SDL2

Tento projekt implementuje jednoduchou hru Pacman v jazyce C s využitím knihovny SDL2. Používá CMake pro správu buildování.

## Požadavky

- **CMake**: Nástroj pro správu buildování.
- **SDL2**: Knihovna pro grafiku a zpracování událostí.

## Instalace závislostí

### Ubuntu / Debian (včetně WSL)

Pokud používáte Ubuntu nebo WSL, nainstalujte potřebné závislosti pomocí následujících příkazů:

```bash
        sudo apt update
        sudo apt install build-essential cmake libsdl2-dev

Vytvořte novou složku pro build, přejděte do ní a spusťte cmake pro konfiguraci projektu:

        mkdir build
        cd build
        cmake ..

Pomocí make přeložte hru:

        make
        make

Po úspěšné kompilaci spusťte hru:
        ./pacman
```

## Pac-Man

### Menu
    Po spuštění se objeví menu se dvěmi možnostmi a na spodu obrazovky je vypsáno nejlepší dosažené skóre.
    V menu se pohybuje šipkama nahoru a dolů a vzbírá klávesou Enter.

### Hra
    Ve hře se pohybujete šipkama.
    Klávesou Escape hru ukončíte s takovým skóre jaké právě máte a vrátíte se zpět do menu.
    Vaším úkolem je sníst všechny perličky na mapě, což ukončí hru a vypíše na obrazovku vaše skóre.
    Ovšem vaše skóre se může zvýšit díky pojídání duchů v "modrém módu".
    Pokud sníte velikou perličku, tak se všichni duchové dostanou na pár sekund do "modrého módu", 
    ve kterém pokud je váš Pac-Man na ducha namířený tak ducha sní a přičte se mu skóre.
    Duch se po jeho snězení promění pouze v oči a hledá cestu domů, kde se následně oživí zpět do "normálního módu",
    ve kterém vás může sníst on.
    Pokud vás duch sní, tak se hra na pár sekund zastaví, Pac-Man bude vrácen zpět na vychozí pozici a ve spodu obrazovky se vám ubere život.
    Pokud se vaše nejlepší skóre zlepší tak se přepíše.

#### Mapa
    Celá logika hry je v textovém souboru map.txt který je editoavtelný i za běhu programu (v menu) a změna bude násleně vidět.
    Snažte se nemazat písmenka která reprezentují duchy a hráče, ani nedělat "neuzavřenou mapu"
    '|' - svislá stěna
    '-' - vodorovná stěna 
    '/' - roh
    'T' - rozvětvení
    ' ' - malá perlička
    '.' - prázdné místo
    'o' - velká perlička
    '0','1' - teleporty


## Poznámky

### LeakSanitizer
    Po ukončení programu Adress Sanitizer zahlásí memmory leaky se kterýma jsem bohužel nepohnul a byly velice záhadné.:(

## CMakeLists.txt
    Svůj CMakeLists soubor jsem musel udělat trošku "zajímavě", jelikož mi stále nemohl najit nainstalované SDL2 knihovny, a tak tam setuju ty proměnné explicitně.
