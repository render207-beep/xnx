# X NX (esqueleto)

Cliente nativo de X para Switch homebrew: libnx + Borealis + libcurl.
Estado: **esqueleto sin compilar ni probar**. Muestra el timeline "Inicio"
(solo lectura). La API es la web no oficial de X: puede romperse y puede
ir contra sus términos. Úsalo con una cuenta que no te importe perder.

## Compilar en GitHub (sin instalar nada)
1. Crea un repo en GitHub y sube TODO el contenido de esta carpeta (incluida `.github`).
2. Pestaña **Actions** > workflow **Build XNX** (corre solo al subir; o *Run workflow*).
3. Al terminar, descarga el artefacto **XNX-nro** (trae `XNX.nro`).
4. Cópialo a `sdmc:/switch/` y crea `sdmc:/switch/xnx/config.json`.
Si falla, copia el log del paso que salió en rojo y pásamelo.

## Compilar en local (devkitPro con switch-dev, switch-curl, switch-zlib, cmake)
    git init && git submodule add https://github.com/xfangfang/borealis third_party/borealis
    cmake -B build_switch -DPLATFORM_SWITCH=ON \
      -DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake
    make -C build_switch XNX.nro
Copia `cacert.pem` a `resources/` y empaquétalo en romfs (o pon
`"insecure_tls": true` solo para depurar).

## Configurar
Copia `config.example.json` a `sdmc:/switch/xnx/config.json` y rellena:
1. `auth_token` y `ct0`: cookies de tu sesión en x.com (DevTools > Application).
2. `bearer`: el Bearer público que el JS de x.com envía en `authorization`.
3. `home_timeline_query_id`: el ID en la URL del request `HomeTimeline`
   (DevTools > Network). X rota estos IDs.
4. Si X responde 400, copia el JSON `features` de ese mismo request a
   `x_client.cpp` (hoy va vacío).

## Pendiente
Ver tweet completo, perfil, búsqueda, imágenes (texturas con libcurl),
publicar/like (POST + `x-client-transaction-id`), guardar sesión cifrada.

## Ojo con la API de Borealis
Los nombres (`registerAction`, `brls::async/sync`, `ScrollingFrame`) siguen
el fork de xfangfang; si tu versión difiere, ajusta `timeline_view.cpp`.
