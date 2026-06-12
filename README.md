# Underwater-GL-Simulation

## O projekcie
Interaktywna aplikacja 3D w C++/OpenGL przedstawiająca podwodną, krętą jaskinię w fioletowo-różowej estetyce (Barbie vibe). Zawiera świecące kryształy, ruchomą roślinność, bąbelki z refrakcją oraz zaawansowane efekty post-processingu.

## Technologie i Biblioteki
* **Język:** C++ / GLSL
* **Biblioteki:** GLFW, GLEW, GLM, Assimp (ładowanie modeli `.obj` / `.gltf`)

## Sterowanie i Interakcje

### Nawigacja
* `W` `A` `S` `D` – Ruch kamerą w przestrzeni
* `Myszka` – Rozglądanie się (płynny obrót)

### Interakcje 
1. `Klawisz E` + Celowanie myszką – **Rozbłysk kryształu** (Raycasting + aktywacja silniejszego efektu Bloom).
2. `Strzałki (Góra / Dół)` – **Zmiana prądu morskiego** (Płynna zmiana siły i szybkości falowania wodorostów).
3. `Klawisz F` – **Mącenie wody** (Zwiększenie gęstości fioletowej mgły głębinowej).

## Zaimplementowane Metody

### Metody Wybrane 
* **[A13] Environment cubemap reflections/refractions** – Efekt Fresnela, odbicia i załamania tła na bąbelkach powietrza (`reflect` / `refract` w GLSL).
* **[B05] Seaweed waving in vertex shader** – Animacja falowania kęp roślinności funkcją sinus opartą na czasie i wysokości wierzchołków.

### Metody Obowiązkowe
* **Quaternion camera control** – Kamera sterowana kwaternionami (`glm::quat`), brak efektu Gimbal Lock.
* **PBR lighting** – Shader PBR (wariacja metallic/roughness) dla kryształów i skał.
* **Normal mapping** – Chropowatość i detale ścian jaskini w przestrzeni stycznej TBN.
* **Shadow mapping** – Dynamiczne cienie z mapy głębokości (filtrowanie PCF + bias).
* **Parallel Transport Frames (PTF)** – Płynny ruch obiektu (np. rybki/świetlika) po krzywej B-Spline bez skręcania osi.
* **Underwater skybox/cubemap** – Sześcienna tekstura głębi wodnej tworząca tło i środowisko do odbić.

### Efekty Dodatkowe
* **Post-processing (Bloom)** – Wyodrębnienie jasnych pikseli z mapy *Emissive* i rozmycie ich (Gaussian Blur) przy użyciu FBO.
* **Distance Fog** – Podwodna mgła dystansowa budująca klimat głębin.
