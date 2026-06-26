# Underwater-GL-Simulation

## O projekcie
Interaktywna aplikacja 3D w C++/OpenGL przedstawiająca podwodną jaskinię w fioletowo-różowej estetyce. Projekt skupia się na zaawansowanym renderingu PBR, efektach post-processingu oraz dynamicznej manipulacji obiektami w czasie rzeczywistym.

## Technologie i Biblioteki
* **Język:** C++ / GLSL
* **Biblioteki:** GLFW, GLEW, GLM, Assimp, SOIL, ImGui

## Sterowanie i Interakcje

### Nawigacja
* `Ctrl + W/A/S/D/Q/E` – Ruch kamerą (wspiera poruszanie się w górę/dół/boki).
* `C` – Przełączanie trybu sterowania myszką (Mouse Look).

### Zarządzanie Sceną (Edytor)
* `TAB` / `F` – Wybór kolejnego obiektu w scenie.
* `Strzałki` – Przesuwanie wybranego obiektu (Oś X/Z).
* `Shift + Strzałki Góra/Dół` – Przesuwanie wybranego obiektu w osi Y.
* `Q / E / R` – Rotacja wybranego obiektu (X / Y / Z).
* `Z / X` – Skalowanie obiektu.
* `P` – Duplikowanie wybranego obiektu.
* `Delete` – Usuwanie wybranego obiektu.
* `M` – Zapisanie aktualnego układu sceny do `generated_scene_layout.hpp`.

### Interakcje Specjalne
* `Space` – Uruchomienie animacji bąbelka (refrakcja).
* `G` – Włączenie/wyłączenie pulsowania blasku kryształów.

## Zaimplementowane Metody

### Metody Wybrane 
* **[A13] Efekty wodne:** Refrakcja i załamanie światła na bąbelkach powietrza z wykorzystaniem tekstury tła.
* **[B05] Animowana roślinność:** Wodorosty falujące w shaderze wierzchołkowym z parametrami prądu morskiego.

### Metody Obowiązkowe
* **Quaternion camera control** – Nawigacja bez efektu Gimbal Lock.
* **PBR lighting** – Oświetlenie oparte na fizyce (metaliczność/szorstkość) dla kryształów, skał i podłoża.
* **Normal mapping** – Detale powierzchni ścian jaskini w przestrzeni TBN.
* **Shadow mapping** – Dynamiczne cienie z filtrowaniem PCF.
* **PTF (Parallel Transport Frames)** – Płynne poruszanie obiektów po 
