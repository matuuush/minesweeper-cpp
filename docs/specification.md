## Špecifikácia projektu Míny + dynamický generátor

Cieľom projektu je vytvorenie dynamického generátora rozloženia mín na hracom poli v hre Míny (Minesweeper). Tento generátor zabezpečí, že hra sa vždy bude dať vyhrať bez tipovania. Dynamický generátor bude hracie pole generovať aktívne počas priebehu hry. Súčasťou riešenia bude kontrolný mechanizmus. Tento mechanizmus bude overovať dôvod, kvôli ktorému hráč klikol na dané políčko. Tým zabezpečí, že hráč hru hraje predpísaným spôsobom. To znamená, že hráč sa nemôže spoliehať na tipovanie. Kontrolný mechanizmus tipovanie odhalí a hráča potrestá tak, že mu na dané políčko vygeneruje mínu. Tým dôjde ku koncu hry.

Dynamický generátor a kontrolný mechanizmus budú dostatočne rýchle na to, aby bolo zabezpečené, že samotná hra nebude viditeľne spomalená a skúsenosť hráča tak nebude ovplyvnená negatívne.

### Obrazovky

- Hra
    - Slúži na:
        - Spustenie novej hry
        - Hranie hry
    - Obsahuje:
        - Dynamické generovanie
        - Kontrolný mechanizmus
- Nastavenia
    - Slúži na nastavenie veľkosti herného poľa

### Ovládanie

- Hra
    - Ľavé tlačidlo myši
        - Odhaľovanie obsahu políčok
        - Zapnutie novej hry
    - Pravé tlačidlo myši
        - Označovanie pozícií mín
- Nastavenia
    - Klávesnica a myš
        - Hráč pomocou klávesnice a myši nastaví a potvrdí veľkosť hracieho poľa

### Použité knižnice

- SFML
    - Použitá za účelom vygenerovania pohodlného užívateľského prostredia pre hru