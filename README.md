# Matice, konečná tělesa, zlomky - knihovna

Tato knihovna specifikuje **C++** `template`y pro počítání s maticemi, zlomky a konečnými tělesy.

Násobení matic probíhá dle definice (Strassenův algoritmus možná dodělám později, pokud bude potřeba).
Počítání inverze a determinantu probíhá pomocí Gaussovy eliminace.
Umocňování probíhá v logaritmickém čase.

Implementaci `bigint`u jsem zkopíroval ze starého projektu.

Knihovna se zaměřuje hlavně na počítání s maticem/zlomky/... - funkce pro výpis na standardní výstup slouží hlavně k debugování.

## Použití knihovny

Hlavním `namespace`em je `matrices`, který obsahuje několik tříd a funkcí.
Dále knihovna obsahuje `namespace number_utils`, který obsahuje několik užitečných funkcí a implementaci `bigint`u.

Máme dvě různé třídy pro matice - `matrices::matrix<T, ROWS, COLS>` a `matrices::dynamic_matrix<T>`.
Tyto matice umožňují stejné operace

### Třídy `matrices::matrix<T, ROWS, COLS>` a `matrices::dynamic_matrix<T>`

Počítání v $T^{ROWS\times COLS}$.

Třída `matrices::matrix<T, ROWS, COLS>` představuje matici, u které známe velikost během kompilace.
Umožňuje kontrolu validity sčítání/násobení (jestli sedí rozměry) během kompilace.

Třída `matrices::dynamic_matrix<T>` představuje matici, které při konstrukci zadáme rozměry a pak už je nesmíme změnit.
Kontrolu validity sčítání/násobení (jestli sedí rozměry) provádí pomocí `assert`ů - při chybě vyhodí `matrices::assert_error`.

K prvkům matice můžeme přistupovat pomocí metody `element(int, int)`, nebo pomocí operátor `[]` - například `m[0][1] = 2`.

Matice podporují konstrukci a přiřazení pomocí `std::initializer_list`, tedy např. `m = { 1, 2, 3, 4 }` nebo `m = { { 1, 2 }, { 3, 4 } }`.

Matice implementují operace `==`, `!=`, `+`, `-`, `*`, `+=`, `-=`, `*=`, `multiply_from_left`, `^`, `^=`, `trace`, `transpose`,
`transpose_self`, `get_REF`, `get_RREF`, `do_REF`, `do_RREF`, `compute_rank`, `compute_inverse_RREF` a `compute_determinant_REF`.
- `^` znamená umocňování.
- `multiply_from_left` je jako `*=`, ale násobí zleva.
- `transpose_self` nahradí matici její transpozicí - lze jen pro čtvercovou matici.
- `get_REF`/`get_RREF` vrátí REF/RREF tvar matice.
- `do_REF`/`do_RREF` provede na matici Gaussovu/Gauss-Jordanovu eliminaci.
- `compute_rank`/`compute_inverse_RREF`/`compute_determinant_REF`
    spočítají rank/inverzi/determinant matice pomocí Gaussovy nebo Gauss-Jordanovy eliminace. Výpočet proběhne při každém zavolání znovu.

Na třídách existují statické metody `identity(int)`, které vrátí jednotkovou matici.
Ještě existuje funkce `matrices::identity_matrix<T, SIZE>()`, která vrací jednotkovou `matrices::matrix<T, SIZE, SIZE>`.

### Třídy `matrices::finite_field<T>` a `matrices::finite_field_template<T, P>`

Počítání v $\mathbb Z_p$.

Třída `matrices::finite_field_template<T, P>` představuje konečné těleso, u kterého známe velikost (prvočíslo P) během kompilace.
Existují zkratky `matrices::int_finite_field<int P>` a `matrices::long_finite_field<long long P>`.

Třída `matrices::finite_field<T>` představuje konečné těleso, u kterého velikost (prvočíslo P) nastavíme během konstrukce.
Velikost můžeme být taky nenastavena (reprezentováno pomocí nuly) - s takovouto proměnnou ale nemůžeme provádět aritmetiku, jen přiřazení.

Žádná ze tříd neprovádí kontrolu prvočíselnosti parametru P.

Třídy implementují operace `==`, `!=`, `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`, `^`, `^=`, `++` a `--`.
- `^` znamená umocňování.
- `++` a `--` jsou suffixové i prefixové.
- `/` a `/=` - dělení číslem $x$ odpovídá násobení $x^{p - 2}$.

Existuje suffix `_Zp`, který vytvoří `matrices::finite_field<T>` s nenastavenou velikostí - nutné pokud chceme přiřazovat do matic:
```
dynamic_matrix<finite_field<int>> m(3, 3, finite_field<int>(5, 0));
m = { 1_Zp, 2_Zp, 3_Zp, 4_Zp, 5_Zp, 6_Zp, 7_Zp, 8_Zp, 9_Zp };
```

### Třída `matrices::fraction<T>`

Počítání v $\mathbb Q$.

Třída si udržuje čitatel a jmenovatel. Po každé operaci zkrátí zlomek na základní tvar.

Třída implementuje operace `==`, `!=`, `<`, `<=`, `>`, `>=`, `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`, `^`, `^=`, `++` a `--`.
- `^` znamená umocňování.
- `++` a `--` jsou suffixové i prefixové.

Třída implementuje metodu `value<U>()` - pro převod na `float` a `double`.

### Třída `matrices::assert_error`

Výjimka, která je vyhozena při pokusu o neplatnou operaci - přířazení/sečtení/násobení matic špatných rozměrů,
aritmetika v $\mathbb Z_p$ pro proměnné s různým $p$ atp.

### Třídy `number_utils::bigint` a `number_utils::bigint10`

Třída `number_utils::bigint` implementuje aritmetiku s číslem v soustavě o základu $2^32$.

Třída `number_utils::bigint10` implementuje aritmetiku s číslem v soustavě o základu $10^9$ - triviální převod do desítkové soustavy.

Jsou naimplementovány standardní operátory.

### Třída `number_utils::standard_numbers<T>` a její specializace

Slouží k poskytnutí čísel 0, 1 a -1 v type `T` - nutné pro konstrukci jednotkové matice typu např. `dynamic_matrix<finite_field<int>>`.

Pokud chcete matici nad nějakým vlastním číselným typem, nejspíš budete muset tuto třídu specializovat.

## Adresářová struktura

Jak je projekt strukturován.

### `src/matrix.hpp`, `src/dynamic_matrix.hpp` a `src/matrix_implementation.hpp`

Tyto soubory implementují třídy `matrices::matrix<T, ROWS, COLS>` a `matrices::dynamic_matrix<T>`.
Soubor `src/matrix_implementation.hpp` obsahuje implementaci Gaussovy eliminace jako template,
který pak používají třídy `matrices::matrix<T, ROWS, COLS>` a `matrices::dynamic_matrix<T>`.

### `src/number_types.hpp`

Implementace tříd `matrices::fraction<T>`, `matrices::finite_field<T>` a `matrices::finite_field_template<T, P>`.

### `src/assert.hpp` a `src/printing.hpp`

Implementace `matrices::assert_error` a operátorů `<<` pro debug výpis na obrazovku.

### `src/numbers.hpp`, `src/bigint.hpp` a `src/bigint10.hpp`

Implementace `number_utils::standard_numbers<T>`, `number_utils::bigint` a `number_utils::bigint10`.

### `test/`

Pár testů, které je možné spustit pomocí `make test`.
