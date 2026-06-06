# GRK projekt - interaktywny swiat podwodny

Folder `projekt` jest szkieletem finalnej sceny. Aktualny stan testuje import assetow: renderowana jest jaskinia z `project_assets/cave`, testowy model krysztalow z `project_assets/cristal`, roslinnosc z `project_assets/plant_1` oraz wodorosty z `project_assets/seaweed_grass` i `project_assets/seaweed_pink`. Kamera stoi w srodku jaskini, a pozostale elementy sceny sa chwilowo wylaczone.

## Wybrane metody

- A13 Environment cubemap reflections/refractions: kod `shaders/refract.*` zostaje w projekcie, ale banka jest chwilowo wylaczona z widoku podczas testu modelu jaskini.
- B05 Seaweed/anemone waving in vertex shader: kod i shadery zostaja w projekcie, ale renderowanie roslinnosci jest chwilowo wylaczone do czasu dodania assetow.
- Bloom: `shaders/blur.*`, `shaders/bloom_final.*`, pipeline zostaje aktywny dla krysztalow, banki i przyszlych emisyjnych assetow.

## Metody obowiazkowe w szkielecie

- Normal mapping: `shaders/pbr.*`, gotowe do uzycia po dodaniu modeli/materialow.
- PBR lighting: Cook-Torrance metallic/roughness w `pbr.frag`, gotowe do uzycia po dodaniu modeli/materialow.
- Quaternion camera control: struktura `QuaternionCamera` w `project_scene.hpp`.
- Shadow mapping: `depth.*`, FBO `depthMapFBO`, PCF i bias w `pbr.frag`; renderowanie obiektow do cienia jest chwilowo puste.
- Parallel Transport Frames: funkcje `buildTransportFrames`, `getFrameAt` zostaja jako baza pod przyszly kabel, luk lub tor dekoracji.
- Underwater skybox/cubemap: `skybox.*`, cubemapa z `project_assets/skybox` jest barwiona na podwodna atmosfere.

## Sterowanie

- Mysz - obrot kamery kwaternionowej w miejscu.
- `C` - wlacz/wylacz przechwycenie myszy.
- `Ctrl + W/A/S/D/Q/E` - tymczasowy ruch debugowy po scenie.
- `P` - wypisz aktualna pozycje kamery w konsoli.
- `Esc` - zamknij program.

## Pliki, na ktorych warto pracowac dalej

- `src/main.cpp` - uruchomienie GLFW, GLEW i ImGui.
- `src/project_scene.hpp` - scena, inicjalizacja, kamera, obiekty, interakcje i pipeline renderowania.
- `shaders/pbr.vert`, `shaders/pbr.frag` - PBR, normal mapping, shadow mapping.
- `shaders/refract.vert`, `shaders/refract.frag` - metoda A13.
- `shaders/seaweed.vert`, `shaders/seaweed.frag` - metoda B05.
- `shaders/skybox.*` - podwodny cubemap/skybox.
- `shaders/blur.*`, `shaders/bloom_final.*` - bloom.

## Co wymienic pozniej

- Zastapic tymczasowa cubemape z `project_assets/skybox` prawdziwa podwodna cubemapa.
- Dodac kolejne docelowe modele z `project_assets`: dodatkowe koralowce, skrzynia i dekoracje sceny.
- Dopracowac materialy PBR: osobne roughness/metallic/ao mapy.
- Rozszerzyc README o podzial pracy trzech osob i screeny do demo.
