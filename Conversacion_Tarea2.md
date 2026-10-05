# Conversación con Claude — Tarea 2 (Generadores de scanners y parsers)

**Fecha:** 2026-10-05  
**Herramienta:** Claude en un Proyecto de claude.ai (Claude Code en la nube), modelo Claude Opus 5.5  
**Turnos:** 14 prompts del usuario con sus respuestas  
**Documento trabajado:** [Tarea 2: Generadores de scanners y parsers](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5) (Claude Doc)

Formato: tus mensajes van como cita (`>`); las acciones que Claude ejecutó (leer o escribir archivos, descargar documentación, compilar, editar el documento) aparecen como líneas con 🔧; la salida completa de esos comandos no se incluye. Los prompts 4 y 14 se escribieron en el chat general del proyecto y se pasaron a este hilo.

---

## 1. Prompt

*(adjunto: `consigna_tarea2.png`)*

> vamos a hacer la tarea 2, ya hemos hecho la 1 (puedes revisar lo hecho en la tarea 1, hay un md de mi conversacion pasada con todo el contexto), no quiero que hags directo todo, peor quiero que hagamos un plan de como hacerlo y quiero que me digas nates de todo que entiendes de todo y asi para asegurarme que si entendiste el contexto

### Respuesta de Claude

> 🔧 *Leyó `consigna_tarea2.png`*

> 🔧 *Bash: revisó el repo Tarea1 y la carpeta Ensamblador*

> 🔧 *Leyó `Conversacion_Tarea1.md` completa*

> 🔧 *Leyó `Documento_Tarea1.pdf`*

> 🔧 *Escribió `notas de memoria`*

Leí la consigna, el repo Tarea1 y tu `Conversacion_Tarea1.md` completa (sí estaba en la carpeta Ensamblador). Esto es lo que entiendo.

**Contexto**
- El mini-proyecto del curso es un compilador en **C++17 con CMake**. Lo vas a hacer en Windows con TDM-GCC (g++ 10.3). La Tarea 1 dejó listas las piezas que va a usar: `Stack` para los operandos, `Queue` para los cuádruplos y `Dictionary` para la tabla de símbolos.
- Tus reglas de la vez pasada: primero un plan y tu aprobación, nada de código para copiar y pegar (yo te guío con ideas y pistas y tú lo escribes), y git lo haces tú a mano. Entregaste un documento corto con "Qué hice" y una sección honesta sobre el uso de IA.

**La consigna**
Es una investigación sobre herramientas que **generan scanners y parsers automáticamente**. Hay que documentar al menos 3: una debe ser Lex&Yacc o Flex&Bison, y las otras dos dependen del lenguaje del mini-proyecto. De cada una hay que cubrir plataforma y lenguaje, formato de entrada y tipo de ejecución, licencia, base teórica (regex/autómatas, LL, LALR, etc.), tipo de interfaz y cómo inyectar código propio. Se entrega un documento con referencias y, de preferencia, un ejercicio hecho con alguna de ellas.

**Plan que propongo**
1. **Elegir las herramientas.** Como el proyecto es C++, propongo tres que usan enfoques teóricos distintos, para que la comparación tenga sentido:
   - **Flex & Bison** (la obligatoria): un DFA a partir de regex, y un parser LALR(1). Generan C/C++.
   - **ANTLR4**: un parser ALL(*) (de la familia LL), con lexer y parser en una sola gramática. La herramienta corre en Java, pero genera C++.
   - **Boost.Spirit X3**: aquí no hay generador. La gramática se escribe dentro de C++ como parser combinators (estilo PEG).
2. **Investigar y armar una tabla comparativa** con los 6 puntos de la consigna, más fuentes oficiales para las referencias.
3. **El ejercicio, en Flex & Bison:** una calculadora que lea expresiones y genere su postfija o sus cuádruplos usando tu `Stack`/`Queue` de la Tarea 1. Lo integramos con CMake, así ya queda como semilla del parser del mini-proyecto. Te guío igual que antes.
4. **Escribir el documento** con el mismo formato que la Tarea 1.

**Lo que necesito que me confirmes**
- ¿Van esas 3 herramientas? Otra opción para la tercera es **re2c + Lemon**, que también son de C/C++.
- ¿El ejercicio va en Flex & Bison? (recomendado)
- ¿Dónde vive: un repo nuevo **Tarea2** (recomendado) o una carpeta dentro de Tarea1?
- ¿Seguimos con las mismas reglas de la Tarea 1?

---

## 2. Prompt

> las 3 herramientas me parecen bien, si hagamos el ejercicio en ese de flex, no quiero hacer otro repo, solo le cambiare luego el nombre de tarea 1 a a Ensamblador o a alg asi, entonces va ahi todo otra vez, las reglas las modificamos, en cuanto a texto que me vas a dar de informacion, quieor que me des siempre la referencia con las palabras exactas y luego con una explicacion tuya del concepto, de esta manera lo escribo yo despues, ademas quiero que anres de inicar hagas un dradft del documento para yo tener en mente ya de manera visual el que se necesita poner y asi

### Respuesta de Claude

Entendido: todo va en el repo actual y cambian las reglas. Primero te armo el borrador visual del documento.

> 🔧 *Claude Docs: creó el borrador del documento "Tarea 2: Generadores de scanners y parsers"*

> 🔧 *Claude Docs: llenó el esqueleto (portada y secciones 1 a 6)*

> 🔧 *Claude Docs: dejó un comentario en el documento*

> 🔧 *Escribió `notas de memoria`*

Ya está el [borrador del documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5). Puedes editarlo directo o dejarme comentarios. Te dejé una pregunta ahí sobre el ejercicio.

Las reglas nuevas quedan así: toda la información te la doy con la referencia y la cita textual de la fuente, luego mi explicación del concepto, y la redacción la haces tú. Todo va en el mismo repo.

Si te late el borrador, empezamos con la ficha de Flex & Bison.

---

## 3. Prompt

> si, me gusta, pero quiero que todo se haga dentro de la carpeta donde esta la tarea 1, solo en un nuevo folder que se llame Tarea2

### Respuesta de Claude

Va: todo en una carpeta `Tarea2` dentro del repo de la Tarea 1. Ajusto el borrador y arranco con Flex & Bison.

> 🔧 *Claude Docs: ajustó el borrador (todo va en la carpeta `Tarea2`)*

> 🔧 *Escribió `notas de memoria`*

> 🔧 *Bash: descargó los manuales oficiales de Flex y Bison (`flex.texi`, `bison.texi`) y el README de WinFlexBison, y buscó las citas*

> 🔧 *Claude Docs: creó la pestaña "Notas: Flex & Bison" y la llenó con las citas y explicaciones*

Listas las notas de Flex & Bison, en una pestaña nueva del [mismo documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5) ("Notas: Flex & Bison"). Cubren los 6 puntos de la consigna. Cada uno trae la referencia, la cita textual del manual en inglés y mi explicación en español. Al final hay una tabla de referencias con IDs (F1, B1…) para que las cites en tu texto.

Saqué las citas directo de los manuales de Flex 2.6.4 y Bison 3.8.2, no de memoria.

Para la carpeta `Tarea2`, créala tú en la raíz del repo, igual que hiciste en la Tarea 1. Cuando redactes la sección 2.1, pásamela y la reviso. ¿O prefieres que primero sigamos con ANTLR4?

---

## 4. Prompt

*(texto pegado en el chat del proyecto)*

````text
2.1 Flex & Bison

1. Plataforma y lenguaje base: Son programas hechos en C y pueden correr nativamente en Linux y en Mac, en windows se usa un port llamado WinFlexBison (en mi caso al ser windows ese usaría). Flex genera código en C y Bison lo genera en C, C + + o en Java.
2. Características básicas:
   1. Formato de entrada Flex (.1): En el documento se separan las secciones (Definitions, rules, user code) por una línea de separación con %%.
      1. Definitions: Es donde van los nombres de patrones que se repiten en el código.
      2. Rules: Es donde van las reglas de patrón, y la acción en C que se ejecuta cuando las reglas se cumplen.
      3. User Code: Es donde van las funciones como main u otras.
   2. Formato de entrada Bison (.y):Tiene 4 secciones, Prologue, Bison declarations, Grammar rules y Epilogue.
      1. Prologue: Tiene todos los #include que necesita el código para funcionar.
      2. Bison declarations: Contiene la declaración de tokens, la precedencia de operadores y el tipo de los valores del código.
      3. Grammar rules: Contiene la gramática en tipo BNF con sus acciones.
      4. Epilogue: Contiene al igual que user code el formato final del código (main, etc).
   3. Tipo de ejecución: Ambos son generadores de código, ninguno analiza el lenguaje directo, pero producen un código en C que se compila con g + + junto al código que haces.
3. Licenciamiento: Ambas son gratuitas y open source.
4. Bases teóricas:
   1. Expresiones regulares y autómatas finitas (Flex): Flex convierte las expresiones regulares a un NFA (autómata finito no determinista) y luego en un DFA (autómata determinista). Si en la tabla de estados dos patrones coinciden aplica dos reglas, gana el que consume más texto y si empatan gana el primero en el archivo.
   2. Gramáticas libres de contexto (LALR)[Bison]: Bison genera un parser bottom-up en donde va metiendo tokens en una pila, y cuando lo que hay arriba de la pila coincide con el lado derecho lo reemplaza por el izquierdo. Para hacer esto mira un token de adelante.
5. Tipo de interfaz: Ambos son herramientas de línea de comandos, ambos se escriben en cualquier editor de texto.
6. Inyección de código propio: Inyectar código es fácil, se utiliza en tres lugares, Al inicio (en donde van los #include y así), en las acciones (va el código C++ que se corre cada vez), y al final (ahi van las funciones como main)

asi puse lo de Flex y Bison que me explico el agente en el thread
````

### Respuesta de Claude

Revisé tu 2.1. Va muy bien encaminada; esto es lo que corregiría y lo que le falta:

**Errores**
- La extensión de Flex es `.l` (letra L), no `.1`.
- Se escribe `C++` y `g++`, sin espacios.
- "autómatas finitas" va en masculino: "autómatas finitos".
- **4.1:** no es que dos patrones coincidan "en la tabla de estados", sino que dos patrones coinciden con el mismo texto de la entrada.
- **2.2, Epilogue:** no es "el formato final del código", sino código que se copia tal cual al final del archivo generado (como `main`, `yylex` o `yyerror`).

**Imprecisiones**
- **1:** Bison también genera D (C, C++, D o Java).
- **2.2, Prologue:** además de los `#include`, lleva declaraciones de funciones y variables que usan las acciones.
- **2.2, Bison declarations:** es el tipo de los *valores semánticos* de tokens y reglas, no "del código".
- **4.2:** di "el lado derecho *de una regla*". Y el título mezcla dos cosas: la gramática es libre de contexto, y el método es LALR(1).

**Lo que falta**
- **3, Licencia:** la consigna pide el *tipo*. Flex es BSD y Bison es GPLv3, con la excepción de que el código generado se puede usar libremente.
- **2.3, Ejecución:** falta la parte en tiempo de ejecución: `yyparse()` le pide tokens a `yylex()` uno por uno.
- **4.1:** di por qué usa un DFA: revisa todos los patrones a la vez, en una sola pasada. Y nombra la regla: *longest match*.
- **4.2:** nombra *shift* y *reduce*, di que la gramática se escribe en BNF y explica que el "(1)" de LALR(1) es ese token de *lookahead*.
- **6:** pon los delimitadores (`%{ %}` al inicio, `{ }` en las acciones), `$$` y `$1`, y aclara que el código se copia tal cual. "Se corre cada vez" queda mejor como "cada vez que se reconoce ese patrón o regla".
- **Citas:** todavía no hay ninguna. Pon el ID [F1], [B4], etc. junto a cada afirmación; las tienes en la tabla de la pestaña de notas.

Cuando lo corrijas me lo pasas, o seguimos con ANTLR4.

---

## 5. Prompt

> continua

### Respuesta de Claude

Va, sigo con ANTLR4: saco las citas de su documentación oficial y armo su pestaña de notas.

> 🔧 *Bash: descargó la documentación oficial de ANTLR 4.13.2 y buscó las citas*

> 🔧 *WebFetch: artículo ALL(*) en antlr.org*

> 🔧 *Claude Docs: creó la pestaña "Notas: ANTLR4" y la llenó con las citas y explicaciones*

Ya está la pestaña "Notas: ANTLR4" en el [documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5). Tiene el mismo formato que la de Flex & Bison y sus referencias van de A1 a A12.

Lo más importante para comparar con Bison:
- **Teoría:** Bison es ascendente (LR). ANTLR es descendente (LL), con el algoritmo ALL(\*), que puede mirar todos los tokens adelante que necesite.
- **Formato:** el lexer y el parser van en un solo archivo `.g4`, sin `%%`.
- **Código propio:** además de las acciones dentro de la gramática, se puede escribir fuera, en *listeners* y *visitors*, que son clases de C++ que heredas.
- **Licencia:** BSD y gratis. La herramienta necesita Java 11 o más.

Cuando tengas tu 2.2 me la pasas, o sigo con Boost.Spirit X3.

---

## 6. Prompt

> sigamos

### Respuesta de Claude

Va, sigo con Boost.Spirit X3: saco las citas de su documentación oficial y armo su pestaña de notas.

> 🔧 *Bash: descargó la documentación oficial de Boost.Spirit X3 (Boost 1.92) y buscó las citas*

> 🔧 *Claude Docs: creó la pestaña "Notas: Spirit X3" y la llenó con las citas y explicaciones*

Ya está la pestaña "Notas: Spirit X3" en el [documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5). Sus referencias van de S1 a S7. Con esta, las tres fichas están completas.

Lo que distingue a Spirit de las otras dos:
- **No genera código.** La gramática se escribe dentro de un `.cpp` con operadores de C++ (`>>`, `|`, `*`), y el propio compilador arma el parser.
- **Teoría:** usa descenso recursivo con PEG. Las alternativas se prueban en orden y los ciclos son voraces.
- **No hay scanner aparte.** Lee caracteres directamente y salta los espacios con un *skip parser*.
- **Código propio:** son lambdas pegadas a la gramática con `p[f]`.
- **Licencia:** Boost Software License, gratis. Es *header-only*, sin interfaz propia.

Pásame tus secciones 2.2 y 2.3 cuando las tengas. Después siguen la tabla comparativa y el ejercicio con Flex & Bison.

---

## 7. Prompt

> dame la tabla comparativa pero esa si damela ya ehcha ya que no a fuerzas voy a tener lo mismo que me des

### Respuesta de Claude

Va, te lleno la tabla comparativa completa en la sección 3 del documento, con sus referencias.

> 🔧 *Claude Docs: llenó la tabla comparativa de la sección 3*

Ya está la tabla comparativa completa en la sección 3 del [documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5). Tiene 13 criterios, cada celda con su referencia (F, B, A, S), una frase de entrada y una recomendación al final: Flex & Bison para el mini-proyecto.

Además de los criterios de la consigna, agregué tres filas que ayudan a comparar: resultado del análisis, recursión por la izquierda y manejo de ambigüedad. Las tres son mías y no tienen cita textual. La de ambigüedad en ANTLR la escribí sin referencia porque la documentación no lo dice con esas palabras. Puedes editar o borrar lo que quieras ahí mismo.

---

## 8. Prompt

> dame lo del ejercicio ahora

### Respuesta de Claude

Va. Te armo la guía del ejercicio paso a paso, con ideas y pistas, sin código para copiar, igual que en la Tarea 1.

> 🔧 *Bash: revisó las versiones de flex, bison, g++ y cmake e instaló flex*

> 🔧 *Bash: revisó `Stack.hpp` y `Queue.hpp` de la Tarea 1*

> 🔧 *Bash: armó y compiló una solución de referencia privada para validar las pistas (no se compartió)*

> 🔧 *Claude Docs: creó la pestaña "Guía: ejercicio" con 6 pasos*

> 🔧 *Bash: verificó que FindFLEX y FindBISON de CMake buscan `win_flex` y `win_bison`*

> 🔧 *Claude Docs: releyó la guía*

> 🔧 *Escribió `notas de memoria`*

Ya está la guía del ejercicio en la pestaña "Guía: ejercicio" del [documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5). Como en la Tarea 1, te doy ideas, pistas y preguntas para pensar; el código lo escribes tú.

Son 6 pasos: instalar WinFlexBison, CMake, el struct `Cuadruplo`, `calc.y`, `calc.l`, y compilar y probar. Al inicio está la salida exacta que debe dar tu programa, para que compares, y al final una tabla con los errores más comunes.

Antes de escribirla armé mi propia versión en privado: compila sin warnings y da esa salida, así que las pistas están probadas.

Empieza por el paso 1 y mándame la salida de `win_flex --version` y `win_bison --version`.

---

## 9. Prompt

> ya lo termine, y recibi los outputs deseados, como pide la tarea que lo ponga en el docs?

### Respuesta de Claude

> 🔧 *Bash: git fetch y revisó los archivos en `origin/main`*

¡Bien! La consigna no pide un formato para el ejercicio, solo dice "de preferencia, desarrolla un ejercicio en alguna de las herramientas". Así que basta con que se entienda qué hiciste y que funciona. Te recomiendo la sección 4 del borrador, de una a dos páginas, en este orden:

1. **Qué hace:** un párrafo y el ejemplo de entrada (las 4 líneas de `entrada.txt`).
2. **Archivos:** la tabla de los 4 archivos de `Tarea2/` y qué hace cada uno. Puedes adaptar la de "La meta" en la guía.
3. **Cómo funciona:** el flujo `calc.l` → Flex → tokens → `calc.y` → Bison → cuádruplos, y la tabla de la pila con `a = 3 + 4 * 2;` (la de la guía, paso 4a, explicada con tus palabras).
4. **Dónde se inyectó código:** este es el punto que conecta con la investigación. Muestra 2 o 3 fragmentos cortos de **tu** código: el `%{ %}` con los `#include` de tu `Stack` y tu `Queue`, una acción con `generar("+")` y la regla de `NUM` en Flex con `strdup`. Cita [F7] y [B9].
5. **Cómo compilar y correr:** los comandos de CMake y el `calc < entrada.txt`.
6. **Evidencia:** una captura de tu terminal con la salida. Una captura es mejor que el texto copiado, porque demuestra que lo corriste tú.
7. **Repositorio:** el link a la carpeta `Tarea2` en GitHub.

Una cosa: en GitHub todavía no veo la carpeta `Tarea2`, el último commit sigue siendo el de la Tarea 1. Acuérdate de hacer commit y push antes de poner el link.

Si quieres que revise tu código antes de que lo subas, pégamelo aquí.

---

## 10. Prompt

> ya hice el commit puedes checalro si gustas para que veas que hice y asi

### Respuesta de Claude

> 🔧 *Bash: git fetch y revisó el commit `52a46b1` (diff y archivos)*

> 🔧 *Bash: copió el commit a una carpeta aparte, lo compiló desde cero y lo corrió con `entrada.txt`*

Lo revisé y está muy bien. Bajé tu commit, lo compilé desde cero y lo corrí:

- **Compila sin un solo warning.**
- Con tu `entrada.txt` da exactamente la salida esperada: los 8 cuádruplos y el error de la línea 3.
- También lo probé con algo que no venía en la guía: un carácter inválido (`$`) y finales de línea de Windows (`\r\n`). Reporta el `$`, se recupera y sigue bien con la siguiente línea.

El código está limpio y se entiende. Me gustaron los comentarios en `generar` (por qué el primer pop es el derecho) y en `'(' expr ')'` (por qué no lleva acción). Los dos te sirven para explicar en el documento.

Detalles opcionales, nada que afecte la calificación:
- El mensaje del commit dice "comit tarea 2". Ya está subido, así que no lo cambies; solo cuida la redacción en los siguientes, como hiciste en la Tarea 1.
- El commit anterior ("Commit") metió `Conversacion_Tarea1.md` y `Documento_Tarea1.pdf` en la raíz. Si vas a renombrar el repo a "Ensamblador", más adelante podrías mover los archivos de la Tarea 1 a su propia carpeta `Tarea1/`, igual que `Tarea2/`.
- En `Tarea2/CMakeLists.txt` usaste `-pedantic` y en la raíz `-Wpedantic`. Hacen lo mismo; solo es por consistencia.

Lo que falta es escribir la sección 4 del documento. Si quieres, cuando la tengas me la pasas y la reviso como la 2.1.

---

## 11. Prompt

> dame el punto 5 del uso de IA y el conocimeinto y asi, y dame de una vez el punto 6 de todas las referencias en formato APA, asegurate de que las referencias se abran bien

### Respuesta de Claude

Va: escribo la sección 5 (conclusión y uso de IA) y la 6 (referencias en APA), y abro cada link para verificarlo.

> 🔧 *Bash y WebFetch: verificó que abriera cada link de las referencias (manuales, licencias, repos y artículos por DOI/Crossref)*

> 🔧 *Bash: comparó el texto de las citas con la versión de los manuales en línea*

> 🔧 *Claude Docs: escribió las secciones 5 y 6*

> 🔧 *Escribió `notas de memoria`*

Ya están las secciones 5 y 6 en el [documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5).

**Sección 5:** tiene dos partes. "Lo que aprendí" cierra con la elección de Flex & Bison para el mini-proyecto. "Uso de IA" es un párrafo honesto, como el de la Tarea 1, que dice qué hice yo y qué hiciste tú. Revisa ese punto: puse que tú redactaste las fichas 2.1, 2.2 y 2.3, pero solo vi la 2.1. Si alguna no la escribiste tú, ajústalo.

**Sección 6:** son 14 referencias en APA 7, en orden alfabético. Abrí casi todos los links y cargan. Hay tres salvedades:
- **Los DOI de ACM (Ford, 2004 y Parr et al., 2014):** ACM bloquea las revisiones automáticas, así que no los pude abrir directamente. Para el de Parr et al. confirmé los datos en Crossref. El de Ford no lo alcancé a confirmar ahí; ábrelo tú en el navegador.
- **El link `tree/4.13.2` de ANTLR:** no lo abrí, pero sí otros archivos de esa misma versión.
- **Versiones de los manuales en línea:** son de Flex 2.6.2 y Bison 3.8.1, no 2.6.4 y 3.8.2. Comparé el texto y las citas que usamos son idénticas, así que las referencias quedaron con la versión que se ve en la web.

Para citar en el texto, los IDs de las notas equivalen a esto:
- F → (Paxson et al., 2016), y la licencia F4 → (Flex Project, 2017)
- B → (Donnelly & Stallman, 2021)
- W1 → (Lexxmark, s.f.)
- A1 a A12 → (ANTLR Project, 2024a), salvo A1 y A8 → (ANTLR Project, 2024b) y A10 → (Parr et al., 2014)
- S1 a S7 → (de Guzman & Kaiser, 2018), salvo S3 → (Boost, 2003)

Lo único que falta es la sección 4 del ejercicio.

---

## 12. Prompt

> escribeme la introduccion

### Respuesta de Claude

> 🔧 *Claude Docs: escribió la introducción (sección 1)*

Ya está la introducción en la sección 1 del [documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5). Explica qué es un scanner, qué es un parser y para qué sirve un generador automático, y luego presenta las tres herramientas y por qué las elegiste. Las dos citas textuales vienen del manual de Flex y del de Bison, y las cité en APA con las referencias de la sección 6.

Antes de entregar, borra dos notas que quedaron del borrador: el párrafo de arriba que empieza con "Este es el esqueleto del documento…" y la línea "Extensión: …" de la portada.

---

## 13. Prompt

> no, hazlo corto deq solo di que se hizo en el docs

### Respuesta de Claude

> 🔧 *Claude Docs: acortó la introducción a un párrafo*

Listo, dejé la introducción en un solo párrafo corto que solo dice qué se hace en el documento. Está en la sección 1 del [documento](https://claude.ai/code/artifact/27a5351c-f902-46e5-8a47-d64dcd3f4da5).

---

## 14. Prompt

*(escrito en el chat del proyecto)*

> generame un md con todas los prompts y repsuestas y asi para mi maestra

### Respuesta de Claude

> 🔧 *Leyó los mensajes del hilo y del chat del proyecto*

> 🔧 *Escribió `Conversacion_Tarea2.md`*

Listo, aquí está [Conversacion_Tarea2.md](/mnt/project-files/Ensamblador/Tarea2/Conversacion_Tarea2.md) con los 14 prompts y sus respuestas, en el mismo formato que la de la Tarea 1. Para entregarlo, cópialo a la carpeta `Tarea2/` del repo y haz tú el commit.

---
